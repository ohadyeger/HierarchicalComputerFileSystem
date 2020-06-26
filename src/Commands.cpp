#include "../include/Commands.h"
#include "../include/Split.h"
#include <algorithm>
#include "../include/Path.h"
#include <typeinfo>
#include "../include/GlobalVariables.h"
using namespace std;

BaseCommand::BaseCommand(string args):args(args){}//constructor

string BaseCommand::getArgs() { return args; }

BaseCommand::~BaseCommand(){}



PwdCommand::PwdCommand(string args):BaseCommand(args){}//constructor

void PwdCommand::execute(FileSystem & fs)
{ cout<< fs.getWorkingDirectory().getAbsolutePath()<<endl;}

string PwdCommand::toString(){return "pwd "+(string)getArgs();}



CdCommand::CdCommand(string args):BaseCommand(args){}//constructor

void CdCommand::execute(FileSystem & fs) {

    Path a;
    string path= getArgs();
    Directory* working = dynamic_cast<Directory*>(a.pathfinder(fs,path));//find working Dir using path function

    if (working != nullptr)//if a Dir is found
        fs.setWorkingDirectory(working);
    else cout << "The system cannot find the path specified" << endl;
}

string CdCommand::toString() { return "cd "+(string)getArgs() ; }

LsCommand::LsCommand(string args):BaseCommand(args){}//constructor

void LsCommand::execute(FileSystem & fs){

    string path="";
    Path a;
    Directory * cur;
    bool sort = false;//false sort by name true sort by size
    if (getArgs()=="")//if empty args we stay at working dir
       cur = &fs.getWorkingDirectory();
    else {//else we check if we need to sort by size
       if (getArgs().substr(0,2)=="-s"){
           sort = true;
           path = getArgs().substr(2);
       }
       else{path = getArgs();}

       cur = dynamic_cast<Directory*>(a.pathfinder(fs,path));//find the dir specified in the path
   }
    //if a path was not found
    if (cur == nullptr) {
        cout << "The system cannot find the path specified" << endl;
        return;
    }
    cur->sortByName();
    if (sort) cur->sortBySize();
    //printing
    for (size_t i = 0;i < cur->getChildren().size();i++) {
        if (typeid(*(cur->getChildren()[i])) == typeid(Directory))//check for type of basefile
            cout << "DIR\t" << cur->getChildren()[i]->getName() << "\t" << cur->getChildren()[i]->getSize() << endl;
        else
            cout << "FILE\t" << cur->getChildren()[i]->getName() << "\t" << cur->getChildren()[i]->getSize() << endl;
    }
}
string LsCommand::toString() { return "ls "+(string)getArgs(); }

MkdirCommand::MkdirCommand(string args):BaseCommand(args){}//constructor

void MkdirCommand::execute(FileSystem & fs) {

    Split a;
    Directory* cur;
    string arg = getArgs();
   //check if we the path specified is absolute or relative
    if (arg.substr(0,1)=="/") {
        cur = &fs.getRootDirectory();
        arg = arg.substr(1);
    }
    else
        cur=&fs.getWorkingDirectory();

    bool found = false;
    Directory* ptr=nullptr;
    Directory* newDir=nullptr;
    vector<string> splited = a.split(arg, '/');//split the path by the token '/'

    for (size_t i = 0; i < splited.size(); i++) {
        ptr = dynamic_cast<Directory*>(cur->find(splited[i]));
        if(ptr==nullptr){
            found = true;
            newDir = new Directory(splited[i],cur);
            if(cur->find(newDir->getName())){//if a dir by the same name exists we delete the new dir and stop the method
                found = false;
                delete(newDir);
                break;
            }
            //continue creating dirs in path
            cur->addFile(newDir);
            cur = newDir;
        } else
            cur = ptr;
    }
    if (!found)
        cout <<"The directory already exists"<<endl;

}

string MkdirCommand::toString() { return "mkdir "+(string)getArgs(); }



MkfileCommand::MkfileCommand(string args):BaseCommand(args){}//constructor

void MkfileCommand::execute(FileSystem & fs) {//add error command if no size specified

    size_t indexLast = getArgs().find_first_of(" ");//find the first space seprating the name and the size of new file
    size_t indexFirst = getArgs().find_last_of(" ");// find the last space
    string size = getArgs().substr(indexFirst);//string of the size of new file
    size_t indexFile = getArgs().find_last_of('/');
    Directory * cur;
    Path a;
    long size2;
    string name;
    const char* arg = size.c_str();
    size2 = static_cast<int>(strtol(arg, nullptr, 10));//covert the size from string to long

    if(indexFile==string::npos){//if we need to make the file in the current dir
        cur = &fs.getWorkingDirectory();
        name = getArgs().substr(0,indexLast);
    }
    else {//if the path points to a different dir
        string path = getArgs().substr(0,indexFile);
        cur =dynamic_cast<Directory*>(a.pathfinder(fs,path));
        name = getArgs().substr(indexFile+1,indexLast-indexFile-1);
    }
    if (cur == nullptr) {
        cout <<"The system cannot find the path specified"<<endl;
        return;
    }
    for(size_t i = 0;i<cur->getChildren().size();i++){
        if(cur->getChildren()[i]->getName()==name)
        {cout <<"File already exists"<<endl;
            return;}
    }
    //if we can make a file at the specified destinion we make and add it
    File *newFile = new File(name,size2);
    cur->addFile(newFile);

}

