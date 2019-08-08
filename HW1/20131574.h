#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>

//History linked list
typedef struct _node{
	struct _node *link;
	char name[256];
}node;

//Hash table linked list
typedef struct _hash{
	int opcode;
	char mnemonic[20];
	char format[20];
	struct _hash *link;
}hash;

/***************************************************************
*Function Name : add_note                                      *
*Function for making linked list                               *
*Input : char *str, node *head                                 *
*Output : None											       *	
***************************************************************/
void add_node(char *str, node *head);

/***************************************************************
*Function Name : hash_function                                 *
*Hash function for getting index                               *
*Input : char *mnemonic                                        *
*Output : int											       *	
***************************************************************/
int hash_function(char *mnemonic);

/***************************************************************
*Function Name : make_hash                                     *
*Hash function for making hash table                           *
*Input : hash **head                                           *
*Output : None											       *	
***************************************************************/
void make_hash(hash **head);

/***************************************************************
*Function Name : add_hash                                      *
*Hash function for making hash linked list                     *
*Input : hash **head, int opcode, char *mnemonic, char *format *
*Output : None											       *	
***************************************************************/
void add_hash(hash **head, int opcode, char *mnemonic, char *format);

/***************************************************************
*Functoin Name : search_opcode                                 *
*Hash function for searching opcode                            *
*Input : hash **head, char *mnemonic                           *
*Output : int											       *	
***************************************************************/
int search_opcode(hash **head, char *mnemonic);
