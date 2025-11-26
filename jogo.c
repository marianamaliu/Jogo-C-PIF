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

#define SCORE_MAX "score_max.dat"

static const char *MAPA[ALTURA] = { //layout do labirinto; cada elemento do array é um ponteiro p um caractere; 
    //cada ponteiro aponta para o inicio de uma linha do labirinto
    "########################################",
    "#...................#..................#",
    "#..####....######...#...######...####..#",
    "#..#  #....#    #...#...#    #...#  #..#",
    "#..####....######.......######...####..#",
    "#......................................#",
    "#..####....####..###G###..####...####..#",
    "#..#  #....# ### #     # ### #...#  #..#",
    "#..####....#   # #     # #   #...####..#",
    "#..........# ### ####### ### #.........#",
    "#..####....# #.............# #...####..#",
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
void desenhar_placar(Jogo *jogo);
void Score_max(Jogo *jogo, int pontuacao_atual);
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
            if(!existe_bolinha(&jogo)){
                jogo.jogo_ativo = false; //se n existe, o jogo acaba 
            }

            if(jogo.tempo_iniciar>0){
                jogo.tempo_iniciar--; //contador dos 3 segundos iniciais ir diminuindo 
            }else{
                atualizar_pacman(&jogo);
                atualizar_fantasma(&jogo); 
                colisao(&jogo);

                if(jogo.inicializarFruta){
                    jogo.fruta.temporizador--;
                    if(jogo.fruta.temporizador<=0){
                        jogo.inicializarFruta=false;
                    }
                }
            }
        }

        BeginDrawing();
            ClearBackground(BLACK); //fundo preto

            if(jogo.jogo_ativo){
                desenhar_mapa(&jogo);
                desenhar_pacman(jogo.pacman);
                desenhar_comida(jogo.comida);
                desenhar_placar(&jogo);
                
                if(jogo.inicializarFruta){
                    desenhar_fruta(&jogo);
                }

                for(int i=0; i<jogo.qntd_fantasmas; i++){
                    if(jogo.tempo_cego > 0){
                        desenhar_fantasma_cego(jogo.fantasmas[i]);
                    } else {
                        desenhar_fantasma(jogo.fantasmas[i], jogo.fantasmas[i].cor);
                    }
                }

                if(jogo.tempo_iniciar>0){
                    const char *ready_text = "READY!";
                    int font_size = 50;
                    int text_width = MeasureText(ready_text, font_size);
                    int x_pos = (JANELA_LARGURA / 2) - (text_width / 2);
                    int y_pos = (JANELA_ALTURA / 2) + TILE_SIZE; // Abaixo da casa dos fantasmas
                    
                    DrawText(ready_text, x_pos, y_pos, font_size, YELLOW);
                }
            }else{
                Score_max(&jogo, jogo.pontuacao_atual);
                char score_text[50];
                char high_score_text[50];

                if(!existe_bolinha(&jogo)){
                    DrawText("VOCÊ VENCEU O JOGO!", JANELA_LARGURA/2 - MeasureText("VOCÊ VENCEU O JOGO!", 40)/2, JANELA_ALTURA/4, 40, GOLD);
                    sprintf(score_text, "SEU SCORE: %d", jogo.pontuacao_atual);
                    sprintf(high_score_text, "HIGH SCORE: %d", jogo.pontuacao_max);

                    DrawText(score_text, JANELA_LARGURA/2 - MeasureText(score_text, 25)/2, JANELA_ALTURA/2, 25, YELLOW);
                    DrawText(high_score_text, JANELA_LARGURA/2 - MeasureText(high_score_text, 25)/2, JANELA_ALTURA/2 + 40, 25, WHITE);

                    DrawText("PRESSIONE ENTER PARA JOGAR NOVAMENTE", 
                            JANELA_LARGURA/2 - MeasureText("PRESSIONE ENTER PARA JOGAR NOVAMENTE", 20)/2, 
                            JANELA_ALTURA/2 + 100, 20, GREEN);
                    
                    if(IsKeyPressed(KEY_ENTER)){
                        MostrarTelaInicial(fundoMenu);
                        liberar(&jogo);
                        inicalizar(&jogo);
                    }
                }else{
                    DrawText("GAME OVER!", JANELA_LARGURA/2 - MeasureText("GAME OVER!", 40)/2, JANELA_ALTURA/4, 40, RED);
                    
                    sprintf(score_text, "SEU SCORE: %d", jogo.pontuacao_atual);
                    sprintf(high_score_text, "HIGH SCORE: %d", jogo.pontuacao_max);

                    DrawText(score_text, JANELA_LARGURA/2 - MeasureText(score_text, 25)/2, JANELA_ALTURA/2, 25, YELLOW);
                    DrawText(high_score_text, JANELA_LARGURA/2 - MeasureText(high_score_text, 25)/2, JANELA_ALTURA/2 + 40, 25, WHITE);

                    DrawText("PRESSIONE ENTER PARA JOGAR NOVAMENTE", 
                            JANELA_LARGURA/2 - MeasureText("PRESSIONE ENTER PARA JOGAR NOVAMENTE", 20)/2, 
                            JANELA_ALTURA/2 + 100, 20, GREEN);
                    
                    if(IsKeyPressed(KEY_ENTER)){
                        MostrarTelaInicial(fundoMenu);
                        liberar(&jogo);
                        inicalizar(&jogo);
                    }
                }
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
    Score_max(jogo, 0);
    jogo->pontoComida=0;
    jogo->maxComida=50;
    jogo->inicializarFruta=false;
    jogo->tempo_iniciar=30; //3 segundos * 10 FPS

    int spawn_caixa_x = 18; // Posição da caixa dos fantasmas (Mantido)
    int spawn_caixa_y = 7;

    // seed do rand (uma vez)
    srand((unsigned)time(NULL));

    //Pacman
    jogo->pacman.x=20;
    jogo->pacman.y=10;
    jogo->pacman.dx=1;
    jogo->pacman.dy=0;

    //Fantasmas
    jogo->qntd_fantasmas=4;
    jogo->fantasmas=(Fantasma*)malloc((jogo->qntd_fantasmas)*sizeof(Fantasma));

    Color cor_fantasma[]={RED, PINK, ORANGE, SKYBLUE};
    int total_cores=sizeof(cor_fantasma)/sizeof(cor_fantasma[0]);

    int spawn_x = spawn_caixa_x + 1; 
    int spawn_y = spawn_caixa_y + 1;

    for(int i=0; i<jogo->qntd_fantasmas; i++){
        Fantasma *f = &jogo->fantasmas[i];

        f->id = i;                                   // importante!
        f->x = spawn_x + i;
        f->y = spawn_y;

        // direção inicial válida (se quiser forçar outra, troque aqui)
        f->dx = -1;
        f->dy = 0;

        f->cor = cor_fantasma[i % total_cores];

        // campos necessários inicializados:
        f->tempo_para_sair = 0;       // 0 = já pode se mover; se quiser delay, coloque >0
        f->alvo_x = f->x;
        f->alvo_y = f->y;
    }

    gerar_comida(jogo);
    //gerar_fruta(jogo);
}


