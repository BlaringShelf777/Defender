#ifndef Defender_h__
#define Defender_h__

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<conio2.h>
#include<math.h>
#include<string.h>
#include<windows.h>

// ========================= CONSTANTES =========================
// Velocidade
#define MAXSPEED 1.25
#define SHOOTSPEED .35
#define ENEMYSPEED .5
// Enemy
#define ENEMYMAX 20
#define ENEMYMOVMX 3
#define ENEMYMOVMN 1
#define MAXSHOOT 5
// MAPA:
#define MXPTR 105 // Limite de impressão
#define MXJU 600 // Valor maximo do J da matrizUniverso
#define MXIU 35 //  Valor maximo do I da matrizUniverso
#define MXJTXT 415+1+1 // '\n' e '\0'


// ========================= STRUCTS =========================
// Player-Shoot:
typedef struct{
    int active;
    int xAsis, yAsis;
    /*
        Onde:
        - int active -> Indica se o tiro esta ativo ou não
        - int xAsis -> Indica a posição do tiro no eixo X
        - int yAsis -> Indica a posição do tiro no eixo Y
    */
}playerShoot_t;
// Player:
typedef struct{
    char ship[2][5];
    int staticXAsis, xAsis, yAsis;
    int lifes, points, shooted;
    double maxSpeed;
    playerShoot_t shoot[MAXSHOOT];
    /*
        Onde:
          - int staticXAsis -> Posição x inicial do jogador
          - int xAsis -> Posição X do jogador
          - int yAsis -> Posiçãp Y do jogador
          - int lifes -> Numero de vidas do jogador
          - int points -> Score do jogador
          - int maxSpeed -> Armazena a velocidade do jogador
          - int shooted -> Indica se o jogador foi atingido
          - char ship[2][5] -> Matriz que armazena a representação "gráfica" do jogador
    */
}player_t;
// Enemy-Shoot:
typedef struct{
    int active;
    int xAsis, yAsis;
    int mov;
    /*
        Onde:
        - int active -> Indica se o tiro esta ativo ou não
        - int xAsis -> Indica a posição do tiro no eixo X
        - int yAsis -> Indica a posição do tiro no eixo Y
        - int mov -> Indica qual a movimentação adotada pelo tiro
    */
}enemyShoot_t;
// Enemy:
typedef struct{
    int active;
    int xAsis, yAsis;
    int print;
    int move, moveFlag, needToMove;
    enemyShoot_t shoot[MAXSHOOT];
    int needToShoot;
    /*
        Onde:
          - int active -> Indica se o inimigo está ativo
          - int xAsis -> Posção no eixo X
          - int yAsis -> Posição no eixo Y
          - int move -> Indica qual sera a movimentação do inimigo
          - int print -> Indica se deve ser impresso;
          - int moveFlag -> Indica a quanto tempo o inimigo se encontra em uma movimentação x
          - int needToMove -> Indica que o inimigo precisa adotar uma movimentação
          - enemyShoot_t shoot -> Tiros do Inimigo
          - int needToShoot -> Indica se um inimigo tem que atirar
    */
}enemy_t;
// Map
typedef struct{
    int initalX, finalX;
    int level;
    int u[MXIU][MXJU];
    /*
        Onde:
          - int initialX -> indica a posição do primeiro elemento do mapa + 1
          - int finalX -> Indica a posição do ultimo elemento do mapa + 1
          - int level -> Indica qual o nivel em que o jogo se encontra
          - int u[MXIU][MXJU] -> Matriz que armazena o mapa do jogo
    */
}map_t;

// ========================= FUNÇÔES =========================
// Função que define o jogador
void makePlayer(player_t *p);
// Função que imprime o jogador
void printPlayer(player_t p);
// Limpa ultima posição do player:
void clearPlayer(player_t p);
// Cria um array de tiros 'Default' do jogador: 
void makePlayerShootArray(playerShoot_t s[MAXSHOOT]);
// Cria tiro do jogador:
void makePlayerShoot(player_t *player);
// Move o tiro do jogador:
void moveAndPrintPlayerShoot(player_t *player, enemy_t e[ENEMYMAX], map_t map, int *enemyNumber);
// Imprime o map
int printMap(map_t m);
// Preenche o mapa lido com 'C's
void fillGaps(char txtFileMap[][MXJTXT]);
// Função que define o jogador
void mk_player(player_t *p);
// Função que cria um Array de inimigos Default:
void makeEnemyArray(enemy_t e[ENEMYMAX]);
// Função que cria um inimigo:
void makeEnemy(int i, int j, enemy_t e[ENEMYMAX], int *enemyNumber);
// Fução que move e imprime os inimigos:
int moveAndPrintEnemy(enemy_t e[ENEMYMAX], map_t m, player_t *player, int *enemyNumber);
// Função que cria um array de tiros Default
void makeEnemyShootArray(enemyShoot_t s[MAXSHOOT]);
// Cria tiros do inimigo:
void makeEnemyShoot(enemy_t e[ENEMYMAX]);
// Move tiro dos Inimigos
void moveAndPrintEnemyShoot(enemy_t e[ENEMYMAX], player_t *player, map_t map, int enemyNumber);
// Imprime Tiro dos inimigos
void printShoot(enemy_t e[ENEMYMAX], map_t map, player_t *player);
// Tranfere o conteudo do mapa lido para o mapa do jogo
int makeMap(map_t *m, enemy_t e[ENEMYMAX], int *enemyNumber, player_t *player);
// Imprime o cabeçalho do jogo:
void header(player_t player, map_t map, int enemyNumber);
// Checa se há colissão com o jogador:
void checkPlayer(player_t *player, enemy_t e[ENEMYMAX], map_t map, int *enemyNumber);
// Escreve arquivo binário com o Load:
void writeLoad(const player_t *player, const enemy_t e[ENEMYMAX], const map_t *map, const int *enemyNumber);
// Atribui os valores salvos em load:
void readLoad( player_t *player, enemy_t e[ENEMYMAX], map_t *map, int *enemyNumber);
// Defender:
int Defender(player_t *player, map_t *map, enemy_t e[ENEMYMAX], int enemyNumber);
// Menu:
int menu(player_t *player, enemy_t e[ENEMYMAX], map_t *map, int *enemyNumber);


#endif