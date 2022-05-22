#include <M5Stack.h>
#include "sd_m5_image_manager.h"


void setup() {
  M5.begin();
  Wire.begin();

  Serial.begin(115200);
  Serial.println("SD saving image");
}

void loop() {
  SdImageManager sdImageManager=SdImageManager("/background.bmp");
  sdImageManager.start();
  sdImageManager.show_bmp_image();
  while(1){
    M5.update();
  }
}
