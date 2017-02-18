/* --------------------------------------------------------------------------- */
/* Developer: Andrew Kirfman, Margaret Baxter                                  */
/* Project: CSCE-313 Machine Problem #1                                        */
/*                                                                             */
/* File: ./MP1/linked_list2.cpp                                                */
/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Standard Library Includes                                                   */
/* --------------------------------------------------------------------------- */

#include <iostream>
#include <string.h>
#include <climits>
#include <iostream>
#include <cstring>
#include <math.h>
/* --------------------------------------------------------------------------- */
/* User Defined Includes                                                       */
/* --------------------------------------------------------------------------- */

#include "linked_list2.h"

linked_list2::linked_list2()
{
	
	setHeadPointer(NULL);		//Pointers set to Null 
	setFrontPointer(NULL);
	setFreePointer(NULL);
	setFreeDataPointer(NULL);

	setBlockSize(0);	//Integer values initialized to 0 rather than Null
	setMemSize(0);		// Based on TA's note on Piazza
	setMaxDataSize(0);
	
	setNumTiers(0);
	
	setInitialized(NULL);

}

void linked_list2::Init(int M, int b, int t)
{
	if (getInitialized()){return;}	//checks if Init() has been caled already, stops additional Init() calls from overwriting the linkedlist
	
	setBlockSize(b);
	setMemSize(M);
	setMaxDataSize(b - sizeof(node));
	setNumTiers(t);
	
	//Set first head pointer to front of first tier list.....right?
	setHeadPointer((char**)malloc(t*sizeof(char*)));			
	setFrontPointer((node**)malloc(t*sizeof(node*)));	
	setFreePointer((node**)malloc(t*sizeof(node*)));	
	setFreeDataPointer((node**)malloc(t*sizeof(node*)));

	for(int i = 0; i < t; i++){
		head_pointer[i] = (char*)malloc(M/t);
		front_pointer[i] = (node*)head_pointer[i];
		free_pointer[i] = (node*)head_pointer[i];
		free_data_pointer[i] = (node*)head_pointer[i];
		free_pointer[i]->key=-2;
		/*Testing comments */
	//	printf("I malloc()ed for the %d time\n", i );
	//	printf("I'm the head pointer:\t\t %p \n", head_pointer[i] );
	//	printf("I'm the front pointer:\t\t %p \n", front_pointer[i]);
	//	printf("I'm the free pointer:\t\t %p \n", free_pointer[i]);
	//	printf("I'm the free_data pointer:\t %p \n",free_data_pointer[i]);
		//node* iterator_node = head_pointer[i];
		//new loop to fill **dummy vars	
		node* node_iterator= (node*)head_pointer[i];
		node_iterator->key=-2;
		node_iterator->value_len =0;
		
		int num_blocks = M/(t*b); 
		for(int j = 0; j < num_blocks; j++){
			if(j == num_blocks - 1){					
				node_iterator->next=NULL;
			}
			else{
				node_iterator->key = -2;
				node_iterator->value_len=0;
				node_iterator->next = ((node*)(b+(char*)node_iterator)); //Commented Code Broken
			}
			node_iterator = node_iterator->next; //updates node_iterator to next locatio
		}	
	}
	
  setInitialized(true);
}


void linked_list2::Destroy()
{
	for(int i = 0; i < getNumTiers(); i++){
		free(head_pointer[i]);
	}
	free(free_pointer);
	free(free_data_pointer);
	free(front_pointer);
	
	initialized = false;
}

void linked_list2::Insert(int k,char * data_ptr, int data_len)
{
	std::cout<<"INSERT key: "<<k<<std::endl;
	
	int tierNum= Find_tier(k);
	
	//std::cout<<"Key: "<<k<<std::endl;
	
	if(data_len < 0){
		std::cout << "ERROR Insert key " << k << ": data_len:" << data_len << " < 0"<<std::endl;
		return;
	}
	else if(data_len > getBlockSize() - sizeof(node)){ 
		std::cout << "ERROR Insert key " << k << ": data_len:" << data_len << " > max value size"<<std::endl;
		return;
	}/*else if(free_pointer[tierNum]->key == 0){
		std::cout << "ERROR Insert key " << k <<": End of Tier reached: Max elements added to Tier"<<std::endl;
		return;
	}*/
	
	//std::cout<<"tierNum: "<<tierNum<<std::endl;
	
	node* temp = free_pointer[tierNum];
	
	while(temp->next != NULL){
		if(temp->key < 0){
		//	std::cout<<"     Insert IF 1 TRUE"<<std::endl; // We entered IF statement 1
			temp->key = k;
			temp->value_len = data_len;
			std::memcpy( (char*)temp+(sizeof(node)),
						data_ptr,
						(getBlockSize()-sizeof(node)) );
			temp = free_data_pointer[tierNum]->next;	//Testing
			//std::cout<<"     Insert Key "<<k<<" succeed!"<<std::endl; // Insert Succeeded.
			return;
		}
		else if(temp->next == NULL){
		//	std::cout<<"ERROR on Insert: End of List reached (2)"<<std::endl;
			return;
		}
		else if (temp == temp){
		//	std::cout<<"     Insert free_data_pointer move: ";	
			temp = temp->next;	
		//	std::cout<<"Succeed"<<std::endl;	
		}
	}
	
	
}

