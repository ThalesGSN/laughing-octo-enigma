#include <stdio.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <stdlib.h>
#include <math.h>
#include "types.h"
#include "historyfile.h"

#define true 1
#define false 0

const float FPS = 100;
const int SCREEN_W = 960;
const int SCREEN_H = 540;

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

Obstaculo getObstaculoRetangular(Globals variables){

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
Cenario iniciaCenario(Globals variables) {
    Obstaculo obstaculo = getObstaculoRetangular(variables);

    Cenario cenario;

    cenario.obstaculoRetangular = obstaculo;

    return cenario;
}

Tanques iniciaTanques(Globals variables) {
    float alpha = M_PI_2 - M_PI_4;
    float internTriangleHeight = variables.FORCE_FIELD_RADIUS * sinf(alpha);
    float internTriangleHalfBase = variables.FORCE_FIELD_RADIUS * sinf(M_PI_4);

    Tanques players = {
            .tanqueA = {
                    .mobileCircle = {
                            .centro = {
                                    .x = 0.5f * (variables.SCREEN_HORIZONTAL_FRACTION),
                                    .y = (variables.SCREEN_VERTICAL_FRACTION) * 2
                            },
                            .velocidade = 0.0f,
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
                    .isColliding = 0,
                    .healthPoints = 5,
                    .points = 0
            },
            .tanqueB = {
                    .mobileCircle = {
                            .centro = {
                                    .x = 5.5f * (variables.SCREEN_HORIZONTAL_FRACTION),
                                    .y = (variables.SCREEN_VERTICAL_FRACTION) * 2
                            },
                            .velocidade = 0.0f,
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
                    .isColliding = 0,
                    .healthPoints = 5,
                    .points = 0
            },
    };

    Missil missilA = {
            .color = al_map_rgb(0, 0, 0),
            .circuloTanque = {
                    .centro = players.tanqueA.A,
                    .raio = 7.0f,
                    .angulo = players.tanqueA.mobileCircle.angulo
            },
            .emMovimento = 0,

    };

    Missil missilB = {
            .color = al_map_rgb(0, 0, 0),
            .circuloTanque = {
                    .centro = players.tanqueB.A,
                    .raio = 7.0f,
                    .angulo = players.tanqueB.mobileCircle.angulo
            },
            .emMovimento = false,
    };

    players.tanqueA.mobileCircle.cossenoEixoX = cosf(players.tanqueA.mobileCircle.angulo);
    players.tanqueA.mobileCircle.senoEixoY = sinf(players.tanqueA.mobileCircle.angulo);
    missilA.circuloTanque.cossenoEixoX = players.tanqueA.mobileCircle.cossenoEixoX;
    missilA.circuloTanque.senoEixoY = players.tanqueA.mobileCircle.senoEixoY;
    players.tanqueA.missile = missilA;
    players.tanqueB.mobileCircle.cossenoEixoX = cosf(players.tanqueB.mobileCircle.angulo);
    players.tanqueB.mobileCircle.senoEixoY = sinf(players.tanqueB.mobileCircle.angulo);

    missilB.circuloTanque.cossenoEixoX = players.tanqueB.mobileCircle.cossenoEixoX;
    missilB.circuloTanque.senoEixoY = players.tanqueB.mobileCircle.senoEixoY;
    players.tanqueB.missile = missilB;

    return players;
}

void drawMissile(Tanque tank){
    if(!tank.missile.emMovimento){
        al_draw_filled_circle(tank.missile.circuloTanque.centro.x + tank.mobileCircle.centro.x,
                              tank.missile.circuloTanque.centro.y + tank.mobileCircle.centro.y,
                              tank.missile.circuloTanque.raio, tank.missile.color);
        return;
    }

    al_draw_filled_circle(tank.missile.circuloTanque.centro.x,
                          tank.missile.circuloTanque.centro.y,
                          tank.missile.circuloTanque.raio, tank.missile.color);
}

void drawTanque(Tanque tank, Globals variables){
    al_draw_circle(tank.mobileCircle.centro.x, tank.mobileCircle.centro.y, variables.FORCE_FIELD_RADIUS, tank.color, 1);
    al_draw_filled_triangle(tank.A.x + tank.mobileCircle.centro.x, tank.A.y + tank.mobileCircle.centro.y,
                            tank.B.x + tank.mobileCircle.centro.x, tank.B.y + tank.mobileCircle.centro.y,
                            tank.C.x + tank.mobileCircle.centro.x, tank.C.y + tank.mobileCircle.centro.y,
                            tank.color);

    drawMissile(tank);
}

void drawTanques(Tanques players, Globals variables){
    drawTanque(players.tanqueA, variables);
    drawTanque(players.tanqueB, variables);
}


void drawCenario(Cenario scenery){
    al_clear_to_color(al_map_rgb(255, 255, 255));
    Obstaculo obstaculo = scenery.obstaculoRetangular;
    al_draw_filled_rectangle(obstaculo.superiorEsquerdo.x, obstaculo.superiorEsquerdo.y,
                             obstaculo.inferiorDireito.x, obstaculo.inferiorDireito.y, obstaculo.color);
}

void drawGameOver(Tanques players, Globals globals){
    char score[20];
    char playerOneWins[35];
    char playerTwoWins[35];
    al_clear_to_color(al_map_rgb(0, 0, 0));
    sprintf(score, "Score: %d x %d", players.tanqueA.points, players.tanqueB.points);
    sprintf(playerOneWins, "Player 1 won: %d times", globals.history.firstPlayerWins);
    sprintf(playerTwoWins, "Player 2 won: %d times", globals.history.secondPlayerWins);

    al_draw_text(globals.size_32, al_map_rgb(0, 200, 30),
                 (2.25f * (float)globals.SCREEN_W/6.0f), (float)globals.SCREEN_H/3.0f, 0, score);

    al_draw_text(globals.size_32, al_map_rgb(0, 200, 30),
                 (2.1f * (float)globals.SCREEN_W/6.0f), 1.5f * (float)globals.SCREEN_H/3, 0, playerOneWins);

    al_draw_text(globals.size_32, al_map_rgb(0, 200, 30),
                 (2.1f * (float)globals.SCREEN_W/6), 2.0f * (float)globals.SCREEN_H/3, 0, playerTwoWins);

}
float distanceBetweenTwoPoints(Ponto first, Ponto second){
    return (float)sqrt(pow((second.x - first.x), 2) + pow((second.y - first.y), 2));
}

int hasScreenVerticalCollision(Globals variables, CirculoTanque mobileCircle){
    float yTankProjectionLimitMax = (mobileCircle.centro.y + mobileCircle.raio +
                                     (mobileCircle.velocidade * (mobileCircle.senoEixoY)));
    float yTankProjectionLimitMin = (mobileCircle.centro.y - mobileCircle.raio +
                                     (mobileCircle.velocidade * (mobileCircle.senoEixoY)));

    return (yTankProjectionLimitMax >= (float)variables.SCREEN_H) ||
           (yTankProjectionLimitMin <= 0);
}

int hasScreenHorizontalCollision(Globals variables, CirculoTanque mobileCircle){
    float xTankProjectionLimitMax = (mobileCircle.centro.x + mobileCircle.raio +
                                     (mobileCircle.velocidade * (mobileCircle.cossenoEixoX)));
    float xTankProjectionLimitMin = (mobileCircle.centro.x - mobileCircle.raio +
                                     (mobileCircle.velocidade * (mobileCircle.cossenoEixoX)));


    return (xTankProjectionLimitMax >= (float)variables.SCREEN_W) ||
           (xTankProjectionLimitMin <= 0);
}

int hasScreenLimitCollision(Globals variables, CirculoTanque mobileCircle) {
    int screenHorizontalCollision = hasScreenHorizontalCollision(variables, mobileCircle);
    int screenVerticalCollision = hasScreenVerticalCollision(variables, mobileCircle);

    return  screenHorizontalCollision || screenVerticalCollision;

}

int hasCollisionWithVertex(Ponto vertex, CirculoTanque mobileCircle){
    float distanceCenterVertex = distanceBetweenTwoPoints(vertex, mobileCircle.centro);

    return distanceCenterVertex <= (mobileCircle.raio + 1);
}

int hasCollisionWithRectangularObstacleVertex(Obstaculo rectangle, CirculoTanque mobileCircle){
    return hasCollisionWithVertex(rectangle.superiorEsquerdo, mobileCircle) ||
           hasCollisionWithVertex(rectangle.inferiorDireito, mobileCircle) ||
           hasCollisionWithVertex(rectangle.superiorDireito, mobileCircle) ||
           hasCollisionWithVertex(rectangle.superiorEsquerdo, mobileCircle);
}

int hasCollisionWithRectangularObstacleVerticalLimit(Obstaculo rectangle, CirculoTanque mobileCircle){
    float xTankProjectionLimitMax = (mobileCircle.centro.x + mobileCircle.raio +
                                     (mobileCircle.velocidade * (mobileCircle.cossenoEixoX)));
    float xTankProjectionLimitMin = (mobileCircle.centro.x - mobileCircle.raio +
                                     (mobileCircle.velocidade * (mobileCircle.cossenoEixoX)));

    if (rectangle.superiorEsquerdo.y > mobileCircle.centro.y || rectangle.inferiorDireito.y < mobileCircle.centro.y) {
        return false;
    }
    return rectangle.superiorEsquerdo.x <= xTankProjectionLimitMax && rectangle.inferiorDireito.x >= xTankProjectionLimitMin;
}

int hasCollisionWithRectangularObstacleHorizontalLimit(Obstaculo rectangle, CirculoTanque mobileCircle){
    float yTankProjectionLimitMax = (mobileCircle.centro.y + mobileCircle.raio +
                                     (mobileCircle.velocidade * (mobileCircle.senoEixoY)));
    float yTankProjectionLimitMin = (mobileCircle.centro.y - mobileCircle.raio +
                                     (mobileCircle.velocidade * (mobileCircle.senoEixoY)));

    if (rectangle.superiorEsquerdo.x > mobileCircle.centro.x || rectangle.inferiorDireito.x < mobileCircle.centro.x) {
        return false;
    }

    return rectangle.superiorEsquerdo.y <= yTankProjectionLimitMax && rectangle.inferiorDireito.y >= yTankProjectionLimitMin;
}

int hasCollisionWithRectangularObstacle(Obstaculo rectangle, CirculoTanque mobileCircle){
    return hasCollisionWithRectangularObstacleHorizontalLimit(rectangle, mobileCircle) ||
           hasCollisionWithRectangularObstacleVerticalLimit(rectangle, mobileCircle);
}

int hasObstacleDiagonalCollision(Cenario scenery, CirculoTanque mobileCircle) {
    return hasCollisionWithRectangularObstacleVertex(scenery.obstaculoRetangular, mobileCircle);
}

int hasObstacleCollision(Cenario scenery, CirculoTanque mobileCircle) {
    return hasCollisionWithRectangularObstacle(scenery.obstaculoRetangular, mobileCircle);
}

int hasScreenCollision(Globals variables, CirculoTanque mobileCircle){
    return hasScreenLimitCollision(variables, mobileCircle);
}

int hasTankCollision(CirculoTanque first, CirculoTanque second){
    float distanceBetweenTanks = distanceBetweenTwoPoints(first.centro, second.centro);

    return (distanceBetweenTanks <= ((first.raio) + (second.raio)) + 1);
}

void draw(Tanques players, Globals variables, Cenario scenery) {
    drawCenario(scenery);
    drawTanques(players, variables);
}

void rotatePoint(Ponto *point, float angle) {
    float x = point->x;
    float y = point->y;

    point->x = (x * cosf(angle)) - (y * sinf(angle));
    point->y = (y * cosf(angle)) + (x * sinf(angle));
}

void rotateTank(Tanque *tank) {

    if (tank->mobileCircle.velocidadeAngular != 0) {

        rotatePoint(&tank->A, tank->mobileCircle.velocidadeAngular);
        rotatePoint(&tank->B, tank->mobileCircle.velocidadeAngular);
        rotatePoint(&tank->C, tank->mobileCircle.velocidadeAngular);

        tank->mobileCircle.angulo += tank->mobileCircle.velocidadeAngular;
        tank->mobileCircle.cossenoEixoX = cosf(tank->mobileCircle.angulo);
        tank->mobileCircle.senoEixoY = sinf(tank->mobileCircle.angulo);

        if(!(tank->missile.emMovimento)){
            rotatePoint(&tank->missile.circuloTanque.centro, tank->mobileCircle.velocidadeAngular);
            tank->missile.circuloTanque.cossenoEixoX = cosf(tank->mobileCircle.angulo);
            tank->missile.circuloTanque.senoEixoY = sinf(tank->mobileCircle.angulo);
        }
    }

}

void moveTank(Tanque *tank) {
    tank->mobileCircle.centro.y += tank->mobileCircle.velocidade * (tank->mobileCircle.senoEixoY);
    tank->mobileCircle.centro.x += tank->mobileCircle.velocidade * (tank->mobileCircle.cossenoEixoX);
}

void moveBackwardTank(Tanque *tank) {
    tank->mobileCircle.centro.y -= tank->mobileCircle.velocidade * (tank->mobileCircle.senoEixoY);
    tank->mobileCircle.centro.x -= tank->mobileCircle.velocidade * (tank->mobileCircle.cossenoEixoX);
}

void handleTankMovement(Tanque *tank, Globals variables, Cenario scenery, Tanque otherTank){
    rotateTank(tank);

    if (!(hasScreenCollision(variables, tank->mobileCircle)) &&
        !(hasObstacleCollision(scenery, tank->mobileCircle))) {
        if (tank->isColliding) {
            moveTank(tank);
            if ((!hasTankCollision(tank->mobileCircle, otherTank.mobileCircle)) &&
                (!hasObstacleDiagonalCollision(scenery, tank->mobileCircle))) {
                tank->isColliding = 0;
            } else {
                moveBackwardTank(tank);
            }
        } else if (!hasTankCollision(tank->mobileCircle, otherTank.mobileCircle) &&
                   !hasObstacleDiagonalCollision(scenery, tank->mobileCircle)) {
            moveTank(tank);
        } else {
            tank->isColliding = 1;
        }
    }
}

void moveMissile(Tanque *tank){
    tank->missile.circuloTanque.centro.y -= tank->missile.circuloTanque.velocidade * (tank->missile.circuloTanque.senoEixoY);
    tank->missile.circuloTanque.centro.x -= tank->missile.circuloTanque.velocidade * (tank->missile.circuloTanque.cossenoEixoX);

}

void resetMissile(Tanque* tank){
    tank->missile.circuloTanque.centro = tank->A;
    tank->missile.emMovimento = 0;
    tank->missile.circuloTanque.velocidade = 0;
}

void handleMissileMovement(Tanque *tank, Globals variables, Cenario scenery, Tanque* otherTank) {
    if (!tank->missile.emMovimento) {
        return;
    }

    int screenCollision = (hasScreenCollision(variables, tank->missile.circuloTanque));
    int obstacleCollision = (hasObstacleCollision(scenery, tank->missile.circuloTanque));
    int obstacleDiagonalCollision = (hasObstacleDiagonalCollision(scenery, tank->missile.circuloTanque));

    if( screenCollision || obstacleCollision || obstacleDiagonalCollision){
        resetMissile(tank);
        return;
    }

    if (!hasTankCollision(tank->missile.circuloTanque, otherTank->mobileCircle)) {
        moveMissile(tank);
        return;
    }

    tank->points++;
    otherTank->healthPoints--;
    resetMissile(tank);
}

void refreshTank(Tanque *tank, Globals variables, Cenario scenery, Tanque* otherTank) {
    handleTankMovement(tank, variables, scenery, *otherTank);
    handleMissileMovement(tank, variables, scenery, otherTank);
}

void refreshTanks(Tanques *players, Globals variables, Cenario scenery) {
    refreshTank(&players->tanqueA, variables, scenery, &players->tanqueB);
    refreshTank(&players->tanqueB, variables, scenery, &players->tanqueA);
}

int verifyGameOver(Tanques players, Globals* globals) {
    if (players.tanqueA.healthPoints > 0 && players.tanqueB.healthPoints > 0) {
        return false;
    }

    globals->history.firstPlayerWins += (players.tanqueA.healthPoints > 0);
    globals->history.secondPlayerWins += (players.tanqueB.healthPoints > 0);
    drawGameOver(players, *globals);
    al_flip_display();
    al_rest(3);

    return true;
}

void handleTimer(Tanques *players,  Globals* globals, Cenario scenery) {
    refreshTanks(players, *globals, scenery);
    draw(*players, *globals, scenery);
    verifyGameOver(*players, globals);
    al_flip_display();
}

void handleReleaseUp(Tanque *tank, float defaultSpeed) {
    tank->mobileCircle.velocidade += defaultSpeed;
}

void handleReleaseDown(Tanque *tank, float defaultSpeed) {
    tank->mobileCircle.velocidade -= defaultSpeed;
}

void handleReleaseRotateRight(Tanque *tank, float defaultAngle) {
    tank->mobileCircle.velocidadeAngular -= defaultAngle;
}

void handleReleaseRotateLeft(Tanque *tank, float defaultAngle) {
    tank->mobileCircle.velocidadeAngular += defaultAngle;
}

void handleKeyUp(Tanques *players, ALLEGRO_EVENT event, Globals variables) {
    switch (event.keyboard.keycode) {
        case ALLEGRO_KEY_W:
            handleReleaseUp(&players->tanqueA, variables.tankSpeed);
            break;
        case ALLEGRO_KEY_S:
            handleReleaseDown(&players->tanqueA, variables.tankSpeed);
            break;
        case ALLEGRO_KEY_D:
            handleReleaseRotateRight(&players->tanqueA, variables.angleVariation);
            break;
        case ALLEGRO_KEY_A:
            handleReleaseRotateLeft(&players->tanqueA, variables.angleVariation);
            break;
        case ALLEGRO_KEY_UP:
            handleReleaseUp(&players->tanqueB, variables.tankSpeed);
            break;
        case ALLEGRO_KEY_DOWN:
            handleReleaseDown(&players->tanqueB, variables.tankSpeed);
            break;
        case ALLEGRO_KEY_RIGHT:
            handleReleaseRotateRight(&players->tanqueB, variables.angleVariation);
            break;
        case ALLEGRO_KEY_LEFT:
            handleReleaseRotateLeft(&players->tanqueB, variables.angleVariation);
            break;
    }
}

void handlePressUp(Tanque *tank, float defaultSpeed) {
    tank->mobileCircle.velocidade -= defaultSpeed;
}

void handlePressDown(Tanque *tank, float defaultSpeed) {
    tank->mobileCircle.velocidade += defaultSpeed;
}

void handlePressRotateRight(Tanque *tank, float defaultAngle) {
    tank->mobileCircle.velocidadeAngular += defaultAngle;
}

void handlePressRotateLeft(Tanque *tank, float defaultAngle) {
    tank->mobileCircle.velocidadeAngular -= defaultAngle;
}

void handlePressShoot(Tanque *tank) {
    if(!tank->missile.emMovimento) {
        tank->missile.circuloTanque.velocidade += 4;
        tank->missile.circuloTanque.cossenoEixoX = tank->mobileCircle.cossenoEixoX;
        tank->missile.circuloTanque.senoEixoY = tank->mobileCircle.senoEixoY;
        tank->missile.circuloTanque.centro.x = tank->mobileCircle.centro.x +
                                               (tank->mobileCircle.raio * (-tank->mobileCircle.cossenoEixoX));
        tank->missile.circuloTanque.centro.y = tank->mobileCircle.centro.y +
                                               (tank->mobileCircle.raio * (-tank->mobileCircle.senoEixoY));
        tank->missile.emMovimento = 1;
    }
}

void handleKeyDown(Tanques *players, ALLEGRO_EVENT event, Globals variables) {
    switch (event.keyboard.keycode) {
        case ALLEGRO_KEY_W:
            handlePressUp(&players->tanqueA, variables.tankSpeed);
            break;
        case ALLEGRO_KEY_S:
            handlePressDown(&players->tanqueA, variables.tankSpeed);
            break;
        case ALLEGRO_KEY_D:
            handlePressRotateRight(&players->tanqueA, variables.angleVariation);
            break;
        case ALLEGRO_KEY_A:
            handlePressRotateLeft(&players->tanqueA, variables.angleVariation);
            break;
        case ALLEGRO_KEY_Q:
            handlePressShoot(&players->tanqueA);
            break;
        case ALLEGRO_KEY_UP:
            handlePressUp(&players->tanqueB, variables.tankSpeed);
            break;
        case ALLEGRO_KEY_DOWN:
            handlePressDown(&players->tanqueB, variables.tankSpeed);
            break;
        case ALLEGRO_KEY_RIGHT:
            handlePressRotateRight(&players->tanqueB, variables.angleVariation);
            break;
        case ALLEGRO_KEY_LEFT:
            handlePressRotateLeft(&players->tanqueB, variables.angleVariation);
            break;
        case ALLEGRO_KEY_ENTER:
            handlePressShoot(&players->tanqueB);
            break;
    }
}

int handleEvent(Tanques *players, ALLEGRO_EVENT event,
                 Globals *globals, Cenario scenery) {
    switch (event.type) {
        case ALLEGRO_EVENT_TIMER:
            handleTimer(players, globals, scenery);
            break;
        case ALLEGRO_EVENT_KEY_UP:
            handleKeyUp(players, event, *globals);
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            handleKeyDown(players, event, *globals);
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            //gameover
            return true;
            break;
    }
}



int main(int argc, char **argv) {
    srand(time(NULL));

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;

    //----------------------- rotinas de inicializacao ---------------------------------------

    //inicializa o Allegro
    if(!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
        fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }

    //inicializa o modulo que permite carregar imagens no jogo
    if(!al_init_image_addon()){
        fprintf(stderr, "failed to initialize image module!\n");
        return -1;
    }

    //cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }

    //cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(timer);
        return -1;
    }

    //instala o teclado
    if(!al_install_keyboard()) {
        fprintf(stderr, "failed to install keyboard!\n");
        return -1;
    }

    //inicializa o modulo allegro que carrega as fontes
    al_init_font_addon();

    //inicializa o modulo allegro que entende arquivos tff de fontes
    if(!al_init_ttf_addon()) {
        fprintf(stderr, "failed to load tff font module!\n");
        return -1;
    }

    //carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    ALLEGRO_FONT *size_32 = al_load_font("arial.ttf", 32, 1);
    if(size_32 == NULL) {
        fprintf(stderr, "font file does not exist or cannot be accessed!\n");
    }

    //cria a fila de eventos
    event_queue = al_create_event_queue();
    if(!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        return -1;
    }

    //registra na fila os eventos de tela (ex: clicar no X na janela)
    al_register_event_source(event_queue, al_get_display_event_source(display));
    //registra na fila os eventos de tempo: quando o tempo altera de t para t+1
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    //registra na fila os eventos de teclado (ex: pressionar uma tecla)
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    int audioInstalled = al_install_audio();
    if (!audioInstalled) {
        return -1;
    }

    //inicia o temporizador
    al_start_timer(timer);
    ALLEGRO_SAMPLE_ID backgoundSound;

    ALLEGRO_SAMPLE *sample;
    sample = al_load_sample("backgroundSound.wav");
    al_play_sample(sample, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, &backgoundSound);

    GameHistory history = getGameHistory();

    Globals globals = {
            .FPS = FPS,
            .SCREEN_H = SCREEN_H,
            .SCREEN_W = SCREEN_W,
            .SCREEN_HORIZONTAL_FRACTION = ((float)SCREEN_W / 6.0f),
            .SCREEN_VERTICAL_FRACTION = ((float)SCREEN_H / 4.0f),
            .FORCE_FIELD_RADIUS = 30.0f,
            .tankSpeed = 2.5f,
            .angleVariation = M_PI_4 / 12,
            .history = history,
            .event_queue = event_queue,
            .display = display,
            .size_32 = size_32,
            .timer = timer
    };

    Tanques tanques = iniciaTanques(globals);

    Cenario cenario = iniciaCenario(globals);
    int gameover = false;
    while(!gameover) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(globals.event_queue, &ev);
        gameover = handleEvent(&tanques, ev, &globals, cenario);
    }

    updateGameHistory(globals.history);

    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

   return 0;
}
