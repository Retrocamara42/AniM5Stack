#include <M5Stack.h>
#include <WiFi.h>
//#include <HTTPClient.h>
//#include <Arduino_JSON.h>
#include "secrets.h"
#include "anim5_server.h"
#include "sd_m5_image_manager.h"

AsyncWebServer server= AsyncWebServer(80);

void setup() {
  M5.begin();
  Wire.begin();

  Serial.begin(115200);
  Serial.println("Serial started");

  WiFi.begin(ssid, password);
  Serial.print("Connecting to wifi..");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\r\nLocal IP:");
  Serial.println(WiFi.localIP());

  configureWebServer(&server);
  server.begin();
  Serial.print("\r\nAsync web server started\r\n");
}

void loop() {
  // Begins SD Image Manager
  SdImageManager sdImageManager=SdImageManager("/background.bmp");
  sdImageManager.start();
  sdImageManager.show_bmp_image();
  while(1){
    M5.update();
    if(M5.BtnC.wasPressed()){ 
      sdImageManager.set_imgpath(getNextFileSD(sdImageManager.get_imgpath()));
      sdImageManager.show_bmp_image();
    }
  }
}
