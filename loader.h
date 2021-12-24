#ifndef LOADER_H
#define LOADER_H

#include "vmath.h"
#include "model.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define OBJ_FILE 0

namespace load
{
    Model      *createObj(std::string path);
    static bool parseLine(std::string line, int fileType, Model *model);
};

Model *
load::createObj(std::string path)
{
    std::ifstream obj_file;
    obj_file.open(path);
    if (!obj_file.is_open()) {
        std::cout << "Could not read " << path << ". Closing.\n";
        exit(1);
    }
    Model *model = new Model();
    std::string line;
    int lineNum = 0;
    while (std::getline(obj_file, line)) {
        if (!load::parseLine(line, OBJ_FILE, model)) { // change from OBJ_FILE in the future, when more file types are supported
            std::cout << "Line " << lineNum << ": Could not determine line type.\n";
        }
        ++lineNum;
    }
    return model;
}

bool
load::parseLine(std::string line, int fileType, Model *model)
{
    std::stringstream ss(line);
    if (fileType == OBJ_FILE) {
        std::string type, x, y, z;
        ss >> type >> x >> y >> z;
        switch (type[0]) {  // line type
            case 'v':
                model->addVertex(std::stod(x, nullptr), std::stod(y, nullptr), std::stod(z, nullptr));
                return true;
            case 'f':
                model->addTri(std::stoi(x, nullptr), std::stoi(y, nullptr), std::stoi(z, nullptr));
                return true;
            case '#':
                return true;
        }
    }
    return false;
}

#endif