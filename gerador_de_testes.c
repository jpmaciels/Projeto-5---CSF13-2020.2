/*============================================================================*/
/* GERADOR DE CASOS DE TESTE PARA O PROBLEMA DO WII SENSOR BAR                */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu - nassu@dainf.ct.utfpr.edu.br                       */
/*============================================================================*/
/** Um gerador de casos de teste para o problema do Wii Sensor Bar. Geramos uma
 * imagem preta com dois c�rculos brancos, adicionamos ru�do bin�rio, aplicamos
 * uma suaviza��o, e adicionamos ru�do Gaussiano. */
/*============================================================================*/

#include <stdlib.h>
#include <math.h>

#include "gerador_de_testes.h"

/*============================================================================*/

#define PI 3.1415926535897932384626433832795

#define N_TAMANHOS 6
int LARGURAS [] = {128,320,640,800,1024,1280};
int ALTURAS [] = {96,240,480,600,768,1024};

/* Tamanho min/max para os c�rculos. */
#define MIN(a,b) ((a<b)? a : b)
#define MAX(a,b) ((a>b)? a : b)
#define RAIO_MIN 3
#define RAIO_MAX(img) (MIN(img->altura,img->largura)/20)

/* Ru�do e suaviza��o. */
#define MAX_PROB_RUIDO_BINARIO 1000 /* Probabilidade m�xima de ru�do bin�rio, multiplicada por 100 (i.e. 100 = 1%) */
#define PARAMETRO_SUAVIZACAO 4 /* Quanto menor, mais borradas as imagens tendem a ficar. */
#define SIGMA_RUIDO 25 /* Quanto maior, mais ru�do. */

/*============================================================================*/
/* Fun��es internas do m�dulo. */

void copiaImagem1C (Imagem1C* img, Imagem1C* copia);
int desenhaLuzes (Imagem1C* img, Coordenada* l, Coordenada* r);
void desenhaCirculo (Imagem1C* img, Coordenada centro, int raio);
void ruidoBinario (Imagem1C* img);
void suavizaImagem (Imagem1C* img);
void adicionaRuido (Imagem1C* img);
double rand_normal (double mean, double stddev);
double anguloEntrePontos (Coordenada pt1, Coordenada pt2);

/*============================================================================*/
/** Fun��o central do gerador, cria um caso de testes.
 *
 * Par�metros: int seed: a semente para o gerador de n�meros aleat�rios.
 *             Coordenada* l: centro do c�rculo esquerdo.
 *             Coordenada* r: centro do c�rculo direito.
 *             double* angulo: angulo entre os dois centros, em radianos no
 *               intervalo (-PI/2, +PI/2).
 *
 * Valor de retorno: uma imagem contendo o caso de teste. Lembre-se de destru�-la! */

Imagem1C* geraCasoDeTeste (int seed, Coordenada* l, Coordenada* r, double* angulo)
{
    Imagem1C* img;
    int i, j, tam;

    /* Escolhe um tamanho e cria a imagem. */
    srand (seed);
    tam = rand () % N_TAMANHOS;
    img = criaImagem1C (LARGURAS [tam], ALTURAS [tam]);

    /* Pinta a imagem de preto. */
    for (i = 0; i < img->altura; i++)
        for (j = 0; j < img->largura; j++)
            img->dados [i][j] = 0;

    /* Agora vai... */
    int ciclos_blur = desenhaLuzes (img, l, r);
    ciclos_blur = MAX (1, ciclos_blur); /* Borra mais quando tem c�rculos maiores. */
    ruidoBinario (img);
    for (i = 0; i < ciclos_blur; i++)
        suavizaImagem (img);
    adicionaRuido (img);
    for (i = 0; i < ciclos_blur/2; i++)
        suavizaImagem (img);
    adicionaRuido (img);

    *angulo = anguloEntrePontos (*l, *r);

    return (img);
}

/*----------------------------------------------------------------------------*/
/** Copia o conte�do de uma imagem para outra.
 *
 * Par�metros: Imagem1C* img: imagem a copiar.
 *             Imagem1C* copia: imagem de destino. Pressupomos que tem o mesmo
 *               tamanho da imagem de entrada.
 *
 * Valor de retorno: nenhum. */

void copiaImagem1C (Imagem1C* img, Imagem1C* copia)
{
    unsigned long i, j;

    for (i = 0; i < copia->altura; i++)
        for (j = 0; j < copia->largura; j++)
            copia->dados [i][j] = img->dados [i][j];
}

/*----------------------------------------------------------------------------*/
/** Desenha dois c�rculos brancos na imagem dada. Armazena as coordenadas dos
 * centros.
 *
 * Par�metros: Imagem1C* img: desenha aqui.
 *             Coordenada* l: centro do c�rculo esquerdo (sa�da).
 *             Coordenada* r: centro do c�rculo direito (sa�da).
 *
 * Valor de retorno: O raio das luzes desenhadas, al�m dos 3 par�metros de sa�da. */

int desenhaLuzes (Imagem1C* img, Coordenada* l, Coordenada* r)
{
    int raio, aux, foi = 0;

    while (!foi) /* Repete at� conseguir algo que preste... */
    {
        /* Define o raio dos c�rculos. */
        raio = rand () % (RAIO_MAX(img)-RAIO_MIN+1) + RAIO_MIN;

        /* Define a posi��o dos c�rculos. */
        l->x = rand () % (img->largura-raio*2) + raio;
        l->y = rand () % (img->altura-raio*2) + raio;
        r->x = rand () % (img->largura-raio*2) + raio;
        r->y = rand () % (img->altura-raio*2) + raio;

        /* Troca l por r, se for preciso. */
        if (l->x > r->x)
        {
            aux = l->x;
            l->x = r->x;
            r->x = aux;
        }

        /* O espa�o entre os c�rculos precisa ser de pelo menos raio pixels. Usamos 3*raio porque medimos a dist�ncia dos centros. */
        /* A dist�ncia horizontal entre os c�rculos tamb�m precisa ser razo�vel. Consideramos que RAIO_MIN*2 � uma boa medida para n�o dar confus�o. */
        if (sqrt ((l->x-r->x)*(l->x-r->x) + (l->y-r->y)*(l->y-r->y)) > 3*raio && r->x - l->x > RAIO_MIN*2)
        {
            /* Desenha os c�rculos. */
            desenhaCirculo (img, *l, raio);
            desenhaCirculo (img, *r, raio);
            foi = 1;
        }
    }

    return (raio);
}

