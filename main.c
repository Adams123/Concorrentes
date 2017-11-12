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

	int npes, myrank, src, dest, msgtag, ret;
	int required=MPI_THREAD_SERIALIZED;
	int provided;
	MPI_Status status;
	MPI_Init_thread(&argc, &argv, required, &provided);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	FILE *fp = fopen("matriz.txt","r+");
	FILE *fp2 = fopen("vetor.txt","r+");

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

 	MPI_Datatype col, coltype;

	MPI_Type_vector(nrows,1, ncols, MPI_DOUBLE, &col);       

	MPI_Type_commit(&col);
	MPI_Type_create_resized(col, 0, 1*sizeof(double), &coltype);
	MPI_Type_commit(&coltype);


	int row=0;
	for(i=0;i<count;i++)
	{
		int aii = matriz[i][i];
		for(j=0;j<=count;j++)
			matriz[i][j]=matriz[i][j]/aii;
		for(row=0;row<count;row++)
			if(row!=i)
			{
				double pivot = matriz[row][i];
				for(j=i;j<=count;j++)
					matriz[row][j]=matriz[row][j]-(pivot*(matriz[i][j]));
			}
	}

	printMatrix(count, matriz);

	MPI_Finalize();

	return 0;
}
