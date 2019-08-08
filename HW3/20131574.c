#include "20131574.h"

symbol *sym_head=NULL;
lst *lst_head=NULL;
obj *obj_head=NULL;
m_rc *mrc_head=NULL;
bp *bp_head = NULL;
estab tab[30];
int error_flag = 0;
int load_addr = 0;
int tot_addr = 0;
char obj_nm[4][255];

static int reg[10]={0};
#define A reg[0]
#define X reg[1]
#define L reg[2]
#define B reg[3]
#define S reg[4]
#define T reg[5]
#define PC reg[8]
#define SW reg[9]

int main()
{
	int var1,var2,var3,cnt;
	int addr,temp_addr=0x0,opcode;
	int i,j,flag=1,flag2=1,flag3=1;
	unsigned char memory[65536][16]={0};
	char buf_in[255],file_name[255],buf_dump[255], ascii[16],buf_op[255], buf_op2[255], buf_op3[255], buf_op4[255];
	DIR *dp;
	struct dirent *dirp;
	struct stat file_info;
	mode_t file_mode;
	node *head = (node*)malloc(sizeof(node));
	node *p;
	hash *hash_head[20];
	hash *h_p;
	symbol *s_p;

	make_hash(hash_head);

	while(1)
	{
		var1=var2=var3=-1;
		strcpy(buf_dump,"");
		strcpy(buf_op,"");
		strcpy(buf_op2,"");
		strcpy(buf_op3,"");
		strcpy(buf_op4,"");
		for(i=0; i<4;i++)
			strcpy(obj_nm[i], "");

		printf("sicsim> ");
		fgets(buf_in, 255, stdin);
		*(buf_in+strlen(buf_in)-1) = '\0';
		sscanf(buf_in, "%s %x,%x,%x", buf_dump, &var1, &var2, &var3);
		sscanf(buf_in, "%s %s %s %s %s", buf_dump, buf_op, buf_op2, buf_op3, buf_op4);
		
		if(!strcmp(buf_dump,"h") || !strcmp(buf_dump,"help")) // help instruction
		{
			printf("h[elp]\n");
			printf("d[ir]\n");
			printf("q[uit]\n");
			printf("hi[story]\n");
			printf("du[mp] [start, end]\n");
			printf("e[dit] address, value\n");
			printf("f[ill] start, end, value\n");
			printf("reset\n");
			printf("opcode mnemonic\n");
			printf("opcodelist\n");
			printf("assemble filename\n");
			printf("type filename\n");
			printf("symbol\n");
			printf("progaddr [load_address]\n");
			printf("loader [filename1] [filename2] [filename3]\n");
			printf("bp [clear or address or not(printf bp)]\n");
			printf("run\n");

			add_node(buf_in, head);
		}

		else if(!strcmp(buf_dump,"d") || !strcmp(buf_dump,"dir")) // dir instruction
		{
			dp = opendir(".");
			if(dp != NULL)
			{

				while((dirp = readdir(dp)))
				{
					strcpy(file_name,dirp->d_name); // file name
					printf("%s", file_name);
					if(stat(file_name,&file_info) == -1)
					{
						printf("file_info error!\n");
						return 0;
					}
					file_mode = file_info.st_mode;

					if(S_ISDIR(file_mode))
						printf("/\t");
					else if(file_mode & S_IXUSR)
						printf("*\t");
					else
						printf("\t");
				}
			}
			closedir(dp);
			printf("\n");

			add_node(buf_in, head);
		}

		else if(!strcmp(buf_dump,"q") || !strcmp(buf_dump,"quit")) // quit instruction
			return 0;

		else if(!strcmp(buf_dump,"hi") || !strcmp(buf_dump,"history")) // history instruction
		{

			add_node(buf_in, head);

			i = 0;
			p = head->link;
			while(p != NULL)
			{
				printf("%d    %s\n", ++i, p->name);
				p = p->link;
			}
		}

		else if(!strcmp(buf_dump,"du") || !strcmp(buf_dump,"dump")) // dump instruction
		{
			cnt=var1%16;
			if(var3 != -1)
			{
				printf("Error!\n");
				continue;
			}
			if(var1 > var2 && var2 != -1){
				printf("Error!\n");
				continue;
			}
			if(var1 > 0xFFFFF || var2 > 0xFFFFF)
			{
				printf("Error!\n");
				continue;
			}
			if(var1 == -1)
				addr = temp_addr;
			else
				addr = var1;
			
			if(var2 == -1) // dump or dump *
			{
				for(i=0; i<10; i++)
				{
					if(flag==1) // 1st line
					{
						printf("%05X ", addr - addr%16);
						for(j=0; j<addr%16; j++)
						{
							ascii[j] = '.';
							printf("   ");
						}
						for(j=addr%16; j<16; j++)
						{
							if(126 < memory[addr/16][j] || memory[addr/16][j] < 32)
								ascii[j] = '.';
							else
								ascii[j] = memory[addr/16][j];
							printf("%02X ", memory[addr/16][j%16]);
						}

						printf("; %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c\n", ascii[0], ascii[1], ascii[2], ascii[3], ascii[4], ascii[5], ascii[6], ascii[7], ascii[8], ascii[9], ascii[10], ascii[11], ascii[12], ascii[13], ascii[14], ascii[15]);
						flag=0;
					}

					else // 2nd ~ 10th line
					{
						for(j=0; j<16; j++)
						{
							if(126 < memory[addr/16][j] || memory[addr/16][j] < 32)
								ascii[j] = '.';
							else
								ascii[j] = memory[addr/16][j];
						}
				
					printf("%05X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X ; %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c\n",addr - addr%16, memory[addr/16][0], memory[addr/16][1], memory[addr/16][2], memory[addr/16][3], memory[addr/16][4], memory[addr/16][5], memory[addr/16][6], memory[addr/16][7], memory[addr/16][8], memory[addr/16][9], memory[addr/16][10], memory[addr/16][11], memory[addr/16][12], memory[addr/16][13], memory[addr/16][14], memory[addr/16][15], ascii[0], ascii[1], ascii[2], ascii[3], ascii[4], ascii[5], ascii[6], ascii[7], ascii[8], ascii[9], ascii[10], ascii[11], ascii[12], ascii[13], ascii[14], ascii[15]);
					}
					addr +=16;
					if(addr > 0xFFFFF)
					{
						flag=1;
						flag2=0;
						temp_addr = 0x0;
						break;
					}
					temp_addr = addr;
				}
				if(flag2==0)
				{
					flag=1;
					flag2=1;
					flag3=1;
					continue;
				}

				if(addr%16 != 0)
				{

					printf("%05X ", addr - addr%16);
					for(j=0; j<addr%16; j++)
					{
						if(126 < memory[addr/16][j] || memory[addr/16][j] < 32)
							ascii[j] = '.';
						else
							ascii[j] = memory[addr/16][j];
						printf("%02X ", memory[addr/16][j%16]);
					}
					for(j=addr%16; j<16; j++)
					{
						ascii[j] = '.';
						printf("   ");
					}

					printf("; %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c\n", ascii[0], ascii[1], ascii[2], ascii[3], ascii[4], ascii[5], ascii[6], ascii[7], ascii[8], ascii[9], ascii[10], ascii[11], ascii[12], ascii[13], ascii[14], ascii[15]);
				}
				flag=1;
				flag2=1;
				flag3=1;
			}

			else // dump *,*
			{
				for(i=var1; i <= var2; i++)
				{
					if(flag == 1) // 1st line
					{
						printf("%05X ", addr - addr%16);
						for(j=0; j < var1%16; j++)
						{
							printf("   ");
							ascii[j] = '.';
						}
						for(j=var1%16; j < 16; j++)
						{
							if(126 < memory[addr/16][j] || memory[addr/16][j] < 32)
								ascii[j] = '.';
							else
								ascii[j] = memory[addr/16][j];
						}
						flag = 0;
					}
					
					printf("%02X ", memory[i/16][i%16]);
					cnt++;

					if(cnt==16)
					{
						if(flag3 == 1)
						{
							flag3=0;
						}
						else
						{
							for(j=0; j < 16; j++)
							{

								if(126 < memory[addr/16][j] || memory[addr/16][j] < 32)
								{
									ascii[j] = '.';
								}
								else
									ascii[j] = memory[addr/16][j];
							}
						}
						cnt=0;
						addr+=16;
						printf("; %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c\n", ascii[0], ascii[1], ascii[2], ascii[3], ascii[4], ascii[5], ascii[6], ascii[7], ascii[8], ascii[9], ascii[10], ascii[11], ascii[12], ascii[13], ascii[14], ascii[15]);
						if(addr > 0xFFFFF || addr > var2)
						{
							flag=1;
							flag2=0;
							temp_addr=0x0;
							break;
						}
						printf("%05X ", addr - addr%16);
					}
				}
				if(flag2==0)
				{
					flag=1;
					flag2=1;
					flag3=1;
					continue;
				}

				for(j=15; j > var2%16; j--) // last line
					printf("   ");

				for(j=0; j <=var2%16; j++)
				{
					if(126 < memory[addr/16][j] || memory[addr/16][j] < 32)
						ascii[j] = '.';
					else
						ascii[j] = memory[addr/16][j];
				}
				for(j=var2%16+1; j < 16; j++)
				{
					ascii[j] = '.';

				}
				if(var2%16 != 15)
					printf("; %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c\n", ascii[0], ascii[1], ascii[2], ascii[3], ascii[4], ascii[5], ascii[6], ascii[7], ascii[8], ascii[9], ascii[10], ascii[11], ascii[12], ascii[13], ascii[14], ascii[15]);
				flag=1;
				flag2=1;
				flag3=1;

				temp_addr = addr - addr%16 + var2%16 + 1;
			}

			add_node(buf_in, head);
		}

		else if(!strcmp(buf_dump,"e") || !strcmp(buf_dump,"edit")) // edit instruction
		{
			if(0 > var2 || var2 > 0xFF)
			{
				printf("Error!\n");
				continue;
			}
			if(var2 == -1)
			{
				printf("Error!\n");
				continue;
			}
			if(var1 > 0xFFFFF)
			{
				printf("Error!\n");
				continue;
			}
			memory[var1/16][var1%16] = var2;
			add_node(buf_in, head);
		}

		else if(!strcmp(buf_dump,"f") || !strcmp(buf_dump,"fill")) // fill instruction
		{
			if(var3 == -1)
			{
				printf("Error!\n");
				continue;
			}
			if(var1 > var2)
			{
				printf("Error!\n");
				continue;
			}
			if(var1 > 0xFFFFF || var2 > 0xFFFFF)
			{
				printf("Error!\n");
				continue;
			}
			for(i=var1; i<=var2; i++)
				memory[i/16][i%16]=var3;

			add_node(buf_in, head);
		}

		else if(!strcmp(buf_dump,"reset")) // reset instruction
		{
			for(i=0; i<65536; i++)
				for(j=0; j<16; j++)
					memory[i][j] = '\0';

			add_node(buf_in, head);
		}

		else if(!strcmp(buf_dump, "opcode")) // opcode instruction
		{
			opcode = search_opcode(hash_head,buf_op);
			if(opcode == -1)
			{
				printf("There is not opcode of %s.\n", buf_op);
				continue;
			}
			printf("opcode is %X\n", search_opcode(hash_head,buf_op));

			add_node(buf_in, head);
		}

		else if(!strcmp(buf_in, "opcodelist")) // opcodelist instruction
		{
			for(i=0; i<20; i++)
			{
				h_p = hash_head[i];
				printf("%d : ", i);
				
				while(h_p != NULL)
				{
					if(h_p->link == NULL)
						printf("[%s,%X] ", h_p->mnemonic, h_p->opcode);
					else
						printf("[%s,%X] -> ", h_p->mnemonic, h_p->opcode);
					h_p = h_p->link;
				}

				printf("\n");
			}

			add_node(buf_in, head);
		}
		else if(!strcmp(buf_dump,"type"))
		{
			read_file(buf_op);
			add_node(buf_in, head);
		}
		else if(!strcmp(buf_dump, "assemble"))
		{
			make_file(buf_op,hash_head);
			add_node(buf_in, head);
		}
		else if(!strcmp(buf_dump, "symbol"))
		{
			add_node(buf_in, head);
			s_p = sym_head;

			while(s_p != NULL)
			{
				printf("\t%s\t%04X\n",s_p->symbol, s_p->lc);
				s_p = s_p -> link;
			}
		}
		else if(!strcmp(buf_dump, "progaddr"))
		{
			if(var1 < 0)
			{
				printf("Program address error.\n");
				continue;
			}

			load_addr = var1;
			add_node(buf_in, head);
		}
		else if(!strcmp(buf_dump, "loader"))
		{
			if(strcmp(buf_op4, ""))
			{
				printf("Error.The numoer of file is more than three.\n");
				continue;
			}
			strcpy(obj_nm[0], buf_op);
			strcpy(obj_nm[1], buf_op2);
			strcpy(obj_nm[2], buf_op3);

			load(memory);
			add_node(buf_in, head);
		}
		else if(!strcmp(buf_dump, "bp"))
		{
			if(!strcmp(buf_op,"clear"))
			{
				clear_bp();
			}
			else if(!strcmp(buf_op, ""))
			{
				print_bp();
			}
			else if(var1 >= 0)
			{
				add_bp(var1);
			}
			else
			{
				printf("bp error\n");
				continue;
			}

			add_node(buf_in, head);
		}
		else if(!strcmp(buf_dump, "run"))
		{
			run(memory, hash_head);
			add_node(buf_in, head);
		}
		else
		{
			printf("Error!\n");
		}
	}

	free(head);
	free(hash_head);
	free(sym_head);
	free(lst_head);
	free(obj_head);
	free(mrc_head);
	free(bp_head);

	return 0;
}

