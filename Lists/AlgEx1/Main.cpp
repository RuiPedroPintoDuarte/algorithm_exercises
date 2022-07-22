#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

typedef enum _BOOLEAN { FALSE = 0, TRUE = 1 } BOOLEAN;
typedef enum _LIST_LOCATION { LIST_START, LIST_END } LIST_LOCATION;
typedef enum _STATUS { OK, ERROR } STATUS;

#define DATA(node) ((node)->data)
#define NEXT(node) ((node)->next)

#define EMPTY NULL
#define NO_LINK NULL
#define MAX 50

typedef struct _LIST_NODE
{
	void* data;
	struct _LIST_NODE* next;
} LIST_NODE;

typedef LIST_NODE* LIST;

typedef struct _ESTACAO
{
	char desig[MAX];
	float custo;
	BOOLEAN ativa, no;
}ESTACAO;

void initList(LIST* list);
void FreeList(LIST* list);
BOOLEAN emptyList(LIST list);
LIST_NODE* NewNode(void* data);
STATUS RemoveNode(LIST* list, void* data);
STATUS InsertIni(LIST* list, void* data);
STATUS InsertEnd(LIST* list, void* data);

int ListSize(LIST list);
void ShowValues(LIST list);

ESTACAO* FindStationName(LIST list, char* str);

int Menu();
LIST ChosList(LIST line1, LIST line2, LIST line3, LIST line4, LIST line5);

STATUS SaveLine(LIST list, const char* filename);
STATUS ReadLine(LIST* list, const char* filename);
void AddEstacao(LIST* list);
void RemoveEstacao(LIST* list);
float Custo(LIST list, char* origem, char* destino);

int main()
{
	LIST linha1 = NULL, linha2 = NULL, linha3 = NULL, linha4 = NULL, linha5 = NULL, linha = NULL;
	ESTACAO* ptr = NULL, * orig = NULL, * dest = NULL;
	char str[20], origem[30], destino[30];
	int est = 0, aux = 0;
	void* pt = NULL;

	if ((ReadLine(&linha1, "Linha1.txt") == OK)
		&& (ReadLine(&linha2, "Linha2.txt") == OK)
		&& (ReadLine(&linha3, "Linha3.txt") == OK)
		&& (ReadLine(&linha4, "Linha4.txt") == OK)
		&& (ReadLine(&linha5, "Linha5.txt") == OK))
	{

		switch (Menu()) {
		case 0:
			return 0;
			break;
		case 1: //Ativar/Desativar uma estacao

			linha = ChosList(linha1, linha2, linha3, linha4, linha5);

			printf("Insere o nome da estação\n >> ");
			while ((getchar()) != '\n');
			gets_s(str);


			if ((ptr = FindStationName(linha, str)) != NULL)
			{

				if ((ptr->ativa) == FALSE)
				{
					ptr->ativa = TRUE;
					printf("Estacao ativada");
				}
				else
				{
					ptr->ativa = FALSE;
					printf("Estacao desativada");
				}
			}
			break;

		case 2: //Adicionar uma estacao a linha

			linha = ChosList(linha1, linha2, linha3, linha4, linha5);
			AddEstacao(&linha);
			ShowValues(linha);
			break;

		case 3: //Remover uma estacao da linha

			linha = ChosList(linha1, linha2, linha3, linha4, linha5);
			RemoveEstacao(&linha);
			break;

		case 4: //Guardar linhas 

			if ((SaveLine(linha1, "Linha1_.txt") == OK)
				&& (SaveLine(linha2, "Linha2_.txt") == OK)
				&& (SaveLine(linha3, "Linha3_.txt") == OK)
				&& (SaveLine(linha4, "Linha4_.txt") == OK)
				&& (SaveLine(linha5, "Linha5_.txt") == OK))
			{
				printf("Linhas guardadas com sucesso\n");
			}
			break;
		case 5: //Calcular custo da viagem

			linha = ChosList(linha1, linha2, linha3, linha4, linha5);

			printf("Insere a estação de origem\n >> ");
			scanf("%s", origem);
			printf("Insere s estação de destino\n >> ");
			scanf("%s", destino);

			printf("Custo = %.1f euros", Custo(linha, origem, destino));

			break;
		default:
			printf("Opção inválida");
			break;

		}
	}
	else printf("Erro na leitura de linhas\n");
	return 0;
}

/****************************************************************
* Funcao: Inicializa a lista
*
* Parametros: list - apontador para lista
* (duplo apontador para o primeiro no')
* Saida: void
***************************************************************/
void initList(LIST* list)
{
	*list = NULL;
}

