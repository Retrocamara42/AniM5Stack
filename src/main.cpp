#include "main.h"

#ifndef AWS_SERVER
AsyncWebServer server= AsyncWebServer(80);
#endif


void setup() {
  M5.begin();
  Wire.begin();

  Serial.begin(115200);
  Serial.println("Serial started");

  WiFi.setHostname("iot_ms");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  connectToWiFi();

  #ifdef AWS_SERVER
  configureMqttConnection(mqtt_server, 8883, iot_cert_pem_start,
          iot_client_cert_pem_start, iot_key_pem_start);
  #else
  configureWebServer(&server);
  server.begin();
  #endif
  Serial.print("\r\nAsync web server started\r\n");
}

void loop() {
  // Begins SD Image Manager
  SdImageManager sdImageManager=SdImageManager("/background.bmp");
  sdImageManager.start();
  sdImageManager.show_bmp_image();
  reconnect();
  publish_sd_info();
  while(1){
    non_blocking_check_connection();
    M5.update();
    if(M5.BtnC.wasPressed()){ 
      sdImageManager.set_imgpath(getNextFileSD(sdImageManager.get_imgpath()));
      sdImageManager.show_bmp_image();
    }
  }
}
