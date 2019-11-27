// ========================= BIBLIOTECAS =========================
#include"Headers/Defender.h"

int main(){
    player_t player;
    enemy_t e[ENEMYMAX];
    map_t map;

    int nextLevel, won, loadFlag, menuFlag;
    int enemyNumber;

    // MENU:
    do{
        menuFlag=menu(&player, e, &map, &enemyNumber);
        //inicia laço de jogo:
        if(menuFlag==1){
            do{
                system("cls");
                won=Defender(&player, &map, e, enemyNumber);
                if(won){
                    map.level++;
                    makePlayer(&player);
                    makeEnemyArray(e);
                    enemyNumber=0;
                    nextLevel=makeMap(&map, e, &enemyNumber, &player);
                    makePlayerShootArray(player.shoot);
                }
            }while(nextLevel&&won);
            if(won){
                gotoxy(50,18);
                textcolor(GREEN);
                printf("YOU WON!");
                Sleep(4000);
                system("cls");
            }else{
                gotoxy(50,18);
                textcolor(RED);
                printf("GAME OVER!");
                Sleep(4000);
                system("cls");
            }
        }
    }while(menuFlag!=2);

}

// DEFENDER:
int Defender(player_t *player, map_t *map, enemy_t e[ENEMYMAX], int enemyNumber){
    // ========================= VARIÁVEIS =========================
    // MISCELLANEOUS:
        int engGame=0, enemyShootFlag=1;
        char c;
        char bin[MXIU][MXJTXT];
    // 'FLAGS':
        int dPressedFlag=0;
        int CoolDownFlag=0, CoolDownFlagAux=0, coolDownCheckerFlag=1;
        int playerSpeedFlag=1;
        int FLAG;
        int moveAndPrintEnemyFlag=1, moveEnemyFlag=1;
        int playerShootFlag=1;
        int moveAndPrintEnemyShootFlag=1;
        int moveAndPrintPlayerShootFlag=1;
        int playerShootedFlag=1;
    // VELOCIDADE DO JOGADOR:
        int coolDown=0;
    // ENEMIES TO PRINT:
        int enemiesToPrint=1;
    // 'TIMERS':
        // CoolDown:
        double CoolDownEnd_t=0, CoolDownAux_t=0, coolDownCheckerStart_t=0;
        // PlayerSpeed:
        double speed=0, speedStart_t=0;
        // PlayerShooted:
        double playerShootedStart_t=0;
        // PlayerShoot:
        double startPlayerShoot_t=0;
        // EnemyShoot:
        double startEnemyShoot_t=0;
        // Enemy:
        double moveEnemyStart_t=0;
        // 'Global':
        double start_t=0, end_t=0; // TEMPO DO LAÇO
    //

    gotoxy(50, 18);
    textcolor(GREEN);
    printf("Level %d", map->level+1);
    Sleep(2500);
    printMap(*map);
    printPlayer(*player);
    header(*player, *map, enemyNumber);
    srand(time(NULL));
    while(!engGame){
        // EndGame
        if((enemyNumber==0&&player->lifes>0)||player->xAsis==MXJTXT-2){
            engGame=1;
            FLAG=1;
        }else if(player->lifes==0){
            engGame=1;
            FLAG=0;
        }
        // Timers:
        start_t=(double)clock()/CLOCKS_PER_SEC;
        if(coolDownCheckerFlag){
            coolDownCheckerStart_t=(double)clock()/CLOCKS_PER_SEC;
            coolDownCheckerFlag=0;
        }
        if(enemyShootFlag){
            startEnemyShoot_t=(double)clock()/CLOCKS_PER_SEC;
            enemyShootFlag=0;
        }
        if(playerSpeedFlag){
            speedStart_t=(double)clock()/CLOCKS_PER_SEC;
            playerSpeedFlag=0;
        }
        if(moveEnemyFlag){
            moveEnemyStart_t=(double)clock()/CLOCKS_PER_SEC;
            moveEnemyFlag=0;
        }
        if(playerShootFlag){
            startPlayerShoot_t=(double)clock()/CLOCKS_PER_SEC;
            playerShootFlag=0;
        }
        dPressedFlag=0;
        // Leitura de teclado
        if(kbhit()){
            c=getch();
            switch (c){
                // D
                case 'd':
                case 'D':
                case 77: // RIGHT-ARROW
                    dPressedFlag=1;
                    if(coolDown){
                        CoolDownAux_t=start_t;
                    }
                    break;
                // W
                case 'w':
                case 'W':
                case 72:  // UP-ARROW
                    clearPlayer(*player);
                    player->yAsis--;
                    // Verifica se não há colisão com o mapa
                    if(map->u[player->yAsis][player->xAsis]=='C'||map->u[player->yAsis][player->xAsis+1]=='C'||map->u[player->yAsis][player->xAsis+2]=='C'||map->u[player->yAsis][player->xAsis+3]=='C'||map->u[player->yAsis-1][player->xAsis]=='C'){
                        player->yAsis=player->yAsis+4;
                        player->shooted=1;
                        player->lifes--;
                        header(*player, *map, enemyNumber);
                    }
                    if(player->lifes>0){
                        printPlayer(*player);
                    }
                    checkPlayer(player, e, *map, &enemyNumber);
                    break;
                // S
                case 's':
                case 'S':
                case 80: // DOWN-ARROW
                        clearPlayer(*player);
                        player->yAsis++;
                        // Verifica se não há colisão com o mapa
                        if(map->u[player->yAsis][player->xAsis]=='C'||map->u[player->yAsis][player->xAsis+1]=='C'||map->u[player->yAsis][player->xAsis+2]=='C'||map->u[player->yAsis][player->xAsis+3]=='C'||map->u[player->yAsis-1][player->xAsis]=='C'){
                            player->yAsis=player->yAsis-4;
                            player->shooted=1;
                            player->lifes--;
                            header(*player, *map, enemyNumber);
                        }
                        if(player->lifes>0){
                            printPlayer(*player);
                        }
                        checkPlayer(player, e, *map, &enemyNumber);
                    break;
                // A
                case 'a':
                case 'A':
                case 75: // LEFT-ARROW
                    player->maxSpeed+=0.25;
                    if(player->maxSpeed>MAXSPEED){
                        player->maxSpeed=MAXSPEED;
                    }
                    checkPlayer(player, e, *map, &enemyNumber);
                    break;
                // SPACEBAR
                case 32:
                    makePlayerShoot(player);
                    break;
                // SAVE:
                case 'G':
                case 'g':
                    writeLoad( player, e, map, &enemyNumber);
                    break;
            }
        }
        // Zera CoolDown
        if(CoolDownAux_t-CoolDownEnd_t>=.75){
            CoolDownAux_t=0;
            CoolDownEnd_t=0;
            coolDown=0;
            CoolDownFlag++;
        }
        end_t=(double)clock()/CLOCKS_PER_SEC;
        // Decrementa a velocidade
        if(end_t-coolDownCheckerStart_t>=1.5){
            coolDownCheckerFlag=1;
            if(CoolDownFlag==CoolDownFlagAux){
                player->maxSpeed+=0.5;
                if(player->maxSpeed>MAXSPEED){
                    player->maxSpeed=MAXSPEED;
                }
                moveAndPrintEnemyFlag=1;
                moveAndPrintEnemyShootFlag=1;
            }
            CoolDownFlagAux=CoolDownFlag;
        }
        // Incrementa Velocidade
        if(dPressedFlag){
            if(!coolDown){
                player->maxSpeed-=0.25;
                if(player->maxSpeed<=0.10){
                    player->maxSpeed=0.10;
                }
                coolDown=1;
                CoolDownEnd_t=(double)clock()/CLOCKS_PER_SEC;
            }
        }
        // Move mapa
        if(end_t-speedStart_t>=player->maxSpeed){
            if(player->maxSpeed!=MAXSPEED){
                // Incrementa posição x da nave
                player->xAsis++;
                // Move o mapa
                map->finalX++;
                map->initalX++;
                checkPlayer(player, e, *map, &enemyNumber);
                printMap(*map);
                if(player->maxSpeed<=SHOOTSPEED){
                    moveAndPrintEnemyShootFlag=0;
                    moveAndPrintPlayerShootFlag=0;
                    moveAndPrintEnemyShoot(e, player, *map, enemyNumber);
                    moveAndPrintEnemyShoot(e, player, *map, enemyNumber);
                    moveAndPrintPlayerShoot(player, e, *map, &enemyNumber);
                    moveAndPrintPlayerShoot(player, e, *map, &enemyNumber);
                }else{
                    moveAndPrintEnemyShootFlag=1;
                    moveAndPrintPlayerShootFlag=1;
                }
                if(player->maxSpeed<=ENEMYSPEED){
                    moveAndPrintEnemyFlag=0;
                    if(enemiesToPrint!=0){
                        enemiesToPrint=moveAndPrintEnemy(e, *map, player, &enemyNumber);
                    }
                    makeEnemyShoot(e);
                }else{
                    moveAndPrintEnemyFlag=1;
                }
                clearPlayer(*player);
                printPlayer(*player);
            }
            playerSpeedFlag=1;
        }
        // Move e imprime os tiros do inimigo:
        if(end_t-startEnemyShoot_t>=SHOOTSPEED){
            if(moveAndPrintEnemyShootFlag){
                moveAndPrintEnemyShoot(e, player, *map, enemyNumber);
            }
            enemyShootFlag=1;
        }
        // Move os inimigos
        if(end_t-moveEnemyStart_t>=ENEMYSPEED&&enemiesToPrint!=0){
            if(moveAndPrintEnemyFlag){
                enemiesToPrint=moveAndPrintEnemy(e, *map, player, &enemyNumber);
                makeEnemyShoot(e);
            }
            moveEnemyFlag=1;
        }
        // Move e Imprime os tiros do jogador:
        if(end_t-startPlayerShoot_t>=SHOOTSPEED-.15){
            if(moveAndPrintPlayerShootFlag){
                moveAndPrintPlayerShoot(player, e, *map, &enemyNumber);
            }
            playerShootFlag=1;
        }
        // Breve 'Imunidade'
        if(player->shooted){
            if(playerShootedFlag){
                playerShootedStart_t=(double)clock()/CLOCKS_PER_SEC;
                playerShootedFlag=0;
            }
            if(end_t-playerShootedStart_t>=5){
                player->shooted=0;
                playerShootedFlag=1;
            }
        }
    }

    return FLAG;
}

