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

void printId(int size, double matriz[size][size+1])
{
	int i,j;
	for(i=0;i<size;i++)
	{
		for(j=size+1;j<size*2;j++)
			printf("%.2lf ", matriz[i][j]);
		printf("\n");
	}
printf("\n");
}

void printMatrix(int size, double matriz[size][size+1])
{
	int i,j;
	for(i=0;i<size;i++)
		{
			for(j=0;j<size;j++)
				printf("%.2lf ", matriz[i][j]);
			printf("\n");
		}
		printf("\n");
}

void multM(int size, double matriz[size][size+1])
{
	double result[size][size];
	int i,j,k;
	for(i=0; i<size; ++i)
        for(j=size; j<size*2; ++j)
            for(k=0; k<size; ++k)
            {
                result[i][j-size]+=matriz[i][k]*matriz[k][j];
            }
    printMatrix(size, result);
}

void printResultado(int size, double matriz[size][size+1])
{
	int j=size+1;
	for(j;j<size*2+1;j++)
	{
		printf("%.2lf\n",matriz[j-size-1][j]);
	}
}


void printall(int size, double matriz[size][size+1])
{
	int i,j;
	for(i=0;i<size;i++)
	{
		for(j=0;j<size*2+1;j++)
		{
			printf("%.2lf ", matriz[i][j]);
		}
		printf("\n");
	}
}
int main(int argc, char* argv[]) {

	FILE *fp = fopen("matriz.txt","r+");
	FILE *fp2 = fopen("vetor.txt", "r+");

	int count = getFileSize(fp);
	rewind(fp);
	double matriz[count][count+1];
	int i=0,j=0;


	for (i = 0; i < count; i++) {
		for (j = 0; j < count; j++) {
			fscanf(fp,"%lf",&matriz[i][j]);
		}

		fscanf(fp2,"%lf",&matriz[i][j]);
	}

	int k;
	double c;
	int tid;
	int nthreads;
	#pragma omp parallel for num_threads(count) private(i,k,j)
	for (j = 0; j < count; j++) {
		// tid = omp_get_thread_num();
		// printf("%d\n", tid);	
		for (i = 0; i < count; i++) {
						if (i == j) {
							continue;
						}

            c = matriz[i][j] / matriz[j][j];
            for (k = 0; k < count + 1; k++) {
                matriz[i][k] = matriz[i][k] - (c * matriz[j][k]);
            }
        }
    }

    double x[count];
    #pragma omp parallel for num_threads(count) private(i)
    for (i = 0; i < count; i++) 
        x[i] = matriz[i][count] / matriz[i][i];
    for(i=0;i<count;i++)
    	printf("%f\n",x[i]);

	return 0;
}
