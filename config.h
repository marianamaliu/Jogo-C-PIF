#ifndef STRUCT_H
#define STRUCT_H

#include <raylib.h>

#define ALTURA 22
#define LARGURA 40
#define TILE_SIZE 20 //tamanho em pixels de cada bloco na tela
#define JANELA_LARGURA (LARGURA*TILE_SIZE) //calcula a largura total da janela em pixels
#define JANELA_ALTURA (ALTURA*TILE_SIZE) //calcula a altura total da janela em pixels
#define GROSSURA_LINHA 2

typedef enum{
    PERSEGUIR,
    ESPALHAR,
    ASSUSTADO,
}ModoFantasma;

typedef struct{
    int x, y; //posicao do pacman
    int dx, dy; //direcao
}Pacman;

typedef struct{
    int x, y; //posicao do fantasma
    int dx, dy; //direcao
    ModoFantasma modo;
    Color cor;
}Fantasma;

typedef struct{
    int x;
    int y;
}Comida;

typedef struct{
    int x;
    int y;
}Fruta;

typedef struct{
    int largura;
    int altura; 
    char **mapa; //mapa[altura][largura]
}Mapa;

typedef struct Score{
    int pontos;
    struct Score *next;
}Score;

typedef struct{
    Mapa mapa;
    Pacman pacman;
    Fantasma *fantasmas; //vetor dinamico p/ ser mais flexivel
    Comida comida;
    Fruta fruta;
    Score score;
    int qntd_fantasmas;
    bool jogo_ativo;
    int pontuacao_atual;
    int pontoComida;
    int maxComida; //qntd de comida p aparecer a fruta
    bool inicializarFruta;
}Jogo;

#endif