int linked_list2::Delete(int delete_key)
{
	
	node* delNode = Lookup(delete_key);
	if(delNode == NULL){
		std::cout<<"Error Delete key "<<delete_key<<": Key not found in List "<<std::endl;
		return 1;
	}
	
	int i = Find_tier(delete_key);
	
	if(delNode == front_pointer[i]){
		front_pointer[i] = front_pointer[i]->next;
		return 0;
	}
	else{
		node* prev = front_pointer[i];
		while(prev->next != delNode){
			prev = prev->next;
		}
		prev->next = delNode->next;
		return 0;
	}

}

node* linked_list2::Lookup(int lookup_key)
{
	
	int temp_tier = Find_tier(lookup_key); //returns where the key should be looked for
	int n = getMemSize()/getBlockSize()/getNumTiers();	//Changed
	
	node* temp_lookup = front_pointer[temp_tier];
	
	for(int i = 0; i < n ; i++){	//What is n here? A: Number of blocks
		if(temp_lookup == NULL){
			//std::cout<<"Lookup Failed: Not found in List: key "<<lookup_key<<std::endl;
			return NULL;
		}
		else if(temp_lookup->key == lookup_key){
			//std::cout<<"Key Found, returning temp." <<std::endl;
			return temp_lookup;
		}
		temp_lookup = temp_lookup->next;
	}
	

}

void linked_list2::PrintList()
{	
	for(int i = 0; i < getNumTiers(); i++){
		
		std::cout<<"Tier "<<i<<std::endl;
		
		node* temp = front_pointer[i];
		while(temp->next != NULL){
			if(temp->key != -2){
				std::cout<<"Node: "<<std::endl;
				std::cout << " - Key: " << temp->key << std::endl;
				std::cout << " - Data: " << (char*)((char*)temp+sizeof(node)) << std::endl;	//INCORRECT?? //loop through payload?
			}
			temp= temp->next; //updates free_pointer to next location
		}	
	}
	
	/* IMPORTANT NOTE!
	 * 
	 * In order for the script that will grade your assignment to work 
	 * (i.e. so you get a grade higher than a 0),
	 * you need to print out each member of the list using the format below.  
	 * Your print list function should be written as a while loop that prints 
	 * the tier of the list and then each node underneath it.  
	 * the following four lines exactly for each node and nothing else.  If
	 * you have any difficulties, talk to your TA and he will explain it further. 
	 * 
	 * The output lines that you should use are provided so that you will know
	 * exactly what you should output.  
	 */ 
	// Do this for every tier
	// std::cout << "Tier " << <TIER NUMBER GOESHERE> << std::endl;
	
	// Do this for every node 
	//std::cout << "Node: " << std::endl;
	//std::cout << " - Key: " << <KEY GOES HERE!> << std::endl;
	//std::cout << " - Data: " << <KEY GOES HERE!> << std::endl;
	
	/* Short example:
	 *   - Assume a list with two tiers each with two elements in each tier.  
	 *     the print out would appear as follows
	 * 
	 * Tier 0
	 * Node: 
	 *  - Key: 1
	 *  - Data: Hello
	 * Node:
	 *  - Key: 2
	 *  - Data: World!
	 * Tier 1
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

int linked_list2::Find_tier(int key) //hashing function, determines tier the key is supposed to be placed into
{
	//std::cout<<"Find_tier: key: "<<key<<std::endl;
	int a = (double)INT_MAX/(double)getNumTiers();
	int t = floor((double)key/a);
	std::cout<<"\t Tier: "<<t<<std::endl;
	return t;//			When compared to calling the getter here, and in
	
}

/* Getter Functions */
char** linked_list2::getHeadPointer()
{
	return head_pointer;
}

node** linked_list2::getFrontPointer()
{
	return front_pointer;
}

node** linked_list2::getFreePointer()
{
	return free_pointer;
}

node** linked_list2::getFreeDataPointer()
{
	return free_data_pointer;
}

int linked_list2::getBlockSize()
{
	return block_size;
}

int linked_list2::getMemSize()
{
	return mem_size;
}

int linked_list2::getMaxDataSize()
{
	return max_data_size; 
}

int linked_list2::getNumTiers()
{
	return num_tiers;
}

int linked_list2::getInitialized()
{
	return initialized;
}

/* Setter Functions */
void linked_list2::setHeadPointer(char** new_head_pointer)
{
	head_pointer = new_head_pointer;
}

void linked_list2::setFrontPointer(node** new_front_pointer)
{
	front_pointer = new_front_pointer;
}

void linked_list2::setFreePointer(node** new_free_pointer)
{
	free_pointer = new_free_pointer;
}

void linked_list2::setFreeDataPointer(node** new_free_data_pointer)
{
	free_data_pointer=new_free_data_pointer;
}

void linked_list2::setBlockSize(int new_block_size)
{
	block_size = new_block_size;
}

void linked_list2::setMemSize(int new_mem_size)
{
	mem_size = new_mem_size;
}

void linked_list2::setMaxDataSize(int new_max_data_size)
{
	max_data_size = new_max_data_size;
}

void linked_list2::setNumTiers(int new_num_tiers)
{
    num_tiers = new_num_tiers;
}

void linked_list2::setInitialized(bool new_initialized)
{
	initialized = new_initialized;
}
