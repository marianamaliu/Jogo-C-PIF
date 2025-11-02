#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <string.h>
#include <stdbool.h>

#define ALTURA 22
#define LARGURA 41

static const char *MAPA[ALTURA] = {
    "########################################",
    "#                   #                  #",
    "#  ####    ######   #   ######   ####  #",
    "#  #  #    #    #   #   #    #   #  #  #",
    "#  ####    ######       ######   ####  #",
    "#                                      #",
    "#  ####    ###             ###   ####  #",
    "#  #  #    # ####       #### #   #  #  #",
    "#  ####    #    #       #    #   ####  #",
    "#          # ####       #### #         #",
    "#  ####    # #      #      # #   ####  #",
    "#          ###      #      ###         #",
    "#                   #                  #",
    "#  ####    ######   #   ######   ####  #",
    "#  #  #    #    #   #   #    #   #  #  #",
    "#  ####    ######       ######   ####  #",
    "#                   #                  #",
    "#  ######   ####    #   ####   ######  #",
    "#  #    #   #  #    #   #  #   #    #  #",
    "#  ######   ####        ####   ######  #",
    "#                                      #",
    "########################################"
};

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
    int cor_ncurses;
}Fantasma;

typedef struct{
    int largura;
    int altura;
    char **mapa;
}Mapa;

typedef struct{
    Mapa mapa;
    Pacman pacman;
    Fantasma *fantasmas; //vetor dinamico p/ ser mais flexivel
    int qntd_fantasmas;
    bool jogo_ativo;
}Jogo;


int main(){




    return 0;
}