/*----------------------------------------------------------------------------*/
/** Desenha um c�rculo branco na imagem dada. O raio n�o inclui o pixel
 * central, ent�o o c�rculo ocupar� uma regi�o que vai de (x-raio,y-raio) a
 * (x+raio,y+raio), inclusive.
 *
 * Par�metros: Imagem1C* img: desenha aqui.
 *             Coordenada centro: centro do c�rculo.
 *             int raio: raio do c�rculo.
 *
 * Valor de retorno: NENHUM */

void desenhaCirculo (Imagem1C* img, Coordenada centro, int raio)
{
    int i, j, lim;

    for (i = -raio; i <= raio; i++)
    {
        lim = sqrt (raio*raio - i*i);

        for (j = -lim; j <= lim; j++)
            img->dados [centro.y + i][centro.x + j] = 255;
    }
}

/*----------------------------------------------------------------------------*/
/** Adiciona ru�do bin�rio a uma imagem. Inverte cada byte que for sorteado.
 *
 * Par�metros: Imagem1C* img: imagem a alterar.
 *
 * Valor de retorno: NENHUM */

void ruidoBinario (Imagem1C* img)
{
    int i, j, prob;

    /* Define a probabilidade de ru�do bin�rio. */
    prob = rand () % (MAX_PROB_RUIDO_BINARIO+1);

    for (i = 0; i < img->altura; i++)
        for (j = 0; j < img->largura; j++)
            if (rand () % 10000 < prob)
                img->dados [i][j] = ~img->dados [i][j];
}

/*----------------------------------------------------------------------------*/
/** Suaviza a imagem com um filtro 3x3. O peso do pixel central � escolhido
 * aleatoriamente, ou seja, a imagem pode ficar mais ou menos borrada.
 *
 * Par�metros: Imagem1C* img: imagem a suavizar.
 *
 * Valor de retorno: nenhum */

void suavizaImagem (Imagem1C* img)
{
    unsigned long row, col;
    int peso_centro;
    Imagem1C* copia; /* Precisamos guardar os dados originais. Seria bem mais eficiente guardar apenas uma janela, mas copiar tudo � bem mais simples... */
    copia = criaImagem1C (img->largura, img->altura);
    copiaImagem1C (img, copia);

    peso_centro = rand () % PARAMETRO_SUAVIZACAO + 4;
    for (row = 1; row < img->altura-1; row++)
        for (col = 1; col < img->largura-1; col++)
            img->dados [row][col] = (copia->dados [row-1][col-1] +
                                     copia->dados [row-1][col]*2 +
                                     copia->dados [row-1][col+1] +
                                     copia->dados [row][col-1]*2 +
                                     copia->dados [row][col]*peso_centro +
                                     copia->dados [row][col+1]*2 +
                                     copia->dados [row+1][col-1] +
                                     copia->dados [row+1][col]*2 +
                                     copia->dados [row+1][col+1])/(peso_centro + 12);
    destroiImagem1C (copia);
}

/*----------------------------------------------------------------------------*/
/** Adiciona ru�do a uma imagem.
 *
 * Par�metros: Imagem1C* img: imagem � qual adicionaremos ru�do.
 *
 * Valor de retorno: nenhum. */

void adicionaRuido (Imagem1C* img)
{
    unsigned long row, col;

    for (row = 0; row < img->altura; row++)
        for (col = 0; col < img->largura; col++)
        {
            double ruido = rand_normal (0, SIGMA_RUIDO);
            img->dados [row][col] = (unsigned char) (MIN (255, MAX (0, img->dados [row][col] + ruido)));
        }
}

/*----------------------------------------------------------------------------*/
/** C�digo copiado de outro lugar para gerar n�meros aleat�rios seguindo uma
 * distribui��o normal. Original em:
 * en.literateprograms.org/Box-Muller_transform_%28C%29#chunk%20use:complete%20Box-Muller%20function */

double rand_normal (double mean, double stddev)
{
    static double n2 = 0.0;
    static int n2_cached = 0;
    if (!n2_cached)
    {
        double x, y, r;
        do
        {
            x = 2.0*rand()/RAND_MAX - 1;
            y = 2.0*rand()/RAND_MAX - 1;
            r = x*x + y*y;
        }
        while (r == 0.0 || r > 1.0);
        {
            double result;
            double d = sqrt(-2.0*log(r)/r);
            double n1 = x*d;
            n2 = y*d;
            result = n1*stddev + mean;
            n2_cached = 1;
            return result;
        }
    }
    else
    {
        n2_cached = 0;
        return n2*stddev + mean;
    }
}

/*----------------------------------------------------------------------------*/
/** Calcula o �ngulo da reta que passa entre dois pontos.
 *
 * Par�metros: Coordenada pt1: um ponto.
 *             Coordenada pt2: outro ponto.
 *
 * Valor de retorno: o �ngulo da reta que passa por pt1 e pt2. */

double anguloEntrePontos (Coordenada pt1, Coordenada pt2)
{
    return (0); // TODO: depois da entrega!
}

/*============================================================================*/
