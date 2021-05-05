/*============================================================================*/
/* GERADOR DE CASOS DE TESTE PARA O PROBLEMA DO WII SENSOR BAR                */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu - nassu@dainf.ct.utfpr.edu.br                       */
/*============================================================================*/
/** Um gerador de casos de teste para o problema do Wii Sensor Bar. Geramos uma
 * imagem preta com dois c�rculos brancos, adicionamos ru�do bin�rio, aplicamos
 * uma suaviza��o Gaussiana, e adicionamos ru�do Gaussiano. */
/*============================================================================*/

#ifndef __GERADOR_DE_TESTES_H
#define __GERADOR_DE_TESTES_H

/*============================================================================*/

#include "imagem.h"
#include "projeto5.h"

/*============================================================================*/

Imagem1C* geraCasoDeTeste (int seed, Coordenada* l, Coordenada* r, double* angulo);

/*============================================================================*/
#endif /* __GERADOR_DE_TESTES_H */
