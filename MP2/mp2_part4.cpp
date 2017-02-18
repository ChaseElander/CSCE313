/* -------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman                                                  */
/* Project: CSCE-313 Machine Problem #3                                       */
/*                                                                            */
/* File: ./mp3_part4.cpp                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* Standard Library Includes                                                  */
/* -------------------------------------------------------------------------- */

#include <stdlib.h>
#include <unistd.h>


int main()
{
    //const char *cmd = "/bin/ls";
    /*const char*argv[3];
    argv[0] = "/bin/ls";
    argv[1] = "-la";
    argv[2] = NULL;*/
    char *argv[] = { "/bin/ls", "-la", NULL };
    execvp("/bin/ls" , argv);
    
    exit(0);
}

