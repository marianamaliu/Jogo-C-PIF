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
    Comida comida;
    Fruta fruta;
    int qntd_fantasmas;
    bool jogo_ativo;
    int pontuacao_atual;

    Score *pontuacao;
}Jogo;

void inicalizar(Jogo *jogo);
void liberar_mapa(Mapa *m);
void desenhar_mapa(Jogo *jogo);
void desenhar_pacman(Pacman pacman);
void desenhar_comida(Comida comida);
void desenhar_fruta(Fruta fruta);
void gerar_comida(Jogo *jogo);
void gerar_fruta(Jogo *jogo);
void colisao(Jogo *jogo);
void atualizar_posi(Jogo *jogo);

int main(){

    Jogo jogo = {0};
    inicalizar(&jogo);

    InitWindow(JANELA_LARGURA, JANELA_ALTURA, "Pacman com raylib em C");
    SetTargetFPS(10);

    while(!WindowShouldClose()){
        atualizar_posi(&jogo);
        colisao(&jogo);

        BeginDrawing();
            ClearBackground(BLACK); //fundo preto
            desenhar_mapa(&jogo);
            desenhar_pacman(jogo.pacman);
            desenhar_comida(jogo.comida);
            desenhar_fruta(jogo.fruta);
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

    jogo->pacman.x=2;
    jogo->pacman.y=1;
    jogo->pacman.dx=1;
    jogo->pacman.dy=0;

    gerar_comida(jogo);
    gerar_fruta(jogo);
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
    //converte as coordenadas da matriz em coodenadas de pixel
    float pixelX = (float)(pacman.x *TILE_SIZE);
    float pixelY = (float)(pacman.y *TILE_SIZE);

    DrawCircle(pixelX + TILE_SIZE/2, pixelY + TILE_SIZE/2, TILE_SIZE/2-1, YELLOW);
    //pixelX (onde o bloco comça), TILE_SIZE/2 (centro do bloco na horizontal), pixelX+TILE_SIZE/2 (soma do ponto inicial com o centro, onde o centro do circulo estará)
    //TILE_SIZE/2-2 (centro do bloco - 1pixel , para definir o tamanhgo do pacman)
}

void desenhar_comida(Comida comida){
    float pixelX=(float)(comida.x *TILE_SIZE);
    float pixelY=(float)(comida.y *TILE_SIZE);

    DrawCircle(pixelX + TILE_SIZE/2, pixelY + TILE_SIZE/2, TILE_SIZE/2-4, YELLOW);

}

void desenhar_fruta(Fruta fruta){
    float pixelX=(float)(fruta.x*TILE_SIZE);
    float pixelY=(float)(fruta.y*TILE_SIZE);

    int centroX=pixelX+TILE_SIZE/2;
    int centroY=pixelY+TILE_SIZE/2;
    int raio_cereja=TILE_SIZE/4;

    //caule 1
    DrawLine(centroX, centroY-raio_cereja*2, centroX-raio_cereja, centroY+raio_cereja-raio_cereja/2, GREEN);
    //caule 2
    DrawLine(centroX, centroY-raio_cereja*2, centroX+raio_cereja, centroY+raio_cereja-raio_cereja/2, GREEN);
    //cereja 1
    DrawCircle(centroX-raio_cereja, centroY+raio_cereja, raio_cereja, RED);
    //cereja 2
    DrawCircle(centroX+raio_cereja, centroY+raio_cereja, raio_cereja, RED);
    //brilho da cereja
    DrawCircle(centroX - raio_cereja - raio_cereja/4, centroY + raio_cereja - raio_cereja/4, raio_cereja/4, WHITE);
    DrawCircle(centroX + raio_cereja - raio_cereja/4, centroY + raio_cereja - raio_cereja/4, raio_cereja/4, WHITE);
}

void gerar_comida(Jogo *jogo){
    Comida *comida=&jogo->comida;
    //aleatoriedade
    int novoX = rand() % (LARGURA-2) + 1; 
    int novoY = rand() % (ALTURA-2) + 1;

    //se for na parede, ele sorteia os valores de novo ate nao ser na parede
    if(jogo->mapa.mapa[novoY][novoX]=='#'){
        gerar_comida(jogo);
    }else{
        comida->x=novoX;
        comida->y=novoY;
    }
}

void gerar_fruta(Jogo *jogo){
    Fruta *fruta=&jogo->fruta;
    Comida *comida=&jogo->comida;

    //aleatoriedade
    int novoX=rand()%(LARGURA-2)+1;
    int novoY=rand()%(ALTURA-1)+1;

    //se for parede ou se for onde ja tem comida, ele gera de novo ate nao ser parede ou nao ter comida
    if(jogo->mapa.mapa[novoY][novoX]=='#' || jogo->mapa.mapa[novoY][novoX]==comida->x && jogo->mapa.mapa[novoY][novoX]==comida->y){
        gerar_fruta(jogo);
    }else{
        fruta->x=novoX;
        fruta->y=novoY;
    }
}

void colisao(Jogo *jogo){
    Pacman *pacman=&jogo->pacman;
    Comida *comida=&jogo->comida;
    Fruta *fruta=&jogo->fruta;

    int proximoX = pacman->x + pacman->dx;
    int proximoY = pacman->y + pacman->dy;

    //detectar parede
    if(proximoX >=0 && proximoX < LARGURA && proximoY>=0 && proximoY<ALTURA &&
    jogo->mapa.mapa[proximoY][proximoX]!='#'){
        pacman->x = proximoX;
        pacman->y = proximoY;

        //detectar bolinhas menores
        if(jogo->mapa.mapa[proximoY][proximoX]=' '){
            jogo->mapa.mapa[proximoY][proximoX]='.'; //ele desenha a bolinha, entao quando vai para a função desenhar, o espaço nao esta mais vazio,
            //entao ele nao desenha a bolinha amarela
        }
    }
    
    //detectar comida
    if(proximoX==comida->x && proximoY==comida->y){
        jogo->pontuacao_atual+=10;
        gerar_comida(jogo);
    }

    //detectar fruta
    if(proximoX==fruta->x && proximoY==fruta->y){
        jogo->pontuacao_atual+=50;
        gerar_fruta(jogo);
    }

}

void atualizar_posi(Jogo *jogo){
    Pacman *pacman = &jogo->pacman;
    
    if(IsKeyDown(KEY_RIGHT)){
        pacman->dx=1;
        pacman->dy=0;
    }else if(IsKeyDown(KEY_LEFT)){
        pacman->dx=-1;
        pacman->dy=0;
    }else if(IsKeyDown(KEY_UP)){
        pacman->dx=0;
        pacman->dy=-1;
    }else if(IsKeyDown(KEY_DOWN)){
        pacman->dx=0;
        pacman->dy=1;
    }

}

void liberar_mapa(Mapa *m){
    for(int i=0; i<m->altura; i++){
        free(m->mapa[i]);
    }
    free(m->mapa);
    m->mapa=NULL;
}