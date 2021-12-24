#ifndef LOADER_H
#define LOADER_H

#include "vmath.h"

#include <iostream>
#include <fstream>

namespace load
{
    int read(std::string path);
};

int
load::read(std::string path)
{
    std::ofstream obj_file;
    obj_file.open(path);
    if (!obj_file.is_open()) {
        std::cout << "Could not read " << path << ". Closing." << std::endl;
        exit(1);
    }
}

#endif