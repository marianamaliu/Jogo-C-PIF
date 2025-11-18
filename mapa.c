#include "mapa.h"
#include <raylib.h>

void desenhar_mapa(Jogo *jogo)
{
    for (int y = 0; y < jogo->mapa.altura; y++) {
        for (int x = 0; x < jogo->mapa.largura; x++) {
            char c = jogo->mapa.mapa[y][x];

            if (c == '#') {
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKBLUE);
            } else if (c == '.') {
                DrawCircle(x * TILE_SIZE + TILE_SIZE/2, y * TILE_SIZE + TILE_SIZE/2, 3, YELLOW);
            }
        }
    }
}
