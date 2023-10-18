# cshell
A simple shell program for linux machines created using C

Introduction. 

The application is a simple command line interpreter built using the C programming language for Linux machine. Running the application spawns a shell to which we can give any Linux command and it would display the output like how the default terminal shell would return the output. Internally , the program uses execl function of the c standard library to execute the respective binaries corresponding to the input. The program utilizes the fact that all the commands that can be run on a linux machine are basically executable binaries that are already stored in the system.  

 

Implementation 

The shell is built in c using basic libraries. When the executable file is started, it waits for input in a loop until exit is entered. There are dynamic variables created to store the input and path where the binaries reside. For ease of use, I copied the binary files to a temporary folder since some of the binaries were inside /usr/bin/ apart from the /bin/ directory where it usually resides.  

 

Once the input is taken, it is appended to the path (local) variable and is prepared to be executed by the execl function. Before execution, a new child process is created using the C fork function and the linux command is executed as a child process. What the execl function is doing is that , it simply executes the input given as an OS command.  And since all the commands are already executables situated under the bin directory, it just gets executed like that.  

 