void add_node(char *str, node *head)
{
	node *p, *temp;

	p = head;

	temp = (node*)malloc(sizeof(node));
	temp->link = NULL;
	strcpy(temp->name, str);

	while(p->link != NULL)
		p = p->link;

	p->link = temp;
}

int hash_function(char *mnemonic)
{
	int i;
	int sum=0;
	
	for(i=0; i<strlen(mnemonic); i++)
		sum += mnemonic[i];

	return sum%20;
}

void make_hash(hash **head)
{
	FILE *fp = fopen("opcode.txt","r");
	int opcode;
	char mnemonic[20], format[20];
	
	if(fp == NULL)
		printf("File does not exist\n");
	else
	{
		while(fscanf(fp,"%x %s %s", &opcode,mnemonic,format) != EOF)
		{
			add_hash(head,opcode,mnemonic,format);
		}

		fclose(fp);
	}
}

void add_hash(hash **head, int opcode, char *mnemonic, char *format)
{
	int index;
	hash *p, *temp;
	index = hash_function(mnemonic);
	p = head[index];
	
	temp = (hash*)malloc(sizeof(hash));
	temp->link = NULL;
	temp->opcode = opcode;
	strcpy(temp->mnemonic, mnemonic);
	strcpy(temp->format, format);

	if(p == NULL)
		head[index] = temp;
	else
	{
		while(p->link != NULL)
			p = p->link;

		p->link = temp;
	}

}

int search_opcode(hash **head, char *mnemonic)
{
	int index, result=-1;
	hash *p;

	index = hash_function(mnemonic);
	p = head[index];
	while(p != NULL)
	{
		if(!strcmp(p->mnemonic, mnemonic))
		{
			result = p->opcode;
			return result;
		}
		p = p->link;
	}

	return result;
}

char *search_format(hash **head, char *mnemonic)
{
	int index;
	char *result=NULL;
	hash *p;

	index = hash_function(mnemonic);
	p = head[index];
	while(p != NULL)
	{
		if(!strcmp(p->mnemonic, mnemonic))
		{
			result = p->format;
			return result;
		}
		p = p->link;
	}
	return result;
}

void read_file(char *filename)
{
	FILE *fp = fopen(filename,"r");
	int chk=1;

	if(fp == NULL)
		printf("File does not exist\n");
	else
	{
		while(1)
		{
			if(chk == EOF)
				break;
			else
				printf("%c", chk);
			chk = fgetc(fp);
		}
		fclose(fp);
	}
}

int check_symbol(char *label)
{
	symbol *p = sym_head;

	while(p != NULL)
	{
		if(!strcmp(p->symbol, label))
			return 1;
		p = p->link;
	}
	return 0;
}

void add_symbol(char *label, int lc)
{
	symbol *p = sym_head;
	symbol *temp;

	temp = (symbol*)malloc(sizeof(symbol));
	temp->link = NULL;

	strcpy(temp->symbol, label);
	temp->lc = lc;
	
	if(p == NULL)
		sym_head = temp;
	else
	{
		if(strcmp(p->symbol, label) < 0)
		{
			temp->link = p;
			sym_head=temp;
			return ;
		}
		
		while(p->link != NULL)
		{
			if(strcmp(p->link->symbol, label) < 0)
			{
				temp->link = p->link;
				p->link = temp;
				return ;
			}
			p = p->link;
		}	
		p->link = temp;
	}
}

int search_symbol(char *label)
{
	symbol *p = sym_head;
	
	while(p != NULL)
	{
		if(!strcmp(p->symbol,label))
		{
			return p->lc;
		}
		p = p->link;
	}
	return -1;
}

