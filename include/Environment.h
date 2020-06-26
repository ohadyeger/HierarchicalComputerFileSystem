#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_
#include <vector>
#include "Files.h"
#include "Commands.h"
#include <iostream>
#include <string>


using namespace std;

class Environment {
private:
	vector<BaseCommand*> commandsHistory;
	FileSystem fs;

public:
	Environment();
	void start();
	FileSystem& getFileSystem() ; // Get a reference to the file system
	void addToHistory(BaseCommand *command); // Add a new command to the history
	const vector<BaseCommand*>& getHistory() const; // Return a reference to the history of commands
    void trim(string &cmd); // trims the command for more convinient programming
	virtual ~Environment();//destructor
	Environment & operator=(const Environment &aEnvironment);//copy assignment operator
	Environment & operator=(Environment &&other);// Move Assignment
	Environment(Environment &&other);	// Move Constructor
	Environment(const Environment &aEnvironment);//copy constructor
	void clear();
};

#endif