#include "../include/Environment.h"
#include "../include/GlobalVariables.h"

Environment::Environment():commandsHistory(vector <BaseCommand*>() ),fs((FileSystem())){}//constructor

void Environment::start() {

    string cmd="";
    string command="";
    string args="";
    bool running = true;

    while (running) {//if command starts with spaces doesnt work

        std::cout << fs.getWorkingDirectory().getAbsolutePath() << ">";
        getline(cin, cmd);
        if (cmd == "exit")break;
        if (verbose == 2 || verbose == 3){
            cout <<cmd<<endl;}

        trim(cmd);
        size_t end = cmd.find(" ");

        if (end != string::npos){
            command = cmd.substr(0,end);
            args = cmd.substr(end+1);
        }
        else  command = cmd;

        BaseCommand *currCmd;


        if (command == "pwd")currCmd = new PwdCommand(args);
        else if (command == "cd") currCmd = new CdCommand(args);
        else if (command == "ls") currCmd = new LsCommand(args);
        else if (command == "mkdir") currCmd = new MkdirCommand(args);
        else if (command == "mkfile") currCmd = new MkfileCommand(args);
        else if (command == "cp") currCmd = new CpCommand(args);
        else if (command == "mv") currCmd = new MvCommand(args);
        else if (command == "rename") currCmd = new RenameCommand(args);
        else if (command == "rm") currCmd = new RmCommand(args);
        else if (command == "history") currCmd = new HistoryCommand(args, commandsHistory);
        else if (command == "verbose") currCmd = new VerboseCommand(args);
        else if (command == "exec") currCmd = new ExecCommand(args, commandsHistory);
        else if (command == "cd") currCmd = new CdCommand(args);
        else currCmd = new ErrorCommand(cmd);

        currCmd->execute(fs);
        addToHistory(currCmd);
        args ="";
    }

}
FileSystem& Environment::getFileSystem()  { return fs; }

void Environment::addToHistory(BaseCommand *command) {commandsHistory.push_back(command);}

const vector<BaseCommand*>& Environment::getHistory() const {return commandsHistory;}

void Environment::trim(string &cmd) {

    if (cmd.size()==0)return;
    size_t indexBegin = 0;
    size_t indexEnd = cmd.size()-1;

    if(cmd[0]==' ') indexBegin = cmd.find_first_not_of(" ");
    if(cmd[cmd.size()-1] == ' ') indexEnd = cmd.find_last_not_of(" ")-indexBegin;

    cmd = cmd.substr(indexBegin,indexEnd+1);
}
void Environment::clear(){//Destructor

    if (&commandsHistory == nullptr || commandsHistory.size()==0)
        return;
    for (size_t i = 0; i < commandsHistory.size()&&commandsHistory[i]!=nullptr; ++i) {
        delete commandsHistory[i];

    }
}
 Environment:: ~Environment() {//destructor
     if (verbose == 1 || verbose == 3)
         cout<<"Environment:: ~Environment()"<<endl;
     if (&commandsHistory == nullptr || commandsHistory.size()==0)
         return;
   for(size_t i =0;i<commandsHistory.size()&&commandsHistory[i]!=nullptr;i++){
       delete commandsHistory[i];}
     commandsHistory.clear();
}
Environment::Environment (const Environment &aEnvironment):Environment(){//copy constructor

    if (verbose == 1 || verbose == 3)
        cout<<"Environment::Environment (const Directory &aDirectory)"<<endl;
    for(size_t i =0;i<aEnvironment.commandsHistory.size();i++) {
        BaseCommand *copy;
        *copy = *aEnvironment.commandsHistory[i];
        commandsHistory.push_back(copy);
    }}

Environment& Environment::operator=(const Environment &aEnvironment) {//copy assignment operator

    if (verbose == 1 || verbose == 3)
        cout<<"Environment& Environment::operator=(const Environment &Environment)"<<endl;
    if (this == &aEnvironment)
        return *this;
    clear();

    commandsHistory = aEnvironment.commandsHistory;
    return *this;
}

Environment::Environment(Environment&& other): Environment(){// Move Constructor

    if (verbose == 1 || verbose == 3)
        cout<<"Environment::Environment(Environment&& other)"<<endl;
    commandsHistory = other.commandsHistory;
    fs=other.getFileSystem();
    other.commandsHistory.clear();

}

Environment & Environment:: operator=(Environment &&other){// Move Assignment operator

    if (verbose == 1 || verbose == 3)
        cout<<"Directory & Directory:: operator=(Directory &&other)"<<endl;
    if (this != &other) {
        clear();
        this->commandsHistory=other.commandsHistory;
        fs=other.fs;
        commandsHistory.clear();

    }

    return *this;
}

