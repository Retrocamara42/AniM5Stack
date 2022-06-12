/**
 * @file utils.h
 * @brief Definition of helper functions
 *
 * This file provides the definition of functions to be used by  
 * the program
 *
 * @author Juan Neyra
 * @date May 21 2022
 *
 */
#ifndef UTILS
#define UTILS

#include <M5Stack.h>
#include <vector>

#define MASK_UINT8 0xFF

/*
 * @brief Prints a number to a file as hexadecimal and using little endian
 * format. It makes sure it takes the amount of bytes equal to num_byte
 *
 * @param file Manages file handling
 * @param number Number to write
 * @param num_bytes Number of bytes needed to be written
 *
 */
void print_little_endian(File file, uint32_t number, uint8_t num_byte);


/*
 * @brief Prints zero to a file for an amount of bytes equal to num_byte
 *
 * @param file Manages file handling
 * @param num_bytes Number of bytes needed to be written
 *
 */
void print_zero(File file, uint8_t num_byte);


/*
 * @brief Gets all files in SD card as array
 *
 *
 */
String getNextFileSD(String currentFile);

#endif