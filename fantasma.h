#ifndef FANTASMA_H
#define FANTASMA_H

#include <raylib.h>
#include <stdlib.h>
#include "config.h"

static void garantir_seed();
static void inicializar_direcao_valida(Fantasma *f, Jogo *j);
static bool bfs_proximo_passo(Jogo *j, int sx, int sy, int tx, int ty, int *next_x, int *next_y);
static void alvo_blinky(Jogo *j, Fantasma *f, int *ax, int *ay);
static void alvo_pinky(Jogo *j, Fantasma *f, int *ax, int *ay);
static void alvo_inky(Jogo *j, Fantasma *f, int *ax, int *ay);
static void alvo_clyde(Jogo *j, Fantasma *f, int *ax, int *ay);
static void definir_alvo(Fantasma *f, Jogo *j);
void desenhar_fantasma(Fantasma fantasma, Color CorFantasma);
void atualizar_fantasma(Jogo *jogo);

#endif