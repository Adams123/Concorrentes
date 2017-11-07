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
	FILE *fp2 = fopen("vetor.txt","r+");

	int count = getFileSize(fp);
	rewind(fp);
	int matriz[count][count+1];
	int i=0,j=0;

	for(i=0;i<count;i++)
	{
		for(j=0;j<count;j++)
		{
			fscanf(fp,"%d",&matriz[i][j]);
		}
		fscanf(fp2,"%d",&matriz[i][count])
	}
	int row=0;
	for(i=0;i<count;i++)
	{
		int aii = matriz[i][i];
		for(j=0;j<=count;j++)
		{
			matriz[i][j]/=aii;
		}
		int aii = matriz[i][i]; //atualiza pivot para usos futuros
		for(row=0;row<count;row++)
			if(row!=i)
				matriz[i][j]=matriz[i][j]-(matriz[j][i]*(matriz[i][j]));
	}


	return 0;
}
