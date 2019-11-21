#include <stdio.h>
#include <stdlib.h>

typedef struct Populacao
{
    int geracao;
    int numIndividuos;
    struct Individuo *individuos;
} populacao;

typedef struct Individuo
{
    int aptidao;
    int gene[8];
} individuo;

/*Função a ser maximizada*/
int calculaAptidao(int *x);

/* Gera primeira população de indivíduos */
void geraIndividuos(populacao *pop, int numIndividuos);

/* Seleciona indivíduos para cruzamento */
individuo *selecionaIndividuos(populacao *pop, int numIndividuos);

/* Cruza indivíduos selecionados para próxima geração */
void cruzaIndividuos(populacao *pop, individuo *selecionados, int numIndividuos);

/* Muta indivíduos da próxima geração */
void mutaIndividuos(populacao *pop);

/* Aplica muta��o nos indiv�duos*/
void aplicaMutacao(populacao *pop, int numIndividuos);

int main()
{

    populacao *pop = malloc(sizeof(populacao));
    individuo *selecionados;
    int numIndividuos;
    int numGeracoes;

    printf("Número de indivíduos: ");
    scanf("%d", &numIndividuos);
    printf("Número de gerações: ");
    scanf("%d", &numGeracoes);

    geraIndividuos(pop, numIndividuos);

    while(pop->geracao < numGeracoes + 1){

        /* Aplica muta��o a cada 5 gera��es */
        if(pop->geracao % 5 == 0) aplicaMutacao(pop, numIndividuos);
        
        printf("\nGeração: %d\n", pop->geracao);
        for(int i = 0; i < numIndividuos; i++){
            printf("%d ", pop->individuos[i].aptidao);
        }
        if(pop->geracao == numGeracoes) printf("\n");
        selecionados = selecionaIndividuos(pop, numIndividuos);
        cruzaIndividuos(pop, selecionados, numIndividuos);
    }

    int maior = 0;
    for(int i = 0; i < numIndividuos; i++){
        if(pop->individuos[i].aptidao > maior) maior = pop->individuos[i].aptidao;
    }
    
    printf("\nMelhor Solução: %d\n", maior);
    
    return 0;
}

/*Função a ser maximizada*/
int calculaAptidao(int *x)
{
    int aptX = 0;

    if (x[0] == 1) aptX += 128;
    if (x[1] == 1) aptX += 64;
    if (x[2] == 1) aptX += 32;
    if (x[3] == 1) aptX += 16;
    if (x[4] == 1) aptX += 8;
    if (x[5] == 1) aptX += 4;
    if (x[6] == 1) aptX += 2;
    if (x[7] == 1) aptX += 1;

    /*Função X²*/
    int apt = aptX * aptX;

    return apt;
}

/* Gera primeira população de indivíduos */
void geraIndividuos(populacao *pop, int numIndividuos)
{
    pop->numIndividuos = numIndividuos;
    pop->geracao = 1;
    pop->individuos = malloc(numIndividuos * sizeof(individuo));

    for (int i = 0; i < numIndividuos; i++)
    {
        for (int t = 0; t < 8; t++)
        {
            /*Os genes são números aleatórios entre 0 e 1*/
            pop->individuos[i].gene[t] = rand() % 2;
        }
        pop->individuos[i].aptidao = calculaAptidao(pop->individuos[i].gene);
    }
}

/* Seleciona indivíduos para cruzamento */
individuo *selecionaIndividuos(populacao *pop, int numIndividuos)
{
    individuo *selecionados;
    selecionados = malloc((numIndividuos / 2) * sizeof(individuo));

    /* Seleciona numIndividuos/2 indivíduos mais aptos em um vetor ordenado por aptidão */
    for (int k = 0; k < numIndividuos; k++)
    {
        for (int j = 0; j < numIndividuos - 1; j++)
        {
            if (pop->individuos[j].aptidao < pop->individuos[j + 1].aptidao)
            {
                individuo aux;
                aux = pop->individuos[j];
                pop->individuos[j] = pop->individuos[j + 1];
                pop->individuos[j + 1] = aux;
            }
        }
    }

    for (int i = 0; i < numIndividuos / 2; i++)
    {
        selecionados[i] = pop->individuos[i];
    }

    return selecionados;
}

/* Cruza indivíduos selecionados para próxima geração */
void cruzaIndividuos(populacao *pop, individuo *selecionados, int numIndividuos)
{

    individuo *novosIndividuos;
    novosIndividuos = (individuo *)malloc((numIndividuos / 2) * sizeof(individuo));

    /*Os individuos recebem metade dos genes do pai e metade da mãe*/
    for (int i = 0; i < numIndividuos / 2; i++)
    {
        if(i == (numIndividuos/2) - 1)
        {
            novosIndividuos[i].gene[0] = selecionados[i].gene[0];
            novosIndividuos[i].gene[1] = selecionados[i].gene[1];
            novosIndividuos[i].gene[2] = selecionados[i].gene[2];
            novosIndividuos[i].gene[3] = selecionados[i].gene[3];
            novosIndividuos[0].gene[4] = selecionados[i].gene[4];
            novosIndividuos[0].gene[5] = selecionados[i].gene[5];
            novosIndividuos[0].gene[6] = selecionados[i].gene[6];
            novosIndividuos[0].gene[7] = selecionados[i].gene[7];
            novosIndividuos[i].aptidao = calculaAptidao(novosIndividuos[i].gene);
        }
        else
        {
            novosIndividuos[i].gene[0] = selecionados[i].gene[0];
            novosIndividuos[i].gene[1] = selecionados[i].gene[1];
            novosIndividuos[i].gene[2] = selecionados[i].gene[2];
            novosIndividuos[i].gene[3] = selecionados[i].gene[3];
            novosIndividuos[i + 1].gene[4] = selecionados[i].gene[4];
            novosIndividuos[i + 1].gene[5] = selecionados[i].gene[5];
            novosIndividuos[i + 1].gene[6] = selecionados[i].gene[6];
            novosIndividuos[i + 1].gene[7] = selecionados[i].gene[7];
            novosIndividuos[i].aptidao = calculaAptidao(novosIndividuos[i].gene);
        }
    }

    /*Os individuos da população antiga se tornam os novos e a geração aumenta*/
    for(int i = 0; i < numIndividuos/2; i++){
        pop->individuos[i] = novosIndividuos[i];
    }
    for(int i = 0; i < numIndividuos/2; i++){
        pop->individuos[i + numIndividuos/2] = selecionados[i];
    }
    pop->geracao++;
}

/* Aplica mutação nos indivíduos*/
void aplicaMutacao(populacao *pop, int numIndividuos){
 
    int *random;
    random = malloc((numIndividuos/2) *sizeof(int));
    
    for(int i = 0; i < numIndividuos/2; i++){
        random[i] = rand() % numIndividuos;
    }       
    
    for(int i = 0; i < numIndividuos/2; i++){
        int genePos = rand() % 8;
        
        if(pop->individuos[random[i]].gene[genePos] == 1) pop->individuos[random[i]].gene[genePos] = 0;
        else pop->individuos[random[i]].gene[genePos] = 1;
        
        pop->individuos[random[i]].aptidao = calculaAptidao(pop->individuos[random[i]].gene);
    }
}