void add_lst(char *tmp1, char *tmp2, char *tmp3, char *tmp4, char *tmp5, int flag, int lc)
{
	lst *p = lst_head;
	lst *temp;

	temp = (lst*)malloc(sizeof(lst));
	temp->link = NULL;
	temp->label[0] = '\0';

	if(flag == 0)
	{
		temp->lc = lc;
		strcpy(temp->label, tmp1);
		strcpy(temp->opcode, tmp2);
		strcpy(temp->operand, tmp3);
	}

	else if(flag == 1)
	{
		temp->lc = lc;
		strcpy(temp->opcode, tmp1);
		strcpy(temp->operand, tmp2);
	}

	else if(flag == 2)
	{
		temp->lc = lc;
		strcpy(temp->label, tmp1);
		strcpy(temp->opcode, tmp2);
		strcpy(temp->operand, tmp3);
		strcat(temp->operand, ",");
		strcat(temp->operand, tmp4);
	}

	else if(flag == 3)
	{
		temp->lc = lc;
		strcpy(temp->opcode, tmp1);
		strcpy(temp->operand, tmp2);
		strcat(temp->operand, ",");
		strcat(temp->operand, tmp3);
	}

	else if(flag == 9)
	{
		strcpy(temp->operand, tmp1);
	}

	if(p == NULL)
		lst_head = temp;
	
	else
	{
		while(p->link != NULL)
			p = p->link;

		p->link = temp;
	}
}

void add_obj(char *str)
{
	obj *p = obj_head;
	obj *temp;

	temp = (obj*)malloc(sizeof(obj));

	strcpy(temp->record,str);
	temp->link = NULL;

	if(p == NULL)
		obj_head = temp;
	
	else
	{
		while(p->link != NULL)
			p = p->link;

		p->link = temp;
	}
}

void add_mrc(int rc)
{
	m_rc *p = mrc_head;
	m_rc *temp;

	temp = (m_rc*)malloc(sizeof(m_rc));
	temp->rc= rc;
	temp->link = NULL;

	if(p == NULL)
		mrc_head = temp;
	
	else
	{
		while(p->link != NULL)
			p = p->link;

		p->link = temp;
	}
}

