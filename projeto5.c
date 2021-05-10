#include "projeto5.h"
#include <stdio.h>

#define DENTRO_CIRCULO 1
#define FORA_CIRCULO 0

unsigned char calculaMediaCursor(Imagem1C* img, int i, int j,int tam)
{
  unsigned char media = img->dados[i][j]/(tam*2+1);
  int k;
  
  if (j-tam >= 0 && j+tam < img->largura)
  {
    //printf("\nmeu pai\n");
    for (k = 0; k < tam; k++)
      media += img->dados[i][j+k+1]/(tam*2+1) + img->dados[i][j-k-1]/(tam*2+1);
  }
  else
      return 0;
  //printf("\n%x\n", media/(tam*2+1));
  return media;
}

double detectaSensorBar (Imagem1C* img, Coordenada* l, Coordenada* r)
{
  int i, j, diam1 = 0, diam2, countDiam, estado;
  // Percorre imagem
  for (i = 0; i < img->altura; i++)
  {
    countDiam = 0;
    for (j = 0; j < img->largura; j++)
    {
      if(calculaMediaCursor(img, i, j, 2) >= 0xc2)
      {
        if (estado)
          countDiam++;

        estado = DENTRO_CIRCULO;
      }
      else
      {
        //printf("\ncu\n");
        if (countDiam > diam1)
        {
          diam1 = countDiam;
          l->x = diam1/2 + j;
          l->y = diam1/2 + i;
          
        }
          
        countDiam = 0;
        estado = FORA_CIRCULO;
      }
    }
  }
  
  return 0;
}