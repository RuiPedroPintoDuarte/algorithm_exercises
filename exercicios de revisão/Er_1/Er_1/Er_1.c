#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef enum _BOOLEAN { FALSE = 0, TRUE = 1 } BOOLEAN;
int* MemoryAlloc(int dim);
void ReadArray(int* w, int dim);
void main(void)
{
	int dim, i, * v = NULL;
	printf("Dimensão do vector: ");
	scanf("%d", &dim);
	if ((v = MemoryAlloc(dim)) == NULL)
		printf("ERRO na alocação de memória");
	else
	{
		ReadArray(v, dim);
		for (i = 0; i < dim; i++)
			printf("V[%d] = %d\n", i, *(v + i)); //*(v+i)==v[i]
		free(v);
	}
}
int* MemoryAlloc(int dim)
{
	int* w = NULL;
	w = (int*)malloc(dim * sizeof(int));
	return(w);
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