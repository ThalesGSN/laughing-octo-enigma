typedef struct GameHistory {
    int firstPlayerWins;
    int secondPlayerWins;
} GameHistory;

typedef struct Globals {
    float FPS;
    int SCREEN_W;
    int SCREEN_H;
    float SCREEN_HORIZONTAL_FRACTION;
    float SCREEN_VERTICAL_FRACTION;
    float FORCE_FIELD_RADIUS;
    float tankSpeed;
    float angleVariation;
    GameHistory history;
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_TIMER* timer;
    ALLEGRO_FONT *size_32;
} Globals;

typedef struct Ponto {
    float x, y;
} Ponto;

typedef struct CirculoTanque {
    Ponto centro;
    float raio;
    float velocidade;
    float angulo;
    float velocidadeAngular;
    float cossenoEixoX;
    float senoEixoY;
} CirculoTanque;

typedef struct Missil {
    CirculoTanque circuloTanque;
    ALLEGRO_COLOR color;
    int emMovimento;
} Missil;

typedef struct Tanque {
    CirculoTanque mobileCircle;
    Ponto A, B, C;
    ALLEGRO_COLOR color;
    float isColliding;
    Missil missile;
    int healthPoints;
    int points;
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
