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

void printMatrix(int size, double matriz[size][size+1])
{
	int i,j;
	for(i=0;i<size;i++)
		{
			for(j=0;j<=size;j++)
				printf("%.2lf ", matriz[i][j]);
			printf("\n");
		}
}

int main(int argc, char* argv[]) {

	FILE *fp = fopen("matriz.txt","r+");
	FILE *fp2 = fopen("vetor.txt","r+");

    if (fp == NULL || fp2 == NULL) {
        printf("Files not found\n");
        return 1;
    }

	int count = getFileSize(fp);
	rewind(fp);
	double matriz[count][count+1];
	int i=0,j=0;

	for(i=0;i<count;i++)
	{
		for(j=0;j<count;j++)
		{
			fscanf(fp,"%lf",&matriz[i][j]);
		}
		fscanf(fp2,"%lf",&matriz[i][count]);
	}
	int row=0;
	for(i=0;i<count;i++)
	{
		int aii = matriz[i][i];
		for(j=0;j<=count;j++)
			matriz[i][j]=matriz[i][j]/aii;
		for(row=0;row<count;row++)
			if(row!=i)
			{
				double el = matriz[row][i];
				for(j=i;j<=count;j++)
					matriz[row][j]=matriz[row][j]-(el*(matriz[i][j]));
			}
	}

	printMatrix(count, matriz);

	return 0;
}
