#All Targets
all: fs
# Tool invocations
# Executable "fs" depends on the files "files"
fs: bin/Commands.o bin bin/Environment.o bin/Files.o bin/FileSystem.o \
	bin/GlobalVariables.o  bin/Path.o bin/Split.o bin/Main.o

	@echo 'Building target: fs'
	@echo 'Invoking: C++ Linker'
	g++ -pthread -o bin/fs  bin/Commands.o bin/Environment.o bin/Files.o bin/FileSystem.o bin/GlobalVariables.o \
		bin/Path.o bin/Split.o bin/Main.o
	@echo 'Finished building target: fs'
	


# Depends on the source and header files
bin/Commands.o: src/Commands.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Commands.o src/Commands.cpp

# Depends on the source and header files
bin/Environment.o: src/Environment.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Environment.o src/Environment.cpp

# Depends on the source and header files
bin/Files.o: src/Files.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Files.o src/Files.cpp

# Depends on the source and header files
bin/FileSystem.o: src/FileSystem.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/FileSystem.o src/FileSystem.cpp

# Depends on the source and header files
bin/GlobalVariables.o: src/GlobalVariables.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/GlobalVariables.o src/GlobalVariables.cpp

# Depends on the source and header files
bin/Path.o: src/Path.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Path.o src/Path.cpp
	
# Depends on the source and header files
bin/Split.o: src/Split.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Split.o src/Split.cpp

# Depends on the source and header files
bin/Main.o: src/Main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp
#Clean the build directory
clean:
	rm -f bin/*

