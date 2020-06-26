#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "Files.h"


class FileSystem {
private:
	Directory* rootDirectory;
	Directory* workingDirectory;
public:
	FileSystem();
	Directory& getRootDirectory() const; // Return reference to the root directory
	Directory& getWorkingDirectory() const; // Return reference to the working directory
	void setWorkingDirectory(Directory *newWorkingDirectory); // Change the working directory of the file system
    virtual ~FileSystem();//destructor
    FileSystem & operator=(const FileSystem &aDirectory);//copy assignment operator
    FileSystem & operator=(FileSystem &&other);// Move Assignment
    FileSystem(FileSystem &&other);	// Move Constructor
	FileSystem (const FileSystem &other);//copy constructor
    virtual void clear();

};


#endif
