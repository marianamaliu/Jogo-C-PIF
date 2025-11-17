#include "itens.h"
#include <stdlib.h>

// desenhar comida e fruta
void desenhar_comida(Comida comida){
    float pixelX = (float)(comida.x * TILE_SIZE);
    float pixelY = (float)(comida.y * TILE_SIZE);
    DrawCircle(pixelX + TILE_SIZE/2, pixelY + TILE_SIZE/2, TILE_SIZE/8, YELLOW);
}

void desenhar_fruta(Fruta fruta){
    float pixelX = (float)(fruta.x * TILE_SIZE);
    float pixelY = (float)(fruta.y * TILE_SIZE);
    int centroX = (int)pixelX + TILE_SIZE/2;
    int centroY = (int)pixelY + TILE_SIZE/2;
    int raio = TILE_SIZE/4;

    DrawLine(centroX, centroY - raio*2, centroX - raio, centroY + raio - raio/2, GREEN);
    DrawLine(centroX, centroY - raio*2, centroX + raio, centroY + raio - raio/2, GREEN);
    DrawCircle(centroX - raio, centroY + raio, raio, RED);
    DrawCircle(centroX + raio, centroY + raio, raio, RED);
    DrawCircle(centroX - raio - raio/4, centroY + raio - raio/4, raio/4, WHITE);
    DrawCircle(centroX + raio - raio/4, centroY + raio - raio/4, raio/4, WHITE);
}

// gerar comida aleatória em tiles que contenham '.'
void gerar_comida(Jogo *jogo){
    int novoX = rand() % (LARGURA - 2) + 1;
    int novoY = rand() % (ALTURA - 2) + 1;

    if (jogo->mapa.mapa[novoY][novoX] != '.') {
        gerar_comida(jogo);
        return;
    }
    jogo->comida.x = novoX;
    jogo->comida.y = novoY;
}

// gerar fruta em local vazio '.'
void gerar_fruta(Jogo *jogo){
    int novoX = rand() % (LARGURA - 2) + 1;
    int novoY = rand() % (ALTURA - 2) + 1;

    if (jogo->mapa.mapa[novoY][novoX] != ' ') {
        gerar_fruta(jogo);
        return;
    }
    jogo->fruta.x = novoX;
    jogo->fruta.y = novoY;
    jogo->inicializarFruta = true;
}