// ========================= PLAYER =========================
// Function que limpa a ultima posição do jogador:
void clearPlayer(player_t p){
    gotoxy(p.staticXAsis, p.yAsis+2);
    printf("    ");
    gotoxy(p.staticXAsis, p.yAsis+1);
    printf(" ");
}
// Cria Jogador:
void makePlayer(player_t *p){
    p->lifes=3;
    p->shooted=0;
    p->maxSpeed=MAXSPEED;
    strncpy(p->ship[0], "@", 5);
    strncpy(p->ship[1], "@@@@", 5);
    makePlayerShootArray(p->shoot);
}
// Imprime Jogador:
void printPlayer(player_t p){
    gotoxy(p.staticXAsis, p.yAsis+2);
    textcolor(WHITE);
    printf("%s", p.ship[1]);
    gotoxy(p.staticXAsis, p.yAsis+1);
    textcolor(WHITE);
    printf("%s", p.ship[0]);
}
// Vê se há colisão:
void checkPlayer(player_t *player, enemy_t e[ENEMYMAX], map_t map, int *enemyNumber){
    int i, j;

    i=0;
    // Vê se há colissão entre o Player e uma nave inimiga ou um tiro inimigo
    do{
        if(e[i].active&&e[i].print&&!(player->shooted)){
            // Verifica se o player atingiu uma nave inimiga:
            if((player->xAsis==e[i].xAsis&&player->yAsis==e[i].yAsis)||(player->xAsis==e[i].xAsis+1&&player->yAsis==e[i].yAsis)||(player->xAsis==e[i].xAsis&&player->yAsis==e[i].yAsis-1)||(player->xAsis==e[i].xAsis+1&&player->yAsis==e[i].yAsis-1)||(player->xAsis+1==e[i].xAsis&&player->yAsis==e[i].yAsis)||(player->xAsis+1==e[i].xAsis+1&&player->yAsis==e[i].yAsis)||(player->xAsis+1==e[i].xAsis&&player->yAsis==e[i].yAsis-1)||(player->xAsis+1==e[i].xAsis+1&&player->yAsis==e[i].yAsis-1)||(player->xAsis+2==e[i].xAsis&&player->yAsis==e[i].yAsis)||(player->xAsis+2==e[i].xAsis+1&&player->yAsis==e[i].yAsis)||(player->xAsis+2==e[i].xAsis&&player->yAsis==e[i].yAsis-1)||(player->xAsis+2==e[i].xAsis+1&&player->yAsis==e[i].yAsis-1)||(player->xAsis+3==e[i].xAsis&&player->yAsis==e[i].yAsis)||(player->xAsis+3==e[i].xAsis+1&&player->yAsis==e[i].yAsis)||(player->xAsis+3==e[i].xAsis&&player->yAsis==e[i].yAsis-1)||(player->xAsis+3==e[i].xAsis+1&&player->yAsis==e[i].yAsis-1)||(player->xAsis==e[i].xAsis&&player->yAsis-1==e[i].yAsis)||(player->xAsis==e[i].xAsis+1&&player->yAsis-1==e[i].yAsis)||(player->xAsis==e[i].xAsis&&player->yAsis-1==e[i].yAsis-1)||(player->xAsis==e[i].xAsis+1&&player->yAsis-1==e[i].yAsis-1)){
                e[i].active=0;
                e[i].needToShoot=0;
                e[i].needToMove=0;
                gotoxy(e[i].xAsis-map.initalX+1, e[i].yAsis+2);
                printf("  ");
                gotoxy(e[i].xAsis-map.initalX+1, e[i].yAsis+1);
                printf("  ");
                (*enemyNumber)--;
                clearPlayer(*player);
                player->lifes--;
                header(*player, map, *enemyNumber);
                player->shooted=1;
                if(player->lifes>0){
                    printPlayer(*player);
                }
            }else{
                j=0;
                do{
                    // Verifica se o player atingiu um tiro do inimigo:
                    if((e[i].shoot[j].xAsis==player->xAsis&&e[i].shoot[j].yAsis==player->yAsis)||(e[i].shoot[j].xAsis==player->xAsis+1&&e[i].shoot[j].yAsis==player->yAsis)||(e[i].shoot[j].xAsis==player->xAsis+2&&e[i].shoot[j].yAsis==player->yAsis)||(e[i].shoot[j].xAsis==player->xAsis+3&&e[i].shoot[j].yAsis==player->yAsis)||(e[i].shoot[j].xAsis==player->xAsis&&e[i].shoot[j].yAsis==player->yAsis-1)){
                        if(player->shooted==0){
                            player->lifes--;
                            header(*player, map, *enemyNumber);
                            clearPlayer(*player);
                            player->shooted=1;
                        }
                        e[i].shoot[j].active=0;
                        e[i].shoot[j].mov=0;
                        if(player->lifes>0){
                            printPlayer(*player);
                        }
                    }
                    j++;
                }while(j<MAXSHOOT&&!(player->shooted));
            }

        }
        i++;
    }while(i<ENEMYMAX&&!(player->shooted));
}

