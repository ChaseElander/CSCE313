/* -------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                  */
/* Project: CSCE-313 Machine Problem #3                                       */
/*                                                                            */
/* File: ./mp3_part7.cpp                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Standard Library Includes                                                  */
/* -------------------------------------------------------------------------- */
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include<sys/types.h>

using namespace std;

int main()
{
   
    int filedes[2];
    pipe(filedes); // filedes[0] is for reading, filedes[1] is for writing.
    
    if(fork() == 0){//allows only child into IF
        
        int filedes2[2];
        pipe(filedes2);
        
        if(fork() == 0){ //grandchild only
            close(filedes2[0]); //close unused read
            dup2(filedes2[1], 1);// go into filedes2
            //dup2(filedes2[0], 1);   //This?  0 is for reading 1 is for writing
            close(filedes2[1]); //close pipe[1]
            char* arg[] = {"tr", "[a-zA-Z0-9]", "a", NULL};
            execvp("tr", arg);
            
        }
        else{  // Child only, not grandchild
            execl("/bin/ls", "/bin/ls", "-la", NULL); //only grandchild runs -la -ls
            //Runs the 'ls -la', now must send the results to the child to run next command
           
            wait(NULL); //Parent waits for child to complete and send temp
            close(filedes2[1]);
            char c;
            while((read(filedes2[0], &c, 1))!=0 ){   //while read from filedes2 hasn't finished
                write(filedes[0], &c, 1);//write to filedes
            }
            
        }

        close(filedes2[0]);
    }else{  //Final parent ONLY
        wait(NULL); //Parent waits for child to complete and send temp
        close(filedes[1]);
        char c;
        while((read(filedes[0], &c, 1))!=0 ){   //not exactly sure what the 1 means
           write(STDOUT_FILENO, &c, 1);
        }
    }
    
    //close(filedes[0]);
    //close(filedes[1]);
     // close(filedes2[0]);
    //close(filedes2[1]);
    exit(0);
}

