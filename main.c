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

void printMatrix(int size, double matriz[size][size*2])
{
	int i,j;
	for(i=0;i<size;i++)
		{
			for(j=0;j<size*2;j++)
				printf("%.2lf ", matriz[i][j]);
			printf("\n");
		}
}

void printResultado(int size, double matriz[size][size*2])
{
	int j=size;
	for(j;j<size*2;j++)
	{
		printf("%.2lf\n",matriz[j-size][j]);		
	}
}

int main(int argc, char* argv[]) {

	FILE *fp = fopen("matriz.txt","r+");

	int count = getFileSize(fp);
	rewind(fp);
	double matriz[count][count*2];
	int i=0,j=0;


	for(i=0;i<count;i++)
	{
		for(j=0;j<count;j++)
			fscanf(fp,"%lf",&matriz[i][j]);
		for(j;j<count*2;j++)
			matriz[i][j]=0;
	}

	for(j=count;j<count*2;j++)
		matriz[j-count][j]=1;

	int row=0;
	for(i=0;i<1;i++)
	{
		int aii = matriz[i][i];
		for(j=0;j<=count;j++)
			matriz[i][j]=matriz[i][j]/aii;
		for(row=0;row<count;row++)
			if(row!=i)
			{
				double pivot = matriz[row][i];
				for(j=i;j<count*2;j++)
					matriz[row][j]=matriz[row][j]-(pivot*(matriz[i][j]));
			}
	}

	printMatrix(count, matriz);

	printResultado(count, matriz);

	return 0;
}
