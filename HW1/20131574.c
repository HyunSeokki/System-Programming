#include "20131574.h"

int main()
{
	int var1,var2,var3,cnt;
	int addr,temp_addr=0x0,opcode;
	int i,j,flag=1,flag2=1,flag3=1;
	unsigned char memory[65536][16]={0};
	char buf_in[255],file_name[255],buf_dump[255], ascii[16],buf_op[20];
	DIR *dp;
	struct dirent *dirp;
	struct stat file_info;
	mode_t file_mode;
	node *head = (node*)malloc(sizeof(node));
	node *p;
	hash *hash_head[20];
	hash *h_p;

	make_hash(hash_head);

	while(1)
	{
		var1=var2=var3=-1;
		strcpy(buf_dump,"");
		strcpy(buf_op,"");
		printf("sicsim> ");
		fgets(buf_in, 255, stdin);
		*(buf_in+strlen(buf_in)-1) = '\0';
		sscanf(buf_in, "%s %x,%x,%x", buf_dump, &var1, &var2, &var3);
		sscanf(buf_in, "%s %s", buf_dump, buf_op);
		
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
//			printf("%x %x\n",var1,var2);
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
		
		else
		{
			printf("Error!\n");
		}
	}

	free(head);
	free(hash_head);

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

	while(fscanf(fp,"%x %s %s", &opcode,mnemonic,format) != EOF)
	{
		add_hash(head,opcode,mnemonic,format);
	}

	fclose(fp);
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
