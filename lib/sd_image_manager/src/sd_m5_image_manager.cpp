/**
 * @file sd_image_manager.cpp
 * @brief Implementation of class sd_image_manager
 *
 * This file provides the implementation of the class sd_image_manager,
 * that saves into sd card and loads from sd card to show into LCD screen
 *
 * @author Juan Neyra
 * @date May 20 2022
 *
 */
#include "sd_m5_image_manager.h"

TFT_eSprite img = TFT_eSprite(&M5.Lcd); 


/*
* @brief Constructor for class SdImageManager
*
* @param imgpath Name of image to be saved or loaded
*
*/
SdImageManager::SdImageManager(char *imgpath) : 
            imgpath(imgpath), width(SCREEN_WIDTH), height(SCREEN_HEIGHT),
            image_len(width*height), next(-1){ 
    for(uint16_t i=0; i<SCREEN_WIDTH; i++){ this->row[i]=0; }
}


/*
* @brief Sets up M5 stack LCD screen
*
*/
void SdImageManager::start(){
    img.setColorDepth(SPRITE_COLOR_DEPTH);
    img.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
}


/*
 * @brief Gets the value of the current row array
 *
 */
uint16_t* SdImageManager::get_row(){ return this->row; }


/*
 * @brief Gets the width of the image
 *
 */
uint16_t SdImageManager::get_width(){ return this->width; }


/*
 * @brief Gets the height of the image
 *
 */
uint16_t SdImageManager::get_height(){ return this->height; }



/*
 * @brief Saves a bitmap image into the sd card
 *
 * @param image Array with values of image to be saved
 *
 */
bool SdImageManager::sd_save_bmp_image(const uint16_t* image){
    Serial.println("SD save bitmap started");
    SD.remove(this->imgpath);
    File file = SD.open(this->imgpath, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return false;
    }
    /* 
     * Bitmap file Header
     *    Image identifier (2 bytes): BM
     *    Size of file (4): image_len+START_ADD_IMG
     *    Reserved (2): 0
     *    Reserved (2): 0
     *    Starting address of image (4): START_ADD_IMG
     */
    file.print("BM");
    print_little_endian(file, this->image_len+START_ADD_IMG, 4);
    print_zero(file, 4);
    print_little_endian(file, START_ADD_IMG, 4);
    

    /*
     * DIB Header
     *    BITMAPINFOHEADER
     *    Size of header (4): 52
     *    Width (4): this->width
     *    Height (4): this->height
     *    Num color planes (2): 1
     *    Depth of image (2): 16
     *    Compression type (4): 0
     *    Image size (4): image_len
     *    Horizontal resolution (4): 0
     *    Vertical resolution (4): 0
     *    Number of colors (4): 0
     *    Number of important colors (4): 0
     *    Red channel bit mask (4): 63488 (0xF800)
     *    Green channel bit mask (4): 2016 (0x07E0)
     *    Blue channel bit mask (4): 31 (0x001F)
     *    
     */
    print_little_endian(file, 52, 4);
    print_little_endian(file, this->width, 4);
    print_little_endian(file, this->height, 4);
    print_little_endian(file, 1, 2);
    print_little_endian(file, 16, 2);
    print_little_endian(file, 3, 4);
    print_little_endian(file, this->image_len, 4);
    print_zero(file, 16);
    print_little_endian(file, 63488, 4);
    print_little_endian(file, 2016, 4);
    print_little_endian(file, 31, 4);
    file.flush();

    /*
     * Image
     */
    for(int y=this->height-1; y>=0; y--){
      for(int x=0; x<this->width; x++){
        print_little_endian(file, image[y*320+x], 2);
      }
      file.flush();
    }
    file.close();
    Serial.println("SD save bitmap ended");
    return true;
}



/*
 * @brief Load next row in loaded image
 *
 */
void SdImageManager::load_next_row(){
    File file = SD.open(this->imgpath);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }
    // Next goes back to 0
    if((++this->next) >= ((int16_t)this->height)){ 
        this->next=0;
    }
    // Go to start of image
    uint32_t last_pos=START_ADD_IMG+(this->height-this->next)*this->width*2;
    file.seek(last_pos);
    // Read row
    for(uint16_t i=0; i<this->width; i++){
        uint32_t number=0;
        if(!file.available()){ break; }
        // Read first byte
        number+=file.read();
        // Read second byte (most significant)
        number+=file.read()*256;
        this->row[i] = number;
    }
    file.close();
}


/*
 * @brief Loads image from sd card and shows it into screen
 *
 */
void SdImageManager::show_bmp_image(){
    for(uint16_t i=0; i<this->height; i++){
        this->load_next_row();
        img.pushImage(0, this->next, this->width, 1, (uint16_t *)this->row);
    }
    img.pushSprite(0, 0);
}
