#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

	//After fork(), two processes are being run simultaneously: the parent and child process.
	//The parent will have a pid_t value of some pos integer, and the child will have pid_t value of 0.
	//We can check which process is being run by checking the pid (such as below), and by doing so
	//We can make it so that only the parent or child process executes a set of a commands.
	//In this code, 3 processes in total will be created:
	//The main process forks, and creates a parent and child process.
	//Before executing the second fork function
	//An if statement checks whether this is being read by parent or child process
	//If it is the child process, it goes ahead and executes another fork, creating another process.
	//The parent process will not execute this fork.
	//Therefore we have: 
	// Parent ---------> Child
	//						|
	//						|
	//						|
	//					Child of child process
	//And 'Hello' is printed 3 times, once for each process.

	//If the second fork command was not blocked by a logic gate, we would have four processes running.
	//The first fork gives parent and child, then the parent would fork to give another child process
	//And the child will also fork to give another child process
	//Hence, the program will print 'Hello' 4 times, once for each open process.
	
	//So, as a general rule, everytime we fork, there will be 2^n number of processes being run
	//With n being the number of times we have called the fork() function
	//And each line of code after all these forks will be executed 2^n number of times.

/*
int	main(void)
{
	pid_t pid;
	pid = fork();

	if (pid == 0)
		pid = fork();
	printf("%i\n", pid);
}
*/

int	main(int argc, char **argv)
{
	char *cmd = "/bin/ls";
	char *arg[] = {"ls", "-l", "read.txt", NULL};
	char *env[] = {NULL};

	if (execve(cmd, arg, env) == -1)
		printf("Execve error\n");
	printf("oops\n");

	// int	i = 0;
	// while (env[i])
	// {
	// 	printf("%s\n", env[i]);
	// 	i++;
	// }
}