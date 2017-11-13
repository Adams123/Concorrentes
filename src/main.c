#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 10000

int getFileSize(FILE *fp) {
	int count = 0;
	while (!feof(fp)) {
		if ((fgetc(fp) == '\n')) {
			count++;
		}
	}

	return count;
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

float** initMatriz(int rows, int cols) {
	float* data = (float*) malloc(rows*cols*sizeof(float));
	float** matriz = (float**) malloc(rows*sizeof(float*));
	for (size_t i = 0; i < rows; i++) {
		matriz[i] = &(data[i*cols]);
	}

	return matriz;
}

void freematriz(float** matriz) {
	if (matriz == NULL) {
		return;
	}

	free(matriz[0]);
	free(matriz);
	return;
}

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	float** matriz = NULL;
	int i=0,j=0;
	int count;

	void* send_ptr = NULL;
	if (world_rank == 0) {
		printf("\n");
		FILE *fp = fopen("matriz.txt","r+");
		FILE *fp2 = fopen("vetor.txt", "r+");

		count = getFileSize(fp);
		rewind(fp);

		matriz = initMatriz(count, count);
		send_ptr = &(matriz[0][0]);
		float value = 0;
		for (i = 0; i < count; i++) {
			for (j = 0; j < count; j++) {
				fscanf(fp,"%f",&matriz[i][j]);
			}

			fscanf(fp2,"%f",&matriz[i][j]);
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Bcast(&count, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int cols_per_proc = (count + 1) / world_size;
	float** sub_matriz = initMatriz(count, cols_per_proc);

	MPI_Datatype matriz_col, matriz_col_type, sub_col, sub_col_type;
	if (world_rank == 0) {
		MPI_Type_vector(count, 1, count, MPI_FLOAT, &matriz_col);

		MPI_Type_commit(&matriz_col);
		MPI_Type_create_resized(matriz_col, 0, 1*sizeof(float), &matriz_col_type);
	}

	MPI_Type_vector(count, 1, cols_per_proc, MPI_FLOAT, &sub_col);

	MPI_Type_commit(&sub_col);
	MPI_Type_create_resized(sub_col, 0, 1*sizeof(float), &sub_col_type);
	MPI_Type_commit(&sub_col_type);

	MPI_Scatter(send_ptr, cols_per_proc, matriz_col_type, &(sub_matriz[0][0]), cols_per_proc, sub_col_type, 0, MPI_COMM_WORLD);

	int k;
	double c;

	for (j = 0; j < cols_per_proc; j++) {
		for (i = 0; i < count; i++) {
			if (i == ((cols_per_proc * world_rank) + j)) {
				continue;
			}

	    c = sub_matriz[i][j] / sub_matriz[j][j];
	    for (k = 0; k < cols_per_proc; k++) {
	      sub_matriz[i][k] = sub_matriz[i][k] - (c * sub_matriz[j][k]);
	    }
	  }

	}

	MPI_Gather(&(sub_matriz[0][0]), cols_per_proc, sub_col_type, send_ptr, 1, matriz_col_type, 0, MPI_COMM_WORLD);
	if (world_rank == 0) {
		for (j = 0; j < count; j++) {
			for (i = 0; i < count; i++) {
				printf("%f ", matriz[i][j]);
			}
			printf("\n");
		}

		float x[count];
    for (i = 0; i < count; i++) {
        x[i] = matriz[i][count+1] / matriz[i][i];
        printf("\nx%d=%f\n", i, x[i]);
    }
	}

	freematriz(matriz);
	MPI_Finalize();
	return 0;
}