void free_list()
{
	symbol *p1 = sym_head;
	symbol *prev1;
		
	lst *p2 = lst_head;
	lst *prev2;

	obj *p3 = obj_head;
	obj *prev3;

	m_rc *p4 = mrc_head;
	m_rc *prev4;

	while(p1 != NULL)
	{
		prev1 = p1;
		p1 = p1->link;
		free(prev1);
	}
	sym_head = NULL;

	while(p2 != NULL)
	{
		prev2 = p2;
		p2 = p2->link;
		free(prev2);
	}
	lst_head = NULL;

	while(p3 != NULL)
	{
		prev3 = p3;
		p3 = p3->link;
		free(prev3);
	}
	obj_head = NULL;

	while(p4 != NULL)
	{
		prev4 = p4;
		p4 = p4->link;
		free(prev4);
	}
	mrc_head = NULL;
}
void make_lst(char *filename, FILE *fp, hash **head)
{
	char str[255];
	char tmp1[255], tmp2[255], tmp3[255], *tmp4, *tmp5, *tmp6;
	unsigned int lc;
	int i,format=0,k1,k2,cnt,tmp7;
	lst *p;

	fgets(str, sizeof(str), fp);
	sscanf(str,"%s %s %s", tmp1, tmp2, tmp3);
	if(!strcmp(tmp2,"START"))
	{
		sscanf(str,"%s %s %x", tmp1, tmp2, &lc);
	}
	else
	{
		lc = 0;
	}
	
	add_lst(tmp1,tmp2,tmp3,NULL,NULL,0,lc);
	cnt = 0;
	while(1)
	{
		tmp7=0;
		cnt+=5;
		format=0;
		memset(tmp1, 0, 255);
		memset(tmp2, 0, 255);
		memset(tmp3, 0, 255);
		fgets(str, sizeof(str), fp);

		sscanf(str,"%s %s %s", tmp1, tmp2, tmp3);
		
		if(!strcmp(tmp2,"BYTE"))
		{
			for(i=14;i<strlen(str);i++)
				tmp3[tmp7++] = str[i];
			tmp3[tmp7-1]='\0';
		}
		if(tmp1[0] == '.') // Comment
		{
			str[strlen(str)-1]='\0';
			add_lst(str,NULL,NULL,NULL,NULL,9,lc);
			continue;
		}

		if(tmp1[0] == '+')
		{
			k1=search_opcode(head,tmp1+1);
		}
		else if(strcmp(tmp1,"BASE"))
		{
			k1=search_opcode(head,tmp1);
		}
		else if(!strcmp(tmp1,"BASE"))
		{
			k1=1;
		}
		if(tmp2[0] == '+')
		{
			k2=search_opcode(head,tmp2+1);
		}
		else if(strcmp(tmp2,"BASE"))
		{
			k2=search_opcode(head,tmp2);
		}
		else if(!strcmp(tmp2,"BASE"))
		{
			k2=1;
		}
		if(!strcmp(tmp1,"END"))
		{
			add_lst(tmp1,tmp2,NULL,NULL,NULL,1,lc);
			break;
		}
		else if(k1 != -1 || k2 != -1)
//		if(search_opcode(head,tmp1) != -1 || search_opcode(head,tmp2) != -1)
		{
			if(strchr(str, ','))
			{	
				tmp4 = strtok(str, ",");
				tmp5 = strtok(NULL, ",");
				memset(tmp3, 0, sizeof(char) * 255);
				sscanf(tmp4,"%s %s %s",tmp1, tmp2, tmp3);
				tmp5[strlen(tmp5)-1] = '\0';

				if(!strcmp(tmp3,""))
				{
					tmp6 = search_format(head, tmp1);
					
					if(tmp6 == NULL)
						format = 0;
					else if(strlen(tmp6) == 3)
						format = 3;
					else
						format = tmp6[0] - '0';

					add_lst(tmp1,tmp2,tmp5,NULL,NULL,3,lc);
					if(tmp1[0] == '+')
						lc+=4;
					else
						if(strcmp(tmp1,"BASE"))	
							lc+=format;
				}
				else
				{
					if(check_symbol(tmp1))
					{
						printf("%d line symbol error!\n", lc);
						break;
					}
					tmp6 = search_format(head, tmp2);
					
					if(tmp6 == NULL)
						format = 0;
					else if(strlen(tmp6) == 3)
						format = 3;
					else
						format = tmp6[0] - '0';
					
					add_lst(tmp1,tmp2,tmp3,tmp5,NULL,2,lc);
					add_symbol(tmp1,lc);
					if(tmp2[0] == '+')
						lc+=4;
					else
						if(strcmp(tmp2,"BASE"))
							lc+=format;
				}
			}

			else
			{
				if(!strcmp(tmp3,"")) // 2개
				{
					tmp6 = search_format(head, tmp1);

					if(tmp6 == NULL)
						format = 0;
					else if(strlen(tmp6) == 3)
						format = 3;
					else
						format = tmp6[0] - '0';

					add_lst(tmp1,tmp2,NULL,NULL,NULL,1,lc);
					
					if(tmp1[0] == '+')
						lc+=4;
					else
						if(strcmp(tmp1,"BASE"))	
							lc+=format;
				}
				else // 3개
				{
					if(check_symbol(tmp1))
					{
						printf("%d line symbol error!\n", lc);
						error_flag=1;
						return;
					}
					tmp6 = search_format(head, tmp2);

					if(tmp6 == NULL)
						format = 0;
					else if(strlen(tmp6) == 3)
						format = 3;
					else
						format = tmp6[0] - '0';

					add_lst(tmp1,tmp2,tmp3,NULL,NULL,0,lc);
					add_symbol(tmp1,lc);
					if(tmp2[0] == '+')
						lc+=4;
					else
						if(strcmp(tmp2,"BASE"))
							lc+=format;
				}
			}
		}
		else if(!strcmp(tmp2,"WORD"))
		{
			if(check_symbol(tmp1))
			{
				printf("%d line symbol error!\n", lc);
				break;
			}

			add_lst(tmp1,tmp2,tmp3,NULL,NULL,0,lc);
			add_symbol(tmp1,lc);
			lc+=3;
		}
		else if(!strcmp(tmp2,"RESW"))
		{
			if(check_symbol(tmp1))
			{
				printf("%d line symbol error!\n", lc);
				error_flag = 1;
				return;
			}

			add_lst(tmp1,tmp2,tmp3,NULL,NULL,0,lc);
			add_symbol(tmp1,lc);
			lc+=3 * strlen(tmp3);
		}
		else if(!strcmp(tmp2,"RESB"))
		{
			if(check_symbol(tmp1))
			{
				printf("%d line symbol error!\n", lc);
				error_flag = 1;
				return;
			}

			int j=1;
			for(i=strlen(tmp3)-1; i>=0; i--)
			{
				format = format + (tmp3[i] -'0') *j;
				j*=10;	
			}
			add_lst(tmp1,tmp2,tmp3,NULL,NULL,0,lc);
			add_symbol(tmp1,lc);
			lc+=format;
		}
		else if(!strcmp(tmp2,"BYTE"))
		{
			if(check_symbol(tmp1))
			{
				printf("%d line symbol error!\n", lc);
				error_flag = 1;
				return;
			}

			if(tmp3[0] == 'C')
				format = strlen(tmp3) - 3;
			else if(tmp3[0] == 'X')			
				format = (strlen(tmp3) - 3) / 2;
			
			add_lst(tmp1,tmp2,tmp3,NULL,NULL,0,lc);
			add_symbol(tmp1,lc);
			lc+=format;
		}
		
		else
		{
			printf("%d line Error!\n",cnt);
			error_flag = 1;
			return;
		}
		
		if(feof(fp)) 
			break;
	}
	//// pass2
	p = lst_head;
	int  pc, ta, oc, flag, b, b_data,oc_tmp=0;

	while(p != NULL)
	{
		b=0;
		oc=0;
		cnt=0;
		oc_tmp=0;
		tmp4 = strtok(p->operand,",");
		tmp5 = strtok(NULL," ");

		if(!strcmp(p->opcode,"START") || p->operand[0] == '.' || !strcmp(p->opcode,"RESB") || !strcmp(p->opcode,"RESW") || !strcmp(p->opcode,"END"))
		{
			p = p->link;
		}
		else if(!strcmp(p->opcode,"BASE"))
		{
			b_data = search_symbol(p->operand);
			p = p->link;
		}
		else if(!strcmp(p->opcode,"RSUB"))
		{
			p->oc[0] = '4';
			p->oc[1] = 'F';
			p->oc[2] = '0';
			p->oc[3] = '0';
			p->oc[4] = '0';
			p->oc[5] = '0';
			p->oc[6] = '\0';

			p = p->link;
		}
		else if(!strcmp(p->opcode,"BYTE"))
		{
			if(p->operand[0] == 'C')
			{
				cnt=2;
				
				while(1)
				{
					if(0 <= p->operand[cnt]/16  && p->operand[cnt]/16 <= 9)
						p->oc[oc_tmp++] = (p->operand[cnt]/16) + '0';
					else
						p->oc[oc_tmp++] = (p->operand[cnt]/16) + 55;

					if(0 <= p->operand[cnt]%16  && p->operand[cnt]%16 <= 9)
						p->oc[oc_tmp++] = (p->operand[cnt]%16) + '0';
					
					else
						p->oc[oc_tmp++] = (p->operand[cnt]%16) + 55;
					
					cnt++;
					if(p->operand[cnt] == '\'')
					{
						p->oc[oc_tmp] = '\0';
						break;
					}
				}
			}
			else if(p->operand[0] == 'X')
			{
				cnt=2;	
				while(1)
				{
					p->oc[cnt-2] = p->operand[cnt];
					cnt++;
					if(p->operand[cnt] =='\'')
					{
						p->oc[cnt] = '\0';
						break;
					}
				}
			}
			else // Error
			{
				error_flag = 1;
				return;
			}
			p = p->link;
		}
		else if(!strcmp(p->opcode,"WORD")) 
		{
			for(i=0; i<strlen(p->operand);i++)
			{
				oc_tmp = oc_tmp * 10 + p->operand[i] - '0';
			}

			for(i=5; i>=0; i--)
			{
				if(0 <= oc_tmp%16 && oc_tmp%16 <= 9)
					p->oc[i] = (oc_tmp%16) + '0';
				else
					p->oc[i] = (oc_tmp%16) + 55;
				oc_tmp/=16;
			}
			p->oc[6] = '\0';

			p = p->link;
		}
		else
		{
			if(p->opcode[0] == '+')
			{
				tmp6 = search_format(head, p->opcode+1);
				oc = search_opcode(head, p->opcode+1);
			}

			else
			{
				if(!strcmp(p->opcode,"RSUB")) // later
				{
					p=p->link;
					continue;
				}
				else
				{
					tmp6 = search_format(head, p->opcode);
					oc = search_opcode(head, p->opcode);
				}
			}

			if(!strcmp(tmp6,"1"))
			{
				for(i=1; i>=0; i--)
				{
					if(0 <= oc%16 && oc%16 <= 9)
						p->oc[i] = (oc%16) + '0';
					else
						p->oc[i] = (oc%16) + 55;
					oc/=16;
				}
				p->oc[2] = '\0';
			}

			else if(!strcmp(tmp6,"2"))
			{
				for(i=1; i>=0; i--)
				{
					if(0 <= oc%16 && oc%16 <= 9)
						p->oc[i] = (oc%16) + '0';
					else
						p->oc[i] = (oc%16) + 55;
					oc/=16;
				}

				if(tmp4[0] == 'A')
					oc = 0;
				else if(tmp4[0] == 'X')
				{
					oc = 1;
				}
				else if(tmp4[0] == 'L')
				{
					oc = 2;
				}
				else if(!strcmp(tmp4,"PC"))
				{
					oc = 8;
				}
				else if(!strcmp(tmp4,"SW"))
				{
					oc = 9;
				}
				else if(tmp4[0] == 'B')
				{
					oc = 3;
				}
				else if(tmp4[0] == 'S')
				{
					oc = 4;
				}
				else if(tmp4[0] == 'T')
				{
					oc = 5;
				}
				else if(tmp4[0] == 'F')
				{
					oc = 6;
				}
				p->oc[2] = oc + '0';	

				if(tmp5 != NULL)
				{
					if(tmp5[0] == 'A')
						oc = 0;
					else if(tmp5[0] == 'X')
					{
						oc = 1;
					}
					else if(tmp5[0] == 'L')
					{
						oc = 2;
					}
					else if(!strcmp(tmp5,"PC"))
					{
						oc = 8;
					}
					else if(!strcmp(tmp5,"SW"))
					{
						oc = 9;
					}
					else if(tmp5[0] == 'B')
					{
						oc = 3;
					}
					else if(tmp5[0] == 'S')
					{
						oc = 4;
					}
					else if(tmp5[0] == 'T')
					{
						oc = 5;
					}
					else if(tmp5[0] == 'F')
					{
						oc = 6;
					}
					p->oc[3] = oc + '0';

				}
				else
					p->oc[3] = '0';
	
				p->oc[4] = '\0';
			}

			else if(!strcmp(tmp6,"3/4"))
			{
				flag = 1;
				if(p->opcode[0] != '+') // format 3
				{
					if(p->operand[0] == '#')
					{
						oc += 1;
						ta = search_symbol(p->operand+1);
						if(ta == -1) // number
							flag = 0;
						else
							flag = 1;
					}
					else if(p->operand[0] == '@')
					{
						oc += 2;
						ta = search_symbol(p->operand+1);
					}
					else
					{
						oc += 3;
						ta = search_symbol(p->operand);
					}
					for(i=1; i>=0; i--)
					{
						if(0 <= oc%16 && oc%16 <= 9)
							p->oc[i] = (oc%16) + '0';
						else
							p->oc[i] = (oc%16) + 55;
						oc/=16;
					}

					if(p->link != NULL)
						pc = p->link->lc;

					oc = 0;
					if((ta - pc >= -2048) && (ta - pc <= 2047)) // p = 1, b = 0
					{
						oc += 2;
					}
					else if(ta - pc < 4096) // b = 1, p = 0
					{
						oc += 4;
						b = 1;
					}
					else
					{
						printf("Error!\n");
						error_flag = 1;
						return;
					}
					if(flag) // #LENGTH, LENGTH
					{
						if(!strcmp(tmp4,"X"))
							oc += 8;
						else if(tmp5 !=NULL && !strcmp(tmp5,"X"))
							oc += 8;

						if(0 <= oc%16 && oc%16 <= 9)
							p->oc[2] = (oc%16) + '0';
						else
							p->oc[2] = (oc%16) + 55;
						
						if(ta - pc < 0)
						{
							if(b)
							{
								ta = ta - b_data;
							}
							else
							{
								ta = 4096 + ta - pc;
							}
							
							for(i=5; i>=3; i--)
							{
								if(0 <= ta%16 && ta%16 <= 9)
									p->oc[i] = (ta%16) + '0';
								else
									p->oc[i] = (ta%16) + 55;
								ta/=16;
							}
						}
						else
						{
							if(b)
							{
								ta = ta - b_data;
							}
							else
							{
								ta = ta - pc;
							}

							for(i=5; i>=3; i--)
							{
								if(0 <= ta%16 && ta%16 <= 9)
									p->oc[i] = (ta%16) + '0';
								else
									p->oc[i] = (ta%16) + 55;
								ta/=16;
							}
						}
					}
					else // #0
					{	
						oc = 0;
						p->oc[2] = '0';	
						p->oc[3] = '0';
						p->oc[4] = '0';
						p->oc[5] = '0';
						
						for(i=1; i<strlen(tmp4) ;i++)
						{
							oc_tmp = oc_tmp * 10 + (tmp4[i] - '0');
						}

						for(i=1; i<=strlen(tmp4); i++)
						{
							p->oc[6-i] = oc_tmp%16 + '0';
							oc_tmp/=16;
						}
					}
					p->oc[6] = '\0';
				}
				else // format 4
				{
					if(p->operand[0] == '#')
					{
						oc += 1;
						ta = search_symbol(p->operand+1);
						if(ta == -1) // number
							flag = 0;
						else
							flag = 1;
					}
					else if(p->operand[0] == '@')
					{
						oc += 2;
						ta = search_symbol(p->operand+1);
					}
					else
					{
						oc += 3;
						ta = search_symbol(p->operand);
					}

					for(i=1; i>=0; i--)
					{
						if(0 <= oc%16 && oc%16 <= 9)
							p->oc[i] = (oc%16) + '0';
						else
							p->oc[i] = (oc%16) + 55;
						oc/=16;
					}
					
					oc = 1;
					if(flag) // #LENGTH, LENGTH
					{
						if(!strcmp(tmp4,"X"))
							oc += 8;
						else if(tmp5 !=NULL && !strcmp(tmp5,"X"))
							oc += 8;

						if(0 <= oc%16 && oc%16 <= 9)
							p->oc[2] = (oc%16) + '0';
						else
							p->oc[2] = (oc%16) + 55;

						for(i=7; i>=3; i--)
						{
							if(0 <= ta%16 && ta%16 <= 9)
								p->oc[i] = (ta%16) + '0';
							else
								p->oc[i] = (ta%16) + 55;
							ta/=16;
						}
					}
					
					else // #0
					{
						if(!strcmp(tmp4,"X"))
							oc += 8;
						else if(tmp5 !=NULL && !strcmp(tmp5,"X"))
							oc += 8;

						p->oc[2] = oc + '0';;	
						p->oc[3] = '0';
						p->oc[4] = '0';
						p->oc[5] = '0';
						p->oc[6] = '0';
						p->oc[7] = '0';

					    oc_tmp = 0;
						for(i=1; i<strlen(tmp4) ;i++)
						{
							oc_tmp = oc_tmp * 10 + (tmp4[i] - '0');
						}

						for(i=1; i<=strlen(tmp4); i++)
						{
							p->oc[8-i] = oc_tmp%16 + '0';
							oc_tmp/=16;
						}	
					}
					p->oc[8] = '\0';
				}
			}
			p = p->link;
		}
	}

	p = lst_head;
	cnt=5;

	filename[strlen(filename)-3] = '\0';
	filename = strcat(filename,"lst");
	FILE *fp2 = fopen(filename, "w");
	while( p!= NULL)
	{
		if(p->operand[0] == '.')
			fprintf(fp2,"%-4d     %s\n",cnt,p->operand);
		else if(!strcmp(p->opcode,"END"))
			fprintf(fp2,"%-4d      %-7s%-7s%-7s\n",cnt,p->label,p->opcode,p->operand);
		else if(p->operand[0] == '#' || p->operand[0] == '@')
			fprintf(fp2,"%-4d %04X %-7s%-6s%-7s           %-5s\n",cnt,p->lc,p->label,p->opcode,p->operand,p->oc);
		else
			fprintf(fp2,"%-4d %04X %-7s%-7s%-7s          %-5s\n",cnt,p->lc,p->label,p->opcode,p->operand,p->oc);
		cnt+=5;
		p = p->link;
	}
	fclose(fp2);
	
}

