/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/linked_list.cpp                                                 */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include "linked_list.h"
#include <iostream>
#include <string>
#include <climits>
#include <cstring>

/* Constructor */
linked_list::linked_list()
{
	setHeadPointer(NULL);		//Pointers set to Null 
	setFrontPointer(NULL);
	setFreePointer(NULL);
	setFreeDataPointer(NULL);

	setBlockSize(0);	//Integer values initialized to 0 rather than Null
	setMemSize(0);		// Based on TA's note on Piazza
	setMaxDataSize(0);
	setInitialized(NULL);

}


void linked_list::Init(int M, int b)
{
	if (getInitialized()){return;}	//checks if Init() has been caled already, stops additional Init() calls from overwriting the linkedlist
	
	setBlockSize(b);
	setMemSize(M);
	setMaxDataSize(b - sizeof(node)); //might be setMaxDataSize(M/b)?
	
	setHeadPointer((char*)malloc(M));			//Pointer always pointing to the head of Memory
	setFrontPointer((node*)getHeadPointer());	//Pointer always pointing to the head of the first node in the list
	setFreePointer((node*)getHeadPointer());	//Pointer always pointing to the head of the last initialized node in the list
	setFreeDataPointer((node*)getHeadPointer());//Pointer always pointing to the head of the next available(unitialized) node for insertion or Null if memory is full

	node* temp = getFreePointer();

	for(int i = 0; i < M/b; i++){			//initializes all nodes of the list to value_len 0 
		if(i == M/b - 1){					
			temp->next=NULL;
		}
		else{
			temp->key = -2;
			temp->value_len=0;
			temp->next = (node*)(b+(char*)temp); //Commented Code Broken
		}
		temp = temp->next; //updates temp to next location
	}
	//setFreePointer((node*)getHeadPointer());
	setInitialized(true);
	
}

void linked_list::Destroy()
{
	free(getHeadPointer());
} 

/* Insert an element into the list with a given key, given data element, and with a given length*/
void linked_list::Insert (int k, char * data_ptr, int data_len)
{
	if(data_len < 0){
		std::cout << "ERROR Insert key " << k << ": data_len:" << data_len << " < 0"<<std::endl;
		return;
	}
	else if(data_len > getBlockSize() - sizeof(node)){ 
		std::cout << "ERROR Insert key " << k << ": data_len:" << data_len << " > max value size"<<std::endl;
		return;
	}
	else if((getFreePointer()->next) == NULL){
		std::cout << "ERROR Insert key " << k <<": End of List reached: Max elements added to List"<<std::endl;
		return;
	}
	
	setFreeDataPointer(getFreePointer()); 
	
	while(true){
	//	std::cout<<"     Insert loop rep"<<std::endl; //Seg faulting HERE on the second insert at the moment.
		//Lets me know when we started a rep of the while loop
		if(getFreeDataPointer()->key < 0){
	//		std::cout<<"     Insert IF 1 TRUE"<<std::endl; // We entered IF statement 1
			getFreeDataPointer()->key = k;
			getFreeDataPointer()->value_len = data_len;
			std::memcpy( (char*)getFreeDataPointer()+(sizeof(node)),
						data_ptr,
						(getBlockSize()-sizeof(node)) );
			setFreePointer(getFreeDataPointer()->next);	//Testing
			//std::cout<<"     Insert Key "<<k<<" succeed!"<<std::endl; // Insert Succeeded.
			return;
		}
		else if(getFreeDataPointer()->next = NULL){
			std::cout<<"ERROR on Insert: End of List reached (2)"<<std::endl;
			return;
		}
		else if (getFreeDataPointer() == getFreePointer()){
		//	std::cout<<"     Insert free_data_pointer move: ";	
			setFreeDataPointer(getFreeDataPointer()->next);	
		//	std::cout<<"Succeed"<<std::endl;					
		}
	}

}


int linked_list::Delete(int delete_key)
{
	// call lookup
	// if left and right exist, connect their pointers
	// delete node //No need to delete node, just pointer swap.
	//extra credit->add "deleted node" location to a list

	//Making elementary implementation.
	//std::cout<<"     DELETE started..."<<std::endl;
	
	node* temp = Lookup(delete_key);	//Lookup returning null?
	if(temp == NULL){
		std::cout<<"Error Delete key "<<delete_key<<": Key not found in List "<<std::endl;
		return 1;
	}
	//implement augmented insert on xtralist
	node* temp_prev = getFrontPointer();// It must start at front pointer to find temp.
	if(temp == getFrontPointer()){
		setFrontPointer(getFrontPointer()->next);
	}
	//std::cout<<"     temp pointers allocated"<<std::endl;
	while(temp_prev->next != temp){
		temp_prev = temp_prev->next;
	}
	//std::cout<<"     temp_prev found location"<<std::endl;
	temp_prev->next = temp->next;
	//std::cout<<"	DELETE SUCCEED."<<std::endl;
	return 0;
}

