#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "types.h"

void renderMissil(Tanque tanque){
    if(!tanque.missil.emMovimento){
        al_draw_filled_circle(tanque.missil.missil.centro.x + tanque.circuloExterno.centro.x,
                              tanque.missil.missil.centro.y + tanque.circuloExterno.centro.y,
                              tanque.missil.missil.raio, tanque.missil.color);
        return;
    }

    al_draw_filled_circle(tanque.missil.missil.centro.x,
                          tanque.missil.missil.centro.y,
                          tanque.missil.missil.raio, tanque.missil.color);
}

void renderTanque(Tanque tanque, VariaveisGlobais variables){
    al_draw_circle(tanque.circuloExterno.centro.x, tanque.circuloExterno.centro.y, variables.RaioCirculo, tanque.color, 1);
    al_draw_filled_triangle(tanque.A.x + tanque.circuloExterno.centro.x, tanque.A.y + tanque.circuloExterno.centro.y,
                            tanque.B.x + tanque.circuloExterno.centro.x, tanque.B.y + tanque.circuloExterno.centro.y,
                            tanque.C.x + tanque.circuloExterno.centro.x, tanque.C.y + tanque.circuloExterno.centro.y,
                            tanque.color);

    renderMissil(tanque);
}


void render(Tanques tanques, Cenario cenario, VariaveisGlobais globais) {
    al_clear_to_color(al_map_rgb(255, 255, 255));
    Obstaculo obstaculo = cenario.obstaculoRetangular;
    al_draw_filled_rectangle(obstaculo.superiorEsquerdo.x, obstaculo.superiorEsquerdo.y,
                             obstaculo.inferiorDireito.x, obstaculo.inferiorDireito.y, obstaculo.color);


    renderTanque(tanques.tanqueA, globais);
    renderTanque(tanques.tanqueB, globais);
}

void renderGameOver(Tanques tanques, VariaveisGlobais globais) {
    ALLEGRO_COLOR red = al_map_rgb(255, 0, 0);

    char score[20];
    sprintf(score, "TanqueA %d x %d TanqueB", tanques.tanqueB.pontos);
    al_draw_text(globais.size_32, red,
                 (2.f * (float)globais.SCREEN_W / 6.0f), (float)globais.SCREEN_H / 3.0f, 0, score);

    char tanqueA[35];
    sprintf(tanqueA, "Tanque A venceu: %d vezes", globais.scoreBoard.tanqueA);
    al_draw_text(globais.size_32, red,
                 (2.1f * (float)globais.SCREEN_W / 6.0f), 1.5f * (float)globais.SCREEN_H / 3, 0, tanqueA);

    char tanqueB[35];
    sprintf(tanqueB, "Tanque B venceu: %d vezes", globais.scoreBoard.tanqueB);
    al_draw_text(globais.size_32, red,
                 (2.1f * (float)globais.SCREEN_W / 6), 2.0f * (float)globais.SCREEN_H / 3, 0, tanqueB);

}