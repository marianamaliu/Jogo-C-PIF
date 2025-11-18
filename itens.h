#ifndef ITENS_H
#define ITENS_H

#include <raylib.h>
#include <stdlib.h>
#include "config.h"

void desenhar_comida(Comida comida);
void desenhar_fruta(Jogo *jogo);
void gerar_comida(Jogo *jogo);
void gerar_fruta(Jogo *jogo);

void desenhar_cereja(int centroX, int centroY, int raio_base);
void desenhar_morango(int centroX, int centroY, int raio_base);
void desenhar_laranja(int centroX, int centroY, int raio_base);

#endif