void make_obj(char *filename)
{
	int i, lc, start_lc, end_lc, length, tmp, tot_length=0;
	char str[70];

	lst *p = lst_head;
	m_rc *p2;	
	obj *p3;
	filename[strlen(filename)-3] = '\0';
	filename = strcat(filename,"obj");
	FILE *fp = fopen(filename, "w");

	while(p != NULL)
	{
		if(!strcmp(p->opcode,"END"))
		{
			end_lc = p->lc;
		}
		p = p->link;
	}

	p = lst_head;

	while(p != NULL)
	{
		memset(str, ' ', 70);
		length = 0;
		if(!strcmp(p->opcode,"START"))
		{
			str[0] = 'H';
			
			for(i=0;i<strlen(p->label);i++)
			{
				str[i+1] = p->label[i];
			}
			start_lc = p->lc;
			lc = p->lc;

			for(i=12;i>=7;i--)
			{
				if(0 <= lc%16 && lc%16 <= 9)
					str[i] = (lc%16) + '0';
				else
					str[i] = (lc%16) + 55;
				
				lc = lc/16;
			}
			lc = end_lc - lc;

			for(i=18;i>=13;i--)
			{
				if(0 <= lc%16 && lc%16 <= 9)
					str[i] = (lc%16) + '0';
				else
					str[i] = (lc%16) + 55;
				
				lc = lc/16;
			}
			str[19]='\0';

			p = p->link;
		}
		else if(!strcmp(p->opcode,"END"))
		{
			memset(str, ' ', 70);
			p2 = mrc_head;

			while(p2 != NULL)
			{
				str[0] = 'M';
				tmp = p2->rc;

				for(i=6;i>=1;i--)
				{
					if(0 <= tmp%16 && tmp%16 <= 9)
						str[i] = (tmp%16) + '0';
					else
						str[i] = (tmp%16) + 55;

					tmp = tmp/16;
				}
				str[7] = '0';
				str[8] = '5';
				str[9] = '\0';
				add_obj(str);
				p2 = p2->link;
			}
			memset(str, ' ', 70);

			str[0] = 'E';

			for(i=6;i>=1;i--)
			{
				if(0 <= start_lc%16 && start_lc%16 <= 9)
					str[i] = (start_lc%16) + '0';
				else
					str[i] = (start_lc%16) + 55;
				
				lc = lc/16;
			}
			str[7]='\0';
			p = p->link;
		}
		else
		{
			if(!strcmp(p->opcode,"RESW") || !strcmp(p->opcode,"RESB") || p->operand[0] == '.')
			{
				p = p->link;
				continue;
			}
			str[0] = 'T';
			
			lc = p->lc;
			
			for(i=6;i>=1;i--)
			{
				if(0 <= lc%16 && lc%16 <= 9)
					str[i] = (lc%16) + '0';
				else
					str[i] = (lc%16) + 55;
				
				lc = lc/16;
			}

			tmp = 9;

			while(1)
			{
				if(length + (strlen(p->oc) / 2) > 30)
					break;
				if(tmp > 68)
					break;
				if(!strcmp(p->opcode,"RESW") || !strcmp(p->opcode,"RESB") || !strcmp(p->opcode,"END"))
					break;

				length += (strlen(p->oc) /2);
				if((strlen(p->oc)/2) == 4)
				{
					if(p->operand[0] == '#' && search_symbol(p->operand+1));
					else
					{
						add_mrc((tot_length/2) +1);
					}
				}
				for(i=0; i<strlen(p->oc); i++)
				{
					str[tmp++] = p->oc[i];
					tot_length++;
				}

				p = p->link;
			}

			if(0 <= length/16 && length/16 <= 9)
				str[7] = (length/16) + '0';
			else
				str[8] = (length/16) + 55;

			if(0 <= length%16 && length%16 <= 9)
				str[8] = (length%16) + '0';
			else
				str[8] = (length%16) + 55;

			str[tmp]='\0';
		}
		add_obj(str);
	}

	p3 = obj_head;
	while(p3 != NULL)
	{
		fprintf(fp, "%s\n",p3->record);
		p3 = p3->link;
	}
	fclose(fp);
}

void make_file(char *filename, hash **head)
{
	FILE *fp = fopen(filename,"r");
	char name1[255], name2[255];	
	strcpy(name1, filename);
	strcpy(name2, filename);
	name1[strlen(name1)-1] = 't';
	name1[strlen(name1)-2] = 's';
	name1[strlen(name1)-3] = 'l';
	name2[strlen(name1)-1] = 'j';
	name2[strlen(name1)-2] = 'b';
	name2[strlen(name1)-3] = 'o';
	error_flag = 0;

	if(fp == NULL)
		printf("File does not exist\n");
	else
	{
		free_list();
		make_lst(filename,fp,head);
		if(!error_flag) 
			make_obj(filename);
		if(!error_flag)
			printf("            output file : [%s], [%s]\n",name1, name2);
		fclose(fp);
	}
}

