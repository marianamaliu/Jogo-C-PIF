#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <raylib.h>

#define ALTURA 22
#define LARGURA 41
#define TILE_SIZE 20 //tamanho em pixels de cada bloco na tela
#define JANELA_LARGURA (LARGURA*TILE_SIZE) //calcula a largura total da janela em pixels
#define JANELA_ALTURA (ALTURA*TILE_SIZE) //calcula a altura total da janela em pixels

static const char *MAPA[ALTURA] = { //layout do labirinto; cada elemento do array é um ponteiro p um caractere; 
    //cada ponteiro aponta para o inicio de uma linha do labirinto
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
void desenhar_pacman(Pacman pacman);

int main(){

    Jogo jogo = {0};
    inicalizar(&jogo);

    InitWindow(JANELA_LARGURA, JANELA_ALTURA, "Pacman com raylib em C");
    SetTargetFPS(60);

    while(!WindowShouldClose()){

        BeginDrawing();
            ClearBackground(BLACK); //fundo preto
            desenhar_mapa(&jogo);
            desenhar_pacman(jogo.pacman);
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
    for(int i=0; i<ALTURA; i++){ //y
        for(int j=0; j<LARGURA; j++){ //x
            char tile = jogo->mapa.mapa[i][j]; //pega a ALTURA e LARGURA do mapa do layout do labirinto

            if(tile=='#'){
                DrawRectangle( j * TILE_SIZE,  i * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKBLUE);
                //pega a posição da linha i/y e multiplica pelo tam do bloco p obter a coordenada i(x) na tela
            }else if(tile == ' '){
                DrawCircle( j * TILE_SIZE + TILE_SIZE/2,   i * TILE_SIZE + TILE_SIZE/2, TILE_SIZE/8, YELLOW);
                //calcula o centro do bloco para o ponto ficar centralizado em cada bloco; TILE_SIZE/8 define o raio do circulo (20/8=2.5 pixels)
            }
        }
    }
}

void desenhar_pacman(Pacman pacman){
    
    float pixelX = (float)(pacman.x *TILE_SIZE);
    float pixelY = (float)(pacman.y *TILE_SIZE);

    DrawCircle(pixelX + TILE_SIZE/2, pixelY + TILE_SIZE/2, TILE_SIZE/2-2, YELLOW);
    //pixelX (onde o bloco comça), TILE_SIZE/2 (centro do bloco na horizontal), pixelX+TILE_SIZE/2 (soma do ponto inicial com o centro, onde o centro do circulo estará)
    //TILE_SIZE/2-2 (centro do bloco - 2pixels , para definir o tamanhgo do pacman)
}

void liberar_mapa(Mapa *m){
    for(int i=0; i<m->altura; i++){
        free(m->mapa[i]);
    }
    free(m->mapa);
    m->mapa=NULL;
}