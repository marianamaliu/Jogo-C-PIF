#ifndef CONFIG_H
#define CONFIG_H

#include <raylib.h>
#include <stdbool.h>

#define ALTURA 22
#define LARGURA 40
#define TILE_SIZE 20
#define JANELA_LARGURA (LARGURA * TILE_SIZE)
#define JANELA_ALTURA (ALTURA * TILE_SIZE)
#define GROSSURA_LINHA 2

typedef enum {
    PERSEGUIR,
    ESPALHAR,
    ASSUSTADO,
} ModoFantasma;

typedef struct {
    int x, y;
    int dx, dy;
} Pacman;

typedef struct {
    int x, y;
    int dx, dy;
    ModoFantasma modo;
    Color cor;
    float velocidade;   // multiplicador de velocidade (1.0 normal, 0.5 assustado)
    float timer_modo;   // temporizador para modos como ASSUSTADO
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
    Comida comida;
    Fruta fruta;
    Score score;
    int qntd_fantasmas;
    bool jogo_ativo;
    int pontuacao_atual;
    int pontoComida;
    int maxComida;
    bool inicializarFruta;
} Jogo;

#endif // CONFIG_H