int check_estab(char *symbol, int index)
{
	int i;
	for(i=0; i<index; i++)
		if(!strcmp(symbol, tab[i].symbol))
			return -1;

	return 1;
}

void load(unsigned char memory[][16])
{
	FILE *fp;
	int i,j,k,t_index = 1, start_addr = load_addr, temp1 = 0, temp2, control_num, tot_len = 0;
	char str[256], str_temp[30];

	for(i=0; i<3; i++)
	{
		if(!strcmp(obj_nm[i], ""))
			continue;
		fp = fopen(obj_nm[i], "r");
		if(fp == NULL)
		{
			printf("File does not exist\n");
			return;
		}
		while(fgets(str, 256, fp) != NULL)
		{
			if(str[0] == 'H')
			{
				strncpy(tab[t_index].control, &str[1], 6);

				for(j=0; j<6; j++)
				{
					if(*(tab[t_index].control + j) == 32)
						*(tab[t_index].control + j) = '\0';
				}
				tab[t_index].addr = start_addr;
				tab[t_index].ref_num = 1;
				strncpy(str_temp, &str[13], 6);
				sscanf(str_temp, "%X", &temp1);
				tab[t_index].len = temp1;
				control_num = t_index;
				start_addr+=tab[t_index].len;
				t_index++;

			}
			else if(str[0] == 'D')
			{
				temp1 = strlen(str) / 12;
				for(j=0; j<temp1; j++)
				{
					tab[t_index].ref_num = 2;
					strncpy(tab[t_index].symbol, &str[j*12+1], 6);

					for(k=0; k<6; k++)
						if(*(tab[t_index].symbol + k) == 32)
							*(tab[t_index].symbol + k) = '\0';

					if(check_estab(tab[t_index].symbol,t_index) == -1)
					{
						printf("Estab error\n");
						return;
					}
					strncpy(str_temp, &str[j*12+7], 6);
					sscanf(str_temp, "%X", &temp2);
					tab[t_index].addr = tab[control_num].addr + temp2;
					t_index++;
				}
			}
		}
		fclose(fp);
	}

	printf("control   symbol     address  length\n");
	printf("section   name\n");
	printf("------------------------------------\n");
	for(i=1 ; i<t_index; i++)
	{
		if(tab[i].ref_num ==1)
		{
			printf("%-10s", tab[i].control);
			printf("          ");
			printf("%X", tab[i].addr);
			printf("      ");
			printf("%04X\n", tab[i].len);
			tot_len += tab[i].len;
		}
		else
		{
			printf("         ");
			printf("%-10s", tab[i].symbol);
			printf(" ");
			printf("%X\n", tab[i].addr);
		}
	}
	printf("------------------------------------\n");
	printf("              total length    %04X\n", tot_len);
	tot_addr = load_addr + tot_len;

	for(i=0; i<3; i++)
	{
		int s_addr = load_addr, tmp_addr = 0, ref_num = 0;
		int ref[30], ob_cnt, value = 0;
		char plus;

		k=0;
		if(!strcmp(obj_nm[i], ""))
			continue;

		fp = fopen(obj_nm[i], "r");

		for(j=0; j<i; j++)
		{
			while(1)
			{
				if(tab[k].ref_num == 1)
				{
					s_addr = s_addr + tab[k].len;
					k++;
					break;
				}
				k++;
			}
		}

		while(fgets(str,256,fp) != NULL)
		{
			if(str[0] == 'H')
			{
				strncpy(str_temp,&str[1], 6);
				for(j=0; j<6; j++)
				{
					if(*(str_temp + j) == 32)
					{
						*(str_temp + j) = '\0';
						break;
					}
				}
				j=0;
				while(1)
				{
					if(!strcmp(str_temp, tab[j].control))
					{
						tmp_addr = tab[j].addr;
						break;
					}
					j++;
				}
				ref[1] = tmp_addr;	
			}

			else if(str[0] == 'R')
			{
				for(j=0; j<strlen(str)/8; j++)
				{
					strncpy(str_temp, &str[j*8+1], 2);
					sscanf(str_temp, "%d", &temp1);
					strncpy(str_temp, &str[j*8+3], 6);

					for(k=0; k<6; k++)
					{
						if(*(str_temp + k) == 32 || *(str_temp + k) == '\n')
						{
							*(str_temp + k) = '\0';
							break;
						}
					}
					k=0;
					while(1)
					{
						if(!strcmp(str_temp, tab[k].symbol))
						{
							tmp_addr = tab[k].addr;
							break;
						}
						k++;
					}
					ref[temp1] = tmp_addr;
				}
			}
			else if(str[0] == 'T')
			{
				strncpy(str_temp, &str[1], 6);
				sscanf(str_temp, "%X", &temp1);
				for(j=0; j<6; j++)
					str_temp[j] = '\0';

				strncpy(str_temp, &str[7], 2);
				sscanf(str_temp, "%X", &ob_cnt);
				for(j=0; j<2; j++)
					str_temp[j] = '\0';

				for(j=0; j<ob_cnt; j++)
				{
					strncpy(str_temp, &str[j*2+9], 2);
					sscanf(str_temp, "%X", &value);
					memory[(s_addr + temp1 + j)	/ 16][(s_addr + temp1 + j) % 16] = value;
					for(k=0; k<2; k++)
						str_temp[k] = '\0';
				}
			}
			else if(str[0] == 'M')
			{
				strncpy(str_temp, &str[1], 6);
				sscanf(str_temp, "%X", &temp1);
				for(j=0; j<6; j++)
					str_temp[j] = '\0';
				tmp_addr = s_addr + temp1;

				strncpy(str_temp, &str[7], 2);
				sscanf(str_temp, "%d", &temp1);
				for(j=0; j<2; j++)
					str_temp[j] = '\0';

				strncpy(str_temp, &str[9], 1);
				sscanf(str_temp, "%c", &plus);
				for(j=0; j<1; j++)
					str_temp[j] = '\0';

				strncpy(str_temp, &str[10], 2);
				sscanf(str_temp, "%d", &ref_num);

				temp2 = ref[ref_num];
				if(plus != '+')
					temp2 = -1 * temp2;

				value = memory[tmp_addr / 16][tmp_addr % 16];
				if(temp1 == 5)
					value = value % 16;
				value = value * 256 * 256;

				tmp_addr +=1;
				value = value + memory[tmp_addr / 16][tmp_addr % 16] * 256;

				tmp_addr +=1;
				value = value + memory[tmp_addr/16][tmp_addr % 16] + temp2;

				if(temp1 == 6)
					memory[(tmp_addr-2) / 16][(tmp_addr-2) % 16] = value/65536;
				else	
					memory[(tmp_addr-2) / 16][(tmp_addr-2) % 16] += value/65536;
				memory[(tmp_addr-1) / 16][(tmp_addr-1) % 16] = value/256;
				memory[tmp_addr / 16][tmp_addr % 16] = value%256;
			}
		}
		fclose(fp);
	}
}

void print_bp()
{
	bp *p = bp_head;

	if(p == NULL)
	{
		printf("There is no breakpoint.\n");
		return ;
	}
	else
	{
		printf("\tbreakpoint\n");
		printf("\t----------\n");
		while(p != NULL)
		{
			printf("\t%X\n", p->bp);
			p = p->link;
		}
	}
}

void clear_bp()
{
	bp *tmp = bp_head;

	if(tmp == NULL)
	{
		printf("There is no breakpoint.\n");
		return ;
	}
	else
	{
		while(tmp->link != NULL)
		{
			bp_head = bp_head->link;
			free(tmp);
			tmp = bp_head;
		}
		free(tmp);
	}

	bp_head = NULL;
	printf("\t[ok] clear all breakpoints\n");
}

void add_bp(int num)
{
	bp *temp,  *p = bp_head;

	temp = (bp*)malloc(sizeof(bp));
	temp->bp = num;
	temp->link = NULL;

	if(bp_head == NULL)
		bp_head = temp;
	else
	{
		while(p->link != NULL)
			p = p->link;

		p->link = temp;
	}

	printf("\t[ok] create breakpoint %X\n", temp->bp);
}

char *search_format_by_opcode(hash **head, int num)
{
	int i;
	char *result=NULL;
	hash *p;

	for(i=0; i<20; i++)
	{
		if(head[i] == NULL)
			continue;
		p = head[i];
		if(p->opcode == num)
		{
			result = p->format;
			return result;
		}
		else
		{
			while(p != NULL)
			{	
				if(p->opcode == num)
				{
					result = p->format;
					return result;
				}

				p = p->link;
			}
		}
	}
	return result;

}

