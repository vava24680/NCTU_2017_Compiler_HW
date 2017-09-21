#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//#define TEST

typedef struct{
	int left,right;
}Line;

/*typedef struct{
	Line *line;
	int head;
}end_point;*/



int line_comp(Line former,Line later)
{
	int comp;

	if(former.right>=later.right)		//if the later is all in the former's range
		comp=later.right-later.left;
	else if(former.right<later.right)	//if the 2 data are not all included in each
		comp=former.right-later.left;
//printf("%d\n",comp);

	return comp;
}	//end of line_comp()



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


	double sort_time_use;
	/*if(SORT==1)
	{


		for(i=0;i<max;i++)
		{
			if(i==0)
				fscanf(input,"%d %d",&data[i].left,&data[i].right);
			else
			{
				int left_temp;
				fscanf(input,"%d",&left_temp);
				int j,temp_p;
				for(j=0;j<i;j++)
				{
					if(data[j].left>left_temp)
					{
						temp_p=j;
						break;
					}
				}
				for(j=i;j>temp_p;j--)
					data[j]=data[j-1];

				data[temp_p].left=left_temp;
				fscanf(input,"%d",&data[temp_p].right);
			}
		}


	}
	else
		for(i=0;i<max;i++)
			fscanf(input,"%d %d",&data[i].left,&data[i].right);*/
	printf("Start sorting...\n");
	clock_t sort_start=clock();

	for(i=0;i<max;i++)
	{
		fscanf(input,"%d %d",&data[i].left,&data[i].right);

		int temp=i;
		int j;
		for(j=0;j<i;j++)
		{//when i is 0, this block will not be executed
			if(line[j]->left>data[i].left)//Find a pair whose left is larger than this pair's left
			{
				temp=j;
				break;
			}
		}
		for(j=i;j>temp;j--)
		{//adjust the order accroading to left
			line[j]=line[j-1];
		}
		line[temp]=&data[i];
	}

	clock_t sort_end=clock();
	sort_time_use=(double)(sort_end-sort_start)/CLOCKS_PER_SEC;
	printf("sorting time use: %.3f sec\n",sort_time_use);

	fclose(input);

	#ifdef TEST
		for(i=0;i<max;i++)
			printf("left:%d right:%d\n",line[i]->left,line[i]->right);
	#endif

	clock_t com_start=clock();

//printf("%d\n",max);

	unsigned long long length=0;

	printf("Start computing...\n");

	for(i=0;i<max;i++)
	{
		int j;
		for(j=0;j<i;j++)
		{
			if(line[j]!=NULL)
			{
				#ifdef TEST
					printf("left:%d right:%d\n",line[j]->left,line[j]->right);
				#endif
				if(line[i]->left>=line[j]->right)//No overlapping
					line[j]=NULL;
				else
					length+=line_comp(*line[j],*line[i]);
			}
		}
		//This length is total overlapping length of total overlapping length of each Line object compared to the Line objects whose left are smaller.
		/*int j;
		for(j=i+1;j<max;j++)
		{
			length+=line_comp(data[i],data[j]);
#ifdef TEST
printf("%d %d\n",i,j);
#endif
		}*/
	}

	clock_t com_end=clock();

	double com_time_use=(double)(com_end-com_start)/CLOCKS_PER_SEC;

	printf("total overlapping length: %llu\n",length);
	printf("computing time use: %.3f sec\n",com_time_use);
	printf("total time use: %.3f sec\n",com_time_use+sort_time_use);
	return 0;
}
