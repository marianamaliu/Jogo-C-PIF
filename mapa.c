#include "mapa.h"

void desenhar_mapa(Jogo *jogo){
    Color cor = DARKBLUE;
    
    //desenho do mapa
    for(int i=0; i<ALTURA; i++){ //y
        for(int j=0; j<LARGURA; j++){ //x
            char tile = jogo->mapa.mapa[i][j]; //pega a ALTURA e LARGURA do mapa do layout do labirinto

            //centro do "bloco" (onde seria o quadrado inteiro), para desenhare as linhas
            float centroX = j * TILE_SIZE + TILE_SIZE / 2; //converte o indice da coluna na matriz p coordenada em pixel
            float centroY = i * TILE_SIZE + TILE_SIZE / 2;

            if(tile=='#'){
                
                if (j + 1 < LARGURA && jogo->mapa.mapa[i][j+1] == '#') {
                    // Se é parede
                    float prox_centroX = (j + 1) * TILE_SIZE + TILE_SIZE / 2;
                    //serve para desenhar do bloco atual ate o centro do prox bloco
                    DrawLineEx((Vector2){centroX, centroY}, (Vector2){prox_centroX, centroY}, GROSSURA_LINHA, cor);
                }
                
                //verifica se o prox bloco tambem é parede
                if (i + 1 < ALTURA && jogo->mapa.mapa[i+1][j] == '#' && jogo->mapa.mapa[i+1][j] != 'G') {
                    float prox_centroY = (i + 1) * TILE_SIZE + TILE_SIZE / 2;
                    DrawLineEx((Vector2){centroX, centroY},(Vector2){centroX, prox_centroY},GROSSURA_LINHA, cor);
                }
            
            }else if(tile == '.'){
                DrawCircle( j * TILE_SIZE + TILE_SIZE/2,   i * TILE_SIZE + TILE_SIZE/2, TILE_SIZE/8, YELLOW);
                //calcula o centro do bloco para o ponto ficar centralizado em cada bloco; TILE_SIZE/8 define o raio do circulo (20/8=2.5 pixels)
            }else if (tile == 'G') {
                float extensao_lateral = TILE_SIZE * 0.25f; // Ajusta o quanto a porta se estende para os lados
                                                            // 0.25f significa 1/4 de um tile para cada lado
                DrawLineEx(
                    (Vector2){j * TILE_SIZE - extensao_lateral, centroY}, // Começa um pouco antes do tile
                    (Vector2){(j + 1) * TILE_SIZE + extensao_lateral, centroY}, // Termina um pouco depois do tile
                    GROSSURA_LINHA,
                    WHITE
                );
            }
        }
    }
}