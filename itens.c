#include "itens.h"
#include <stdlib.h>
#include <raylib.h>

void desenhar_comida(Comida comida) {
    float pixelX = comida.x * TILE_SIZE + TILE_SIZE/2;
    float pixelY = comida.y * TILE_SIZE + TILE_SIZE/2;
    DrawCircle(pixelX, pixelY, TILE_SIZE/8, YELLOW);
}

void desenhar_fruta(Fruta fruta) {
    float pixelX = fruta.x * TILE_SIZE + TILE_SIZE/2;
    float pixelY = fruta.y * TILE_SIZE + TILE_SIZE/2;
    int raio = TILE_SIZE/4;
    DrawCircle(pixelX - raio/2, pixelY, raio, RED);
    DrawCircle(pixelX + raio/2, pixelY, raio, RED);
}

void gerar_comida(Jogo *jogo) {
    // escolhe aleatoriamente um tile '.' (caso queira manter comidas no mapa, não é necessário)
    // aqui apenas garante que exista comida inicial se quiser
    // NÃO usamos isso pra cada bolinha: o mapa já tem '.' que são desenhados direto
    (void)jogo;
}

void gerar_fruta(Jogo *jogo) {
    if (!jogo) return;
    int attempts = 0;
    while (attempts < 200) {
        int nx = rand() % (LARGURA - 2) + 1;
        int ny = rand() % (ALTURA - 2) + 1;
        if (jogo->mapa.mapa[ny][nx] == '.') {
            jogo->fruta.x = nx;
            jogo->fruta.y = ny;
            jogo->inicializarFruta = true;
            return;
        }
        attempts++;
    }
    // se falhar, não inicializa
    jogo->inicializarFruta = false;
}
