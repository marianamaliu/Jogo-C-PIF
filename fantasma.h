#ifndef FANTASMA_H
#define FANTASMA_H

#include "struct.h"
#include <raylib.h>

void desenhar_fantasma(Fantasma fantasma, Color cor);
void atualizar_fantasma(Jogo *jogo);
void ativar_modo_assustado(Jogo *jogo);

#endif
