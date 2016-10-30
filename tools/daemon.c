#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void newDaemon(void (*cb)())
{
	pid_t proc_id = fork();

	if (proc_id < 0)
	{
		puts("Fork failed!");
		exit(1);
	}
	else if (proc_id > 0)
	{
		// Parent
		printf("Starting daemon process with pid %d.\n", proc_id);
		exit(0);
	}

	// Child
	// umask(0); // unmask file mode
	pid_t sid = setsid(); // new session
	if (sid < 0) exit(1);
	
	//chdir("/"); // change dir to root

	close(STDIN_FILENO);
	close(STDERR_FILENO);
	close(STDOUT_FILENO);

	FILE* fp = fopen("log.txt", "w+");
	
	while (1)
	{
		sleep(1);
		fprintf(fp, "Logging info... \n");
		fflush(fp);
	}
	
	fclose(fp);
}

void daemonFunc()
{
	
}

int main()
{

	newDaemon(daemonFunc);
}