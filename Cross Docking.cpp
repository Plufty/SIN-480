#define _CRT_SECURE_NO_DEPRECATE
#include<iostream>
#include<stdio.h>
#include <sstream>
#include <time.h>

#include <ilcplex/ilocplex.h>


typedef IloArray<IloNumVarArray> IloNumVarMatrix;
typedef IloArray<IloNumVarMatrix> IloNumVar3Matrix;
typedef IloArray<IloNumVar3Matrix> IloNumVar4Matrix;

int tamanho(int arr[])//Função para definir o tamanho de um array
{
	return sizeof arr / sizeof arr[0];
}

int maior(int a, int b)//Função para determinar o maior dentre 2 números
{
	if (a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}
using namespace std;

int main(int argc, char* argv[])
{

	// Declarando conjuntos de entrada
	int T;		//Conjunto discreto dos períodos de tempo. Se inicia em 0
	int J1;		//Conjunto dos caminhões do estágio 1. Se inicia em 1
	int J2;		//Conjunto dos caminhões do estágio 2. Se inicia em 1
	int P;		//Conjunto dos produtos. Se inicia em 1

	// Dados de entrada dependentes dos conjuntos

	int* tempo;//tempo para o conjunto dos períodos de tempo 

	char** nomeCaminhaoJ1;//Nome dos caminhões do estágio 1
	int* r;			//data de início por caminhão do estágio 1	
	int** A;			//é subconjunto de P, porém se dá por J1
	int** nA;			//numero de produtos carregados em A
	float* p1;	//tempo de processamento por j1. Na prática é o somatório por produto  pertencente a A do tempo de descarregamento

	char** nomeCaminhaoJ2;//Nome dos caminhões do estágio 2
	int** B;			//é subconjunto de P, porém se dá por j2
	int** nB;			//numero de produtos carregados em B
	float* p2;	//tempo de processamento por j2. Na prática é o somatório por produto  pertencente a B do tempo de carregamento

	char** nomeProduto; //nome do produto
	float* pd;		//Tempo de descarregamento por produto
	float* pc;		//Tempo de carregamento por produto	
	int* d;			//Limite de tempo para entrega por produto
	float* s;		//Peso por produto
	int* w;			//Importância por produto
	int* n;			//numero de produtos por produto
	int** dc;		//tempo de viagem por client i, j. Em que i,j pertencem ao conjunto P União com 0

	float ht;		// Horizonte de tempo, na prática é o somatório por produto de tempo de carregamento+tempo de descarregamento
	int m1;			//Número de docas no estágio 1
	int m2;			//Número de docas no estágio 2
	int n1;			//Número de caminhões no estágio 1
	int n2;			//Número de caminhões no estágio 2
	float k;		//capacidade dos caminhões de saída
	long long int m;		//Número suficientemente grande


	// Leitura do Arquivo

//	static const char* teste = R"(C:/Users/Plufty/Desktop/UFV/6º Período/SIN 480 - Pesquisa Operacional/Projeto/Código/teste.txt)";
	FILE* fp;
	fp = fopen(argv[1], "r");

	if (fp == NULL)
	{
		printf("Erro ao abrir o arquivo!\n");
		exit(1);
	}

	// Leitura dos conjuntos
	fscanf(fp, "%d", &T);	//Leitura do conjunto T
	fscanf(fp, "%d", &J1);	//Leitura do conjunto J2
	fscanf(fp, "%d", &J2);	//Leitura do conjunto J1
	fscanf(fp, "%d", &P);	//Leitura do conjunto P

	// Definindo os dados de entrada de acordo com o tamanho dos conjuntos lidos

	//por T
	tempo = new int[T];

	//por J1
	nomeCaminhaoJ1 = new char*[J1];
	for (int j1 = 0; j1 < J1; j1++)
	{
		nomeCaminhaoJ1[j1] = new char[51];
	}
	A = new int*[J1];
	for (int j1 = 0; j1 < J1; j1++)
	{
		A[j1] = new int[P];
	}
	nA = new int* [J1];
	for (int j1 = 0; j1 < J1; j1++)
	{
		nA[j1] = new int[P];
	}
	r = new int[J1];
	p1 = new float[J1];

	//por J2
	nomeCaminhaoJ2 = new char*[J2];
	for (int j2 = 0; j2 < J2; j2++)
	{
		nomeCaminhaoJ2[j2] = new char[51];
	}
	B = new int*[J2];
	for (int j2 = 0; j2 < J2; j2++)
	{
		B[j2] = new int[P];
	}
	nB = new int*[J2];
	for (int j2 = 0; j2 < J2; j2++)
	{
		nB[j2] = new int[P];
	}
	p2 = new float[J2];

	//Por produto
	nomeProduto = new char*[P];
	for (int p = 0; p < P; p++)
	{
		nomeProduto[p] = new char[51];
	}
	pd = new float[P];
	pc = new float[P];
	d = new int[P];
	s = new float[P];
	w = new int[P];
	n = new int[P];
	dc = new int*[P];
	for (int i = 0; i < P; i++)
	{
		dc[i] = new int[P];
	}

	// Após a declaração dos tamanhos dos dados de entrada, realizando a leitura


	// Conjunto T
	for (int t = 0; t < T; t++)
	{
		fscanf(fp, "%d", &tempo[t]);
	}

	// Conjunto J1

	for (int j1 = 0; j1 < J1; j1++)
	{
		fscanf(fp, "%s", nomeCaminhaoJ1[j1]);
		fscanf(fp, "%d", &r[j1]);
		for (int p = 1; p < P; p++)
		{
			fscanf(fp, "%d", &nA[j1][p]); //Lendo Número de produtos carregados em A
		}
	}


	// Conjunto J2

	for (int j2 = 0; j2 < J2; j2++)
	{
		fscanf(fp, "%s", nomeCaminhaoJ2[j2]);
		for (int p = 1; p < P; p++)
		{
			fscanf(fp, "%d", &nB[j2][p]); //Lendo Número de produtos carregados em B
		}
	}

	// Conjunto P
	for (int p = 0; p < P; p++)
	{
		fscanf(fp, "%s", nomeProduto[p]);
		fscanf(fp, "%f", &pd[p]); //Lendo tempo de descarregamento
		fscanf(fp, "%f", &pc[p]); //Lendo tempo de carregamento
		fscanf(fp, "%f", &s[p]); //Lendo peso do produto
		fscanf(fp, "%d", &d[p]); //Lendo Limite de tempo para entrega
		fscanf(fp, "%d", &w[p]); //Lendo importância
		fscanf(fp, "%d", &n[p]); //Lendo Número de produtos	
	}
	for (int i = 0; i < P; i++)
	{
		for (int j = 0; j < P; j++)
		{
			fscanf(fp, "%d", &dc[i][j]);//Lendo a matriz de tempo de viagem do cliente i ao j
		}
	}

	//Variáveis independentes de conjuntos
	
	for (int p = 1; p < P; p++)
	{
		ht = pc[p] + pd[p];
	}
	for (int j1 = 0; j1 < J1; j1++)
	{
		for (int p = 1; p < sizeof(B); p++)
		{
			p1[j1] += pd[p];
		}
	}
	for (int j2 = 0; j2 < J2; j2++)
	{
		for (int p = 1; p < sizeof(B); p++)
		{
			p2[j2] += pc[p];
		}
	}
	fscanf(fp, "%d", &m1);
	fscanf(fp, "%d", &m2);
	fscanf(fp, "%d", &n1);
	fscanf(fp, "%d", &n2);
	fscanf(fp, "%f", &k);
	fscanf(fp, "%lld", &m);


	// Impressão para Verificação dos dados

	printf("Numero de tempos: %d \n", T);
	printf("Numero de itens em J1: %d \n", J1);
	printf("Numero de itens em J2: %d \n", J2);
	printf("Numero de produtos: %d \n", P);

	printf("Tempos: \n");
	for (int t = 0; t < T; t++)
	{
		printf("%d \t", tempo[t]);
	}

	printf("\n");

	printf("Caminhoes do primeiro estagio: \n");
	for (int j1 = 0; j1 < J1; j1++)
	{
		printf("%s \t %.2f \t  %d \n", nomeCaminhaoJ1[j1], p1[j1], r[j1]);
		printf("\nProdutos carregados no caminhao %s \t \n", nomeCaminhaoJ1[j1]);
		for (int p = 1; p < P; p++)
		{
			printf("%s \t %d \n", nomeProduto[p], nA[j1][p]);
		}
		putchar('\n');
	}

	printf("\n");

	printf("Caminhoes do segundo estagio: \n");
	for (int j2 = 0; j2 < J2; j2++)
	{
		printf("%s \t %.2f \n", nomeCaminhaoJ2[j2], p2[j2]);
		printf("\nProdutos carregados no caminhao %s \t \n", nomeCaminhaoJ2[j2]);
		for (int p = 1; p < P; p++) 
		{
			printf("%s \t %d \n", nomeProduto[p], nB[j2][p]);
		}
		putchar('\n');
	}

	printf("\n");

	printf("Produtos: \n");
	for (int p = 1; p < P; p++) 
	{
		printf("%s \t %.2f \t %.2f \t %d \t %.2f \t %d \t %d \n", nomeProduto[p], pd[p], pc[p], d[p], s[p], w[p], n[p]);
	}

	printf("\n");

	printf("Matriz dos tempos de viagem do cliente i ao j:\n");
	for (int i = 0; i < P; i++)
	{
		for (int p = 0; p < P; p++)
		{
			printf("%d ", dc[i][p]);
		}
		putchar('\n');
	}

	printf("Horizonte de tempo: %.2f \n", ht);
	printf("Numero de Caminhoes do primeiro estagio: %d \n", m1);
	printf("Numero de Caminhoes do segundo estagio: %d \n", m2);
	printf("Numero de docas do primeiro estagio: %d \n", n1);
	printf("Numero de docas do segundo estagio: %d \n", n2);
	printf("Capacidade dos caminhoes de saida: %.2f\n", k);
	printf("Numero suficientemente grande para comparacoes: %lld\n ", m);

	// DECLARANDO O AMBIENTE E O MODELO MATEMATICO
	IloEnv env;
	IloModel modelo;
	// iniciando o modelo
	modelo = IloModel(env);



	//VARIÁVEIS DE UMA DIMENSÃO***************************************************************************************************************

	//***************************rdp********************************// tempo em que o produto p termina de ser descarregado no estágio 1

	IloNumVarArray rd(env, P, 0, IloInfinity, ILOFLOAT); // tipo float

	// adicionar as variáveis ao modelo
	for (int p = 1; p < P; p++) //P só inclui o 0 em caso de tratar com clientes
	{
		stringstream var;
		var << "rd[" << p << "]";
		rd[p].setName(var.str().c_str());
		modelo.add(rd[p]);
	}

	//***************************atrasop********************************// tempo de atraso de entrega do produto p

	IloNumVarArray atraso(env, P, 0, IloInfinity, ILOFLOAT); // tipo float

	// adicionar as variáveis ao modelo
	for (int p = 0; p < P; p++)// P só inclui o 0 em caso de representar clientes
	{
		stringstream var;
		var << "atraso[" << p << "]";
		atraso[p].setName(var.str().c_str());
		modelo.add(atraso[p]);
	}

	//***************************Cj********************************// tempo de conclusão de carregamento do caminhão j de J2 no segundo estágio.

	IloNumVarArray C(env, J2, 0, IloInfinity, ILOFLOAT); // tipo float

	// adicionar as variáveis ao modelo
	for (int j2 = 0; j2 < J2; j2++)
	{
		stringstream var;
		var << "C[" << j2 << "]";
		C[j2].setName(var.str().c_str());
		modelo.add(C[j2]);
	}

	//***************************tentregap********************************// tempo em que o produto p de P foi entregue ao cliente

	IloNumVarArray tentrega(env, P, 0, IloInfinity, ILOFLOAT); // tipo float

	// adicionar as variáveis ao modelo
	for (int p = 1; p < P; p++)// P só inclui o 0 em caso de representar clientes
	{
		stringstream var;
		var << "tentrega[" << p << "]";
		tentrega[p].setName(var.str().c_str());
		modelo.add(tentrega[p]);
	}




	//VARIÁVEIS DE DUAS DIMENSÕES***************************************************************************************************************

	//***************************Xjt********************************// Se o caminhão j de J1 começa o processamento no tempo t=0

	IloNumVarMatrix x(env, J1);
	for (int j1 = 0; j1 < J1; j1++)
	{
		x[j1] = IloNumVarArray(env, T, 0, 1, ILOBOOL); // Variável Binária
	}
	// adicionar as variáveis no modelo
	for (int j1 = 0; j1 < J1; j1++)
	{
		for (int t = 0; t < T; t++)
		{
			stringstream var;
			var << "x[" << j1 << "][" << t << "]";
			x[j1][t].setName(var.str().c_str());
			modelo.add(x[j1][t]);
		}
	}

	//***************************Yjt********************************// Se o caminhão j de J2 começa o processamento no tempo t=0

	IloNumVarMatrix y(env, J2);
	for (int j2 = 0; j2 < J2; j2++)
	{
		y[j2] = IloNumVarArray(env, T, 0, 1, ILOBOOL);// Variável binária
	}
	// adicionar as variáveis no modelo
	for (int j2 = 0; j2 < J2; j2++)
	{
		for (int t = 0; t < T; t++)
		{
			stringstream var;
			var << "y[" << j2 << "][" << t << "]";
			y[j2][t].setName(var.str().c_str());
			modelo.add(y[j2][t]);
		}
	}

	//***************************temposjn********************************// tempo de viagem do caminhão j ao cliente n

	IloNumVarMatrix tempos(env, J2);
	for (int j2 = 0; j2 < J2; j2++)
	{
		tempos[j2] = IloNumVarArray(env, P, 0, IloInfinity, ILOFLOAT); // tipo float
	}
	// adicionar as variáveis no modelo
	for (int j2 = 0; j2 < J2; j2++)
	{
		for (int n = 0; n < P; n++) // P se inicia em 0 em caso de P representar clientes
		{
			stringstream var;
			var << "tempos[" << j2 << "][" << n << "]";
			tempos[j2][n].setName(var.str().c_str());
			modelo.add(tempos[j2][n]);
		}
	}

	//VARIÁVEIS DE TRÊS DIMENSÕES***************************************************************************************************************

	//***************************rotasjik********************************// Variável binária que informa se o caminhão fez o percurso entre os clientes i e k

	IloNumVar3Matrix rotas(env, J2);
	for (int j2 = 0; j2 < J2; j2++)
	{
		rotas[j2] = IloNumVarMatrix(env, P);
		for (int i = 0; i < P; i++)
		{
			rotas[j2][i] = IloNumVarArray(env, P, 0, 1, ILOBOOL); // Variável Binária
		}
	}
	// adicionar as variáveis no modelo
	for (int j2 = 0; j2 < J2; j2++)
	{
		for (int i = 0; i < P; i++) // P se inicia em 0 em caso de P representar clientes
		{
			for (int K = 0; K < P; K++)
			{
				stringstream var;
				var << "rotas[" << j2 << "][" << i << "][" << K << "]";
				rotas[j2][i][K].setName(var.str().c_str());
				modelo.add(rotas[j2][i][K]);
			}
		}
	}





	// DECLARAÇÃO DA FUNÇÃO OBJETIVO

	IloExpr fo(env);

	for (int p = 1; p < P; p++) //Somatório por p de P, atraso+importância, P só inclui o 0 em caso de representar com clientes
	{
		fo += atraso[p] * w[p];
	}


	//IloMinimize e IloMaximize
	modelo.add(IloMinimize(env, fo));






	// DECLARAÇÃO DAS RESTRIÇÕES DO PROBLEMA

	// declarando a restrição
	// IloRange parametros: ambiente, valor min, expressão, valor maximo

	//Restrição(1)
	//Essa restrição garante que os caminhões do primeiro estágio só podem ser alocados após chegarem ao centro de distribuição.

	for (int j1 = 0; j1 < J1; j1++)//para todo
	{
		IloExpr soma(env);
		for (int t = 0; t < (ht - p1[j1]); t++)
		{
			soma += tempo[t] * x[j1][t];
		}
		//declarar a restição
		IloRange rest_um(env, r[j1], soma, IloInfinity);
		//nomeando a restrição
		stringstream rest;
		rest << "um: ";
		rest_um.setName(rest.str().c_str());
		//adicionar ao modelo
		modelo.add(rest_um);
	}
	
	//Restrição(2)
	//Essa restrição garante que cada caminhão do priemiro estágio só seja alocado em uam data no horizonte de tempo. 
	for (int j1 = 0; j1 < J1; j1++)//para todo
	{
		IloExpr soma(env);
		for (int t = 0; t < (ht - p1[j1]); t++)
		{
			soma += tempo[t] * x[j1][t];
		}
		//declarar a restição
		IloRange rest_dois(env, 1, soma, 1);
		//nomeando a restrição
		stringstream rest;
		rest << "dois: ";
		rest_dois.setName(rest.str().c_str());
		//adicionar ao modelo
		modelo.add(rest_dois);
	}

	//Restrição(3)
	//Essa restrição garante que cada caminhão do segundo estágio só seja alocado em uma data do horizonte de tempo
	for (int j2 = 0; j2 < J2; j2++)//para todo
	{
		IloExpr soma(env);
		for (int t = 0; t < (ht - p2[j2]); t++)
		{
			soma += tempo[t] * y[j2][t];
		}
		//declarar a restição
		IloRange rest_tres(env, 1, soma, 1);
		//nomeando a restrição
		stringstream rest;
		rest << "tres: ";
		rest_tres.setName(rest.str().c_str());
		//adicionar ao modelo
		modelo.add(rest_tres);
	}

	//Restrição(4)
	//Essa restrição garante que o número de caminhões sendo processados no primeiro estágio é menor ou igual ao número de docas
	for (int t = 0; t < T; t++)//para todo
	{
		IloExpr soma(env);
		for (int j1 = 0; j1 < J1; j1++)
		{
			for (int s = maior(0, (t - p1[j1] + 1)); s < t; s++)
			{
				soma += x[j1][s];
			}
		}
		//declarar a restição
		IloRange rest_quatro(env, -IloInfinity, soma, m1);
		//nomeando a restrição
		stringstream rest;
		rest << "quatro: ";
		rest_quatro.setName(rest.str().c_str());
		//adicionar ao modelo
		modelo.add(rest_quatro);
	}


	//Restrição(5)
	//Essa restrição garante que o número de caminhões sendo processados no segundo estágio é menor ou igual ao número de docas

	for (int t = 0; t < T; t++)//para todo
	{
		IloExpr soma(env);
		for (int j2 = 0; j2 < J2; j2++)
		{
			for (int s = maior(0, (t - p2[j2] + 1)); s < t; s++)
			{
				soma += y[j2][s];
			}
		}
		//declarar a restrição
		IloRange rest_cinco(env, -IloInfinity, soma, m2);
		//nomeando a restrição
		stringstream rest;
		rest << "cinco: ";
		rest_cinco.setName(rest.str().c_str());
		//adicionar ao modelo
		modelo.add(rest_cinco);
	}

	//Restrição(6)
	// Essa restrição garante que a data de disponibilidade de cada produto para ser carregado no segundo estágio é maior ou igual ao tempo de conclusão de processamento do caminhão no qual o produto estava carregado no primeiro estágio
	
	for (int j1 = 0; j1 < J1; j1++)//para todo
	{
		for (int a = 1; a < tamanho(A[j1]); a++)//para todo
		{
			IloExpr soma(env);
			for (int t = 0; t < T; t++)
			{
				soma += (tempo[t] + p1[j1])*x[j1][t];
			}
			//declarar a restrição
			IloRange rest_seis(env, 0, rd[a]-soma, IloInfinity);
			//nomeando a restrição
			stringstream rest;
			rest << "seis: ";
			rest_seis.setName(rest.str().c_str());
			//adicionar ao modelo
			modelo.add(rest_seis);
		}
	}

	//Restrição(7)
	//Essa restrição garante que os caminhões no segundo estágio só podem iniciar seu processamento 
	for (int j2 = 0; j2 < J2; j2++)//para todo
	{
		for (int b = 1; b < tamanho(B[j2]); b++)//para todo
		{
			IloExpr soma(env);
			for (int t = 0; t < (ht-p2[j2]); t++)
			{
				soma += tempo[t]*y[j2][t];
			}
			//declarar a restrição
			IloRange rest_sete(env, 0, soma-rd[b], IloInfinity);
			//nomeando a restrição
			stringstream rest;
			rest << "sete: ";
			rest_sete.setName(rest.str().c_str());
			//adicionar ao modelo
			modelo.add(rest_sete);
		}
	}
	//Restrição(8)
	//Essa restrição garante que a data de conclusão de um caminhão do segundo estágio é maior ou igual a data de início do mesmo, mais o tempo de processamento de um caminhão j no segundo estágio. 
	for (int j2 = 0; j2 < J2; j2++)//para todo
	{
		for (int b = 1; b < tamanho(B[j2]); b++)//para todo
		{
			IloExpr soma(env);
			for (int t = 0; t < (ht - p2[j2]); t++)
			{
				soma += tempo[t]*y[j2][t];
			}
			soma += p2[j2];
			//declarar restrição
			IloRange rest_oito(env, 0, C[j2]-soma, IloInfinity);
			//nomeando a restrição
			stringstream rest;
			rest << "oito: ";
			rest_oito.setName(rest.str().c_str());
			//adicionar ao modelo
			modelo.add(rest_oito);

		}
	}

	//Restrição(9)
	//Essa restrição garante que o trajeto dos caminhões do segundo estágio inicia seus trajetos no centro de Cross-Docking (CD), que é o ponto de origem, onde ocorre o carregamento dos produtos. 
	for (int j2 = 0; j2 < J2; j2++)//para todo
	{
		IloExpr soma(env);
		for (int p = 1; p < P; p++)
		{
			soma += rotas[j2][0][p];
		}
		//declarar restrição
		IloRange rest_nove(env, 1, soma, 1);
		//nomeando a restrição
		stringstream rest;
		rest << "nove: ";
		rest_nove.setName(rest.str().c_str());
		//adicionar ao modelo
		modelo.add(rest_nove);
	}

	//Restrição(10)
	//Ambas as restrições 10 e 11 juntas garantem que todos os clientes de cada caminhão serão visitados uma única vez.
	for (int j2 = 0; j2 < J2; j2++)//para todo
	{
		for (int b = 1; b < tamanho(B[j2]); b++)//para todo
		{
			IloExpr soma(env);
			for (int i = 0; i < P; i++)
			{
				if (b != i)
				{
					soma += rotas[j2][b][i];
				}
			}
			//declarar restrição
			IloRange rest_dez(env, 1, soma, 1);
			//nomeando a restrição
			stringstream rest;
			rest << "dez: ";
			rest_dez.setName(rest.str().c_str());
			//adicionar ao modelo
			modelo.add(rest_dez);

		}
	}

	//Restrição(11)
	//Ambas as restrições 10 e 11 juntas garantem que todos os clientes de cada caminhão serão visitados uma única vez.
	for (int j2 = 0; j2 < J2; j2++)//para todo
	{
		for (int i = 1; i < tamanho(B[j2]); i++)//para todo
		{
			IloExpr soma(env);
			for (int p = 0; p < P; p++)
			{
				if (p != i)
				{
					soma += rotas[j2][i][p];
				}
			}
			//declarar restrição
			IloRange rest_onze(env, 1, soma, 1);
			//nomeando a restrição
			stringstream rest;
			rest << "onze: ";
			rest_onze.setName(rest.str().c_str());
			//adicionar ao modelo
			modelo.add(rest_onze);

		}
	}


	//Restrição(12)
	//Essa restrição garante que o tempo de entrega do produto “p” é maior ou igual ao tempo de conclusão de carregamento do caminhão que contém esse produto (Cj,) somado ao tempo de viagem até o ponto de entrega. 
	for (int j2 = 0; j2 < J2; j2++) // para todo j2
	{
		for (int p = 0; p < P; p++)//para todo p, sendo P clientes a variável inicia em 0
		{
			IloExpr soma(env);
			soma = tempos[j2][p] + C[j2];
			//declarar restrição
			IloRange rest_doze(env, 0 , tentrega[p]- soma, IloInfinity);
			//nomeando a restrição
			stringstream rest;
			rest << "doze: ";
			rest_doze.setName(rest.str().c_str());
			//adicionar ao modelo
			modelo.add(rest_doze);
		}
	}


	//Restrição(13)
	//Essa restrição garante que o atraso de entrega do produto p é maior ou igual a diferença do tempo de entrega do produto “p” pelo tempo esperado de entrega do produto “p”. 
	for (int p=0; p < P; p++)//para todo
	{
		IloExpr sub(env);
		sub += tentrega[p] - d[p];
		//declarar a restrição
		IloRange rest_treze(env, 0, atraso[p] - sub, IloInfinity);
		//nomeando a restrição
		stringstream rest;
		rest << "treze: ";
		rest_treze.setName(rest.str().c_str());
		//adicionar ao modelo
		modelo.add(rest_treze);
	}

	//Restrição(14)
	
	for (int j2 = 0; j2 < J2; j2++)//para todo
	{
		for (int n=0; n < P; n++)//para todo
		{
			for (int i=0; i < P; i++)
			{
				IloExpr soma(env);
				if (n != 0)
				{
					soma = tempos[j2][i] + dc[i][n] - m*(1 - rotas[j2][i][n]);
				}
				IloRange rest_quatorze(env, 0, tempos[j2][n]-soma, IloInfinity);
				//nomeando a restrição
				stringstream rest;
				rest << "quatorze: ";
				rest_quatorze.setName(rest.str().c_str());
				//adicionar ao modelo
				modelo.add(rest_quatorze);

			}
		}
	}







// RESOLVENDO O MODELO

// Carregando o módulo do Cplex
	IloCplex cplex(modelo);
	// exportando o lp
	cplex.exportModel("crossdocking.lp");
	// Executando o modelo
	cplex.solve();

	// PEGAR OS VALORES DAS VARIÁVEIS 
	//cplex.getValue(NOME_VAR) 

	for (int p = 0; p < P; p++)//Variáveis de uma dimensão dependentes de P
	{
		float variavel1 = cplex.getValue(rd[p]);
		float variavel2 = cplex.getValue(atraso[p]);
		float variavel4 = cplex.getValue(tentrega[p]);
		printf("********\n");
		printf("Tempo em que o produto %s termina de ser descarregado no estagio 1: %f\n", nomeProduto[p], variavel1);
		printf("********\n");
		printf("Tempo de atraso do produto %s: %f\n", nomeProduto[p], variavel2);
		printf("********\n");
		printf("Tempo em que o produto %s foi entregue: %f\n", nomeProduto[p], variavel4);
	}

	for (int j2 = 0; j2 < J2; j2++)//C
	{

		float variavel = cplex.getValue(C[j2]);
		printf("********\n");
		printf("Tempo em que o caminhao %s termina de ser carregado no estagio 2: %f\n", nomeCaminhaoJ2[j2], variavel);
	}

	for (int j1 = 0; j1 < J1; j1++)//X
	{
		for (int t = 0; t < T; t++)
		{
			int variavel = cplex.getValue(x[j1][t]);
			printf("********\n");
			printf("1- Verdadeiro  0- Falso\nO caminhao %s comeca o processamento no tempo 0: %d\n", nomeCaminhaoJ1[j1], variavel);
		}
	}

	for (int j2 = 0; j2 < J2; j2++)//Y
	{
		for (int t = 0; t < T; t++)
		{
			int variavel = cplex.getValue(y[j2][t]);
			printf("********\n");
			printf("1- Verdadeiro  0- Falso\nO caminhao %s comeca o processamento no tempo 0: %d\n", nomeCaminhaoJ2[j2], variavel);
		}
	}

	for (int j2 = 0; j2 < J2; j2++)//tempos
	{
		for (int p = 0; p < P; p++)
		{
			float variavel = cplex.getValue(tempos[j2][p]);
			printf("********\n");
			printf("Tempo de viagem de um cliente ao outro do caminhao %s: %.2f\n", nomeCaminhaoJ2[j2], variavel);
		}
	}

	for (int j2 = 0; j2 < J2; j2++)//rotas
	{
		for (int i = 0; i < P; i++)
		{
			for (int k = 0; k < P; k++)
			{
				int variavel = cplex.getValue(rotas[j2][i][k]);
				printf("********\n");
				printf("1- Verdadeiro  0- Falso\nO caminhao %s faz o percurso entre os clientes %d e %d: %d\n", nomeCaminhaoJ2[j2], i, k, variavel);
			}
			
		}
	}


	printf("Funcao objetivo: %.2f\n", cplex.getObjValue());

	return 0;
}












