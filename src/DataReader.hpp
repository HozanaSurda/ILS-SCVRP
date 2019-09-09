#ifndef DATA_CVRP_READER_H_
#define DATA_CVRP_READER_H_

#include <cstdio>
#include <cmath> 
#include <vector>
#include <cstring>
#include <fstream>
#include "Data.hpp"

#define MAX_CHARS_PER_LINE 1000
#define MAX_TOKENS_PER_LINE 100
#define DELIMITER " "

using namespace std;

int ParseDataset(
        const char * dataset_filename,
        Data &data);

#endif
