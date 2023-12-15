#include<stdio.h>
#include<string>
#include<vector>
#include<fstream>
#include <ios>
#include <ctime>
#include<string.h>
#include<map>
#include<list>
#include <sstream>
/////////////////////////////
#include<iostream>
#include<stdio.h>
///////////////////////////

using namespace std;

struct node
{
	int data;     	// Data
	node *prev;  	// A reference to the previous node
	node *next; 	// A reference to the next node
};

class Doubly_Linked_List
{
	int length;
	node *front;  	// points to first node of list
	node *end;   	// points to first las of list
	public:
	Doubly_Linked_List()
	{
		length=0;
		front = NULL;
		end = NULL;
	}
	

	void add_front(int );
	void add_after(node* , int );
	void add_before(node* , int );
	void add_end(int );
	void delete_node(node*);
	void forward_traverse();
	void backward_traverse();
	void clear();
	int empty();
	int size();
	
	
	node* getend();
	node* getfront();
	
};