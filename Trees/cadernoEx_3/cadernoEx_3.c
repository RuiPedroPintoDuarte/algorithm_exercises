#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_NAME 20
#define STAGES 15 //Eliminatórias


typedef struct _PLAYER
{
	char name[MAX_NAME];
	int sets;
}PLAYER;


typedef struct _BTREE_NODE
{
	void* data;
	struct _BTREE_NODE* left;
	struct _BTREE_NODE* right;
} BTREE_NODE;


typedef BTREE_NODE* BTREE;//ponteiro para um nó

typedef enum _BTREE_LOCATION { BTREE_LEFT, BTREE_RIGHT } BTREE_LOCATION;//Não usamos em principio
typedef enum _BOOLEAN { FALSE = 0, TRUE = 1 } BOOLEAN;
typedef enum _STATUS { ERROR = 0, OK = 1 } STATUS;



#define DATA(node) ((node)->data)
#define LEFT(node) ((node)->left)
#define RIGHT(node) ((node)->right)
#define EMPTY NULL
#define NO_LINK NULL



BTREE_NODE* NewBtreeNode(void* data); //Novo nó
BTREE_NODE* AddBtreeNode(BTREE_NODE* upnode, BTREE_NODE* node, BTREE_LOCATION where);//Adicionar nó
int BtreeSize(BTREE btree); //Tamanho da arvore
int BtreeDeep(BTREE btree); //Altura da arvore
BOOLEAN BtreeLeaf(BTREE_NODE* node);//Se é folha ou não; Nó Folha -> são os nós de baixo porque não têm filhos
BTREE_NODE* InitNode(void*, BTREE_NODE*, BTREE_NODE*);// Inicia o nó esq ou dir
BTREE_NODE* CreateBtree(void**, int, int);//Criar arvore a partir de um vetor
STATUS ReadPlayersFromFile(void**, char*);//para ler ficheiro
void PrintLeafs(BTREE);//Imprimir nós folhas
void BtreeFree(BTREE);//libertar memoria de uma arvore
void PrintWinnerGames(BTREE);//Vencedor de cada jogo
int CountTotalSets(BTREE);//Total de sets realizados
int CountWinnerSets(BTREE, void*);//Quantos sets o jogador que venceu o torneio ganhou
void PrintAllGames(BTREE);//Mostrar todos os jogos
void PrintGame(BTREE);
int CountLeafs(BTREE);
void RightLeaf(BTREE);


int main()
{
	BTREE Btree;
	void* players[STAGES];//vetor para guardar jogadores
	char file_name[MAX_NAME];//
	//printf("Nome do ficheiro: ");
	//scanf("%s", file_name);

	//if (ReadPlayersFromFile(players, file_name))
	if (ReadPlayersFromFile(players, "torneio.txt"))//Booleano
	{
		Btree = CreateBtree(players, 0, STAGES);//Retorna ponteiro para nó com tamanho de vetor
		printf("\nLista dos %d participantes: \n", CountLeafs(Btree));
		PrintLeafs(Btree);
		printf("\nLista de Jogos:\n");
		PrintAllGames(Btree);
		printf("\nNumero de eliminatorias: %d", BtreeDeep(Btree) - 1);
		printf("\nNumero de Jogos: %d", BtreeSize(Btree) / 2);
		printf("\nNumero de Sets: %d", CountTotalSets(Btree));
		printf("\nVencedor do torneio: %s\n", ((PLAYER*)DATA(Btree))->name);
		printf("\nJogos disputados pelo Vencedor:\n");
		PrintWinnerGames(Btree);
		printf("\nSets ganhos pelo Vencedor: %d\n", CountWinnerSets(Btree, DATA(Btree)));
		BtreeFree(Btree);
		system("pause");
	}
	else
		printf("ERRO na leitura do ficheiro\n");
	system("pause");
	return 0;
}

