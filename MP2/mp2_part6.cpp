/* -------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                  */
/* Project: CSCE-313 Machine Problem #3                                       */
/*                                                                            */
/* File: ./mp3_part6.cpp                                                      */
/* -------------------------------------------------------------------------- */
/* Programmer: Chase Elander, Spring 2017                                     */
/* -------------------------------------------------------------------------- */
/* Standard Library Includes                                                  */
/* -------------------------------------------------------------------------- */
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>



int main()
{
    int filedes[2];
    //char buf[PIPE_BUF];
    //char buf = [PIPE_BUF];
    //ssize_t numBytes;
    
    pipe(filedes); // filedes[0] is for reading, filedes[1] is for writing.
    pid_t pid = fork();
    if (pid < 0) {
        std::cout << "ERROR" << strerror(errno) <<std::endl; 
    }else if(pid == 0){//allows only child into IF
        close(filedes[0]); //close unused read
        dup2(filedes[1], 1);
        close(filedes[1]);
        char* arg[] = {"ls", "-la", NULL};
        execvp("ls", arg); //only child runs -la -ls         
    }else{
        waitpid(pid, NULL, WNOHANG); //Parent waits for child to complete and send temp
        close(filedes[1]);
        char c;
        while((read(filedes[0], &c, 1))!=0 ){   //not exactly sure what the 1 means
           write(STDOUT_FILENO, &c, 1);
        }
    }

    
    exit(0);
}

