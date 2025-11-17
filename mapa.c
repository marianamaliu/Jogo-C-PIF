#include "mapa.h"
#include <raylib.h>

void desenhar_mapa(Jogo *jogo){
    for(int i = 0; i < ALTURA; i++){
        for(int j = 0; j < LARGURA; j++){
            char tile = jogo->mapa.mapa[i][j];
            float centroX = j * TILE_SIZE + TILE_SIZE/2;
            float centroY = i * TILE_SIZE + TILE_SIZE/2;

            if (tile == '#') {
                if (j + 1 < LARGURA && jogo->mapa.mapa[i][j+1] == '#') {
                    float prox_centroX = (j + 1) * TILE_SIZE + TILE_SIZE/2;
                    DrawLineEx((Vector2){centroX, centroY}, (Vector2){prox_centroX, centroY}, GROSSURA_LINHA, DARKBLUE);
                }
                if (i + 1 < ALTURA && jogo->mapa.mapa[i+1][j] == '#') {
                    float prox_centroY = (i + 1) * TILE_SIZE + TILE_SIZE/2;
                    DrawLineEx((Vector2){centroX, centroY}, (Vector2){centroX, prox_centroY}, GROSSURA_LINHA, DARKBLUE);
                }
            } else if (tile == '.') {
                DrawCircle(j * TILE_SIZE + TILE_SIZE/2, i * TILE_SIZE + TILE_SIZE/2, TILE_SIZE/8, YELLOW);
            }
        }
    }
}