void desenhar_placar(Jogo *jogo) {
    char score_buffer[32];
    int font_size = 20; 

    int placar_y = TILE_SIZE / 2; 

    //score Atual
    sprintf(score_buffer, "SCORE: %d", jogo->pontuacao_atual);
    DrawText(score_buffer, TILE_SIZE, placar_y, font_size, YELLOW);
}

void Score_max(Jogo *jogo, int pontuacao_atual) {
    int recorde_salvo = 0;
    FILE *arquivo = NULL;

    arquivo = fopen(SCORE_MAX, "rb"); //tenta abrir para leitura binária
    
    if (arquivo != NULL) {
        if (fread(&recorde_salvo, sizeof(int), 1, arquivo) != 1) {
            recorde_salvo = 0; 
        }
        fclose(arquivo);
    }
    
    if (recorde_salvo > jogo->pontuacao_max) {
        jogo->pontuacao_max = recorde_salvo;
    }

    if (jogo->pontuacao_atual > jogo->pontuacao_max) {
        
        jogo->pontuacao_max = jogo->pontuacao_atual;
        recorde_salvo = jogo->pontuacao_atual; //atualiza o valor 
        // Salvar o novo recorde
        arquivo = fopen(SCORE_MAX, "wb"); //sobrescreve
        
        if (arquivo != NULL) {
            fwrite(&recorde_salvo, sizeof(int), 1, arquivo);
            fclose(arquivo);
        }
    }
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