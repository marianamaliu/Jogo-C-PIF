#ifndef FANTASMA_H
#define FANTASMA_H

#include <raylib.h>
#include "config.h"

void desenhar_fantasma(Fantasma fantasma, Color CorFantasma);
void atualizar_fantasma(Jogo *jogo);
void ativar_modo_assustado(Jogo *jogo);

#endif // FANTASMA_H