/****************************************************************
* Funcao: verifica se a lista é vazia
*
* Parametros: list - apontador para lista
* Saida: TRUE se a lista for vazia, ERROR caso contrário
***************************************************************/
BOOLEAN emptyList(LIST list)
{
	return (list == NULL) ? TRUE : FALSE;
}

/****************************************************************
* Funcao: Cria um no'
*
* Parametros: data - apontador generico para os dados a inserir no no' criado
* Saida: apontador para o no' criado ou NULL em caso de erro
***************************************************************/
LIST_NODE* NewNode(void* data)
{
	LIST_NODE* new_node;
	if ((new_node = (LIST_NODE*)malloc(sizeof(LIST_NODE))) != NULL)
	{
		DATA(new_node) = data;
		NEXT(new_node) = NULL;
	}
	return(new_node);
}

/****************************************************************
* Funcao: Insere um no' no inicio da lista
*
* Parametros: list - apontador para lista
* (duplo apontador para o primeiro no')
* data - apontador generico para os dados a inserir no no' criado
* Saida: OK se o nó foi inserido na LISTA e ERROR caso contrário
***************************************************************/
STATUS InsertIni(LIST* list, void* data)
{
	LIST_NODE* new_node;
	if ((new_node = NewNode(data)) != NULL)
	{
		NEXT(new_node) = *list;
		*list = new_node;
		return OK;
	}
	return ERROR;
}

/****************************************************************
* Funcao: Insere um no' no fim da lista
*
* Parametros: list - apontador para lista
* (duplo apontador para o primeiro no')
* data - apontador generico para os dados a inserir no no' criado
* Saida: OK se o nó foi inserido na LISTA e ERROR caso contrário
***************************************************************/
STATUS InsertEnd(LIST* list, void* data)
{
	LIST_NODE* new_node, * temp;
	if ((new_node = NewNode(data)) != NULL)
	{
		if (*list == NULL)
			*list = new_node;
		else
		{
			temp = *list;
			while (NEXT(temp) != NULL)
				temp = NEXT(temp);
			NEXT(temp) = new_node;
		}
		return(OK);
	}
	return(ERROR);
}

/***************************************************************************
* Funcao: calcula quantos elementos contem a lista
*
* Parametros: list - apontador para uma lista
* Saida: numero de elementos da lista
***************************************************************************/
int ListSize(LIST list)
{
	int count = 0;
	while (list != NULL)
	{
		count++;
		list = NEXT(list);
	}
	return count;
}

/****************************************************************
* Funcao: Escreve no ecra o conteudo da lista
*
* Parametros: list - apontador para o primeiro no'
* Saida: void
***************************************************************/
void ShowValues(LIST list)
{
	if (emptyList(list) == TRUE)
	{
		printf("\nLista vazia.\n");
		return;
	}
	printf("LISTA ->\n");
	while (list != NULL)
	{
		printf("%s;dd%.1f;%d;%d;\n", ((ESTACAO*)DATA(list))->desig, ((ESTACAO*)DATA(list))->custo, ((ESTACAO*)DATA(list))->ativa, ((ESTACAO*)DATA(list))->no);
		list = NEXT(list);
	}
	printf(" FIM");
}

STATUS SaveLine(LIST list, const char* filename)
{
	FILE* fp = NULL;
	ESTACAO* pt = NULL;
	int ativa = 0, no = 0;

	if ((fp = fopen(filename, "w")) != NULL)
	{
		while (list != NULL)
		{
			pt = (ESTACAO*)DATA(list);
			if (pt->ativa) ativa = 1;
			if (pt->no) no = 1;
			fprintf(fp, "%c;%.1f;%d;%d;\n", pt->desig, pt->custo, ativa, no);
			list = NEXT(list);
		}
		fclose(fp);
		printf("Linha salva com sucesso\n");
		return OK;
	}
	else {
		fclose(fp);
		printf("ERRO\n");
		return ERROR;
	}

}

STATUS ReadLine(LIST* list, const char* filename)
{
	FILE* fp = NULL;
	ESTACAO* pt = NULL;
	int ativa = 0, no = 0;

	if (((fp = fopen(filename, "r")) != NULL))
	{
		while (!feof(fp))
		{
			if (((pt = (ESTACAO*)malloc(sizeof(ESTACAO))) != NULL) && (InsertEnd(list, pt) == OK))
			{
				fscanf(fp, "%[^;];%f;%d;%d;\n", pt->desig, &(pt->custo), &ativa, &no);
				pt->ativa = pt->no = FALSE;
				if (ativa)
					pt->ativa = TRUE;
				if (no)
					pt->no = TRUE;
			}
			else
			{
				FreeList(list);
				return (ERROR);
			}
		}
		fclose(fp);
		return(OK);
	}
	return(ERROR);
}

