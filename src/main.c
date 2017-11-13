#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int getFileSize(FILE *fp, int size)
{
	rewind(fp);
	char buf[size*size];
	int count=0;
	while(fgets(buf,sizeof(buf),fp) != NULL)
	  count++;
	rewind(fp);
	return count;
}

void printall(int size, double matriz[size][size+1])
{
	int i,j;
	for(i=0;i<size;i++)
	{
		for(j=0;j<size+1;j++)
		{
			printf("%.2lf ", matriz[i][j]);
		}
		printf("\n");
	}
}

void randomMatriz(FILE *fp, FILE *fp2, int size)
{
	int i,j;
	for(i=0;i<size;i++)
	{
		for(j=0;j<size-1;j++)
		{
			fprintf(fp,"%d ", (rand()%1000)+1);
		}
		fprintf(fp,"%d\n", (rand()%1000)+1);
		fprintf(fp2,"%d\n", (rand()%1000)+1);
	}
	rewind(fp);
	rewind(fp2);
}

int main(int argc, char* argv[]) {

	FILE *fp = fopen("matriz.txt","w+");
	FILE *fp2 = fopen("vetor.txt", "w+");
	FILE *fp3 = fopen("resultado.txt", "w+");
	int count;
	sscanf(argv[1],"%d",&count);
	randomMatriz(fp, fp2, count);
	double matriz[count][count+1];
	int i=0,j=0;


	for (i = 0; i < count; i++) {
		for (j = 0; j < count; j++) {
			fscanf(fp,"%lf",&matriz[i][j]);
		}
		fscanf(fp2,"%lf",&matriz[i][j]);
	}
	//printall(count, matriz);
	int teste;

	int k;
	double c;
	for (j = 0; j < count; j++) {
        for (i = 0; i < count; i++) {
			if (i == j) {
				continue;
			}
			c = matriz[i][j] / matriz[j][j];
            for (k = 0; k < count + 1; k++) {
                matriz[i][k] = matriz[i][k] - (c * matriz[j][k]);
            }
        }
        //printall(count, matriz);
    }
    double x[count];
    for (i = 0; i < count; i++) {
        x[i] = matriz[i][count] / matriz[i][i];
        fprintf(fp3,"%.2lf\n",x[i]);
        //printf("%.2lf\n",x[i]);
    }

    fclose(fp);
    fclose(fp2);
    fclose(fp3);

	return 0;
}
