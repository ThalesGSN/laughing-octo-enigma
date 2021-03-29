//
// Created by thale on 29/03/2021.
//
#include <math.h>
#include "types.h"
#include "draw.h"

#define true 1
#define false 0


float distanciaEntrePontos(Ponto first, Ponto second){
    return (float)sqrt(pow((second.x - first.x), 2) + pow((second.y - first.y), 2));
}

int temColisaoComAtela(VariaveisGlobais variables, Circulo mobileCircle) {
    float xMaximoDoTanque = (mobileCircle.centro.x + mobileCircle.raio +
                             (mobileCircle.velocidadeLinear * (mobileCircle.cossenoEixoX)));
    float xMinimoDotanque = (mobileCircle.centro.x - mobileCircle.raio +
                             (mobileCircle.velocidadeLinear * (mobileCircle.cossenoEixoX)));

    int colisaoHorizontal = (xMaximoDoTanque >= (float)variables.SCREEN_W) ||
                            (xMinimoDotanque <= 0);

    float yMaximoDoTanque = (mobileCircle.centro.y + mobileCircle.raio +
                             (mobileCircle.velocidadeLinear * (mobileCircle.senoEixoY)));
    float yMinimoDoTanque = (mobileCircle.centro.y - mobileCircle.raio +
                             (mobileCircle.velocidadeLinear * (mobileCircle.senoEixoY)));


    int colisaoVertical = (yMaximoDoTanque >= (float)variables.SCREEN_H) ||
                          (yMinimoDoTanque <= 0);

    return colisaoHorizontal || colisaoVertical;
}



int colisaoObstaculoVertical(Obstaculo obstaculo, Circulo circuloExterno){
    float xTankProjectionLimitMax = (circuloExterno.centro.x + circuloExterno.raio +
                                     (circuloExterno.velocidadeLinear * (circuloExterno.cossenoEixoX)));
    float xTankProjectionLimitMin = (circuloExterno.centro.x - circuloExterno.raio +
                                     (circuloExterno.velocidadeLinear * (circuloExterno.cossenoEixoX)));

    if (obstaculo.superiorEsquerdo.y > circuloExterno.centro.y || obstaculo.inferiorDireito.y < circuloExterno.centro.y) {
        return false;
    }
    return obstaculo.superiorEsquerdo.x <= xTankProjectionLimitMax && obstaculo.inferiorDireito.x >= xTankProjectionLimitMin;
}

int colisaoObstaculoHorizontal(Obstaculo rectangle, Circulo mobileCircle){
    float yTankProjectionLimitMax = (mobileCircle.centro.y + mobileCircle.raio +
                                     (mobileCircle.velocidadeLinear * (mobileCircle.senoEixoY)));
    float yTankProjectionLimitMin = (mobileCircle.centro.y - mobileCircle.raio +
                                     (mobileCircle.velocidadeLinear * (mobileCircle.senoEixoY)));

    if (rectangle.superiorEsquerdo.x > mobileCircle.centro.x || rectangle.inferiorDireito.x < mobileCircle.centro.x) {
        return false;
    }

    return rectangle.superiorEsquerdo.y <= yTankProjectionLimitMax && rectangle.inferiorDireito.y >= yTankProjectionLimitMin;
}

int temColisaoComObstaculo(Cenario scenery, Circulo mobileCircle) {
    return colisaoObstaculoHorizontal(scenery.obstaculoRetangular, mobileCircle) ||
    colisaoObstaculoVertical(scenery.obstaculoRetangular, mobileCircle);
}

int temColisaoComATela(VariaveisGlobais variables, Circulo mobileCircle){
    return temColisaoComAtela(variables, mobileCircle);
}

int colisaocomTanque(Circulo first, Circulo second){
    float distanceBetweenTanks = distanciaEntrePontos(first.centro, second.centro);

    return (distanceBetweenTanks <= ((first.raio) + (second.raio)) + 1);
}

void angulaPonto(Ponto *point, float angle) {
    float x = point->x;
    float y = point->y;

    point->x = (x * cosf(angle)) - (y * sinf(angle));
    point->y = (y * cosf(angle)) + (x * sinf(angle));
}

