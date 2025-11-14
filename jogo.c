#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <raylib.h>
#include "config.h"
#include "tela_inical.h"
#include "mapa.h"
#include "pacman.h"
#include "fantasma.h"
#include "itens.h"


static const char *MAPA[ALTURA] = { //layout do labirinto; cada elemento do array é um ponteiro p um caractere; 
    //cada ponteiro aponta para o inicio de uma linha do labirinto
    "########################################",
    "#...................#..................#",
    "#..####....######...#...######...####..#",
    "#..#  #....#    #...#...#    #...#  #..#",
    "#..####....######.......######...####..#",
    "#......................................#",
    "#..####....###.............###...####..#",
    "#..#  #....# ####.......#### #...#  #..#",
    "#..####....#    #.......#    #...####..#",
    "#..........# ####.......#### #.........#",
    "#..####....# #......#......# #...####..#",
    "#..........###......#......###.........#",
    "#...................#..................#",
    "#..####....######...#...######...####..#",
    "#..#  #....#    #...#...#    #...#  #..#",
    "#..####....######.......######...####..#",
    "#...................#..................#",
    "#..######...####....#...####...######..#",
    "#..#    #...#  #....#...#  #...#    #..#",
    "#..######...####........####...######..#",
    "#......................................#",
    "########################################"
};


void inicalizar(Jogo *jogo);
void atualizar_pacman(Jogo *jogo);
void atualizar_fantasma(Jogo *jogo);
void liberar(Jogo *jogo);

int main(){

    Jogo jogo = {0};

    InitWindow(JANELA_LARGURA, JANELA_ALTURA, "Pacman com raylib em C");
    SetTargetFPS(10);

    Texture2D fundoMenu = LoadTexture("assets/menu.png");
    MostrarTelaInicial(fundoMenu);
    inicalizar(&jogo);
    

    while(!WindowShouldClose()){
        //if criado para verificar se vai ser GameOver
        if(jogo.jogo_ativo){
            atualizar_pacman(&jogo);
            atualizar_fantasma(&jogo); 
            colisao(&jogo);
        }

        BeginDrawing();
            ClearBackground(BLACK); //fundo preto

            if(jogo.jogo_ativo){
                desenhar_mapa(&jogo);
                desenhar_pacman(jogo.pacman);
                desenhar_comida(jogo.comida);
                
                if(jogo.inicializarFruta){
                    desenhar_fruta(jogo.fruta);
                }

                for(int i=0; i<jogo.qntd_fantasmas; i++){
                    desenhar_fantasma(jogo.fantasmas[i], jogo.fantasmas[i].cor);
                }
            }else{
                DrawText("GAME OVER!", JANELA_LARGURA/2-100, JANELA_ALTURA/2-20, 40, RED);
            }
            
        EndDrawing();
    }
    liberar(&jogo);
    CloseWindow();
    return 0;
}

void inicalizar(Jogo *jogo){
    //Mapa
    jogo->mapa.largura = LARGURA;
    jogo->mapa.altura = ALTURA;
    jogo->mapa.mapa = (char**)malloc(ALTURA*sizeof(char*)); //alocar a memoria

    for(int i=0; i<ALTURA; i++){
        jogo->mapa.mapa[i]=(char*)malloc((LARGURA+1)*sizeof(char)); //alocar a memoria
        strcpy(jogo->mapa.mapa[i], MAPA[i]);
    }

    jogo->jogo_ativo=true;
    jogo->pontuacao_atual=0;
    jogo->pontoComida=0;
    jogo->maxComida=50;
    jogo->inicializarFruta=false;

    //Pacman
    jogo->pacman.x=2;
    jogo->pacman.y=1;
    jogo->pacman.dx=1;
    jogo->pacman.dy=0;

    //Fantasmas
    jogo->qntd_fantasmas=4;
    jogo->fantasmas=(Fantasma*)malloc((jogo->qntd_fantasmas)*sizeof(Fantasma));

    Color cor_fantasma[]={RED, PINK, ORANGE, SKYBLUE};
    int total_cores=sizeof(cor_fantasma)/sizeof(cor_fantasma[0]);

    for(int i=0; i<jogo->qntd_fantasmas; i++){
        jogo->fantasmas[i].x=20 + i; //posição inicial
        jogo->fantasmas[i].y =8; //posição inicial
        jogo->fantasmas[i].dx=-1;
        jogo->fantasmas[i].dy=0;

        jogo->fantasmas[i].cor = cor_fantasma[i%total_cores];
    }

    gerar_comida(jogo);
    //gerar_fruta(jogo);
}

void liberar(Jogo *jogo){
    for(int i=0; i<jogo->mapa.altura; i++){
        free(jogo->mapa.mapa[i]);
    }
    free(jogo->mapa.mapa);
    jogo->mapa.mapa=NULL;

    if(jogo->fantasmas!=NULL){
        free(jogo->fantasmas);
        jogo->fantasmas=NULL;
    }
}