void AddEstacao(LIST* list)
{
	ESTACAO* ptr;
	int ativa, no;
	if (((ptr = (ESTACAO*)malloc(sizeof(ESTACAO))) != NULL) && (InsertEnd(list, ptr) == OK))
	{
		ptr->ativa = ptr->no = FALSE;
		printf("Designação: ");
		scanf("%s", &ptr->desig);
		printf("Custo: ");
		scanf("%f", &ptr->custo);
		printf("Ativo: ");
		scanf("%d", &ativa);
		if (ativa) ptr->ativa = TRUE;
		printf("No: ");
		scanf("%d", &no);
		if (no) ptr->no = TRUE;
	}
}

void RemoveEstacao(LIST* list)
{
	char str[30];
	ESTACAO* ptr;

	//ShowValues(*list);

	printf("\n\n\n");
	printf("Designação da estação: ");
	while ((getchar()) != '\n');
	gets_s(str);

	if ((ptr = FindStationName(*list, str)) != NULL)
		RemoveNode(list, ptr);
	else
		printf("Estação inexistente\n");

	//ShowValues(*list);
}

float Custo(LIST list, char* origem, char* destino)
{
	bool flag = 0;
	float custo = 0;

	while (list != NULL)
	{
		if (flag)
		{
			if (!strcmp(((ESTACAO*)DATA(list))->desig, destino)) {
				return custo;
			}
			else custo = custo + ((ESTACAO*)DATA(list))->custo;
		}

		if (!strcmp(((ESTACAO*)DATA(list))->desig, origem))
		{
			flag = 1;
			custo = custo + ((ESTACAO*)DATA(list))->custo;
		}

		list = NEXT(list);
	}
	return -1;
}

STATUS RemoveNode(LIST* list, void* data)
{
	LIST_NODE* temp = *list, * ant = *list;

	while (temp != NULL)
	{
		if (temp->data == data) {
			if (temp = *list)   *list = (*list)->next;
			else                ant->next = temp->next;

			free(temp->data);
			free(temp);

			return OK;
		}
		ant = temp;
		temp = temp->next;
	}
	return ERROR;
}

void FreeList(LIST* list)
{
	LIST_NODE* node;
	while (*list != NULL)
	{
		node = (*list)->next;
		free((*list)->data);
		free(*list);
		*list = node;
	}
}

int Menu()
{
	int op;
	do {
		printf("\n===================================\n");
		printf("\t\tMENU\t\n");
		printf("===================================\n");
		printf("  1. Ativar/Desativar uma estacao\n");
		printf("  2. Adicionar uma estacao\n");
		printf("  3. Eliminar uma estacao\n");
		printf("  4. Guardar as linhas da rede\n");
		printf("  5. Calcular custo de viagem\n");
		printf("  0. Sair\n");

		printf("\n >> ");
		scanf_s("%d", &op);
	} while (op < 0 && op >5);

	return op;
}

LIST ChosList(LIST line1, LIST line2, LIST line3, LIST line4, LIST line5)
{
	int est;

	printf("Insere a linha\n >> ");
	scanf("%d", &est);

	switch (est) {
	case 1: return line1;
		break;
	case 2: return line2;
		break;
	case 3: return line3;
		break;
	case 4: return line4;
		break;
	case 5: return line5;
		break;
	default:
		printf("Linha nao existe \n");
		break;
	}
}

ESTACAO* FindStationName(LIST list, char* str)
{
	while (list != NULL)
	{
		if (!strcmp(((ESTACAO*)list->data)->desig, str))
		{
			printf("Estacao encontrada\n");
			return (ESTACAO*)DATA(list);
		}
		list = NEXT(list);
	}
	return NULL;

}

BOOLEAN EstacaoExiste(LIST lista, char* nome)
{
	while (lista != NULL) {
		if (!strcmp(((ESTACAO*)DATA(lista))->desig, nome))
			return TRUE;
		lista = NEXT(lista);
	}
	return FALSE;
}

float CustoLinha(LIST lista) {
	float total = 0;
	while (lista != NULL) {
		total += ((ESTACAO*)DATA(lista))->custo;
		lista = NEXT(lista);
	}
	return total;
}