void rotacionaTanque(Tanque *tanque) {

    int tanqueRotacionando = tanque->circuloExterno.velocidadeAngular == false;
    if (tanqueRotacionando) {
        return;
    }

    angulaPonto(&tanque->A, tanque->circuloExterno.velocidadeAngular);
    angulaPonto(&tanque->B, tanque->circuloExterno.velocidadeAngular);
    angulaPonto(&tanque->C, tanque->circuloExterno.velocidadeAngular);

    tanque->circuloExterno.angulo += tanque->circuloExterno.velocidadeAngular;
    tanque->circuloExterno.cossenoEixoX = cosf(tanque->circuloExterno.angulo);
    tanque->circuloExterno.senoEixoY = sinf(tanque->circuloExterno.angulo);

    int missilEmMovimento = tanque->missil.emMovimento;
    if (missilEmMovimento){
        return;
    }

    angulaPonto(&tanque->missil.missil.centro, tanque->circuloExterno.velocidadeAngular);
    tanque->missil.missil.cossenoEixoX = cosf(tanque->circuloExterno.angulo);
    tanque->missil.missil.senoEixoY = sinf(tanque->circuloExterno.angulo);
}

void moveTanque(Tanque *tank) {
    tank->circuloExterno.centro.y += tank->circuloExterno.velocidadeLinear * (tank->circuloExterno.senoEixoY);
    tank->circuloExterno.centro.x += tank->circuloExterno.velocidadeLinear * (tank->circuloExterno.cossenoEixoX);
}

void moveTanqueRe(Tanque *tank) {
    tank->circuloExterno.centro.y -= tank->circuloExterno.velocidadeLinear * (tank->circuloExterno.senoEixoY);
    tank->circuloExterno.centro.x -= tank->circuloExterno.velocidadeLinear * (tank->circuloExterno.cossenoEixoX);
}

void movimentaTanque(Tanque *tanque, VariaveisGlobais variables, Cenario scenery, Tanque otherTank){
    rotacionaTanque(tanque);

    int tanqueEmColisao = temColisaoComATela(variables, tanque->circuloExterno) ||
                          temColisaoComObstaculo(scenery, tanque->circuloExterno);
    if (tanqueEmColisao) {
        return;
    }

    if (tanque->emColisao) {
        moveTanque(tanque);
        if ((!colisaocomTanque(tanque->circuloExterno, otherTank.circuloExterno))) {
            tanque->emColisao = 0;
        } else {
            moveTanqueRe(tanque);
        }
        return;
    }
    if (colisaocomTanque(tanque->circuloExterno, otherTank.circuloExterno)) {
        tanque->emColisao = true;
        return;
    }

    moveTanque(tanque);
}

void movimentaMissel(Tanque *tanque, VariaveisGlobais variables, Cenario scenery, Tanque* otherTank) {
    if (!tanque->missil.emMovimento) {
        return;
    }

    int colisaoComATela = (temColisaoComATela(variables, tanque->missil.missil));
    int colisaoComObstaculo = (temColisaoComObstaculo(scenery, tanque->missil.missil));

    if(colisaoComATela || colisaoComObstaculo){
        tanque->missil.missil.centro = tanque->A;
        tanque->missil.emMovimento = false;
        tanque->missil.missil.velocidadeLinear = 0;
        return;
    }

    if (!colisaocomTanque(tanque->missil.missil, otherTank->circuloExterno)) {
        tanque->missil.missil.centro.y -= tanque->missil.missil.velocidadeLinear * (tanque->missil.missil.senoEixoY);
        tanque->missil.missil.centro.x -= tanque->missil.missil.velocidadeLinear * (tanque->missil.missil.cossenoEixoX);
        return;
    }

    otherTank->HP--;
    tanque->pontos++;
    tanque->missil.missil.centro = tanque->A;
    tanque->missil.emMovimento = false;
    tanque->missil.missil.velocidadeLinear = 0;
}

void refreshTanques(Tanques *tanques, VariaveisGlobais variables, Cenario scenery) {
    movimentaTanque(&tanques->tanqueA, variables, scenery, tanques->tanqueB);
    movimentaMissel(&tanques->tanqueA, variables, scenery, &tanques->tanqueB);

    movimentaTanque(&tanques->tanqueB, variables, scenery, tanques->tanqueA);
    movimentaMissel(&tanques->tanqueB, variables, scenery, &tanques->tanqueA);
}

int verificaGameover(Tanques players, VariaveisGlobais* globals) {
    if (players.tanqueA.HP > 0 && players.tanqueB.HP > 0) {
        return false;
    }

    globals->history.tanqueA += (players.tanqueA.HP > 0);
    globals->history.tanqueB += (players.tanqueB.HP > 0);

    al_flip_display();
    al_rest(3);

    return true;
}

