/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/Part_1/main.cpp                                                 */
/* --------------------------------------------------------------------------- */
/* Programmers:  UIN:                                    */
/* Programmers: Chase Elander UIN:423005409                                    */
/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include "linked_list.h"

int main(int argc, char ** argv) 
{
	int b = 128;
	int M = b * 11;  // so we have space for 11 items
	
	int option = 0;
	while ((option = getopt(argc, argv,"b:s:")) != -1) {
        switch (option) {
             case 'b' : b = atoi(optarg); 
                break;
             case 's' : M = atoi(optarg);
                break;
             default:
				std::cout<<"No input arguments were provided."<<std::endl;
        }
    }
	
	char buf [1024];
	memset (buf, 1, 1024);		// set each byte to 1
	
	char * msg = "a sample message";
	
    linked_list* test_list = new linked_list();

	test_list->Init(M,b); // initialize
	
    // test operations
	int testnums [] = {100, 5, 200, 7, 39, 25, 400, 50, 200, 300};
	int i = 0;
	
	/*testing 
	test_list->Insert(100,buf,50);
	test_list->PrintList();
	std::cout << "Above this is testing printList()" << std::endl;
	
	testing*/
	
    // some sample insertions
	for (i=0; i< 10; i ++)
	{
		std::cout<<"Sample Insert key "<<testnums[i]<<": Should succeed."<<std::endl;
		test_list->Insert(testnums [i], buf, 50);   // insert 50 bytes from the buffer as value for each of the insertions
	}
	
    // This Insert should fail
    //std::cout<<"Insert Key 150: Should Fail."<<std::endl; //testing 
    test_list->Insert (150, buf, 200);
	//std::cout<<"PrintList1"<<std::endl;					//testing 
	test_list->PrintList ();
	//std::cout<<"Delete Key 7"<<std::endl;				//testing 
	test_list->Delete (7);
	//std::cout<<"Insert Key 13. Will Fail until extra credit, deleted memory not reallocated until then"<<std::endl;				//testing 
	test_list->Insert (13, msg, strlen(msg)+1);		// insertion of strings, copies the null byte at the end
	//std::cout<<"Delete Key 55. Should Fail."<<std::endl;				//testing 
	test_list->Delete (55);
	//std::cout<<"Insert Key 15"<<std::endl;				//testing 
	test_list->Insert (15, "test msg", 8);
	//std::cout<<"Delete Key 3"<<std::endl;				//testing 
	test_list->Delete (3);
	//std::cout<<"PrintList2"<<std::endl;					//testing 
	test_list->PrintList ();
	
	// a sample lookup operations that should return null, because it is looking up a non-existent number
	std::cout<<"Lookup Key 3. Should fail."<<std::endl;
	node* kv = test_list->Lookup (3);
	if (kv)
    {
        std::cout << "Key = " << kv->key << ", Value Len = " << kv->value_len << std::endl;
    }
	
	// end test operations	
	//std::cout<<"Destroy"<<std::endl;
	test_list->Destroy ();
	
}
