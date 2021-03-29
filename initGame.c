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

Tanque getTanqueA(VariaveisGlobais globais) {
    float alturaInternaTriangulo = globais.RaioCirculo * sinf(M_PI_2 - M_PI_4);
    float semiBaseInternaDoTriangulo = globais.RaioCirculo * sinf(M_PI_4);

    Ponto pontaTanqueA = {
            .x = 0,
            .y = -globais.RaioCirculo
    };

    Circulo circuloTanqueA = {
            .centro = {
                    .x = 0.5f * (globais.SCREEN_HORIZONTAL_FRACTION),
                    .y = (globais.SCREEN_VERTICAL_FRACTION) * 2
            },
            .velocidadeLinear = 0.0f,
            .angulo = M_PI_2,
            .velocidadeAngular = 0.0f,
            .raio = globais.RaioCirculo,
            .senoEixoY = sinf(M_PI_2),
            .cossenoEixoX = cosf(M_PI_2)
    };

    Missil missilA = {
            .color = al_map_rgb(0, 0, 0),
            .missil = {
                    .centro = pontaTanqueA,
                    .raio = 7.0f,
                    .angulo = circuloTanqueA.angulo,
                    .senoEixoY = circuloTanqueA.senoEixoY,
                    .cossenoEixoX = circuloTanqueA.cossenoEixoX
            },
            .emMovimento = false,

    };

    Tanque tanqueA = {
            .circuloExterno = circuloTanqueA,
            .A = pontaTanqueA,
            .B = {
                    .x = -semiBaseInternaDoTriangulo,
                    .y = alturaInternaTriangulo
            },
            .C = {
                    .x = semiBaseInternaDoTriangulo,
                    .y = alturaInternaTriangulo
            },
            .color = al_map_rgb(255, 0, 0),
            .emColisao = 0,
            .HP = 5,
            .pontos = 0,
            .missil = missilA
    };

    return tanqueA;
}

Tanque getTanqueB(VariaveisGlobais globais) {
    float alturaInternaTriangulo = globais.RaioCirculo * sinf(M_PI_2 - M_PI_4);
    float semiBaseInternaDoTriangulo = globais.RaioCirculo * sinf(M_PI_4);

    Ponto pontaTanqueB ={
            .x = 0,
            .y = -globais.RaioCirculo
    };

    Circulo circuloTanqueB = {
            .centro = {
                    .x = 5.5f * (globais.SCREEN_HORIZONTAL_FRACTION),
                    .y = (globais.SCREEN_VERTICAL_FRACTION) * 2
            },
            .velocidadeLinear = 0,
            .angulo = M_PI_2,
            .velocidadeAngular = 0,
            .raio = globais.RaioCirculo,
            .senoEixoY = sinf(M_PI_2),
            .cossenoEixoX = cosf(M_PI_2)
    };

    Missil missilB = {
            .color = al_map_rgb(0, 0, 0),
            .missil = {
                    .centro = pontaTanqueB,
                    .raio = 7.0f,
                    .angulo = circuloTanqueB.angulo,
                    .senoEixoY = circuloTanqueB.senoEixoY,
                    .cossenoEixoX = circuloTanqueB.cossenoEixoX
            },
            .emMovimento = false,
    };

    Tanque tanqueB = {
            .circuloExterno = circuloTanqueB,
            .A = pontaTanqueB,
            .B = {
                    .x = -semiBaseInternaDoTriangulo,
                    .y = alturaInternaTriangulo
            },
            .C = {
                    .x = semiBaseInternaDoTriangulo,
                    .y = alturaInternaTriangulo
            },
            .color = al_map_rgb(0, 0, 255),
            .emColisao = false,
            .HP = 5,
            .pontos = 0,
            .missil = missilB
    };

    return tanqueB;
}

Tanques iniciaTanques(VariaveisGlobais globais) {

    Tanques tanques = {
            .tanqueA = getTanqueA(globais),
            .tanqueB = getTanqueB(globais)
    };

    return tanques;
}