#ifndef PACMAN_H
#define PACMAN_H

#include <raylib.h>
#include <math.h>
#include "config.h"
#include "itens.h"

void desenhar_pacman(Pacman pacman);
void atualizar_pacman(Jogo *jogo);
void colisao(Jogo *jogo);

#endif