// ========================= PLAYER SHOOTING =========================
void makePlayerShootArray(playerShoot_t s[MAXSHOOT]){
    int i;

    for(i=0;i<MAXSHOOT;i++){
        s[i].active=0;
        s[i].xAsis=0;
        s[i].yAsis=0;
    }
}
void makePlayerShoot(player_t *player){
    int i=0, flag=0;

    do{
        if(!(player->shoot[i].active)){
            player->shoot[i].active=1;
            player->shoot[i].xAsis=player->xAsis+4;
            player->shoot[i].yAsis=player->yAsis;
            flag=1;
        }
        i++;
    }while(!flag&&i<MAXSHOOT);
}
void moveAndPrintPlayerShoot(player_t *player, enemy_t e[ENEMYMAX], map_t map, int *enemyNumber){
    int i, j, hited=0;

    for(i=0;i<MAXSHOOT;i++){
        if(player->shoot[i].active){
            gotoxy(player->shoot[i].xAsis-map.initalX+1, player->shoot[i].yAsis+2);
            printf("  ");
            player->shoot[i].xAsis++;
            if(map.u[player->shoot[i].yAsis][player->shoot[i].xAsis]=='C'||map.u[player->shoot[i].yAsis][player->shoot[i].xAsis+1]=='C'||map.u[player->shoot[i].yAsis][player->shoot[i].xAsis+2]=='C'||map.finalX<=player->shoot[i].xAsis-1){
                player->shoot[i].active=0;
                player->shoot[i].xAsis=0;
                player->shoot[i].yAsis=0;
            }
            j=0;
            do{
                if(e[j].active&&e[j].print){
                    if((player->shoot[i].xAsis==e[j].xAsis&&player->shoot[i].yAsis==e[j].yAsis)||(player->shoot[i].xAsis+1==e[j].xAsis&&player->shoot[i].yAsis==e[j].yAsis)||(player->shoot[i].xAsis==e[j].xAsis+1&&player->shoot[i].yAsis==e[j].yAsis)||(player->shoot[i].xAsis+1==e[j].xAsis+1&&player->shoot[i].yAsis==e[j].yAsis)||(player->shoot[i].xAsis==e[j].xAsis&&player->shoot[i].yAsis==e[j].yAsis-1)||(player->shoot[i].xAsis+1==e[j].xAsis&&player->shoot[i].yAsis==e[j].yAsis-1)||(player->shoot[i].xAsis==e[j].xAsis+1&&player->shoot[i].yAsis==e[j].yAsis-1)||(player->shoot[i].xAsis+1==e[j].xAsis+1&&player->shoot[i].yAsis==e[j].yAsis-1)){
                        hited=1;
                        (player->points)+=10;
                        player->shoot[i].active=0;
                        player->shoot[i].xAsis=0;
                        player->shoot[i].yAsis=0;
                        e[j].active=0;
                        e[j].needToShoot=0;
                        e[j].needToMove=0;
                        gotoxy(e[j].xAsis-map.initalX+1, e[j].yAsis+2);
                        printf("  ");
                        gotoxy(e[j].xAsis-map.initalX+1, e[j].yAsis+1);
                        printf("  ");
                        (*enemyNumber)--;
                        header(*player, map, *enemyNumber);
                    }
                }
                j++;
            }while(j<ENEMYMAX&&!hited);
            if(player->shoot[i].active){
                gotoxy(player->shoot[i].xAsis-map.initalX+1, player->shoot[i].yAsis+2);
                textcolor(GREEN);
                printf("--");
            }
        }
    }
}

