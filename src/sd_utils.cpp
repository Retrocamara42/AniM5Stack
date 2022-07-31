#include "sd_utils.h"

String humanReadableSize(uint32_t bytes){
  if(bytes>=1000000000){
    return String(bytes/1000000000)+" GB";
  }
  else if(bytes>=1000000){
    return String(bytes/1000000)+" MB";  
  }
  else if(bytes>=1000){
    return String(bytes/1000)+" KB";
  }
  else{
    return String(bytes)+" B";
  }
}

String listFiles(){
  String sDirectory="";
  File root = SD.open("/");
  while (true) {
    File entry =  root.openNextFile();
    if (! entry) {
      break;
    }
    Serial.print(entry.name());
    if(entry.isDirectory()) {
      sDirectory=sDirectory+String(entry.name())+"... Directory\r\n";
    } else {
      sDirectory=sDirectory+String(entry.name())+"..."+
          humanReadableSize(entry.size())+"\r\n";
    }
    entry.close();
    sDirectory+="\r\n";
  }
  return sDirectory;
}



String findFileByKeywords(String keyword1, String keyword2=""){
  File root = SD.open("/");
  while (true) {
    File entry =  root.openNextFile();
    if (! entry) {
      break;
    }
    String entry_name=entry.name();
    if(!entry.isDirectory()) {
      if(entry_name.indexOf(keyword1)>=0 &&
            entry_name.indexOf(keyword2)>=0){
        return entry_name;
      }
    }
    entry.close();
  }
  return "";
}



void merge_two_files(String appended_into_filename, 
        String read_from_filename, String out_filename){
  /*Serial.println("Merging files "+read_from_filename+" and "+
      appended_into_filename+" into "+out_filename);*/
  SD.remove(out_filename);
  File file1=SD.open(appended_into_filename, FILE_APPEND);
  File file2=SD.open(read_from_filename, FILE_READ);
  while(file2.available()){
    file1.write(file2.read());
  }
  file2.close();
  file1.close();  
  SD.remove(read_from_filename);
  SD.rename(appended_into_filename,out_filename);  
}



void merge_with_prev_files(String filename, String num_part, uint16_t total_parts){
  String curr_filename="/"+num_part+"_"+filename;
  uint16_t i=num_part.toInt()-1; // Num of merged part
  for(i; i>=0; i--){
    String block_filename=findFileByKeywords("/"+String(i)+"_"+filename);
    if(block_filename==""){
      block_filename=findFileByKeywords("_"+String(i)+"_"+filename);
    }
    if(block_filename!=""){
      int8_t index_underscore=block_filename.indexOf("_");
      String num_merged_part=block_filename.substring(1,index_underscore);
      String out_filename="/"+num_merged_part+"_"+num_part+"_"+filename;
      if(num_part.toInt()==total_parts){
        out_filename="/"+filename;
      }
      merge_two_files(block_filename, curr_filename, out_filename);
      curr_filename=out_filename;
    }
    else{ 
      //Serial.println("No more previous files found");
      break; 
    }
  }
  // Merge with parts higher than num_part if they exist
  String next_file=findFileByKeywords("/"+String(num_part.toInt()+1)+"_",filename);
  //Serial.println("Next file found: "+next_file);
  if(next_file!=""){
    // Get part from next file
    int8_t index_underscore=next_file.indexOf("_");
    int8_t index_second_underscore=next_file.indexOf("_",index_underscore+1);
    String num_merged_part;
    if(index_second_underscore>index_underscore){
      num_merged_part=next_file.substring(index_underscore+1,index_second_underscore);
    }
    else{
      num_merged_part=next_file.substring(1,index_underscore);
    }
    // Get part from current file
    index_underscore=curr_filename.indexOf("_");
    String num_curr_part=curr_filename.substring(1,index_underscore);
    String out_filename="/"+num_curr_part+"_"+num_merged_part+"_"+filename;
    if(num_merged_part.toInt()==total_parts){
      out_filename="/"+filename;
    }
    merge_two_files(curr_filename, next_file, out_filename);
  }
}


void merge_files(String filename, String num_part, uint16_t total_parts){
  //Serial.println("Merging files starting from "+num_part+" for file "+filename);
  // Merging with previous files
  merge_with_prev_files(filename,num_part,total_parts);
  // Checking if last file has already arrived
  String last_part_filename="/"+String(total_parts)+"_"+filename;
  if(SD.exists(last_part_filename)){
    //Serial.println("Merging last file");
    // Check if all necessary files to do merge have arrived
    bool nec_files_exist=true;
    for(uint16_t i=num_part.toInt();i<total_parts;i++){
      String next_filename="/"+num_part+"_"+filename;
      if(!SD.exists(next_filename)){
        nec_files_exist=false;
        break;
      }
    }
    // Merge all files into final output
    if(nec_files_exist){
      merge_with_prev_files(filename, String(total_parts), total_parts);
    }
  }
}


String request_missing_files(String filename){
  File root = SD.open("/");
  String missing_files_indexes=filename+";";
  while (true) {
    File entry =  root.openNextFile();
    if(!entry){ break; }
    String entry_filename=entry.name();
    Serial.println(entry_filename);
    if(!entry.isDirectory()) {
      if(entry_filename.indexOf("_"+filename)>=0){
        int8_t index_underscore=entry_filename.indexOf("_");
        int8_t index_second_underscore=entry_filename.indexOf("_",index_underscore+1);
        if(index_second_underscore>index_underscore){
          String num_merged_pt1=entry_filename.substring(1,index_underscore);
          String num_merged_pt2=entry_filename.substring(index_underscore+1,index_second_underscore);
          missing_files_indexes+=num_merged_pt1+"_"+num_merged_pt2+";";
        }
        else{
          missing_files_indexes+=entry_filename.substring(1,index_underscore)+";";
        }
      }
    }
    entry.close();
  }
  return missing_files_indexes;
}