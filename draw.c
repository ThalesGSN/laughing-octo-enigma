#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "types.h"

void drawMissil(Tanque tanque){
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

void drawTanque(Tanque tanque, VariaveisGlobais variables){
    al_draw_circle(tanque.circuloExterno.centro.x, tanque.circuloExterno.centro.y, variables.FORCE_FIELD_RADIUS, tanque.color, 1);
    al_draw_filled_triangle(tanque.A.x + tanque.circuloExterno.centro.x, tanque.A.y + tanque.circuloExterno.centro.y,
                            tanque.B.x + tanque.circuloExterno.centro.x, tanque.B.y + tanque.circuloExterno.centro.y,
                            tanque.C.x + tanque.circuloExterno.centro.x, tanque.C.y + tanque.circuloExterno.centro.y,
                            tanque.color);

    drawMissil(tanque);
}


void render(Tanques tanques, Cenario cenario, VariaveisGlobais globais) {
    al_clear_to_color(al_map_rgb(255, 255, 255));
    Obstaculo obstaculo = cenario.obstaculoRetangular;
    al_draw_filled_rectangle(obstaculo.superiorEsquerdo.x, obstaculo.superiorEsquerdo.y,
                             obstaculo.inferiorDireito.x, obstaculo.inferiorDireito.y, obstaculo.color);


    drawTanque(tanques.tanqueA, globais);
    drawTanque(tanques.tanqueB, globais);
}

void renderGameOver(Tanques players, VariaveisGlobais globals) {
    char score[20];
    char tanqueA[35];
    char tanqueB[35];

    sprintf(score, "TanqueA %d x %d TanqueB", players.tanqueA.pontos, players.tanqueB.pontos);
    sprintf(tanqueA, "Tanque A venceu: %d vezes", globals.history.tanqueA);
    sprintf(tanqueB, "Tanque B venceu: %d vezes", globals.history.tanqueB);

    ALLEGRO_COLOR red = al_map_rgb(255, 0, 0);
    al_draw_text(globals.size_32, red,
                 (2.25f * (float)globals.SCREEN_W/6.0f), (float)globals.SCREEN_H/3.0f, 0, score);

    al_draw_text(globals.size_32, red,
                 (2.1f * (float)globals.SCREEN_W/6.0f), 1.5f * (float)globals.SCREEN_H/3, 0, tanqueA);

    al_draw_text(globals.size_32, red,
                 (2.1f * (float)globals.SCREEN_W/6), 2.0f * (float)globals.SCREEN_H/3, 0, tanqueB);

}