// ========================= ENEMY =========================
// Cria Array de inimigos Default:
void makeEnemyArray(enemy_t e[ENEMYMAX]){
    int i;

    for(i=0;i<ENEMYMAX;i++){
        e[i].active=0;
        e[i].xAsis=0;
        e[i].yAsis=0;
        e[i].move=0;
        e[i].print=0;
        e[i].moveFlag=0;
        e[i].needToMove=1;
        e[i].needToShoot=0;
        makeEnemyShootArray(e[i].shoot);
    }
}
// Cria um inimigo
void makeEnemy(int i, int j, enemy_t e[ENEMYMAX], int *enemyNumber){
    int k, flag;

    if(*enemyNumber<ENEMYMAX){
        flag=0;
        k=0;
        do{
            if(!e[k].active){
                e[k].active=1;
                e[k].xAsis=j;
                e[k].yAsis=i;
                flag=1;
                (*enemyNumber)++;
            }else{
                k++;
            }
        }while(!flag);
    }
}
// Movimenta e Imprime os inimigos
int moveAndPrintEnemy(enemy_t e[ENEMYMAX], map_t m, player_t *player, int *enemyNumber){
    int i, j, hited;
    int enemiesToPrint=0;
    printf(" ");

    srand(time(NULL));
    if(*enemyNumber>0){
        for(i=0;i<ENEMYMAX;i++){
            hited=0;
            if(e[i].xAsis<=m.initalX){
                e[i].needToMove=0;
            }
            if(e[i].xAsis>m.initalX){
                enemiesToPrint++;
            }
            // Move se estiver ativo
            if(e[i].active){
                if(e[i].needToMove){
                    if(e[i].moveFlag==0){
                        e[i].move=0;
                    }
                    // Coloca uma posição
                    if(e[i].move==0){
                        e[i].move=ENEMYMOVMN+(rand()%(ENEMYMOVMX-ENEMYMOVMN+1));
                        e[i].moveFlag=4;
                    }
                    // Limpa posição anterir
                    if(e[i].print){
                        gotoxy(e[i].xAsis-m.initalX+1, e[i].yAsis+2);
                        printf("  ");
                        gotoxy(e[i].xAsis-m.initalX+1, e[i].yAsis+1);
                        printf("  ");
                    }
                    // Move eixo X
                    if(m.u[e[i].yAsis][e[i].xAsis-1]=='C'||m.u[e[i].yAsis][e[i].xAsis-2]=='C'||m.u[e[i].yAsis][e[i].xAsis-3]=='C'||m.u[e[i].yAsis][e[i].xAsis-4]=='C'||m.u[e[i].yAsis][e[i].xAsis-5]=='C'||m.u[e[i].yAsis-1][e[i].xAsis-1]=='C'||m.u[e[i].yAsis-1][e[i].xAsis-2]=='C'||m.u[e[i].yAsis-1][e[i].xAsis-3]=='C'||m.u[e[i].yAsis-1][e[i].xAsis-4]=='C'||m.u[e[i].yAsis-1][e[i].xAsis-5]=='C'){
                        if(e[i].yAsis<=17){
                            e[i].yAsis++;
                        }else if(e[i].yAsis>=18){
                            e[i].yAsis--;
                        }
                        e[i].moveFlag=0;
                    }else{
                        e[i].xAsis--;
                        // Move eixo Y
                        switch (e[i].move){
                            // Para cima
                            case 1:
                                if(m.u[e[i].yAsis-2][e[i].xAsis]=='C'||m.u[e[i].yAsis-3][e[i].xAsis]=='C'||m.u[e[i].yAsis-4][e[i].xAsis]=='C'||m.u[e[i].yAsis-5][e[i].xAsis]=='C'||m.u[e[i].yAsis-6][e[i].xAsis]=='C'||m.u[e[i].yAsis-2][e[i].xAsis+1]=='C'||m.u[e[i].yAsis-3][e[i].xAsis+1]=='C'||m.u[e[i].yAsis-4][e[i].xAsis+1]=='C'||m.u[e[i].yAsis-5][e[i].xAsis+1]=='C'||m.u[e[i].yAsis-6][e[i].xAsis+1]=='C'){
                                    e[i].yAsis++;
                                    e[i].move=3;
                                    e[i].moveFlag=4;
                                }else{
                                    e[i].yAsis--;
                                }
                                break;
                            // Linha reta
                            case 2:
                                if(m.u[e[i].yAsis-2][e[i].xAsis]=='C'||m.u[e[i].yAsis-2][e[i].xAsis+1]=='C'){
                                    e[i].yAsis++;
                                }else if(m.u[e[i].yAsis+1][e[i].xAsis+1]=='C'||m.u[e[i].yAsis+1][e[i].xAsis]=='C'){
                                    e[i].yAsis--;
                                }
                                break;
                            // Para baixo
                            case 3:
                                if(m.u[e[i].yAsis+1][e[i].xAsis]=='C'||m.u[e[i].yAsis+2][e[i].xAsis]=='C'||m.u[e[i].yAsis+3][e[i].xAsis]=='C'||m.u[e[i].yAsis+1][e[i].xAsis+1]=='C'||m.u[e[i].yAsis+2][e[i].xAsis+1]=='C'||m.u[e[i].yAsis+3][e[i].xAsis+1]=='C'){
                                    e[i].yAsis--;
                                    e[i].move=1;
                                    e[i].moveFlag=4;
                                }else{
                                    e[i].yAsis++;
                                }
                                break;
                        }
                        e[i].moveFlag--;
                        if(e[i].moveFlag<0){
                            e[i].moveFlag=0;
                        }
                    }
                    j=0;
                    // Vê se foi atingido por tiro
                    do{
                        if((e[i].xAsis==player->shoot[j].xAsis&&e[i].yAsis==player->shoot[j].yAsis)||(e[i].xAsis==player->shoot[j].xAsis+1&&e[i].yAsis==player->shoot[j].yAsis)||(e[i].xAsis+1==player->shoot[j].xAsis&&e[i].yAsis==player->shoot[j].yAsis)||(e[i].xAsis+1==player->shoot[j].xAsis+1&&e[i].yAsis==player->shoot[j].yAsis)||(e[i].xAsis==player->shoot[j].xAsis&&e[i].yAsis-1==player->shoot[j].yAsis)||(e[i].xAsis==player->shoot[j].xAsis+1&&e[i].yAsis-1==player->shoot[j].yAsis)||(e[i].xAsis+1==player->shoot[j].xAsis&&e[i].yAsis-1==player->shoot[j].yAsis)||(e[i].xAsis+1==player->shoot[j].xAsis+1&&e[i].yAsis-1==player->shoot[j].yAsis)){
                            hited=1;
                            // Limpa posição do tiro
                            gotoxy(player->shoot[j].xAsis-m.initalX+1, player->shoot[j].yAsis+2);
                            printf("  ");
                            player->shoot[j].active=0;
                            player->shoot[j].xAsis=0;
                            player->shoot[j].yAsis=0;
                            e[i].active=0;
                            e[i].needToShoot=0;
                            e[i].needToMove=0;
                            (*enemyNumber)--;
                            // limpa posição do inimigo
                            gotoxy(e[i].xAsis-m.initalX+1, e[i].yAsis+2);
                            printf("  ");
                            gotoxy(e[i].xAsis-m.initalX+1, e[i].yAsis+1);
                            printf("  ");
                            (player->points)+=10;
                            header(*player, m, *enemyNumber);
                        }
                        j++;
                    }while(!hited&&j<MAXSHOOT);
                    if(e[i].xAsis>m.initalX&&e[i].xAsis<m.finalX&&e[i].active){
                        e[i].print=1;
                    }else{
                        e[i].print=0;
                    }
                    // PrintEnemy
                    if(e[i].print){
                        gotoxy(e[i].xAsis-m.initalX+1, e[i].yAsis+2);
                        textcolor(WHITE);
                        printf("##");
                        gotoxy(e[i].xAsis-m.initalX+1, e[i].yAsis+1);
                        textcolor(WHITE);
                        printf("##");
                    }
                    // Testa colissão entre Player e Enemy:
                    if((player->xAsis==e[i].xAsis&&player->yAsis==e[i].yAsis)||(player->xAsis==e[i].xAsis+1&&player->yAsis==e[i].yAsis)||(player->xAsis==e[i].xAsis&&player->yAsis==e[i].yAsis-1)||(player->xAsis==e[i].xAsis+1&&player->yAsis==e[i].yAsis-1)||(player->xAsis+1==e[i].xAsis&&player->yAsis==e[i].yAsis)||(player->xAsis+1==e[i].xAsis+1&&player->yAsis==e[i].yAsis)||(player->xAsis+1==e[i].xAsis&&player->yAsis==e[i].yAsis-1)||(player->xAsis+1==e[i].xAsis+1&&player->yAsis==e[i].yAsis-1)||(player->xAsis+2==e[i].xAsis&&player->yAsis==e[i].yAsis)||(player->xAsis+2==e[i].xAsis+1&&player->yAsis==e[i].yAsis)||(player->xAsis+2==e[i].xAsis&&player->yAsis==e[i].yAsis-1)||(player->xAsis+2==e[i].xAsis+1&&player->yAsis==e[i].yAsis-1)||(player->xAsis+3==e[i].xAsis&&player->yAsis==e[i].yAsis)||(player->xAsis+3==e[i].xAsis+1&&player->yAsis==e[i].yAsis)||(player->xAsis+3==e[i].xAsis&&player->yAsis==e[i].yAsis-1)||(player->xAsis+3==e[i].xAsis+1&&player->yAsis==e[i].yAsis-1)||(player->xAsis==e[i].xAsis&&player->yAsis-1==e[i].yAsis)||(player->xAsis==e[i].xAsis+1&&player->yAsis-1==e[i].yAsis)||(player->xAsis==e[i].xAsis&&player->yAsis-1==e[i].yAsis-1)||(player->xAsis==e[i].xAsis+1&&player->yAsis-1==e[i].yAsis-1)){
                        e[i].active=0;
                        e[i].needToShoot=0;
                        e[i].needToMove=0;
                        (*enemyNumber)--;
                        clearPlayer(*player);
                        gotoxy(e[i].xAsis-m.initalX+1, e[i].yAsis+2);
                        printf("  ");
                        gotoxy(e[i].xAsis-m.initalX+1, e[i].yAsis+1);
                        printf("  ");
                        (player->points)+=10;
                        player->shooted=1;
                        player->lifes--;
                        header(*player, m, *enemyNumber);
                        if(player->lifes>0){
                            printPlayer(*player);
                        }
                    }
                }
            }
        }
    }

    return enemiesToPrint;
}

