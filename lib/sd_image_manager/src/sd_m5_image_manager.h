/**
 * @file sd_image_manager.h
 * @brief Definition of class sd_image_manager
 *
 * This file provides the definition of the class sd_image_manager,
 * that saves into sd card and loads from sd card to show into LCD screen
 *
 * @author Juan Neyra
 * @date May 20 2022
 *
 */
#ifndef SD_IMAGE_MANAGER
#define SD_IMAGE_MANAGER

#include <M5Stack.h>
#include "utils.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SPRITE_COLOR_DEPTH 8
#define DIGIT_NUM 5

#define START_ADD_IMG 66

class SdImageManager{
    private:
        char *imgpath;
        uint16_t width;
        uint16_t height;
        uint32_t image_len;
        int16_t next;
        uint16_t row[SCREEN_WIDTH];

    public:
        /*
        * @brief Constructor for class SdImageManager
        * @param imgpath Name of image to be saved or loaded
        */
        SdImageManager(char *imgpath);

        /*
        * @brief Sets up M5 stack LCD screen
        */
        void start();

        /* @brief Gets the value of the current row array */
        uint16_t* get_row();
        /* @brief Gets the width of the image */        
        uint16_t get_width();
        /* @brief Gets the height of the image */
        uint16_t get_height();

        /*
        * @brief Saves a bitmap image into the sd card
        * @param image Array with values of image to be saved
        */
        bool sd_save_bmp_image(const uint16_t* image);
        /* @brief Load next row in loaded image */
        void load_next_row();
        /*
        * @brief Loads image from sd card and shows it 
        * into screen
        */
        void show_bmp_image();
};

#endif