#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>


typedef enum _BOOLEAN { FALSE = 0, TRUE = 1 } BOOLEAN;
BOOLEAN MemoryAlloc(int*** v, int size);
void ReadArray(int** v, int size);
void ReadInteger(int* x);
void main(int)
{
	int** v = NULL;
	int size, i;
	printf("Dimensão do vector: ");
	scanf("%d", &size);
	if (MemoryAlloc(&v, size))
	{
		ReadArray(v, size);
		for (i = 0; i < size; i++)
			printf("V[%d] - %d\n", i, **(v + i));
		for (i = 0; i < size; i++)
			free(*(v + i)); //*(v+i)==v[i]
		free(v);
	}
	else
		printf("ERRO na alocação de memória");
}
BOOLEAN MemoryAlloc(int*** v, int size)
{ 
	int i, j;
	if ((*v = (int**)malloc(size * sizeof(int*))) == NULL)
		return(FALSE);
	for (i = 0; i < size; i++)
		if ((*(*v + i) = (int*)malloc(sizeof(int))) == NULL)
		{
			for (j = i - 1; j >= 0; j--)
				free(*(*v + i));
			return(FALSE);
		}
	return(TRUE);
}

void ReadArray(int** v, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		printf("v[%d]: ", i);
		ReadInteger(*(v + i));
	}
}
void ReadInteger(int* x)
{
	printf("Introduza um inteiro: ");
	scanf("%d", x);
}