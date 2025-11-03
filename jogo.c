#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <raylib.h>

#define ALTURA 22
#define LARGURA 41
#define TILE_SIZE 20
#define SCREEN_WIDTH (LARGURA*TILE_SIZE)
#define SCREEN_HEIGH (ALTURA*TILE_SIZE)

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
    int qntd_fantasmas;
    bool jogo_ativo;
    int pontuacao_atual;

    Score *pontuacao;
}Jogo;

void inicalizar(Jogo *Jogo);
void liberar_mapa(Mapa *m);
void desenhar_mapa(Jogo *jogo);

int main(){

    Jogo jogo = {0};
    inicalizar(&jogo);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGH, "Pacman com raylib em C");
    SetTargetFPS(60);

    while(!WindowShouldClose()){

        BeginDrawing();
            ClearBackground(BLACK); //fundo preto
            desenhar_mapa(&jogo);
        EndDrawing();
    }
    liberar_mapa(&jogo.mapa);
    CloseWindow();


    return 0;
}

void inicalizar(Jogo *jogo){
    jogo->mapa.largura = LARGURA;
    jogo->mapa.altura = ALTURA;
    jogo->mapa.mapa = (char**)malloc(ALTURA*sizeof(char*)); //alocar a memoria

    for(int i=0; i<ALTURA; i++){
        jogo->mapa.mapa[i]=(char*)malloc((LARGURA+1)*sizeof(char)); //alocar a memoria
        strcpy(jogo->mapa.mapa[i], MAPA[i]);
    }

    jogo->jogo_ativo=true;
    jogo->pontuacao_atual=0;

    jogo->pacman.x=1;
    jogo->pacman.y=1;
    jogo->pacman.dx=1;
    jogo->pacman.dy=0;
}

void desenhar_mapa(Jogo *jogo){
    for(int i=0; i<ALTURA; i++){
        for(int j=0; j<LARGURA; j++){
            char tile = jogo->mapa.mapa[i][j];

            if(tile=='#'){
                DrawRectangle(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKBLUE);

            }else if(tile == ' '){
                DrawCircle(i * TILE_SIZE + TILE_SIZE/2, j * TILE_SIZE + TILE_SIZE/2, TILE_SIZE/8, YELLOW);
            }
        }
    }
}

void liberar_mapa(Mapa *m){
    for(int i=0; i<m->altura; i++){
        free(m->mapa[i]);
    }
    free(m->mapa);
    m->mapa=NULL;
}