#ifndef STRUCT_H
#define STRUCT_H

#include <raylib.h>
#include <stdbool.h>

#define ALTURA 22
#define LARGURA 40
#define TILE_SIZE 20
#define JANELA_LARGURA (LARGURA * TILE_SIZE)
#define JANELA_ALTURA (ALTURA * TILE_SIZE)

// modos do fantasma
typedef enum {
    MODO_SCATTER,
    MODO_CHASE,
    MODO_FRIGHTENED
} ModoFantasma;

// tipos do fantasma
typedef enum {
    BLINKY = 0,
    PINKY  = 1,
    INKY   = 2,
    CLYDE  = 3
} TipoFantasma;

typedef struct {
    int x, y;
    int dx, dy;
    bool vivo;
} Pacman;

typedef struct {
    int x, y;
    int dx, dy;
    int spawnX, spawnY;

    ModoFantasma modo;
    TipoFantasma tipo;

    float velocidade;
    float timer_modo;

    int targetX, targetY;
    Color cor;
} Fantasma;

typedef struct {
    int x, y;
} Comida;

typedef struct {
    int x, y;
} Fruta;

typedef struct {
    int largura;
    int altura;
    char **mapa;
} Mapa;

typedef struct Score {
    int pontos;
    struct Score *next;
} Score;

typedef struct {
    Mapa mapa;
    Pacman pacman;
    Fantasma *fantasmas;
    int qntd_fantasmas;
    Comida comida;
    Fruta fruta;
    Score score;
    bool jogo_ativo;
    int pontuacao_atual;
    int pontoComida;
    int maxComida;
    bool inicializarFruta;
} Jogo;

#endif