BTREE_NODE* NewBtreeNode(void* data)
{
	BTREE_NODE* tmp_pt;
	if ((tmp_pt = (BTREE_NODE*)malloc(sizeof(BTREE_NODE))) != NULL)
	{
		DATA(tmp_pt) = data;
		LEFT(tmp_pt) = RIGHT(tmp_pt) = NULL;
	}
	return tmp_pt;
}

BTREE_NODE* AddBtreeNode(BTREE_NODE* upnode, BTREE_NODE* node, BTREE_LOCATION where)
{
	BTREE_NODE* tmp_pt = upnode;
	if (where == BTREE_LEFT)
	{
		if (LEFT(upnode) == NULL)
			LEFT(upnode) = node;
		else
			tmp_pt = NULL;
	}
	else
	{
		if (RIGHT(upnode) == NULL)
			RIGHT(upnode) = node;
		else
			tmp_pt = NULL;
	}
	return tmp_pt;
}
BTREE_NODE* InitNode(void* ptr_data, BTREE_NODE* node1, BTREE_NODE* node2)
{
	BTREE_NODE* tmp_pt = NULL;
	tmp_pt = NewBtreeNode(ptr_data);
	LEFT(tmp_pt) = node1;
	RIGHT(tmp_pt) = node2;
	return(tmp_pt);
}
BTREE_NODE* CreateBtree(void** v, int i, int size)
{
	if (i >= size) //maior que o size
		return(NULL);
	else
		return(InitNode(*(v + i), CreateBtree(v, 2 * i + 1, size), CreateBtree(v, 2 * i + 2, size)));//Dados metidos na arvore, no na esquerda, no direita
}
void BtreeFree(BTREE btree)
{
	if (btree != NULL)
	{
		BtreeFree(LEFT(btree));
		BtreeFree(RIGHT(btree));
		free(btree);
	}
}
int BtreeSize(BTREE btree)
{
	int count = 0;
	if (btree != NULL)
		count = 1 + BtreeSize(LEFT(btree)) + BtreeSize(RIGHT(btree));
	return(count);
}
BOOLEAN BtreeLeaf(BTREE_NODE* btree)
{
	if ((LEFT(btree) == NULL) && (RIGHT(btree) == NULL))
		return(TRUE);
	else
		return(FALSE);
}
int BtreeDeep(BTREE btree)
{
	int deep = 0, left, right;
	if (btree != NULL)
	{
		left = BtreeDeep(LEFT(btree));
		right = BtreeDeep(RIGHT(btree));
		deep = 1 + ((left > right) ? left : right);
	}
	return(deep);
}
STATUS ReadPlayersFromFile(void** players, char* file_name)
{
	FILE* fp;   //pontiero po file
	int j, i = 0;//para iterar
	void* ptr_data;//guardar dados
	if ((fp = fopen(file_name, "r")) != NULL)//abrir leitura
	{
		while (!feof(fp))//enquanto nao chegar ao fim
		{
			if ((ptr_data = malloc(sizeof(PLAYER))) != NULL)//alocar memoria para um player
			{
				fscanf(fp, "%[^;];", ((PLAYER*)ptr_data)->name);//ler ficheiro string até ;
				fscanf(fp, "%d\n", &(((PLAYER*)ptr_data)->sets));//até \n lê os sets
				players[i] = ptr_data;
				i++;
			}
			else //deu erro, apagr todos os que já estavam no vetor
			{
				for (j = i; j >= 0; j--)
					free(players[j]);
				return(ERROR);
			}
		}
		fclose(fp);
		return(OK);
	}
	else //erro na abertura
		return(ERROR);
}
void PrintLeafs(BTREE btree)
{
	if (btree != NULL)
	{
		PrintLeafs(LEFT(btree));
		if (BtreeLeaf(btree))
			printf("\n%s\n", ((PLAYER*)DATA(btree))->name);
		PrintLeafs(RIGHT(btree));
	}
}

