#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t childpid = fork();
    if (childpid == -1)
    {
        perror("Failed to fork");
        return 1;
    }
    else if (childpid == 0)
    {
        printf("Child: Hello from child!\n");
    }    
    else
    {
        printf("Parent: Hello from parent!\n");
        wait(NULL);

        printf("Status: Child is finished\n");
    }

    sleep(4);

    printf("Bye!\n");

    return 0;
}
