#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//#define TEST

typedef struct{
	int left,right;
}Line;

int main(int argc,char *argv[])
{
	if(argc>2)//most 2
	{
		printf("Invalid parameter input!\n");
		return 2;
	}

	/*int SORT=0;
	if(argc==3 && strcmp(argv[2],"-sort")==0)
		SORT=1;*/

	FILE *input=fopen(argv[1],"r");

	if(!input)
	{
		printf("Can't find the file!\n");
		return 1;
	}


	int i=0;
	char c;
	while((c=fgetc(input))!=EOF)	//count the number of the data
	{
		if(c=='\n')
			i++;
	}

	rewind(input);


	int max=i;//max indicates how many lines in the data

	Line *data=(Line *)malloc(sizeof(Line)*max);
	//end_point *ep=(end_point *)malloc(sizeof(end_point)*max);
	Line **line=(Line **)malloc(sizeof(Line *)*max);
	fscanf(input,"%d %d",&data[0].left,&data[0].right);
	printf("data[0].left : %p\n",&data[0]);
	printf("%p\n",line[0]);
	for(int i=0;i>0;i++)
	{
		printf("yes\n");
	}
	//printf("%d\n",sizeof(Line));
	//printf("%d\n",sizeof(Line *));
return 0;
}