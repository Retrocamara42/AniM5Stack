#include "anim5_server.h"


void handleUpload(AsyncWebServerRequest *request, 
    String filename, size_t index, uint8_t *data, size_t len, bool final) {
  Serial.println("Client:" + request->client()->remoteIP().toString() + \
                 " " + request->url());

  if(filename.indexOf(".bmp")>0){
    if(!index){
      request->_tempFile = SD.open("/" + filename, "w");
      Serial.println("Downloading file " + filename +" from server started");
    }

    if(len){
      for(size_t i=0; i<len; i++){
        request->_tempFile.write(data[i]);
      }
      //Serial.println("Downloading file " + filename + " part "+\
      //        String(index) + " with size " + String(len));
    }

    if(final){
      request->_tempFile.close();
      Serial.println("File " + filename + " downloaded. Size: " + \
              String(index + len));
      request->redirect("/");
    }
  }
}


void configureWebServer(AsyncWebServer *server){
  server->on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("Client:" + request->client()->remoteIP().toString() + \
            " " + request->url());
    request->send_P(200, "text/html", index_html, processor);
  });

  server->on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200);
      }, handleUpload);
}


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


String processor(const String& var) {
  if (var == "FILES") {
    return listFiles();
  }
  if (var == "MEM_TOTAL") {
    return humanReadableSize(SD.totalBytes());
  }
  if (var == "MEM_FREE") {
    return humanReadableSize((SD.totalBytes() - SD.usedBytes()));
  }
  if (var == "MEM_USED") {
    return humanReadableSize(SD.usedBytes());
  }

  return String();
}