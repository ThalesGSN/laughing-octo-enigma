#include <math.h>
#include "types.h"

Obstaculo getObstaculo(Ponto superiorEsquerdo, Ponto inferiorDireito){
    Obstaculo obstaculo = {
            .superiorEsquerdo = superiorEsquerdo,
            .superiorDireito = {
                    .x = inferiorDireito.x,
                    .y = superiorEsquerdo.y,
            },
            .inferiorEsquerdo = {
                    .x = superiorEsquerdo.x,
                    .y = inferiorDireito.y,
            },
            .inferiorDireito = inferiorDireito,
            .color = al_map_rgb(255, 165, 0)
    };

    return obstaculo;
}

Obstaculo getObstaculoRetangular(VariaveisGlobais variables){

    Ponto superiorEsquerdo = {
            .x = 2 * (variables.SCREEN_HORIZONTAL_FRACTION),
            .y = 0.9f * (variables.SCREEN_VERTICAL_FRACTION)
    };

    Ponto inferiorDireito = {
            .x = 3 * (variables.SCREEN_HORIZONTAL_FRACTION),
            .y = 3 * (variables.SCREEN_VERTICAL_FRACTION)
    };

    return getObstaculo(superiorEsquerdo, inferiorDireito);

}
Cenario iniciaCenario(VariaveisGlobais variables) {
    Obstaculo obstaculo = getObstaculoRetangular(variables);

    Cenario cenario;

    cenario.obstaculoRetangular = obstaculo;

    return cenario;
}

Tanques iniciaTanques(VariaveisGlobais variables) {
    float alpha = M_PI_2 - M_PI_4;
    float internTriangleHeight = variables.FORCE_FIELD_RADIUS * sinf(alpha);
    float internTriangleHalfBase = variables.FORCE_FIELD_RADIUS * sinf(M_PI_4);

    Tanques tanques = {
            .tanqueA = {
                    .circuloExterno = {
                            .centro = {
                                    .x = 0.5f * (variables.SCREEN_HORIZONTAL_FRACTION),
                                    .y = (variables.SCREEN_VERTICAL_FRACTION) * 2
                            },
                            .velocidadeLinear = 0.0f,
                            .angulo = M_PI_2,
                            .velocidadeAngular = 0.0f,
                            .raio = variables.FORCE_FIELD_RADIUS,
                    },
                    .A = {
                            .x = 0,
                            .y = -variables.FORCE_FIELD_RADIUS
                    },
                    .B = {
                            .x = -internTriangleHalfBase,
                            .y = internTriangleHeight
                    },
                    .C = {
                            .x = internTriangleHalfBase,
                            .y = internTriangleHeight
                    },
                    .color = al_map_rgb(255, 0, 0),
                    .emColisao = 0,
                    .HP = 5,
                    .pontos = 0
            },
            .tanqueB = {
                    .circuloExterno = {
                            .centro = {
                                    .x = 5.5f * (variables.SCREEN_HORIZONTAL_FRACTION),
                                    .y = (variables.SCREEN_VERTICAL_FRACTION) * 2
                            },
                            .velocidadeLinear = 0.0f,
                            .angulo = M_PI_2,
                            .velocidadeAngular = 0.0f,
                            .raio = variables.FORCE_FIELD_RADIUS,
                    },
                    .A = {
                            .x = 0,
                            .y = -variables.FORCE_FIELD_RADIUS
                    },
                    .B = {
                            .x = -internTriangleHalfBase,
                            .y = internTriangleHeight
                    },
                    .C = {
                            .x = internTriangleHalfBase,
                            .y = internTriangleHeight
                    },
                    .color = al_map_rgb(0, 0, 255),
                    .emColisao = 0,
                    .HP = 5,
                    .pontos = 0
            },
    };

    Missil missilA = {
            .color = al_map_rgb(0, 0, 0),
            .missil = {
                    .centro = tanques.tanqueA.A,
                    .raio = 7.0f,
                    .angulo = tanques.tanqueA.circuloExterno.angulo
            },
            .emMovimento = 0,

    };

    Missil missilB = {
            .color = al_map_rgb(0, 0, 0),
            .missil = {
                    .centro = tanques.tanqueB.A,
                    .raio = 7.0f,
                    .angulo = tanques.tanqueB.circuloExterno.angulo
            },
            .emMovimento = false,
    };

    tanques.tanqueA.circuloExterno.cossenoEixoX = cosf(tanques.tanqueA.circuloExterno.angulo);
    tanques.tanqueA.circuloExterno.senoEixoY = sinf(tanques.tanqueA.circuloExterno.angulo);
    missilA.missil.cossenoEixoX = tanques.tanqueA.circuloExterno.cossenoEixoX;
    missilA.missil.senoEixoY = tanques.tanqueA.circuloExterno.senoEixoY;
    tanques.tanqueA.missil = missilA;
    tanques.tanqueB.circuloExterno.cossenoEixoX = cosf(tanques.tanqueB.circuloExterno.angulo);
    tanques.tanqueB.circuloExterno.senoEixoY = sinf(tanques.tanqueB.circuloExterno.angulo);

    missilB.missil.cossenoEixoX = tanques.tanqueB.circuloExterno.cossenoEixoX;
    missilB.missil.senoEixoY = tanques.tanqueB.circuloExterno.senoEixoY;
    tanques.tanqueB.missil = missilB;

    return tanques;
}