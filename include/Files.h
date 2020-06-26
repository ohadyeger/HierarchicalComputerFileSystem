#ifndef FILES_H_
#define FILES_H_

#include <string>
#include <vector>
using namespace std;

class BaseFile {
private:
	string name;


	
public:
	BaseFile(string name);
	string getName() const;
	void setName(string newName);
	virtual int getSize() = 0;
    virtual void clear()=0;
	virtual ~BaseFile();
    BaseFile(const BaseFile& other);//copy constructor
    virtual BaseFile& operator=(const BaseFile& other);//copy assignment operator
    virtual BaseFile& operator=( BaseFile&& other);//move assignment operator
    BaseFile(BaseFile&& other);//move constructor
};

class File : public BaseFile {
private:
	int size;

		
public:
	File(string name, int size); // Constructor
	int getSize(); // Return the size of the file
    virtual void clear();//

};

class Directory : public BaseFile {
private:
	vector<BaseFile*> children;
	Directory *parent;



public:

	Directory(string name, Directory *parent); // Constructor
	Directory *getParent() const; // Return a pointer to the parent of this directory
	void setParent(Directory *newParent); // Change the parent of this directory
	void addFile(BaseFile* file); // Add the file to children
	void removeFile(string name); // Remove the file with the specified name from children
	void removeFile(BaseFile* file); // Remove the file from children
	void sortByName(); // Sort children by name alphabetically (not recursively)
	void sortBySize(); // Sort children by size (not recursively)
	BaseFile * find(string name);//finds file or directory in children
	vector<BaseFile*> getChildren(); // Return children
	int getSize(); // Return the size of the directory (recursively)
	string getAbsolutePath();  //Return the path from the root to this
	virtual ~Directory();//destructor
	Directory & operator=(const Directory &aDirectory);//copy assignment operator
    Directory & operator=(Directory &&other);// Move Assignment
	Directory(Directory &&other);	// Move Constructor
    Directory(const Directory &aDirectory);//copy constructor
    void copy(vector<BaseFile *> &my);
    static bool compareByString(BaseFile *bf1, BaseFile *bf2);
    static bool compareByInt(BaseFile *bf1, BaseFile *bf2);
    virtual void clear();

};

#endif
