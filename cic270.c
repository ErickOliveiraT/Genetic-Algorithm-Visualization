#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

#define TRUE 1
#define FALSE 0
#define TAM_POP 100
#define QNT_GER 5

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

/* Aplica mutação nos indivíduos*/
void aplicaMutacao(populacao *pop, int numIndividuos);

float xRotated = 90.0, yRotated = 0.0, zRotated = 0.0;
float radius = 0.4, space = 0.95;
int line = 10, column = 10, ger_atual = 0, acabou = FALSE, maior = 0;
int populacoes[QNT_GER+10][TAM_POP+10], rodar_genetico = TRUE;

//------------------------------  reshapeFunc  ---------------------------------

void reshapeFunc (int x, int y)
{
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
 
    gluPerspective (40.0, (GLdouble)x / (GLdouble)y, 0.5, 20.0);
    glMatrixMode   (GL_MODELVIEW);
    glViewport     (0, 0, x, y); 
}

//------------------------------  Draw_Spheres   -------------------------------

void Draw_Spheres (void)
{
    glMatrixMode   (GL_MODELVIEW);
    glClear        (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity ();
    glTranslatef    (0.0, 0.0, -15.0);

    int x, i, j, cont = 0;
    int populacao[TAM_POP+1]; //Valores de x da população atual
    float RGB[3];
    
    if(acabou == TRUE) goto label;
    printf("Geracao %d:\n\n", ger_atual+1);
    for (i = 0; i < TAM_POP; i++) { //Gerando números aleatórios apenas para simular uma população
        populacao[i] = populacoes[ger_atual][i];
        printf("%d ", populacao[i]);
    }
    printf("\n\n");
    ger_atual++;
    
    if (ger_atual == QNT_GER) { //Acabou
        printf("Algoritmo Genetico finalizado\n\n");
        for(int i = 0; i < TAM_POP; i++) if(populacao[i] > maior) maior = populacao[i];
        printf("Melhor Solução: %d\n\n", maior);
        double erro = (255.0 - maior)/255.0;
        printf("Erro: %lf %%\n\n", erro);
        acabou = TRUE;
    }
    
    label:
    for (i = 0; i < line; i++){
        for(j = 0; j < column; j++){
            //Definindo a classe (cor)
            x = populacao[cont];
            RGB[0] = 1.0;
            RGB[1] = -0.0039215686275*x + 1;
            RGB[2] = RGB[1];
            if (x == 255) RGB[0] = 0.5;

            if (acabou == TRUE && x == maior) {RGB[0] = 0; RGB[1] = 1.0; RGB[2] = 0;}

            glColor3f (RGB[0],RGB[1],RGB[2]); //Cor
            glPushMatrix ();
            glTranslatef    ((-4.5 + i * space), (4 - j * space), 0.0);
            glRotatef       (60.0, 1,0,0);
            glRotatef       (zRotated*2.0, 0,0,1);   // Red ball rotates at twice the rate of blue ball.
            glutSolidSphere (radius, 10, 20);
            glPopMatrix ();
            cont++;
        }
    }

    glutSwapBuffers();
    //Esperar
    sleep(5);
    if (acabou == TRUE) sleep(1000);
}

//--------------------------------  idleFunc  ----------------------------------
 
void idleFunc (void)
{
    zRotated += 0.3;
    glutPostRedisplay();
}

//----------------------------------  main  ------------------------------------
 
void main (int argc, char **argv)
{

    if (rodar_genetico == TRUE) { //Roda o algoritmo genético
        
        populacao *pop = malloc(sizeof(populacao));
        individuo *selecionados;
        int numIndividuos = TAM_POP;
        int numGeracoes = QNT_GER;
        int cont = 0;

        geraIndividuos(pop, numIndividuos); //Gera população inicial
        for (int i = 0; i < numIndividuos; i++) {
            populacoes[cont][i] = sqrt(pop->individuos[i].aptidao);
        }
        cont++;

        while(pop->geracao < numGeracoes + 1){

            /* Aplica mutação a cada 5 gerações */
            if(pop->geracao % 5 == 0) aplicaMutacao(pop, numIndividuos);
            
            //printf("\nGeração: %d\n", pop->geracao);
            for(int i = 0; i < numIndividuos; i++){
                //printf("%d ", pop->individuos[i].aptidao);
                populacoes[cont][i] = sqrt(pop->individuos[i].aptidao);
            }
            cont++;
            if(pop->geracao == numGeracoes) printf("\n");
            selecionados = selecionaIndividuos(pop, numIndividuos);
            cruzaIndividuos(pop, selecionados, numIndividuos);
        }

        rodar_genetico == FALSE;
    }

    glutInit               (&argc, argv);
    glutInitDisplayMode    (GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize     (800, 800);
    glutInitWindowPosition (0, 0);
    glutCreateWindow       ("Visualizacao de Algoritmos Geneticos");
    glPolygonMode          (GL_FRONT_AND_BACK, GL_LINE);
 
    glClearColor (0.0, 0.0, 0.0, 0.0);

    glutDisplayFunc (Draw_Spheres);
    glutReshapeFunc (reshapeFunc);
    glutIdleFunc    (idleFunc);
    
    glutMainLoop();
}

//--------------------------------- Algoritmo Genético --------------------------

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
    srand(time(NULL));
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
void aplicaMutacao(populacao *pop, int numIndividuos) {
 
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