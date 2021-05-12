#include "projeto5.h"
#include <stdio.h>
#include <math.h>

#define PI 3.1415926535897932384626433832795
#define PAREDE 0
#define BRANCO 255

int mediaGeral(Imagem1C* img)
{
  int i , j;
  long soma = 0;
  
  //percorre a imagem
  for (i = 0; i < img->altura; i++)
    for (j = 0; j < img->largura; j++)
      soma += img->dados[i][j];
  
  return soma/(i*j);
}

// Binariza Imagem
void binariza(Imagem1C* img)
{
  int i, j, limiar;
  
  limiar = mediaGeral(img) + 95;
  
  //percorre a imagem
  for (i = 0; i < img->altura; i++)
    for (j = 0; j < img->largura; j++)
      if(img->dados[i][j] < limiar)
        img->dados[i][j] = 0;
      else
        img->dados[i][j] = 255;
}

void removeRuido(Imagem1C* img)
{
  int i, j;

  //percorre a imagem - simplificavel
  for (i = 1; i < img->altura - 1; i++)
    for (j = 1; j < img->largura - 1; j++)
      if(!img->dados[i+1][j] && !img->dados[i-1][j] && !img->dados[i][j+1] && !img->dados[i][j-1])
        img->dados[i][j] = 0;
}

int mapeiaCirculo(Imagem1C* img)
{
  int i, j, count = 1, quantidade_caminhos = 0, raio;

  //percorre a imagem - simplificavel
  for (i = 1; i < img->altura - 1; i++)
    for (j = 1; j < img->largura - 1; j++)
      if (img->dados[i][j] == BRANCO )
        quantidade_caminhos++;

  raio = sqrt((double)quantidade_caminhos/(2.0*PI));
  printf("raio: %d\n",raio);
  return raio;
}

// Função usada para borrar a área de um círculo achado
//
//
void cancelaCirculo(Imagem1C* img, int i, int j, int raio)
{
  int k, w;
  for (k = i; k < i+raio*2; k++)
    for (w = j-raio; w < j+raio; w++)
      img->dados[k][w] = 0;
}

void encontraCentro(Imagem1C* img, int raio, Coordenada* l, Coordenada* r)
{
  int i, j, k, count = 0, maior=0, maior2;
  //percorre a imagem
  for (i = raio+1; i < img->altura-1-raio; i++)
    for (j = raio+1; j < img->largura-1-raio; j++)
    {
      if (img->dados[i][j] == BRANCO)
      {
        for (k = 1; k <= raio; k++)
        {
          if (img->dados[i+k][j])
            count++;
          if (img->dados[i-k][j])
            count++;
          if (img->dados[i][j+k])
            count++;
          if (img->dados[i][j-k])
            count++;
        }
        if (count >= raio*4-2)
        {
          cancelaCirculo(img, i, j, raio*2);
          r->y = l->y;
          r->x = l->x;
          maior2 = maior;
          maior = count;
          l->y = i;
          l->x = j;
        }
      }   
    }
}

double detectaSensorBar (Imagem1C* img, Coordenada* l, Coordenada* r)
{
  int limiar, raio, aux;
  double angulo;
  int i, j;

  //printf("media cores: %d\n", mediaGeral(img));
  binariza(img);
  removeRuido(img);

  raio = mapeiaCirculo(img);
  encontraCentro(img, raio, l, r);

  if (l->x > r->x)
  {
    aux = l->x;
    l->x = r->x;
    r->x = aux;
    aux = l->y;
    l->y = r->y;
    r->y = aux;
  }
  
  angulo = atan2(r->y-l->y, r->x-l->x);
  //angulo *= (180.0 / M_PI);
  return angulo;  
}