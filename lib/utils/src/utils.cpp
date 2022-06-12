/**
 * @file utils.h
 * @brief Implementation of helper functions
 *
 * This file provides the implementation of functions to be used by  
 * the program
 *
 * @author Juan Neyra
 * @date May 21 2022
 *
 */
#include "utils.h"

/*
 * @brief Prints a number to a file as hexadecimal and using little endian
 * format. It makes sure it takes the amount of bytes equal to num_byte
 *
 * @param file Manages file handling
 * @param number Number to write
 * @param num_bytes Number of bytes needed to be written
 *
 */
void print_little_endian(File file, uint32_t number, uint8_t num_byte){
    for(uint8_t i=0; i<num_byte; i++){
        file.write((number>>i*8) & MASK_UINT8);
    }
}

/*
 * @brief Prints zero to a file for an amount of bytes equal to num_byte
 *
 * @param file Manages file handling
 * @param num_bytes Number of bytes needed to be written
 *
 */
void print_zero(File file, uint8_t num_byte){
    for(uint8_t i=0; i<num_byte; i++){
        file.write(0);
    }
}



String getNextFileSD(String currentFile){
  String filename="";
  File root = SD.open("/");
  while (true) {
    File entry =  root.openNextFile();
    if (!entry) { break; }
    if(String(entry.name())==currentFile){
        File entry = root.openNextFile();
        if (!entry) { 
            root = SD.open("/");
            entry = root.openNextFile();
        }
        Serial.println("Entry name: ");
        Serial.print(entry.name());
        Serial.println();
        filename=entry.name();
        entry.close();
        break;
    }
    entry.close();
  }
  return filename;
}