/**
 * @file sd_utils.h
 * @brief Header file for sd_utils.cpp
 *
 * SD utils contains all functions related to SD card management
 * for program Anim5Stack
 *
 * @author Juan Neyra
 * @date September 4th 2021
 *
 */
#ifndef SD_UTILS
#define SD_UTILS

#include <M5Stack.h>

/**
 * @brief Returns number of bytes in human readable format
 *
 * Given an integer number of bytes, this function will return a String
 * with the value in B, KB, MB or GB depending on readability
 *
 * @param bytes Number of bytes
 *
 * @return human readable size as String
 */
String humanReadableSize(uint32_t bytes);

/**
 * @brief Return all directories and files on the SD card
 *
 * Goes through each folder in SD card and returns an String
 * containing all files (and their sizes) and directories 
 * separeted by a new line
 *
 * @return String with files and folders separated by a new line
 */
String listFiles();

/**
 * @brief Return the first file found given two keywords
 *
 * Goes through each file in SD card, if the name of the file contains
 * both keywords, it returns the name of the file
 * 
 * @param keyword1 First keyword to match in filename
 * @param keyword2 Second keyword to match in filename
 *
 * @return String with name of matched file
 */
String findFileByKeyword(String keyword1, String keyword2="");

#endif
