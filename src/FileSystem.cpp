
#include "../include/FileSystem.h"
#include "../include/Files.h"
#include "../include/GlobalVariables.h"
#include <iostream>

FileSystem::FileSystem():rootDirectory(new Directory("",nullptr)),workingDirectory(rootDirectory){}// constructor

Directory& FileSystem::getRootDirectory() const {return *rootDirectory;}

Directory& FileSystem::getWorkingDirectory() const {return *workingDirectory;}

void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory) {workingDirectory=newWorkingDirectory;}

FileSystem::~FileSystem() {//destructor

    if (verbose == 1 || verbose == 3)
        cout<<"FileSystem::~FileSystem()"<<endl;
    clear();
}
void FileSystem::clear() {
if(rootDirectory==nullptr)return;
    delete rootDirectory;
  workingDirectory= nullptr;
  rootDirectory= nullptr;
}

FileSystem & FileSystem::operator=(const FileSystem &other){//copy assignment operator

    if (verbose == 1 || verbose == 3)
        cout<<"FileSystem & FileSystem::operator=(const FileSystem &other)"<<endl;
    if (this == &other)
        return *this;

    clear();
    this->rootDirectory = other.rootDirectory;
    this->workingDirectory = other.workingDirectory;
    return *this;
}

FileSystem & FileSystem:: operator=(FileSystem &&other){// Move Assignment

    if (verbose == 1 || verbose == 3)
        cout<<"FileSystem & FileSystem:: operator=(FileSystem &&other)"<<endl;

    clear();
    *rootDirectory = other.getRootDirectory();
    *workingDirectory = other.getWorkingDirectory();
    other.setWorkingDirectory(nullptr);
    other.workingDirectory=nullptr;
return *this;}

FileSystem::FileSystem(FileSystem &&other)
        :rootDirectory(&other.getRootDirectory()),workingDirectory(&other.getWorkingDirectory()) {// Move Constructor

    if (verbose == 1 || verbose == 3)
        cout<<"FileSystem::FileSystem(FileSystem &&other)"<<endl;

    other.workingDirectory=nullptr;
    other.rootDirectory=nullptr;
}

FileSystem::FileSystem (const FileSystem &other):FileSystem(){//copy constructor

    if (verbose == 1 || verbose == 3)
        cout<<"FileSystem::FileSystem (const FileSystem &other)"<<endl;

    this->rootDirectory=other.rootDirectory;
    this->workingDirectory=other.workingDirectory;
}
