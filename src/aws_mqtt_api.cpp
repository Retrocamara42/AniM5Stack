/**
 * @file aws_mqtt_api.cpp
 * @brief Implementation file of aws iot related functions
 *
 * Contains the implementation of functions related to the use of
 * mqtt to connect to aws iot core
 *
 * @author Juan Neyra
 * @date September 4th 2021
 *
 */
#include "aws_mqtt_api.h"

WiFiClientSecure net;
MQTTClient client = MQTTClient(256);
WiFiClient* net_http;
HTTPClient http;
uint16_t previousMillis = 0;
const uint16_t REC_INTERVAL = 6000;
const char* _mqtt_server="a596fikiaw4lr-ats.iot.us-east-2.amazonaws.com";
uint16_t _port=8883;


uint8_t fromHexCharToInt(char hex_char){
  return (hex_char <= '9') ? hex_char-'0':hex_char-'W';
}


int8_t download_file(String sd_filename){
  Serial.println("Download file started");
  // Disconnect from mqtt first
  client.disconnect();
  // Connect to http
  http.begin(WEB_SERVER,WEB_PORT,WEB_URI,(const char*)server_sd_file_pem_start);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String httpRequestData = "device="+String(DEVICE_NAME)+ 
      "&token="+String(DEVICE_TOKEN)+"&filename="+String(sd_filename);
  int httpResponseCode = http.POST(httpRequestData);
  if(httpResponseCode>=0){
    Serial.println("Response code: "+String(httpResponseCode));
    if(SD.exists(sd_filename)){ SD.remove(sd_filename); }
    File file = SD.open("/"+sd_filename, FILE_WRITE);
    if (!file) {
      Serial.println("Failed to open file for writing");
      return 1;
    }
    net_http =http.getStreamPtr();
    while(!net_http->available());
    char read_val=net_http->read();
    uint8_t dec;
    bool recv_units=false;
    while(1){
      while(!net_http->available());
      read_val=net_http->read();
      if(read_val=='"'){ break; }
      if(recv_units){
        uint8_t units = fromHexCharToInt(read_val);
        file.write(16*dec+units);
        recv_units=false;
      }
      else{ 
        dec = fromHexCharToInt(read_val);
        recv_units=true; 
      }
      yield();
    }
    file.close();
    Serial.println("File downloaded");
  }
  else{
    Serial.println("Response: "+http.errorToString(httpResponseCode));
  }
  net_http->stop();
  http.end();
  Serial.println("Download file ended");
  return 0;
}



void callback(String &topic, String &payload){
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  if(topic == "sd_file"){
    int16_t dev_ind=payload.indexOf("\"dv\":")+5;
    int16_t dev_ind_end=payload.indexOf(',',dev_ind);
    int16_t tk_ind=payload.indexOf("\"tk\":")+5;
    int16_t tk_ind_end=payload.indexOf(',',tk_ind);
    int16_t fn_ind=payload.indexOf("\"fn\":")+5;
    int16_t fn_ind_end=payload.indexOf('}',fn_ind);
    String device=payload.substring(dev_ind,dev_ind_end);
    String token=payload.substring(tk_ind,tk_ind_end);
    Serial.println("Device name: "+device);
    Serial.println("Token: "+token);
    if(device==DEVICE_NAME && token==DEVICE_TOKEN){
      Serial.println("Token and device are correct");
      String sd_filename=payload.substring(fn_ind,fn_ind_end);
      Serial.println("Filename: "+sd_filename);
      download_file(sd_filename);
    }
  }
}


void publish_sd_info(){
  Serial.print("Publish SD info...\r\n");
  DynamicJsonDocument sd_info(256);
  sd_info["dev_name"] = DEVICE_NAME;
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
  m5_alive["dev_name"] = DEVICE_NAME;
  char str_m5_alive[50];
  serializeJson(m5_alive, str_m5_alive);
  client.publish("m5_alive", str_m5_alive);
  Serial.print("Publish m5 alive...OK\r\n");
}


void configureMqttConnection(const char* server, const uint16_t port,
      const char* iot_cert_pem, const char* iot_client_cert_pem, 
      const char* iot_key_pem){
  net.setCACert((const char*)iot_cert_pem);
  net.setCertificate((const char*)iot_client_cert_pem);
  net.setPrivateKey((const char*)iot_key_pem);
  client.begin(server, _port, net);
  client.setCleanSession(false);
  client.onMessage(callback);
}



void connectToWiFi(){
  Serial.print("Checking wifi connection..");
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\r\nWifi connection ok");
  //Serial.println("\r\nLocal IP:");
  //Serial.println(WiFi.localIP()");
}

void reconnect(){
  // Loop until we're reconnected 
  while (!client.connected()) {
    connectToWiFi();
    Serial.print("Attempting MQTT connection...");
    if(client.connect(DEVICE_NAME)) {
      Serial.println("connected");
      client.subscribe(TOPIC_SD_FILE, LWMQTT_QOS0);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.lastError());
      Serial.printf(" will try again in %d miliseconds\r\n", REC_INTERVAL);
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