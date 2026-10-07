#pragma once

#include <SPI.h>
#include <SD.h>

bool readIntArrayFromSD(const char* filename, uint32_t* arr, uint32_t* size, uint32_t max_size);
bool readMultipleIntArraysFromSD(const char* filenames[], uint32_t* arrays[], uint32_t* sizes[], uint32_t max_size, uint32_t num_files);