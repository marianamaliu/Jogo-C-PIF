#include "itens.h"

//desenhar
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

//gerar
void gerar_comida(Jogo *jogo){
    Comida *comida=&jogo->comida;
    //aleatoriedade
    int novoX = rand() % (LARGURA-2) + 1; 
    int novoY = rand() % (ALTURA-2) + 1;

    //se for na parede, ele sorteia os valores de novo ate nao ser na parede
    if(jogo->mapa.mapa[novoY][novoX]=='#' || jogo->mapa.mapa[novoY][novoX]!='.'){ //ainda ta aparecendo entre
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
    if(jogo->mapa.mapa[novoY][novoX]=='#' || jogo->mapa.mapa[novoY][novoX]==comida->x && jogo->mapa.mapa[novoY][novoX]==comida->y && jogo->mapa.mapa[novoY][novoX]){
        gerar_fruta(jogo);
    }else{
        jogo->inicializarFruta=true;
        fruta->x=novoX;
        fruta->y=novoY;
    }
}