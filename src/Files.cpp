#include "../include/Files.h"
#include <algorithm>
#include <utility>
#include "../include/GlobalVariables.h"
#include <iostream>





BaseFile::BaseFile(string name):name(name){ }

string BaseFile::getName() const { return this->name; }

void BaseFile::setName(string newName) { this->name = newName; }

BaseFile:: ~BaseFile(){}

BaseFile::BaseFile(const BaseFile &other):name(other.getName()) {

}

BaseFile &BaseFile::operator=(const BaseFile &other) {//copy constructor
    if(this==&other)return *this;
    setName(other.getName());
    return *this;
}

BaseFile &BaseFile::operator=(BaseFile &&other) {//copy assignment operator

    if (this!= &other) {
        setName(other.getName());
        other.clear();
    }
    return *this;
}

BaseFile::BaseFile(BaseFile &&other):name(other.getName()) {//move constructor
    other.setName(nullptr);


}


File::File(string name, int size):BaseFile(name),size(size){}

int File::getSize() { return this-> size; }

void File::clear(){}

Directory::Directory (string name, Directory *parent) : BaseFile(name),children(),parent(parent){}

Directory * Directory::getParent() const { return parent; }

void Directory::setParent(Directory *newParent) { parent=newParent; }

void Directory::addFile(BaseFile* file) { children.push_back(file); }

void Directory::removeFile(string name) {

    for (size_t i = 0; i < children.size(); ++i) {//remove and delete child
        if (children[i]->getName()==name) {
            delete children[i];
            children.erase(children.begin() + i);
            break;
        }
    }
}

void Directory::removeFile(BaseFile* file) {//only remove pointer

    for (size_t i = 0; i < children.size(); ++i) {
        if (children[i] == file) {
            children.erase(children.begin() + i);
            break;
        }
    }
}

void Directory::sortByName(){//sort using stable sort
    std::stable_sort(children.begin(),children.end(),compareByString);
}

void Directory::sortBySize(){//sort by size stable sort
    std::stable_sort(children.begin(),children.end(),compareByInt);
}

vector<BaseFile*> Directory:: getChildren(){ return children; }

int Directory:: getSize(){//computes recurs the size of the dir

    size_t output=0;

    for (size_t i = 0; i < children.size(); ++i)
        output+=children[i]->getSize();

    return output;
}

string Directory::getAbsolutePath(){

    string output;
    string name = this->getName();

    if (parent!=nullptr)
        output.append(parent->getAbsolutePath());

    if (parent!=nullptr&&parent->getName() == "")
        output.append(this->getName());

    else output.append("/" + this->getName());

    return output;
}

Directory::~Directory() {//destructor

    if (verbose == 1 || verbose == 3)
        cout<<"Directory::~Directory()"<<endl;
    clear();
}

void Directory::clear(){//Destructor

    if (&children == nullptr || children.size()==0)
        return;
    for (size_t i = 0; i < children.size()&&children[i]!=nullptr; ++i) {
        delete children[i];

    }
    parent=nullptr;
}

Directory::Directory (const Directory &aDirectory)://copy constructor
        BaseFile(aDirectory.getName()),children(aDirectory.children),parent(aDirectory.getParent()){//copy constructor
    this->copy(children);
    if (verbose == 1 || verbose == 3)
        cout<<"Directory::Directory (const Directory &aDirectory)"<<endl;

}

Directory& Directory::operator=(const Directory &aDirectory) {//copy assignment operator

    if (verbose == 1 || verbose == 3)
        cout<<"Directory& Directory::operator=(const Directory &aDirectory)"<<endl;
    if (this == &aDirectory)
        return *this;

    clear();
    children = aDirectory.children;
    parent=aDirectory.getParent();
    this->setName(aDirectory.getName());
    this->copy(children);
    return *this;// return this List
}

Directory::Directory(Directory&& other):
        BaseFile(other.getName()),children(other.getChildren()),parent(other.getParent()){// Move Constructor

    if (verbose == 1 || verbose == 3)
        cout<<"Directory::Directory(Directory&& other)"<<endl;

    other.setName(nullptr);
    other.parent=nullptr;
    other.children.clear();
}

Directory & Directory:: operator=(Directory &&other){// Move Assignment operator

    if (verbose == 1 || verbose == 3)
        cout<<"Directory & Directory:: operator=(Directory &&other)"<<endl;
    if (this != &other) {
        clear();
        setName(other.getName());
        children = other.children;
        parent=other.parent;
        other.parent=nullptr;
        other.setName(nullptr);
        other.children.clear();
    }

    return *this;
}

bool Directory::compareByString(BaseFile *bf1, BaseFile *bf2){
    return bf1->getName()<bf2->getName();
}

bool Directory::compareByInt(BaseFile *bf1, BaseFile *bf2){
    return bf1->getSize()<bf2->getSize();
}

BaseFile* Directory::find(string name) {//find by name
    if (name == "..") return this;

    for (size_t i = 0; i < children.size(); ++i)
        if (children[i]->getName()==name)
            return children[i];

    return nullptr;
}
//function to deep copy the the data at children
void Directory::copy(vector<BaseFile*> &my) {
    if(getChildren().size()==0)return;
    for(size_t i = 0;i<my.size();i++){

        if(dynamic_cast<Directory*>(my[i])){
            Directory* toAdd = new Directory(*dynamic_cast<Directory*>(my[i]));
            toAdd->setParent(this);
            my[i]=toAdd;
            vector<BaseFile*> toAddch=toAdd->getChildren();
            toAdd->copy(toAddch);
        }
        else {
            File* file = new File(dynamic_cast<File*>(my[i])->getName(),dynamic_cast<File*>(my[i])->getSize());
            my[i]=file;
        }

    }
}