void run(unsigned char memory[][16], hash **head)
{

	int end_addr;
	int i;
	int target_address;
	int target_value;
	int first_target_register, second_target_register;
	bp *bp_p = bp_head;

	if(PC == 0)
		PC = load_addr;
	if(bp_head == NULL)
		end_addr = tot_addr;
	else
	{
		while(bp_p != NULL)
		{
			if(bp_p->bp <= PC)
				bp_p = bp_p->link;
			else
			{
				end_addr = bp_p->bp;
				break;
			}
			end_addr = tot_addr;
		}
	}
	while (1) {

		if (0x18 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x1C) { // ADD : A <- (A) + (m..m+2) 
			Deassemble(3, &target_address, &target_value, memory);

			A += target_value;
		}
		else if (0x58 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x5C) { // ADDF : F <- (F) + (m..m+5) (floating-point instruction -> cannot express)
			Deassemble(3, &target_address, &target_value, memory);
		}
		else if (0x90 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x94) { // ADDR : r2 <- (r2) + (r1)
			Deassemble(2, &first_target_register, &second_target_register, memory);

			second_target_register += first_target_register;
		}
		else if (0x40 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x44) { // AND : A <- (A) & (m..m+2) 
			Deassemble(3, &target_address, &target_value, memory);

			A = A & target_value;
		}
		else if (0xB4 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0xB8) { // CLEAR : r1 <- 0
			Deassemble(2, &first_target_register, &second_target_register, memory);

			first_target_register = 0;
		}
		else if (0x28 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x2C) { // COMP : (A) : (m..m+2)
			Deassemble(3, &target_address, &target_value, memory);

			if (A > target_value) {
				SW = 2; // '>'
			}
			else if (A == target_value) {
				SW = 0; // '='
			}
			else {
				SW = -2; // '<'
			}
		}
		else if (0x88 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x8C) { // COMPF : (F) : (m..m+5) (floating-point instruction -> cannot express)
			Deassemble(3, &target_address, &target_value, memory);
		}
		else if (0xA0 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0xA4) { // COMPR : (r1) : (r2)
			Deassemble(2, &first_target_register, &second_target_register, memory);
			if (reg[first_target_register] > reg[second_target_register]) {
				SW = 2; // '>'
			}
			else if (reg[first_target_register] == reg[second_target_register]) {
				SW = 0; // '='
			}
			else {
				SW = -2; // '<'
			}
		}
		else if (0x24 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x28) { // DIV : A <- (A) / (m..m+2) 
			Deassemble(3, &target_address, &target_value, memory);

			A /= target_value;
		}
		else if (0x64 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x68) { // DIVF : F <- (F) / (m..m+5) (floating-point instruction -> cannot express)
			Deassemble(3, &target_address, &target_value, memory);
		}
		else if (0x9C <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0xA0) { // DIVR : r2 <- (r2) / (r1)
			Deassemble(2, &first_target_register, &second_target_register, memory);

			reg[second_target_register] /= reg[first_target_register];
		}
		else if (0x3C <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x40) { // J : PC <- m
			Deassemble(3, &target_address, &target_value, memory);

			PC = target_address + load_addr;
		}
		else if (0x30 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x34) { // JEQ : PC <- m if CC is set to =
			Deassemble(3, &target_address, &target_value, memory);
			if (SW == 0) {
				PC = target_address + load_addr;
			}
		}
		else if (0x34 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x38) { // JGT : PC <- m if CC is set to >
			Deassemble(3, &target_address, &target_value, memory);

			if (SW == 2) {
				PC = target_address + load_addr;
			}
		}
		else if (0x38 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x3C) { // JLT : PC <- m if CC is set to <
			Deassemble(3, &target_address, &target_value, memory);

			if (SW == -2) {
				PC = target_address + load_addr;
			}
		}
		else if (0x48 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x4C) { // JSUB : L <- (PC); PC <- m 
			Deassemble(3, &target_address, &target_value, memory);

			L = PC;
			PC = target_address + load_addr;
		}
		else if (0x00 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x04) { // LDA : A <- (m..m+2) 
			Deassemble(3, &target_address, &target_value, memory);

			A = target_value;
		}
		else if (0x68 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x6C) { // LDB : B <- (m..m+2) 
			Deassemble(3, &target_address, &target_value, memory);

			B = target_value;
		}
		else if (0x50 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x54) { // LDCH : A[rightmost byte] <- (m) 
			Deassemble(3, &target_address, &target_value, memory);

			A = A - A % 0x100 + memory[target_address / 16][target_address % 16];
		}
		else if (0x70 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x74) { // LDF : F <- (m..m+5) (floating-point instruction -> cannot express)
			Deassemble(3, &target_address, &target_value, memory);
		}
		else if (0x08 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x0C) { // LDL : L <- (m..m+2) 
			Deassemble(3, &target_address, &target_value, memory);

			L = target_value;
		}
		else if (0x6C <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x70) { // LDS : S <- (m..m+2) 
			Deassemble(3, &target_address, &target_value, memory);

			S = target_value;
		}
		else if (0x74 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x78) { // LDT : T <- (m..m+2) 
			Deassemble(3, &target_address, &target_value, memory);

			T = target_value;
		}
		else if (0x04 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x08) { // LDX : X <- (m..m+2) 
			Deassemble(3, &target_address, &target_value, memory);

			X = target_value;
		}
		else if (0xD0 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0xD4) { // LPS : Load processor status from information beginning at address m (cannot express)
			Deassemble(3, &target_address, &target_value, memory);
		}
		else if (0x20 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x24) { // MUL : A <- (A) * (m..m+2) 
			Deassemble(3, &target_address, &target_value, memory);

			A *= target_value;
		}
		else if (0x60 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x64) { // MULF : F <- (F) * (m..m+5) (floating-point instruction -> cannot express)
			Deassemble(3, &target_address, &target_value, memory);
		}
		else if (0x98 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x9C) { // MULR : r2 <- (r2) * (r1)
			Deassemble(2, &first_target_register, &second_target_register, memory);

			reg[second_target_register] *= reg[first_target_register];
		}
		else if (0x44 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x48) { // OR : A <- (A) | (m..m+2) 
			Deassemble(3, &target_address, &target_value, memory);

			A = A | target_value;
		}
		else if (0xD8 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0xDC) { // RD : A[rightmost byte] <- data from device (previleged)
			Deassemble(3, &target_address, &target_value, memory);
		}
		else if (0xAC <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0xB0) { // RMO : r2 <- (r1)
			Deassemble(2, &first_target_register, &second_target_register, memory);

			reg[second_target_register] = reg[first_target_register];
		}
		else if (0x4C <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x50) { // RSUB : PC <- (L)
			Deassemble(3, &target_address, &target_value, memory);

			PC = L + load_addr;
		}
		else if (0xA4 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0xA8) { // SHIFTL : r1 <- (r1); left circular shift
			Deassemble(2, &first_target_register, &second_target_register, memory);

			for (i = 0; i < second_target_register; i++) {
				reg[first_target_register] = reg[first_target_register] << 1;
				reg[first_target_register] = reg[first_target_register] - (reg[first_target_register] % 2) + reg[first_target_register] % 0x10000000 / 0x1000000 % 2;
			}
		}
		else if (0xA8 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0xAC) { // SHIFTR : r1 <- (r1); right circular shift
			Deassemble(2, &first_target_register, &second_target_register, memory);

			for (i = 0; i < reg[second_target_register]; i++) {
				reg[first_target_register] = reg[first_target_register] % 0x1000000 + (reg[first_target_register] % 2 * 0x1000000);
				reg[first_target_register] = reg[first_target_register] >> 1;
			}
		}
		else if (0xEC <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0xF0) { // SSK : Protection key for addres m <- (A) (cannot express) 
			Deassemble(3, &target_address, &target_value, memory);
		}
		else if (0x0C <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x10) { // STA : m..m+2 <- (A) 
			Deassemble(3, &target_address, &target_value, memory);

			memory[target_address / 16][target_address % 16] = A / 0x10000;
			memory[(target_address + 1) / 16][(target_address + 1) % 16] = A % 0x10000 / 0x100;
			memory[(target_address + 2) / 16][(target_address + 2) % 16] = A % 0x100;
		}
		else if (0x78 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x7C) { // STB : m..m+2 <- (B) 
			Deassemble(3, &target_address, &target_value, memory);

			memory[target_address / 16][target_address % 16] = B / 0x10000;
			memory[(target_address + 1) / 16][(target_address + 1) % 16] = B % 0x10000 / 0x100;
			memory[(target_address + 2) / 16][(target_address + 2) % 16] = B % 0x100;
		}
		else if (0x54 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x58) { // STCH : m <- (A)[rightmost byte]
			Deassemble(3, &target_address, &target_value, memory);

			memory[target_address / 16][target_address % 16] = A % 0x100;
		}
		else if (0x80 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x84) { // STF : m..m+5 <- (F)  (floating-point instruction -> cannot express)
			Deassemble(3, &target_address, &target_value, memory);
		}
		else if (0xD4 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0xD8) { // STI : Interval timer value <- (m..m+2) (cannot express)
			Deassemble(3, &target_address, &target_value, memory);
		}
		else if (0x14 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x18) { // STL : m..m+2 <- (L) 
			Deassemble(3, &target_address, &target_value, memory);

			memory[target_address / 16][target_address % 16] = L / 0x10000;
			memory[(target_address + 1) / 16][(target_address + 1) % 16] = L % 0x10000 / 0x100;
			memory[(target_address + 2) / 16][(target_address + 2) % 16] = L % 0x100;
		}
		else if (0x7C <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x80) { // STS : m..m+2 <- (S) 
			Deassemble(3, &target_address, &target_value, memory);

			memory[target_address / 16][target_address % 16] = S / 0x10000;
			memory[(target_address + 1) / 16][(target_address + 1) % 16] = S % 0x10000 / 0x100;
			memory[(target_address + 2) / 16][(target_address + 2) % 16] = S % 0x100;
		}
		else if (0xE8 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0xEC) { // STSW : m..m+2 <- (SW) (privileged)
			Deassemble(3, &target_address, &target_value, memory);
		}
		else if (0x84 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x88) { // STT : m..m+2 <- (T) 
			Deassemble(3, &target_address, &target_value, memory);

			memory[target_address / 16][target_address % 16] = T / 0x10000;
			memory[(target_address + 1) / 16][(target_address + 1) % 16] = T % 0x10000 / 0x100;
			memory[(target_address + 2) / 16][(target_address + 2) % 16] = T % 0x100;
		}
		else if (0x10 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x14) { // STX : m..m+2 <- (X) 
			Deassemble(3, &target_address, &target_value, memory);

			memory[target_address / 16][target_address % 16] = X / 0x10000;
			memory[(target_address + 1) / 16][(target_address + 1) % 16] = X % 0x10000 / 0x100;
			memory[(target_address + 2) / 16][(target_address + 2) % 16] = X % 0x100;
		}
		else if (0x1C <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x20) { // SUB : A <- (A) - (m..m+2) 
			Deassemble(3, &target_address, &target_value, memory);

			A -= target_value;
		}
		else if (0x5C <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x60) { // SUBF : F <- (F) - (m..m+5) (floating-point instruction -> cannot express)
			Deassemble(3, &target_address, &target_value, memory);
		}
		else if (0x94 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x98) { // SUBR : r2 <- (r2) - (r1)
			Deassemble(2, &first_target_register, &second_target_register, memory);

			reg[second_target_register] -= reg[first_target_register];
		}
		else if (0xB0 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0xB4) { // SVC : Generate SVC interrupt (cannnot express)
			Deassemble(2, &first_target_register, &second_target_register, memory);
		}
		else if (0xE0 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0xE4) { // TD : Test device specified by (m) (previleged)
			Deassemble(3, &target_address, &target_value, memory);

			SW = 2; // '<' means ready
		}
		else if (0x2C <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0x30) { // TIX : X <- (X) + 1; (X) : (m..m+2)
			Deassemble(3, &target_address, &target_value, memory);

			X++;
			if (X > target_value) {
				SW = 2; // '>'
			}
			else if (X == target_value) {
				SW = 0; // '='
			}
			else {
				SW = -2; // '<'	
			}
		}
		else if (0xB8 <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0xBC) { // TIXR : X <- (X) + 1; (X) : (r1)
			Deassemble(2, &first_target_register, &second_target_register, memory);

			X++;
			if (X > reg[first_target_register]) {
				SW = 2; // '>'
			}
			else if (X == reg[first_target_register]) {
				SW = 0; // '='
			}
			else {
				SW = -2; // '<'
			}
		}
		else if (0xDC <= memory[PC / 16][PC % 16] && memory[PC / 16][PC % 16] < 0xE0) { // WD : Device specified by (m) <- (A)[rightmostbyte] (previleged)
			Deassemble(3, &target_address, &target_value, memory);
		}
		else {
			PC++;
		}
	//	printf("%X--%X\n",PC,end_addr);
		if(PC >= end_addr - 4) 
			break;
	}
	
	if(end_addr == tot_addr)
	{
		printf("\t\tA : %06X X : %06X\n", A, X);
		printf("\t\tL : %06X PC: %06X\n", L, PC+4);
		printf("\t\tB : %06X S : %06X\n", B, S);
		printf("\t\tT : %06X\n", T);
		printf("End program\n");
		PC = load_addr;
	}
	else
	{
		printf("\t\tA : %06X X : %06X\n", A, X);
		printf("\t\tL : %06X PC: %06X\n", L, PC);
		printf("\t\tB : %06X S : %06X\n", B, S);
		printf("\t\tT : %06X\n", T);
		printf("Stop at checkpoint [%04X]\n", end_addr);
	}
}


