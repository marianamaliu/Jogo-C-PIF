#ifndef FANTASMA_H
#define FANTASMA_H

#include <raylib.h>
#include <stdlib.h>
#include "config.h"

void desenhar_fantasma(Fantasma fantasma, Color CorFantasma);
void atualizar_fantasma(Jogo *jogo);
void desenhar_fantasma_cego(Fantasma fantasma);

#endif