// ========================= ENEMY SHOOTING =========================
// Faz um array de tiros do inimigo Default:
void makeEnemyShootArray(enemyShoot_t s[MAXSHOOT]){
    int i;

    for(i=0;i<MAXSHOOT;i++){
        s[i].active=0;
        s[i].xAsis=0;
        s[i].yAsis=0;
        s[i].mov=0;
    }
}
// Cria tiro dos Inimigos
void makeEnemyShoot(enemy_t e[ENEMYMAX]){
    int i, j;

    srand(time(NULL));
    for(i=0;i<ENEMYMAX;i++){
        j=0;
        if(e[i].active&&e[i].print){
            e[i].needToShoot=0+(rand()%(1-0+1));
        }
        if(e[i].needToShoot){
            do{
                if(!e[i].shoot[j].active){
                    e[i].shoot[j].active=1;
                    e[i].shoot[j].xAsis=e[i].xAsis-1;
                    e[i].shoot[j].yAsis=e[i].yAsis;
                }else{
                    j++;
                }
            }while(!e[i].shoot[j].active&&j<MAXSHOOT);
        }
    }
}
// Move tiro dos Inimigos
void moveAndPrintEnemyShoot(enemy_t e[ENEMYMAX], player_t *player, map_t map, int enemyNumber){
    int i, j;

    for(i=0;i<ENEMYMAX;i++){
        for(j=0;j<MAXSHOOT;j++){
            if(e[i].shoot[j].active){
                // Dá um movimento para o tiro:
                if(e[i].shoot[j].mov==0){
                    // linha reta:
                    if(player->yAsis==e[i].yAsis||player->yAsis-1==e[i].yAsis){
                        e[i].shoot[j].mov=2;
                    // Para Baixo - Linha Reta
                    }else if((player->xAsis==e[i].xAsis&&player->yAsis>e[i].yAsis)||(player->xAsis+1==e[i].xAsis&&player->yAsis>e[i].yAsis)||(player->xAsis+2==e[i].xAsis&&player->yAsis>e[i].yAsis)||(player->xAsis+3==e[i].xAsis&&player->yAsis>e[i].yAsis)||(player->xAsis==e[i].xAsis+1&&player->yAsis>e[i].yAsis)||(player->xAsis+1==e[i].xAsis+1&&player->yAsis>e[i].yAsis)||(player->xAsis+2==e[i].xAsis+1&&player->yAsis>e[i].yAsis)||(player->xAsis+3==e[i].xAsis+1&&player->yAsis>e[i].yAsis)){
                        e[i].shoot[j].mov=4;
                    // Para Cima - Linha Reta
                    }else if((player->xAsis==e[i].xAsis&&player->yAsis<e[i].yAsis)||(player->xAsis+1==e[i].xAsis&&player->yAsis<e[i].yAsis)||(player->xAsis+2==e[i].xAsis&&player->yAsis<e[i].yAsis)||(player->xAsis+3==e[i].xAsis&&player->yAsis<e[i].yAsis)||(player->xAsis==e[i].xAsis+1&&player->yAsis<e[i].yAsis)||(player->xAsis+1==e[i].xAsis+1&&player->yAsis<e[i].yAsis)||(player->xAsis+2==e[i].xAsis+1&&player->yAsis<e[i].yAsis)||(player->xAsis+3==e[i].xAsis+1&&player->yAsis<e[i].yAsis)){
                        e[i].shoot[j].mov=5;
                    // Para Baixo - Diagonal
                    }else if(player->yAsis>e[i].yAsis){
                        e[i].shoot[j].mov=3;
                    // Para cima - Diagonal
                    }else if(player->yAsis-1<e[i].yAsis){
                        e[i].shoot[j].mov=1;
                    }
                }
                // Apaga ultima posição do tiro:
                gotoxy(e[i].shoot[j].xAsis-map.initalX+1, e[i].shoot[j].yAsis+2);
                printf(" ");
                // Move os tiros:
                switch (e[i].shoot[j].mov){
                    // Tiro para cima em Diagonal:
                    case 1:
                        e[i].shoot[j].xAsis--;
                        e[i].shoot[j].yAsis--;
                    break;
                    // Tiro em linha reta:
                    case 2:
                        e[i].shoot[j].xAsis--;
                    break;
                    // Tiro para baixo em Diagonal:
                    case 3:
                        e[i].shoot[j].xAsis--;
                        e[i].shoot[j].yAsis++;
                    break;
                    // Tiro para baixo em Linha Reta:
                    case 4:
                        e[i].shoot[j].yAsis++;
                        break;
                    // Tiro para cima em Linha Reta:
                    case 5:
                        e[i].shoot[j].yAsis--;
                        break;

                }
                // Verifica se os tiros atingem o cenário:
                if(map.u[e[i].shoot[j].yAsis-1][e[i].shoot[j].xAsis]=='C'||map.u[e[i].shoot[j].yAsis+1][e[i].shoot[j].xAsis]=='C'||map.u[e[i].shoot[j].yAsis][e[i].shoot[j].xAsis-1]=='C'||map.initalX>=e[i].shoot[j].xAsis-1){
                    e[i].shoot[j].active=0;
                    e[i].shoot[j].mov=0;
                }
                if(e[i].shoot[j].active){
                    // Imprime os Tiros:
                    gotoxy(e[i].shoot[j].xAsis-map.initalX+1, e[i].shoot[j].yAsis+2);
                    textcolor(RED);
                    printf("%c", '.');
                }
                // Verifica se atingiram o Jogador:
                if((e[i].shoot[j].xAsis==player->xAsis&&e[i].shoot[j].yAsis==player->yAsis)||(e[i].shoot[j].xAsis==player->xAsis+1&&e[i].shoot[j].yAsis==player->yAsis)||(e[i].shoot[j].xAsis==player->xAsis+2&&e[i].shoot[j].yAsis==player->yAsis)||(e[i].shoot[j].xAsis==player->xAsis+3&&e[i].shoot[j].yAsis==player->yAsis)||(e[i].shoot[j].xAsis==player->xAsis&&e[i].shoot[j].yAsis==player->yAsis-1)){
                    if(player->shooted==0){
                        player->lifes--;
                        header(*player, map, enemyNumber);
                        clearPlayer(*player);
                        player->shooted=1;
                    }
                    e[i].shoot[j].active=0;
                    e[i].shoot[j].mov=0;
                    if(player->lifes>0){
                        printPlayer(*player);
                    }
                }
            }

        }
    }
}

