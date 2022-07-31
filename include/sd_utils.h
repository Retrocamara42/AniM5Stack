#ifndef SD_UTILS
#define SD_UTILS

#include <M5Stack.h>

String humanReadableSize(uint32_t bytes);

String listFiles();

String findFileByKeyword(String keyword);

void merge_with_prev_files(String filename, String num_part, uint16_t total_parts);

void merge_files(String filename, String num_part, uint16_t total_parts);

String request_missing_files(String filename);

#endif
