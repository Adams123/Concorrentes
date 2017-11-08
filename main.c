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

void printMatrix(int size, float matriz[size][size+1])
{
	int i,j;
	for(i=0;i<size;i++)
		{
			for(j=0;j<=size;j++)
				printf("%.2f ", matriz[i][j]);
			printf("\n");
		}
}

int main(int argc, char* argv[]) {

	FILE *fp = fopen("matriz.txt","r+");
	FILE *fp2 = fopen("vetor.txt","r+");

	int count = getFileSize(fp);
	rewind(fp);
	float matriz[count][count+1];
	int i=0,j=0;

	for(i=0;i<count;i++)
	{
		for(j=0;j<count;j++)
		{
			fscanf(fp,"%f",&matriz[i][j]);
		}
		fscanf(fp2,"%f",&matriz[i][count]);
	}
	int row=0;
	for(i=0;i<2;i++)
	{
		int aii = matriz[i][i];
		for(j=0;j<=count;j++)
		{
			matriz[i][j]/=aii;
		}
		printf("New pivot %d\n", aii);
		printf("%f\n", matriz[j][i]);
		for(row=0;row<count;row++)
			if(row!=i)
				matriz[row][j]=matriz[row][j]-(matriz[j][row]*(matriz[row][j]));
	}

	printMatrix(count, matriz);

	return 0;
}