//Com dois exemplos
void PrintWinnerGames(BTREE btree)
{

	if (btree != NULL && !BtreeLeaf(btree))
	{
		PrintGame(btree);

		//if (!strcmp(((PLAYER*)DATA(btree))->name, ((PLAYER*)DATA(LEFT(btree)))->name))

		//	PrintWinnerGames(LEFT(btree));		
		//else
		//	PrintWinnerGames(RIGHT(btree));

		if (((PLAYER*)DATA(btree))->sets > ((PLAYER*)DATA(LEFT(btree)))->sets)
			PrintWinnerGames(RIGHT(btree));
		else
			PrintWinnerGames(LEFT(btree));

	}

}

int CountTotalSets(BTREE btree)
{
	int count = 0;

	if (btree != NULL)
		count = ((PLAYER*)DATA(btree))->sets + CountTotalSets(LEFT(btree)) + CountTotalSets(RIGHT(btree));
	return(count);
}

int CountWinnerSets(BTREE btree, void* winner)
{
	int count = 0;

	if (btree != NULL)
	{
		if (!strcmp(((PLAYER*)DATA(btree))->name, winner))
			count = ((PLAYER*)DATA(btree))->sets + CountWinnerSets(LEFT(btree), winner) + CountWinnerSets(RIGHT(btree), winner);
	}
	return (count);

	
}

void PrintGame(BTREE btree)
{
	if (btree != NULL && LEFT(btree) != NULL && RIGHT(btree) != NULL)
		printf("%s (%d) vs (%d) %s \n", ((PLAYER*)DATA(LEFT(btree)))->name, ((PLAYER*)DATA(LEFT(btree)))->sets, ((PLAYER*)DATA(RIGHT(btree)))->sets, ((PLAYER*)DATA(RIGHT(btree)))->name);
}

void PrintAllGames(BTREE btree)
{
	if (btree != NULL && !BtreeLeaf(btree))
	{
		PrintAllGames(LEFT(btree));
		PrintGame(btree);
		PrintAllGames(RIGHT(btree));
	}

}

int CountLeafs(BTREE btree) {

	int count = 0;

	if (btree != NULL) {
		if (BtreeLeaf(btree))
        count++;
        count += CountLeafs(RIGHT(btree)) + CountLeafs(LEFT(btree));
	}
		
	return count;
	
}

void RightLeaf(BTREE btree)
{
	if (btree != NULL) {
		if(BtreeLeaf(btree))
		printf("\n%s\n", ((PLAYER*)DATA(btree))->name);
        RightLeaf(RIGHT(btree));
	}
}

BTREE_NODE* MaisDireita(BTREE btree)
{
	if (btree == NULL)
		return NULL;
	if (RIGHT(btree) != NULL)
		return MaisDireita(RIGHT(btree));
}

-- - DIR * -- -

// 1 - Mostrar as Diretorias que tenham mais de 1000Kbs
void show1000(BTREE btree) {
	if (btree != NULL) {
		if (btree->left == NULL && btree->right == NULL)
			if (((DIR*)btree->data)->n_kb > 1000)
				printf("%s, %d, %d\n", ((DIR*)btree->data)->nome, ((DIR*)btree->data)->n_fich, ((DIR*)btree->data)->n_kb);
		show1000(btree->left);
		show1000(btree->right);
	}
}

// 2 - Somar o nº de ficheiros de uma dada diretoria+suas subdiretorias (parcialmente correto)
int totalFich(BTREE btree) { // soma left subtree com right subtree
	int soma = 0;

	if (btree != NULL)
		soma = ((DIR*)btree->data)->n_fich + totalFich(btree->left) + totalFich(btree->right);
	return soma;
}

int startingNode(BTREE btree, char* dir) { //soma a partir da DIR que introduzimos
	int soma = 0;
	if (btree != NULL) {
		if (!strcmp(dir, ((DIR*)btree->data)->nome))
			soma = totalFich(btree);
		startingNode(btree->left, dir);
		startingNode(btree->right, dir);
	}
	return soma;
}

