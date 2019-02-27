#include<stdio.h>
#include<stdlib.h>

#define NUM_MODES 5

int is_valid_mode(char mode);
void open_file(FILE *fp, char* file_addr, char mode);
void read_binary_file(FILE *fp, char mode);
void print_content(char *buffer, long size, char mode);
void print_element(char *buffer, unsigned int index, char mode);

void error(char *msg)
{
	printf("%s\n",msg);
	exit(1);
}

void print_r(char c, int len, int new_line)
{
	if(len==0)
	{
		if(new_line)
			printf("\n");
		return ;
	}
	printf("%c",c);
	print_r(c,len-1,new_line);
}

int main(int argc, char* argv[])
{
	if(argc<3)
		error("ERROR: incomplete arguments!");
	if(!is_valid_mode(argv[2][0]))
		error("ERROR: invalid mode!");	
	FILE *fp;
	open_file(fp, argv[1], argv[2][0]);
	return 0;
}

int is_valid_mode(char mode)
{
	char modes[] = {'x','c','d','C','D'};
	for(int i=0;i<NUM_MODES;i++)
		if(mode==modes[i])
			return 1;
	return 0;
}

void open_file(FILE *fp, char* file_addr, char mode)
{
	fp = fopen(file_addr, "rb");
	if(fp == NULL)
		error("ERROR: Unable to open file!\n");
	read_binary_file(fp, mode);	
}

void read_binary_file(FILE *fp, char mode)
{
	long file_size;
	size_t result;
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	rewind(fp);
	
	char *buffer = (char*)malloc(sizeof(char)*file_size);
	if(buffer==NULL)
	{
		free(buffer);
		printf("ERROR Info:\nfile_size=%lu\n",file_size);
		error("ERROR: Insufficient heap space for allocation!");
	}
	result = fread(buffer, 1, file_size, fp);
	if(result!=file_size)
	{
		free(buffer);
		printf("ERROR Info:\nfile_size=%lu\nresult=%lu\n",file_size, result);
		error("ERROR: File reading error!");
	}
	print_content(buffer, file_size, mode);	
	printf("\n");
	printf("Size = %lu bytes\n", file_size);
	fclose(fp);
	free(buffer);
}

void print_content(char *buffer, long size, char mode)
{
	for(unsigned int i=0;i<size;i++)
	{
		print_element(buffer, i, mode);
	}
}

void print_element(char *buffer, unsigned int index, char mode)
{
	switch(mode)
	{
		case 'x':
			if(index%16==0)
			{
				if(index%256==0&&index>0)
				{
					printf("\n");
					print_r('-',75,0);
				}
				printf("\n%05x: ", index);
			}
			printf("%02x ", (int)(*(unsigned char*)(&buffer[index])));
			break;
		case 'c':
			printf("%c", buffer[index]);
			break;
		case 'd':
			if(index%16==0)
			{
				if(index%256==0&&index>0)
				{
					printf("\n");
					print_r('-',90,0);
				}
				printf("\n%05x: ", index);
			}
			printf("%03d ", (int)(*(unsigned char*)(&buffer[index])));
			break;
		case 'C':
			if(index%16==0)
			{
				if(index)
				{
					printf(" |");
					for(unsigned int i=index-16;i<index;i++)
					{
						printf("%c", (buffer[i]<32||buffer[i]>126?'.':buffer[i]));
					}
					printf("|");
				}
				if(index%256==0&&index>0)
				{
					printf("\n");
					print_r('-',74,0);
				}
				printf("\n%05x: ", index);
			}
			printf("%02x ", (int)(*(unsigned char*)(&buffer[index])));
			
			break;
		case 'D':
			if(index%16==0)
			{
				if(index)
				{
					printf(" |");
					for(unsigned int i=index-16;i<index;i++)
					{
						printf("%03d ", (int)(*(unsigned char*)(&buffer[index])));
					}
					printf("|");
				}
				if(index%256==0&&index>0)
				{
					printf("\n");
					print_r('-',122,0);
				}
				printf("\n%05x: ", index);
			}
			printf("%02x ", (int)(*(unsigned char*)(&buffer[index])));
			
			break;
	}
}