// ========================= MAP =========================
// Copia a matriz txtFileMap quantas vezes for necessario
// até preencher a matrizUniverso - mapa do jogo
int makeMap(map_t *m, enemy_t e[ENEMYMAX], int *enemyNumber, player_t *player){
    int i, j; // (x, y) da matriz Universo
    int k, l; // (x, y) da matriz lida do arquivo
    int flag;
    char c;
    char bin[MXIU][MXJTXT];
    char levels[10][50]={"Map/nivel1.txt", "Map/nivel2.txt", "Map/nivel3.txt", "Map/nivel4.txt", "Map/nivel5.txt", "Map/nivel6.txt", "Map/nivel7.txt", "Map/nivel8.txt", "Map/nivel9.txt", "Map/nivel10.txt"};
    FILE *file;

    file=NULL;
    file=fopen(levels[m->level], "r");
    if(file==NULL){
        flag=0;
    }else{
        flag=1;
        for(i=0;i<MXIU;i++){
            fgets(bin[i], MXJTXT, file);
        }
        fclose(file);
        fillGaps(bin);
        for(j=0, l=0; j<MXJU;j++, l++){
            if(l==MXJTXT-2){
                l=0;
            }
            for(i=0;i<MXIU;i++){
                m->u[i][j]=0+bin[i][l];
                if(bin[i][l]=='@'){
                    player->yAsis=i;
                    player->xAsis=j;
                    player->staticXAsis=j;
                    bin[i][l]=' ';
                }else if(bin[i][l]=='X'){
                    makeEnemy(i, j, e, enemyNumber);
                }
            }
        }
        m->initalX=1;
        m->finalX=MXPTR;
    }
    return flag;
}
// Imprime o mapa
int printMap(map_t m){
    int i, j, flag;

    flag=0;
    gotoxy(1, 2);
    if(m.initalX!=415){
        for(i=0; i<MXIU; i++){
            for(j=m.initalX; j<MXPTR+m.initalX; j++){
                if(m.u[i][j]=='C'){
                    textcolor(WHITE);
                    printf("%c", 219);
                }else{
                    printf(" ");
                }
            }
            printf("\n");
        }
    }else{
        flag=1;
    }

    return flag;
}
// Preenche os espaços com uns:
void fillGaps(char bin[][MXJTXT]){
    // FLAGS:
    int flag=0, iFlag=0;
    // AUX:
    int auxI=0, i=0, j=0;

    j=0;
    do{
        flag=0;
        i=0;
        do{
            iFlag=0;
            if(bin[i][j]=='C'){
                //Parte Superior do Mapa
                if(i<=17){
                    auxI=i-1;
                    if(auxI<0){
                        auxI=0;
                    }else{
                        auxI=1;
                    }
                    // Preenche
                    if(auxI){
                        if(bin[i-1][j]==' '){
                            for(auxI=i-1;auxI>=0;auxI--){
                                bin[auxI][j]='C';
                            }
                        }
                    }
                }
                //Parte Inferior do Mapa
                if(i>=18){
                    // Testa se não acessa areas de memoria indesejadas
                    auxI=i+1;
                    if(auxI>MXIU){
                        auxI=0;
                    }else{
                        auxI=1;
                    }
                    if(auxI){
                        if(bin[i+1][j]==' '){
                            for(auxI=i+1;auxI<MXIU;auxI++){
                                bin[auxI][j]='C';
                            }
                            iFlag=1;
                        }
                    }
                }
            }
            i++;
        }while(i<MXIU&&!iFlag);
        j++;
    }while(j!=MXJTXT-2);
}

