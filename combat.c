#include <stdio.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <math.h>
#include "types.h"
#include "scoreBoard.h"
#include "initGame.h"
#include "render.h"
#include "eventos.h"

#define true 1
#define false 0
#define OK 0
#define INIT_ERROR -1

const float FPS = 100;
const int SCREEN_W = 960;
const int SCREEN_H = 540;


int main(int argc, char **argv) {
    srand(time(NULL));

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;

    //----------------------- rotinas de inicializacao ---------------------------------------

    //inicializa o Allegro
    if(!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return INIT_ERROR;
    }

    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
        fprintf(stderr, "failed to initialize primitives!\n");
        return INIT_ERROR;
    }

    //inicializa o modulo que permite carregar imagens no jogo
    if(!al_init_image_addon()){
        fprintf(stderr, "failed to initialize image module!\n");
        return INIT_ERROR;
    }

    //cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        fprintf(stderr, "failed to create timer!\n");
        return INIT_ERROR;
    }

    //cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(timer);
        return INIT_ERROR;
    }

    //instala o teclado
    if(!al_install_keyboard()) {
        fprintf(stderr, "failed to install keyboard!\n");
        return INIT_ERROR;
    }

    //inicializa o modulo allegro que carrega as fontes
    al_init_font_addon();

    //inicializa o modulo allegro que entende arquivos tff de fontes
    if(!al_init_ttf_addon()) {
        fprintf(stderr, "failed to load tff font module!\n");
        return INIT_ERROR;
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
        return INIT_ERROR;
    }

    //registra na fila os eventos de tela (ex: clicar no X na janela)
    al_register_event_source(event_queue, al_get_display_event_source(display));
    //registra na fila os eventos de tempo: quando o tempo altera de t para t+1
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    //registra na fila os eventos de teclado (ex: pressionar uma tecla)
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    //inicia o temporizador
    al_start_timer(timer);

    ScoreBoard history = obtemScoreBoard();

    VariaveisGlobais globais = {
            .FPS = FPS,
            .SCREEN_H = SCREEN_H,
            .SCREEN_W = SCREEN_W,
            .SCREEN_HORIZONTAL_FRACTION = ((float)SCREEN_W / 6.0f),
            .SCREEN_VERTICAL_FRACTION = ((float)SCREEN_H / 4.0f),
            .RaioCirculo = 30.0f,
            .velocidadeDoTanque = 2.5f,
            .velocidadeDoMissil = 4,
            .velocidadeAngular = M_PI_4 / 12,
            .scoreBoard = history,
            .event_queue = event_queue,
            .display = display,
            .size_32 = size_32,
            .timer = timer
    };

    Tanques tanques = iniciaTanques(globais);

    Cenario cenario = iniciaCenario(globais);

    int gameover = false;
    while(!gameover) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(globais.event_queue, &ev);
        gameover = eventos(&tanques, ev, &globais, cenario);
    }

    renderGameOver(tanques, globais);
    al_flip_display();
    al_rest(5.5);
    atualizaScoreBoard(globais.scoreBoard);

    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

   return OK;
}
