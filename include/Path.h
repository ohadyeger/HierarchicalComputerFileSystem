#ifndef PATH_H_
#define PATH_H_
#include "../include/Files.h"
#include "../include/FileSystem.h"

class Path {
public:
    BaseFile* pathfinder(FileSystem &fs, string path);//gets a path and returns the Dir or File at the end of the path returns nullptr if not found
    void trim(string &args);//trim spaces from both sides of the command

};



#endif