// ========================= HEADER =========================
void header(player_t player, map_t map, int enemyNumber){
    gotoxy(1,1);
    printf("                                                                          ");
    gotoxy(1,1);
    textcolor(LIGHTGREEN);
    printf("                   PONTOS: %d    VIDAS: %d     NIVEL: %d     INIMIGOS: %d", player.points, player.lifes, map.level+1, enemyNumber);
}

// ========================= MENU =========================
// Menu:
int menu(player_t *player, enemy_t e[ENEMYMAX], map_t *map, int *enemyNumber){
    char op;
    int FLAG=0, i, j;
    char defenderTxt[13][160]={0};
    FILE *file;

    system("cls");
    file=NULL;
    file=fopen("Files/defender.txt", "r");
    if(file==NULL){
        textcolor(RED);
        printf("\n\n\n\n                             DEFENDER\n\n\n\n");
    }else{
        for(i=0;i<13;i++){
            fgets(defenderTxt[i], 160, file);
        }
        fclose(file);
        textcolor(RED);
        for(i=0;i<13;i++){
            for(j=0;j<160;j++){
                if(defenderTxt[i][j]=='D'){
                    printf("%c", 219);
                }else{
                    printf("%c", defenderTxt[i][j]);
                }
            }
        }
    }
    textcolor(WHITE);
    gotoxy(63, 30);
    printf("SELECIONE A OPCAO DE JOGO");
    gotoxy(67, 34);
    printf("  N- NOVO JOGO");
    gotoxy(67, 36);
    printf("C- CARREGAR JOGO");
    gotoxy(67, 38);
    printf(" Q-SAIR DO JOGO");
    do
    {
        op=getch(); //espera a tecla de opcao
    }while(op!='n'&&op!='c'&&op!='q');
    switch(op){
        // New Game
        case'n':
            map->level=0;
            player->points=0;
            makePlayer(player);
            makeEnemyArray(e);
            (*enemyNumber)=0;
            makeMap(map, e, enemyNumber, player);
            makePlayerShootArray(player->shoot);
            return 1;
        // Load Game
        case'c':
            readLoad( player, e, map, enemyNumber);
            return 1;
        // Quit Game
        case'q':
            system("cls"); // apaga tudo que estava na tela
            gotoxy(27, 10);
            textcolor(WHITE);
            printf("*******FIM DE JOGO*******\n\n\n\n\n\n");
            return 2;
    }
}

