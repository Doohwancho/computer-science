#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

/*

multitaking: 1 core switching between multiple processes
multiprocessing: 2 core each takes a process, run in paralell.
    in this example, parent processor and child processor runs in paralell.
*/

void create_process(const int& number, pid_t* processTable) {
	pid_t forkedProcess;

	if (number > 0) {
		// fork() creates new child process, which is duplicate of the calling (parent)process. parent process and child process run in separate memory space.
		forkedProcess = fork(); //forkedProcess is 0 -> else

		if (forkedProcess == -1) {
			perror("Error occured when creating process\n");
			exit(EXIT_FAILURE);
		}
		else if (forkedProcess > 0) {
			cout << "This is parent process " << getpid() << endl;
			// wait() is a system call to block the calling process until all of it's child processes finish their execution
			wait(nullptr);
			cout << "Finishing termination " << getpid() << endl;
		}
		else {
			cout << "Printed from child process " << getpid() << endl;
			create_process(number - 1, processTable);
		}
	}
}

int main() {
	int NUMBER_OF_PROCESSES = 3;
	int mainProcess = getpid();

    cout << mainProcess << endl;

	// pid_t : data type that is used to indicate PID. This is a signed integer.
	pid_t processTable[3] = {-1};

	create_process(NUMBER_OF_PROCESSES, processTable);

	return 0;
}

/* console.log

This is parent process 95225
Printed from child process 95226
This is parent process 95226
Printed from child process 95227
This is parent process 95227
Printed from child process 95228
Finishing termination 95227
Finishing termination 95226
Finishing termination 95225

*/