string MkfileCommand::toString() { return "mkfile "+(string)getArgs(); }



CpCommand::CpCommand(string args):BaseCommand(args){}//constructor

void CpCommand::execute(FileSystem & fs) {

    Path a;
    string args = getArgs();
    size_t firstOfSpaces = args.find_first_of(' ');
    size_t lastOfSpaces = args.find_last_of(' ');
    string from = args.substr(0,firstOfSpaces);
    string to = args.substr(lastOfSpaces+1);
    BaseFile* toCopy = a.pathfinder(fs,from);
    //find the destination dir using the class path
    Directory* t = dynamic_cast<Directory*>(a.pathfinder(fs,to));

    //if path returns null means the destination of the dir/file tobe copied does not exist
    if(t==nullptr ||toCopy== nullptr){
        cout<<"No such file or directory"<<endl;
        return;
    }
    if(typeid(*toCopy)== typeid(Directory)){//if the object to copy is a dir copy it
        if(t->find(toCopy->getName())!= nullptr)return;
        Directory* toC = dynamic_cast<Directory*>(toCopy);
        Directory* newDir = new Directory(*toC);
        newDir->setParent(t);
        t->addFile(newDir);
    }
    else {//if the object is a file
        if(t->find(toCopy->getName())!=nullptr)return;
        File* newFile = new File(toCopy->getName(),toCopy->getSize());
        t->addFile(newFile);
    }

}

string CpCommand::toString() { return "cp "+(string)getArgs(); }



MvCommand::MvCommand(string args):BaseCommand(args){}//constructor

void MvCommand::execute(FileSystem & fs) {

    Path a;
    string args = getArgs();
    size_t firstOfSpaces = args.find_first_of(' ');
    size_t lastOfSpaces = args.find_last_of(' ');
    string fromP;
    string from = args.substr(0,firstOfSpaces);
    size_t lastBS = from.find_last_of('/');
    Directory* parentMoved;//parent of the object to be moved
    BaseFile* toBeMoved;//the object we want to move
    string to = args.substr(lastOfSpaces+1);
    Directory* movingDir = dynamic_cast<Directory*>(a.pathfinder(fs,to));//dir to mv the object to

    if(lastBS==string::npos) {//no back slashs means we mv from the working dir
        parentMoved = &fs.getWorkingDirectory();
        toBeMoved = parentMoved->find(from);
    }
    else{ fromP=from.substr(0,lastBS+1);//find the dir containing the object
        parentMoved= dynamic_cast<Directory*>(a.pathfinder(fs,fromP));
        toBeMoved = parentMoved->find(from.substr(lastBS+1));
    }

    if (toBeMoved!=nullptr) {//if the object exists but its the root or a parent dir
        if (toBeMoved == &fs.getWorkingDirectory() || toBeMoved == &fs.getRootDirectory() ||
            toBeMoved == fs.getWorkingDirectory().getParent()) {
            cout << "Can't move directory" << endl;
            return;
        }
    }
    if ((toBeMoved==nullptr) | (movingDir == nullptr)) {//object not found
        cout<<"No such file or directory"<<endl;
        return;
    }

    //if all conditions are true we can move the object
    parentMoved->removeFile(toBeMoved);
    movingDir->addFile(toBeMoved);
    //check if object is dir then we have to set it to its new parent
    if(typeid(*toBeMoved)== typeid(Directory))
        dynamic_cast<Directory*>(toBeMoved)->setParent(movingDir);


}

string MvCommand::toString() { return "mv "+(string)getArgs(); }



RenameCommand::RenameCommand(string args):BaseCommand(args){}//constructor

