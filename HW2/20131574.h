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

typedef struct _symbol{
	char symbol[8];
	int lc;
	struct _symbol *link;
}symbol;

typedef struct _lst{
	int lc;
	char label[7];
	char opcode[7];
	char operand[255];
	char oc[20];
	struct _lst *link;
}lst;

typedef struct _obj{
	char record[70];
	struct _obj *link;
}obj;


typedef struct _m_rc{
	int rc;
	struct _m_rc *link;
}m_rc;

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
char *search_format(hash **head, char *mnemonic);
void read_file(char *filename);
int check_symbol(char *label);
void add_symbol(char *label, int lc);
int search_symbol(char *label);
void add_lst(char *tmp1, char *tmp2, char *tmp3, char*tmp4, char *tmp5, int flag, int lc);
void add_obj(char *str);
void add_mrc(int rc);
void free_list();
void make_lst(char *filename, FILE *fp, hash **head);
void make_obj(char *filename);
void make_file(char *filename, hash **head);