// 3 - Mostrar o nº de DIRs vazias e quais são
int nEmptyDir(BTREE btree) {
	int cont = 0;
	if (btree != NULL) {
		if (btree->left == NULL && btree->right == NULL && ((DIR*)btree->data)->n_fich == 0 && ((DIR*)btree->data)->n_kb == 0)
			cont = 1 + nEmptyDir(btree->left) + nEmptyDir(btree->right);
		else
			cont = nEmptyDir(btree->left) + nEmptyDir(btree->right);
	}
	return cont;
}

void emptyDir(BTREE btree) {
	if (btree != NULL) {
		if (btree->left == NULL && btree->right == NULL)
			if (((DIR*)btree->data)->n_fich == 0 && ((DIR*)btree->data)->n_kb == 0)
				printf("\n%s, %d, %d", ((DIR*)btree->data)->nome, ((DIR*)btree->data)->n_fich, ((DIR*)btree->data)->n_kb);
		emptyDir(btree->left);
		emptyDir(btree->right);
	}
}

-- - DEPT * -- -

// 4 - Calcular o lucro total (ganho-despesa) (3 funcoes)
float calcGanho(BTREE btree) {
	float totalGanho = 0.0;
	if (btree != NULL) {
		if (btree->left == NULL && btree->right == NULL)
			totalGanho = ((DEPT*)btree->data)->ganhos + calcGanho(btree->left) + calcGanho(btree->right);
		else
			totalGanho = calcGanho(btree->left) + calcGanho(btree->right);
	}
	return totalGanho;
}

float calcDespesa(BTREE btree) {
	float totalDespesa = 0;
	if (btree != NULL) {
		if (btree->left == NULL && btree->right == NULL)
			totalDespesa = ((DEPT*)btree->data)->despesas + calcDespesa(btree->left) + calcDespesa(btree->right);
		else
			totalDespesa = calcDespesa(btree->left) + calcDespesa(btree->right);
	}
	return totalDespesa;
}

float showLucros(BTREE btree) {
	return (calcGanho(btree) - calcDespesa(btree));
}

// 5 - Mostrar DEPT com mais pessoas
int maiorPessoas(BTREE btree) {
	if (btree == NULL) return -1;

	int max = ((DEPT*)btree->data)->n_pessoas;

	if (max < maiorPessoas(btree->left))
		max = maiorPessoas(btree->left);

	if (max < maiorPessoas(btree->right))
		max = maiorPessoas(btree->right);

	return max;
}

void maiorDept(BTREE btree, int max) {
	if (btree != NULL) {
		if (((DEPT*)btree->data)->n_pessoas == max)
			printf("%s, %d, %.1f, %.1f\n", ((DEPT*)btree->data)->desig, ((DEPT*)btree->data)->n_pessoas, ((DEPT*)btree->data)->ganhos, ((DEPT*)btree->data)->despesas);
		else
			maiorDept(btree->left, max);
		maiorDept(btree->right, max);
	}
}

-- - PERSON * -- -

// 6 - Mostrar apenas os vivos
void showAlive(BTREE btree) {
	if (btree != NULL) {
		if (btree->left == NULL && btree->right == NULL)
			if (((PERSON*)btree->data)->alive)
				printf("%s, %d\n", ((PERSON*)btree->data)->name, ((PERSON*)btree->data)->age);
		showAlive(btree->left);
		showAlive(btree->right);
	}
}

// 7 - Mudar nome de uma pessoa
void changeName(BTREE btree, char* nomeOriginal, char* novoNome) {
	if (btree != NULL) {
		if (!strcmp(((PERSON*)btree->data)->name, nomeOriginal))
			strcpy(((PERSON*)btree->data)->name, novoNome);
		changeName(btree->left, nomeOriginal, novoNome);
		changeName(btree->right, nomeOriginal, novoNome);
	}
}


// 9 - Mostrar ALGUÉM em específico
void specificPerson(BTREE btree, char* fam) {
	if (btree != NULL) {
		if (btree->left == NULL && btree->right == NULL) {
			if (!strcmp(fam, ((PERSON*)btree->data)->name))
				showFam(btree);
		}
		specificPerson(btree->left, fam);
		specificPerson(btree->right, fam);
	}
}