void RenameCommand::execute(FileSystem & fs) {

    Path a;
    string args = getArgs();
    size_t firstOfSpaces = args.find_first_of(' ');
    size_t lastOfSpaces = args.find_last_of(' ');
    string fromP;
    string from = args.substr(0,firstOfSpaces);
    size_t lastBS = from.find_last_of('/');
    Directory* parentRenamed;
    BaseFile* toBeRenamed;
    string newName = args.substr(lastOfSpaces+1);

    if(lastBS==string::npos)//if no bs we stay at the current dir
        parentRenamed = &fs.getWorkingDirectory();
    else{//else we find the dir parent of object to be renamed
        fromP=from.substr(0,lastBS+1);
        parentRenamed= dynamic_cast<Directory*>(a.pathfinder(fs,fromP));
        from = from.substr(lastBS+1);
    }
    if (parentRenamed==nullptr) {//not found
        cout<<"No such file or directory"<<endl;
        return;
    }
    BaseFile* check = parentRenamed->find(newName);//if a file having the same name is in the dir
    if (check!= nullptr) return;

    toBeRenamed = parentRenamed->find(from);

    if (toBeRenamed==nullptr) {
        cout<<"No such file or directory"<<endl;
        return;
    }
    if (toBeRenamed==&fs.getWorkingDirectory()||toBeRenamed==&fs.getRootDirectory()) {
        cout<<"Can't rename the working directory"<<endl;
        return;
    }

    toBeRenamed->setName(newName);
}

string RenameCommand::toString() { return "rename "+(string)getArgs(); }



RmCommand::RmCommand(string args):BaseCommand(args){}//constructor
//rm is the same like rename but at the end we delete the object and not change its name
void RmCommand::execute(FileSystem & fs) {

    Path a;
    string args = getArgs();
    size_t firstOfSpaces = args.find_first_of(' ');
    size_t lastOfSpaces = args.find_last_of(' ');
    string fromP;
    string from = args.substr(0,firstOfSpaces);
    size_t lastBS = from.find_last_of('/');
    Directory* parentRemove;
    BaseFile* toBeRemoved;
    string to = args.substr(lastOfSpaces+1);

    if(args == "/"){
        cout<<"Can't remove directory"<<endl;
        return;
    }

    if(lastBS==string::npos) {
        parentRemove = &fs.getWorkingDirectory();
        toBeRemoved = parentRemove->find(from);
    }
    else{
        fromP=from.substr(0,lastBS+1);
        parentRemove= dynamic_cast<Directory*>(a.pathfinder(fs,fromP));
        if (parentRemove!=nullptr)
            toBeRemoved = parentRemove->find(from.substr(lastBS+1));
        else {
            cout<<"Can't remove directory"<<endl;
            return;
        }
    }
    if (toBeRemoved==nullptr ) {
        cout<<"No such file or directory"<<endl;
        return;
    }
    if (toBeRemoved==&fs.getWorkingDirectory() || toBeRemoved == &fs.getRootDirectory()) {
        cout<<"Can't remove directory"<<endl;
        return;
    }

    parentRemove->removeFile(toBeRemoved->getName());
}

string RmCommand::toString() { return "rm " +(string)getArgs();}

HistoryCommand::HistoryCommand(string args, const vector<BaseCommand *> & history):
        BaseCommand(args), history(history){}//constructor

void HistoryCommand::execute(FileSystem & fs) {//prints the history as specified in assig1.pdf

    for (size_t i = 0; i < history.size(); ++i)
        cout<<i<<"\t"<<history[i]->toString()<<endl;
}

string HistoryCommand::toString() { return "history "+(string)getArgs(); }//constructor

VerboseCommand::VerboseCommand(string args):BaseCommand(args){}

void VerboseCommand::execute(FileSystem & fs) {
    Path p;
    string argument = getArgs();
    p.trim(argument);
    const char* arg = argument.c_str();
    int index = static_cast<int>(strtol(arg, nullptr, 10));

    if(index < 0 || index > 3)
        cout <<"Wrong verbose input"<<endl;
    else
        verbose = index;
}

string VerboseCommand::toString() { return "verbose "+(string)getArgs(); }

ExecCommand::ExecCommand(string args, const vector<BaseCommand *> & history):
        BaseCommand(args), history(history){}//constructor

void ExecCommand::execute(FileSystem & fs) {

    Path p;
    string argument = getArgs();
    p.trim(argument);
    const char* arg = argument.c_str();
    size_t index = static_cast<size_t>(strtol(arg, nullptr, 10));

    if ((index>=0) && (index<history.size()))
        history[index]->execute(fs);
    else
        cout <<"Command not found"<<endl;
}

string ExecCommand::toString() { return "exec "+(string)getArgs(); }

ErrorCommand::ErrorCommand(string args):BaseCommand(args) {}//constructor

void ErrorCommand::execute(FileSystem & fs) {

    string command;
    size_t end = getArgs().find(" ");

    if (end != string::npos)
        command = getArgs().substr(0,end);

    else  command = getArgs();

    cout <<command<<": Unknown command"<<endl;
}

string ErrorCommand::toString() { return (string)getArgs() ; }