void atiraMissil(Tanque *tank, VariaveisGlobais variaveisGlobais) {
    if (tank->missil.emMovimento) {
        return;
    }
    tank->missil.missil.velocidadeLinear += variaveisGlobais.velocidadeDoMissil;
    tank->missil.missil.cossenoEixoX = tank->circuloExterno.cossenoEixoX;
    tank->missil.missil.senoEixoY = tank->circuloExterno.senoEixoY;
    tank->missil.missil.centro.x = tank->circuloExterno.centro.x +
                                   (tank->circuloExterno.raio * (-tank->circuloExterno.cossenoEixoX));
    tank->missil.missil.centro.y = tank->circuloExterno.centro.y +
                                   (tank->circuloExterno.raio * (-tank->circuloExterno.senoEixoY));
    tank->missil.emMovimento = true;
}

void teclado(Tanques *tanques, ALLEGRO_EVENT event, VariaveisGlobais globais) {
    if (event.type == ALLEGRO_EVENT_KEY_UP) {
        switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_W:
                tanques->tanqueA.circuloExterno.velocidadeLinear += globais.velocidadeDoTanque;
                break;
            case ALLEGRO_KEY_S:
                tanques->tanqueA.circuloExterno.velocidadeLinear -= globais.velocidadeDoTanque;
                break;
            case ALLEGRO_KEY_D:
                tanques->tanqueA.circuloExterno.velocidadeAngular -= globais.velocidadeAngular;
                break;
            case ALLEGRO_KEY_A:
                tanques->tanqueA.circuloExterno.velocidadeAngular += globais.velocidadeAngular;
                break;
            case ALLEGRO_KEY_UP:
                tanques->tanqueB.circuloExterno.velocidadeLinear += globais.velocidadeDoTanque;
                break;
            case ALLEGRO_KEY_DOWN:
                tanques->tanqueB.circuloExterno.velocidadeLinear -= globais.velocidadeDoTanque;
                break;
            case ALLEGRO_KEY_RIGHT:
                tanques->tanqueB.circuloExterno.velocidadeAngular -= globais.velocidadeAngular;
                break;
            case ALLEGRO_KEY_LEFT:
                tanques->tanqueB.circuloExterno.velocidadeAngular += globais.velocidadeAngular;
                break;
        }
        return;
    }

    switch (event.keyboard.keycode) {
        case ALLEGRO_KEY_W:
            tanques->tanqueA.circuloExterno.velocidadeLinear -= globais.velocidadeDoTanque;
            break;
        case ALLEGRO_KEY_S:
            tanques->tanqueA.circuloExterno.velocidadeLinear += globais.velocidadeDoTanque;
            break;
        case ALLEGRO_KEY_D:
            tanques->tanqueA.circuloExterno.velocidadeAngular += globais.velocidadeAngular;
            break;
        case ALLEGRO_KEY_A:
            tanques->tanqueA.circuloExterno.velocidadeAngular -= globais.velocidadeAngular;
            break;
        case ALLEGRO_KEY_UP:
            tanques->tanqueB.circuloExterno.velocidadeLinear -= globais.velocidadeDoTanque;
            break;
        case ALLEGRO_KEY_DOWN:
            tanques->tanqueB.circuloExterno.velocidadeLinear += globais.velocidadeDoTanque;
            break;
        case ALLEGRO_KEY_RIGHT:
            tanques->tanqueB.circuloExterno.velocidadeAngular += globais.velocidadeAngular;
            break;
        case ALLEGRO_KEY_LEFT:
            tanques->tanqueB.circuloExterno.velocidadeAngular -= globais.velocidadeAngular;
            break;
        case ALLEGRO_KEY_Q:
            atiraMissil(&tanques->tanqueA, globais);
            break;
        case ALLEGRO_KEY_ENTER:
            atiraMissil(&tanques->tanqueB, globais);
            break;
    }
}

int eventos(Tanques *tanques, ALLEGRO_EVENT event,
            VariaveisGlobais *globais, Cenario scenery) {

    switch (event.type) {
        case ALLEGRO_EVENT_TIMER:
            refreshTanques(tanques, *globais, scenery);
            render(*tanques, scenery, *globais);
            al_flip_display();
            return verificaGameover(*tanques, globais);

        case ALLEGRO_EVENT_KEY_UP:
        case ALLEGRO_EVENT_KEY_DOWN:
            teclado(tanques, event, *globais);
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            //gameover
            return true;
    }


    return false;
}