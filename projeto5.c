#include "projeto5.h"
#include <stdio.h>
#include <math.h>

#define LIMIAR 176

int ehBranco(Imagem1C* img, int i, int j);
void cancelaCirculo(Imagem1C* img, int i, int j, int raio);
double detectaSensorBar (Imagem1C* img, Coordenada* l, Coordenada* r);
int calculaLimiar(int larg, int alt);
int calculaDiametro(Imagem1C* img, int i, int j);

// vamo faze o seguinte, vamo fazer que nessa função ele ja diz se estora o branco
// adicionar o escape do fracasso
// Cursor em forma de cruz 
//      x
//      x
//  x x x x x
//      x
//      x
int ehBranco(Imagem1C* img, int i, int j)
{
  unsigned char media;

  media = img->dados[i][j]/9 + img->dados[i-1][j]/9 + img->dados[i+1][j]/9 + img->dados[i][j-1]/9 + img->dados[i][j+1]/9 + img->dados[i-2][j]/9 + img->dados[i+2][j]/9 + img->dados[i][j-2]/9 + img->dados[i][j+2]/9;

  /*media = img->dados[i][j]/5 + img->dados[i-1][j]/5 + img->dados[i+1][j]/5 + img->dados[i][j-1]/5 + img->dados[i][j+1]/5;*/
  
  if (media >= LIMIAR)
    return 1;
  return 0;
}

// Função usada para borrar a área de um círculo achado
//
//
void cancelaCirculo(Imagem1C* img, int i, int j, int diam)
{
  int k, w;
  for (k = i; k < i+diam; k++)
    for (w = j-diam/2; w < j+diam/2; w++)
      img->dados[k][w] = 0;
}

//
//
//
int calculaDiametro(Imagem1C* img, int i, int j)
{
  int diam = 0;
  while(ehBranco(img, i, j))
  {
    diam++;
    i++;
  }
  return diam;
}

//
//
//
double detectaSensorBar (Imagem1C* img, Coordenada* l, Coordenada* r)
{
  int i, j, diam;
  double angulo;
  l->x = 0;
  // começa no 1 devido ao tamanho do cursor
  for (i = 2; i < img->altura - 2; i++)
    for (j = 2; j < img->largura - 2; j++)    {
      if (ehBranco(img, i, j))
      {
        diam = calculaDiametro(img, i, j);
        cancelaCirculo(img, i, j, diam);
        if (!l->x)
        {
          l->y = i + diam/2;
          l->x = j;
        }
        else
        {
          r->y = i + diam/2;
          r->x = j;
        }
      }
    }

  int aux;
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