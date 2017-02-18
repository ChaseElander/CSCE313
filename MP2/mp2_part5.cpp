/* -------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                  */
/* Project: CSCE-313 Machine Problem #3                                       */
/*                                                                            */
/* File: ./mp3_part5.cpp                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Standard Library Includes                                                  */
/* -------------------------------------------------------------------------- */

#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>


int main()
{
	
	pid_t pid = fork();
	if (pid == 0){ //If statment only passed by the child.
        std::cout<<"Hello";
     	exit(0);
    }else {
        waitpid(pid,NULL,0); //else only accessed by the parent
        std::cout<<"World!" <<std::endl;
     	exit(0);
    }
    
 return(0);
}

