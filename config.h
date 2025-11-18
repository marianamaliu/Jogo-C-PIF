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
    FANTASMA_SEGUIR,
    FANTASMA_ESPALHAR,
    FANTASMA_CEGO,
    FANTASMA_NA_CAIXA,
    FANTASMA_VOLTANDO_CAIXA,
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
    int id; //cada fantasma
    int tempo_para_sair; //tempo p sair da caixa
    int alvo_x;
    int alvo_y; 
}Fantasma;

typedef struct{
    int x;
    int y;
}Comida;

typedef enum{
    CEREJA,
    MORANGO,
    LARANJA
}TipoFruta;

typedef struct{
    int x;
    int y;
    TipoFruta tipo;
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
    char **matriz;
    Color cor_mapa;
    float velocidade; //p aumentar a velocidade dos fantasmas
    int caixa_x; //onde os fantasmas reaparecem no mapa
    int caixa_y;
    int caixa_largura;
    int caixa_altura;
    TipoFruta fruta_nivel;
}Nivel;

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
    int pontuacao_max;
    int pontoComida;
    int maxComida; //qntd de comida p aparecer a fruta
    bool inicializarFruta;
    
    Nivel niveis[3];
    int nivel_atual;
    int vidas;
    int tempo_cego; //contador p o fantasma cego
}Jogo;

#endif