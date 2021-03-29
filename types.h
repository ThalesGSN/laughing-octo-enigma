#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>

typedef struct GameHistory {
    int tanqueA;
    int tanqueB;
} ScoreBoard;

typedef struct VariaveisGlobais {
    float FPS;
    int SCREEN_W;
    int SCREEN_H;
    float SCREEN_HORIZONTAL_FRACTION;
    float SCREEN_VERTICAL_FRACTION;
    float FORCE_FIELD_RADIUS;
    float velocidadeDoTanque;
    float velocidadeDoMissil;
    float velocidadeAngular;
    ScoreBoard history;
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_TIMER* timer;
    ALLEGRO_FONT *size_32;
} VariaveisGlobais;

typedef struct Ponto {
    float x, y;
} Ponto;

typedef struct Circulo {
    Ponto centro;
    float raio;
    float velocidadeLinear;
    float angulo;
    float velocidadeAngular;
    float cossenoEixoX;
    float senoEixoY;
} Circulo;

typedef struct Missil {
    Circulo missil;
    ALLEGRO_COLOR color;
    int emMovimento;
} Missil;

typedef struct Tanque {
    Circulo circuloExterno;
    Ponto A, B, C;
    ALLEGRO_COLOR color;
    float emColisao;
    Missil missil;
    int HP;
    int pontos;
} Tanque;

typedef struct Tanques {
    Tanque tanqueA;
    Tanque tanqueB;
} Tanques;

typedef struct Obstaculo {
    Ponto superiorEsquerdo, superiorDireito, inferiorEsquerdo, inferiorDireito;
    ALLEGRO_COLOR color;
} Obstaculo;

typedef struct Cenario {
    Obstaculo obstaculoRetangular;
} Cenario;
