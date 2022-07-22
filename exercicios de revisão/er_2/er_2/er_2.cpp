#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
typedef enum _BOOLEAN { FALSE = 0, TRUE = 1 } BOOLEAN;
typedef enum _STATUS { ERROR = 0, OK = 1 } STATUS;
STATUS MemoryAlloc(int** w, int dim);
void ReadArray(int* w, int dim);
void main(void)
{
	int dim, i, * v = NULL;
	printf("Dimensão do vector: ");
	scanf("%d", &dim);
	if (MemoryAlloc(&v, dim))
	{
		ReadArray(v, dim);
		for (i = 0; i < dim; i++)
			printf("V[%d] = %d\n", i, *(v + i)); //*(v+i)==v[i]
		free(v);
	}
	else
		printf("ERRO na alocação de memória");
}
STATUS MemoryAlloc(int** w, int dim)
{
	if ((*w = (int*)malloc(dim * sizeof(int))) == NULL)
		return(ERROR);
	return(OK);
}
void ReadArray(int* w, int dim)
{
	int i;
	for (i = 0; i < dim; i++)
	{
		printf("\tV[%d]= ", i);
		scanf("%d", w + i);
	}
}