/* Iterate through the list, if a given key exists, return the pointer to it's node */
/* otherwise, return NULL                                                           */
struct node* linked_list::Lookup(int lookup_key)
{
	//std::cout<<"	LOOKUP finding key "<<lookup_key<<std::endl;
	node* temp = getFrontPointer();
	int n = getMemSize()/getBlockSize();
	for(int i = 0; i < n; i++){			//iterates list
		if(temp == NULL){	//should this be temp->key==NULL
			std::cout<<"Lookup 'failed': Not found in List: key "<<lookup_key<<std::endl;
			return NULL;
		}
		else if(temp->key == lookup_key){
			//std::cout<<"	key found. Returning temp."<<std::endl;
			return temp; //need to continue interation until key of -2 or null is found and freepointer reset to that
		}
		
		//std::cout<<"	temp is iterated."<<std::endl;
		temp = temp->next; //feeds next freepointer into loop
	}
	
}

/* Prints the list by printing the key and the data of each node */
void linked_list::PrintList()
{
	node* printTemp = getFrontPointer();
	while(printTemp->next > 0){
		std::cout << "Node: " << std::endl;
		std::cout << " - Key: " << printTemp->key << std::endl;
		std::cout << " - Data: " << printTemp->value_len << std::endl;
		printTemp = printTemp->next; //gets next pointer
	}
	
	/* IMPORTANT NOTE!
	 * 
	 * In order for the script that will grade your assignment to work 
	 * (i.e. so you get a grade higher than a 0),
	 * you need to print out each member of the list using the format below.  
	 * Your print list function should be written as a while loop that prints
	 * the following three lines exactly for each node and nothing else.  If
	 * you have any difficulties, talk to your TA and he will explain it further.  
	 * 
	 * The output lines that you should use are provided so that you will know
	 * exactly what you should output.  
	 */ 
	//std::cout << "Node: " << std::std::endl;
	//std::cout << " - Key: " << <KEY GOES HERE!> << std::std::endl;
	//std::cout << " - Data: " << <KEY GOES HERE!> << std::std::endl;
	
	/* Short example:
	 *   - Assume that you have a list with 4 elements.  
	 *     Your output should appear as follows
	 * 
	 * Node:
	 *  - Key: 1
	 *  - Data: Hello
	 * Node:
	 *  - Key: 2
	 *  - Data: World!
	 * Node:
	 *  - Key: 3
	 *  - Data: Hello
	 * Node:
	 *  - Key: 4
	 *  - Data: World!
	 * 
	 * ^^ Your output needs to exactly match this model to be counted as correct.  
	 * (With the exception that the values for key and data will be different 
	 * depending on what insertions you perform into your list.  The values provided
	 * here are for pedagogical purposes only)
	 */
}

/* Getter Functions */
char* linked_list::getHeadPointer()
{
	return head_pointer;
}

node* linked_list::getFrontPointer()
{
	return front_pointer;
}

node* linked_list::getFreePointer()
{
	return free_pointer;
}

node* linked_list::getFreeDataPointer()
{
	return free_data_pointer;
}

int linked_list::getBlockSize()
{
	return block_size;
}

int linked_list::getMemSize()
{
	return mem_size;
}

int linked_list::getMaxDataSize()
{
	return max_data_size;
}

bool linked_list::getInitialized()
{
	return initialized; 
}

/* Setter Functions */
void linked_list::setHeadPointer(char *new_pointer)
{
	head_pointer = new_pointer;
}

void linked_list::setFrontPointer(node* new_pointer)
{
	front_pointer = new_pointer;
}

void linked_list::setFreePointer(node* new_pointer)
{
	free_pointer = new_pointer;
}

void linked_list::setFreeDataPointer(node* new_pointer)
{
	free_data_pointer = new_pointer;
}

void linked_list::setBlockSize(int new_block_size)
{
	block_size = new_block_size;
}

void linked_list::setMemSize(int new_mem_size)
{
	mem_size = new_mem_size;
}

void linked_list::setMaxDataSize(int new_max_data_size)
{
	max_data_size = new_max_data_size; 
}

void linked_list::setInitialized(bool new_initialized)
{
	initialized = new_initialized;
}