void Deassemble(int type, int *first_para, int *second_para, unsigned char memory[][16])
{
	int ni;
	int xbpe;
	int negative_sub_num = 0;
	if (type == 3) { // first_para = address, second_para = value
		ni = memory[PC / 16][PC % 16] % 4;
		xbpe = memory[(PC + 1) / 16][(PC + 1) % 16] / 0x10;
		*first_para = (memory[(PC + 1) / 16][(PC + 1) % 16] % 0x10) * 0x100 + memory[(PC + 2) / 16][(PC + 2) % 16];
		if (*first_para / 0x100 >= 8) {
			negative_sub_num = 0x1000;
		}

		*second_para = 0;

		if (xbpe / 8 == 1) { // if x = 1 (indexed mode)
			*first_para += X;
		}

		if (ni / 2 == 0 && ni % 2 == 0) { // if n = 0, i = 0 (sic) 
			*first_para += xbpe % 8 * 0x1000;
			*second_para = memory[(*first_para) / 16][(*first_para) % 16];
			PC += 3;
		}
		else {
			if (xbpe % 2 == 1) { // if e = 1  (extended, format 4)
				*first_para = *first_para * 0x100 + memory[(PC + 3) / 16][(PC + 3) % 16];
				negative_sub_num *= 0x100;
				PC += 4;
			}
			else {
				PC += 3;
			}

			if ((xbpe % 8) / 4 == 0 && (xbpe % 4) / 2 == 1) { // if b = 0, p = 1 (pc relatvie)
				*first_para = PC + *first_para - negative_sub_num;
			}
			else if ((xbpe % 8) / 4 == 1 && (xbpe % 4) / 2 == 0) { // if b = 1, p = 0 (base relative)
				*first_para = B + *first_para - negative_sub_num;
			}
			else if ((xbpe % 8) / 4 == 0 && (xbpe % 4) / 2 == 0) { // if b = 0, p = 0 (direct addressing)
				*first_para = *first_para - negative_sub_num;
			}

			if (ni / 2 == 1 && ni % 2 == 0) { // if n = 1, i = 0 (indirect)
				*second_para = memory[(*first_para) / 16][(*first_para) % 16] * 0x10000;
				*second_para += memory[(*first_para + 1) / 16][(*first_para + 1) % 16] * 0x100;
				*second_para += memory[(*first_para + 2) / 16][(*first_para + 2) % 16];

				*first_para = *second_para;

				*second_para = memory[(*first_para) / 16][(*first_para) % 16] * 0x10000;
				*second_para += memory[(*first_para + 1) / 16][(*first_para + 1) % 16] * 0x100;
				*second_para += memory[(*first_para + 2) / 16][(*first_para + 2) % 16];
			}
			else if (ni / 2 == 0 && ni % 2 == 1) { // if n = 0, i = 1 (immediate)
				*second_para = *first_para;
			}
			else if (ni / 2 == 1 && ni % 2 == 1) { // if n = 1, i = 1 (simple)
				*second_para = memory[(*first_para) / 16][(*first_para) % 16] * 0x10000;
				*second_para += memory[(*first_para + 1) / 16][(*first_para + 1) % 16] * 0x100;
				*second_para += memory[(*first_para + 2) / 16][(*first_para + 2) % 16];
			}
		}
	}
	else if (type == 2) { // first_para = first_register, second_para = second_register
		*first_para = memory[(PC + 1) / 16][(PC + 1) % 16] / 0x10;
		*second_para = memory[(PC + 1) / 16][(PC + 1) % 16] % 0x10;
		PC += 2;
	}
}
