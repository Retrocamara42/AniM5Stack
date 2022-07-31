#include "aws_mqtt_api.h"

WiFiClientSecure net;
MQTTClient client = MQTTClient(2048);
uint16_t previousMillis = 0;
const uint16_t REC_INTERVAL = 5000;
String downloading_filename="";


void save_part(String filename, String num_part, String part, uint16_t total_parts){
  //Serial.println("SD save part as image with name: "+num_part+"_"+filename);
  String sd_filename="/"+num_part+"_"+filename;
  SD.remove(sd_filename);
  File file = SD.open(sd_filename, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  //Serial.print("Part "+num_part+": "+part+"\r\n");
  for(uint16_t i=0; i<part.length(); i+=2){
    file.write(strtoul(part.substring(i,i+2).c_str(),NULL,16));
  }
  file.flush();
  file.close();
  merge_files(filename, num_part, total_parts);
  if(SD.exists(filename)){ downloading_filename=""; }
}


void callback(String &topic, String &payload){
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);
  //Serial.println(payload);

  if(String(topic) == "sd_file"){
    int16_t part_ind=payload.indexOf("\"pt\":");
    int16_t filename_ind=payload.indexOf("\"fn\":");
    int16_t filename_ind_end=payload.indexOf(',',filename_ind);
    int16_t total_part_ind=payload.indexOf("\"tt\":");
    int16_t total_part_ind_end=payload.indexOf(',',total_part_ind);
    int16_t num_part_ind=payload.indexOf("\"pn\":");
    int16_t num_part_ind_end=payload.indexOf(',',num_part_ind);
    if(part_ind>=0 && filename_ind>=0 && num_part_ind>=0){
      String filename=payload.substring(filename_ind+6,filename_ind_end-1);
      String num_part=payload.substring(num_part_ind+5,num_part_ind_end);
      uint16_t total_parts=payload.substring(total_part_ind+5,total_part_ind_end).toInt();
      String part=payload.substring(part_ind+7,payload.length()-2);
      if(downloading_filename==""){
        SD.remove(filename);
        downloading_filename=filename;
      }
      save_part(filename,num_part,part,total_parts);
    }
  }
}



void publish_missing_files(String missing_files){
  Serial.print("Publish SD missing files...\r\n");
  DynamicJsonDocument sd_miss_files(256);
  sd_miss_files["dev_name"] = "anim5";
  sd_miss_files["miss_files"] = missing_files;
  char str_sd_miss_files[200];
  serializeJson(sd_miss_files, str_sd_miss_files);
  client.publish("sd_missing_file", str_sd_miss_files);
  Serial.print("Publish SD missing files...OK\r\n");
}




void publish_sd_info(){
  Serial.print("Publish SD info...\r\n");
  DynamicJsonDocument sd_info(256);
  sd_info["dev_name"] = "anim5";
  sd_info["total_storage"] = SD.totalBytes();
  sd_info["free_storage"] = (SD.totalBytes() - SD.usedBytes());
  sd_info["used_storage"] = SD.usedBytes();
  char str_sd_info[100];
  serializeJson(sd_info, str_sd_info);
  client.publish("m5_sd_info", str_sd_info);
  Serial.print("Publish SD info...OK\r\n");
}


void publish_alive(){
  Serial.print("Publish m5 alive...\r\n");
  DynamicJsonDocument m5_alive(256);
  m5_alive["dev_name"] = "anim5";
  char str_m5_alive[50];
  serializeJson(m5_alive, str_m5_alive);
  client.publish("m5_alive", str_m5_alive);
  Serial.print("Publish m5 alive...OK\r\n");
}


void configureMqttConnection(const char* server, uint16_t port,
      const char* iot_cert_pem, const char* iot_client_cert_pem, 
      const char* iot_key_pem){
  Serial.print("Configuring mqtt server\r\n");
  net.setCACert((const char*)iot_cert_pem);
  net.setCertificate((const char*)iot_client_cert_pem);
  net.setPrivateKey((const char*)iot_key_pem);
  client.begin(server, port, net);
  client.setCleanSession(false);
  client.onMessage(callback);
  Serial.print("Configuring mqtt server...OK\r\n");
}


void connectToWiFi(){
  Serial.print("Checking wifi connection..");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\r\nWifi connection ok");
  //Serial.println("\r\nLocal IP:");
  //Serial.println(WiFi.localIP());
}


void reconnect(){
  // Loop until we're reconnected 
  while (!client.connected()) {
    connectToWiFi();
    Serial.print("Attempting MQTT connection...");
    if (client.connect("anim5stack")) {
      Serial.println("connected");
      //client.subscribe("m5_sd_info");
      client.subscribe("sd_file", LWMQTT_QOS0);
      if(downloading_filename!=""){
        Serial.println("Asking for missing files");
        String missing_files=request_missing_files(downloading_filename);
        publish_missing_files(missing_files);
        downloading_filename="";
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.lastError());
      Serial.printf(" will try again in %d seconds\r\n", REC_INTERVAL);
    }
  }
}


void non_blocking_check_connection(void) {
  if(millis() - previousMillis >= REC_INTERVAL && !client.connected()){
    previousMillis = millis();
    reconnect();
  }
  else{
    client.loop();
  }
}