#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 10000

int getFileSize(FILE *fp)
{
	char buf[MAX_SIZE];
	int count=0;
	while(fgets(buf,sizeof(buf),fp) != NULL)
	  count++;
	return count;
}

int main(int argc, char* argv[]) {

	FILE *fp = fopen("matriz.txt","r+");

	int count = getFileSize(fp);
	rewind(fp);
	int matriz[count][count];
	int i=0,j=0;
	int test;
	for(i=0;i<count;i++)
		for(j=0;j<count;j++)
			fscanf(fp,"%d",&matriz[i][j]);
				


	for(i=0;i<count;i++)
	{
		for(j=0;j<count;j++)
		{
			printf("%d ",matriz[i][j]);
		}
		printf("\n");
	}
	fclose(fp);
	return 0;
}