// ========================= LOAD =========================
void writeLoad(const player_t *player, const enemy_t e[ENEMYMAX], const map_t *map, const int *enemyNumber){
    FILE *file;
    int i, flag;

    file=NULL;
    file=fopen("Files/load.bin", "w");
    if(file==NULL){
        gotoxy(1, 37);
        textcolor(RED);
        printf(" Nao foi possivel salvar o jogo!");
    }else{
        fwrite(player, sizeof(*player), 1, file);
        fwrite(map, sizeof(*map), 1, file);
        fwrite(enemyNumber, sizeof(*enemyNumber), 1, file);
        for(i=0;i<ENEMYMAX;i++, e++){
            fwrite(e, sizeof(e[i]), 1, file);
        }
        gotoxy(1, 38);
        textcolor(GREEN);
        printf(" Jogo salvo com sucesso!");
    }
    fclose(file);
}
void readLoad( player_t *player, enemy_t e[ENEMYMAX], map_t *map, int *enemyNumber){
    FILE *file;
    int i;

    file=NULL;
    file=fopen("Files/load.bin", "r");
    if(file==NULL){
        system("cls");
        gotoxy(40, 18);
        printf("Voce nao possui nenhum jogo salvo!");
        gotoxy(40, 19);
        printf("   Um novo jogo sera iniciado!");
        Sleep(3500);
        map->level=0;
        player->points=0;
        makePlayer(player);
        makeEnemyArray(e);
        (*enemyNumber)=0;
        makeMap(map, e, enemyNumber, player);
        makePlayerShootArray(player->shoot);
    }else{
        fread(player, sizeof(*player), 1, file);
        fread(map, sizeof(*map), 1, file);
        fread(enemyNumber, sizeof(*enemyNumber), 1, file);
        for(i=0;i<ENEMYMAX;i++, e++){
            fread(e, sizeof(e[i]), 1, file);
        }
    }
    fclose(file);
}

