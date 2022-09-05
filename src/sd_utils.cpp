/**
 * @file sd_utils.cpp
 * @brief Implemenation file of sd card related functions
 *
 * Contains the implementation of functions related to the use of
 * SD Card in Anim5Stack
 *
 * @author Juan Neyra
 * @date September 4th 2021
 *
 */
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
