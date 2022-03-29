#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <conio2.h>

#ifdef _WIN32
#   include <windows.h>
#elif defined(__linux__)
#   include <errno.h>

int Sleep(long msec);
#endif

int main()
{
    int MAX_COLUMNS = 80;
    int MAX_LINES = 24;

    /* Game states */
    int MENU_STATE = 0;
    int GAME_STATE = 1;
    int RULES_STATE = 2;
    int GAMEOVER_STATE = 3;

    int state = MENU_STATE;
    int prev_state = -1;

    /** Menu state variables */
    int menu_curr_option = 0;

    /* Collector */
    int COLLECTOR_WIDTH = 11;

    int collector_x = MAX_COLUMNS / 2 - COLLECTOR_WIDTH / 2;
    int collector_y = MAX_LINES * 0.8;

    int collector_points = 0;

    /* Ball */
    int ball_x = 1 + rand() % MAX_COLUMNS;
    int ball_y = 4;
    float ball_move_y = 0;
    float ball_y_velocity = 0.0005;

    /* General variables */
    int quit_game = 0;
    int redraw = 1;

    int clear_color1, clear_color2;

    int i, j;
    char key = 0;

    clock_t time_start = clock();

    srand(time(NULL));
    setlocale(LC_ALL, "pt_BR.UTF-8");

    for (i = 1; i <= MAX_LINES; ++i) {
        for (j = 1; j <= MAX_COLUMNS; ++j) {
            gotoxy(j, i);

            if (i == 1 || i == MAX_LINES) {
                if (j == 1 || j == MAX_COLUMNS)
                    cprintf("+");
                else
                    cprintf(" ");
            } else {
                if (j == 1 || j == MAX_COLUMNS)
                    cprintf(":");
                else
                    cprintf(" ");
            }
        }
    }

    i = MAX_COLUMNS / 2 - 36;
    j = MAX_LINES / 2 - 3;

    gotoxy(i, j++);
    cprintf("_______ _______ ___     ___     _______ _______ _______ _______ _______");
    gotoxy(i, j++);
    cprintf("|   _   |   _   |   |   |   |   |   _   |   _   |       |   _   |   _   \\");
    gotoxy(i, j++);
    cprintf("|.  1___|.  |   |.  |   |.  |   |.  1___|.  1___|.|   | |.  |   |.  l   /");
    gotoxy(i, j++);
    cprintf("|.  |___|.  |   |.  |___|.  |___|.  __)_|.  |___`-|.  |-|.  |   |.  _   1");
    gotoxy(i, j++);
    cprintf("|:  1   |:  1   |:  1   |:  1   |:  1   |:  1   | |:  | |:  1   |:  |   |");
    gotoxy(i, j++);
    cprintf("|::.. . |::.. . |::.. . |::.. . |::.. . |::.. . | |::.| |::.. . |::.|:. |");
    gotoxy(i, j++);
    cprintf("`-------`-------`-------`-------`-------`-------' `---' `-------`--- ---'");

    i = 1;
    do {
        gotoxy(MAX_COLUMNS / 2 - 18, MAX_LINES * 0.8);

        if ((i = !i))
            cprintf("Precione qualquer tecla para iniciar!");
        else
            cprintf("                                     ");

        gotoxy(MAX_COLUMNS / 2, MAX_LINES);
        Sleep(500);
    } while (!kbhit());

    clear_color1 = BLACK;
    clear_color2 = BLACK;

    while (!quit_game) {
        if (prev_state != state) {
            prev_state = state;
            redraw = 1;

            for (i = 1; i <= MAX_LINES / 2; ++i) {
                for (j = 1; j <= MAX_COLUMNS; ++j) {
                    textbackground(WHITE);
                    gotoxy(j, i);
                    cprintf(" ");

                    gotoxy(j, MAX_LINES - (i - 1));
                    cprintf(" ");

                    if (i > 1) {
                        textbackground(clear_color1);
                        textcolor(clear_color1);
                        gotoxy(j, i - 1);
                        cprintf(" ");

                        if (i <= MAX_LINES * 0.2)
                            textbackground(clear_color2);

                        gotoxy(j, MAX_LINES - (i - 2));
                        cprintf(" ");
                    }
                }

                gotoxy(MAX_COLUMNS / 2, MAX_LINES / 2);
                Sleep(20);
            }

            textbackground(clear_color1);
            for (j = MAX_COLUMNS / 2; j > 0; --j) {
                for (i = 0; i < 2; ++i) {
                    gotoxy(j, MAX_LINES / 2 + i);
                    cprintf(" ");

                    gotoxy(MAX_COLUMNS - (j - 1), MAX_LINES / 2 + i);
                    cprintf(" ");
                }

                gotoxy(MAX_COLUMNS / 2, MAX_LINES);
                Sleep(15);
            }

            while (kbhit()) getch();
        }

        if (state == MENU_STATE) {
            redraw += 1;

            if (key == '\r') {
                switch (menu_curr_option) {
                case 0:
                    clear_color1 = BLUE;
                    clear_color2 = GREEN;

                    state = GAME_STATE;
                    break;

                case 1:
                    clear_color1 = BLACK;
                    clear_color2 = BLACK;

                    state = RULES_STATE;
                    break;

                default:
                    quit_game = 1;
                }
            }

            if (key == 'w') 
                menu_curr_option -= menu_curr_option > 0;
            else if (key == 's')
                menu_curr_option += menu_curr_option < 2;
            else
                redraw -= redraw > 0;

            if (redraw) {
                textcolor(WHITE);

                i = MAX_COLUMNS / 2 - 36;
                j = MAX_LINES * 0.3;

                gotoxy(i, j++);
                cprintf("_______ _______ ___     ___     _______ _______ _______ _______ _______");
                gotoxy(i, j++);
                cprintf("|   _   |   _   |   |   |   |   |   _   |   _   |       |   _   |   _   \\");
                gotoxy(i, j++);
                cprintf("|.  1___|.  |   |.  |   |.  |   |.  1___|.  1___|.|   | |.  |   |.  l   /");
                gotoxy(i, j++);
                cprintf("|.  |___|.  |   |.  |___|.  |___|.  __)_|.  |___`-|.  |-|.  |   |.  _   1");
                gotoxy(i, j++);
                cprintf("|:  1   |:  1   |:  1   |:  1   |:  1   |:  1   | |:  | |:  1   |:  |   |");
                gotoxy(i, j++);
                cprintf("|::.. . |::.. . |::.. . |::.. . |::.. . |::.. . | |::.| |::.. . |::.|:. |");
                gotoxy(i, j++);
                cprintf("`-------`-------`-------`-------`-------`-------' `---' `-------`--- ---'");

                if (menu_curr_option == 0)
                    textcolor(RED);
                else
                    textcolor(WHITE);

                gotoxy(MAX_COLUMNS / 2 - 5, ++j);
                cprintf("1. Iniciar");

                if (menu_curr_option == 1)
                    textcolor(RED);
                else
                    textcolor(WHITE);

                gotoxy(MAX_COLUMNS / 2 - 5, ++j);
                cprintf("2. Regras");

                if (menu_curr_option == 2)
                    textcolor(RED);
                else
                    textcolor(WHITE);

                gotoxy(MAX_COLUMNS / 2 - 5, ++j);
                cprintf("3. Sair");

                textcolor(WHITE);
                gotoxy(1, MAX_LINES - 1);
                cprintf("w: opção superior");

                gotoxy(1, MAX_LINES - 0);
                cprintf("s: opção inferior");

                redraw = 0;
            }
        } else if (state == GAME_STATE) {
            redraw += 1;

            if (key == 'b') {
                state = MENU_STATE;

                clear_color1 = BLACK;
                clear_color2 = BLACK;
            } else if (key == 'd') {
                collector_x += collector_x + COLLECTOR_WIDTH <= MAX_COLUMNS;
            } else if (key == 'a') {
                collector_x -= collector_x > 1;
            } else {
                redraw -= redraw > 0;
            }

            if ((clock() - time_start) / CLOCKS_PER_SEC > 1) {
                if ((ball_move_y += ball_y_velocity) > 1) {
                    ball_move_y = 0;
                    ++ball_y;
                    redraw = 1;

                    time_start = clock();
                }
            }

            if (ball_y == collector_y) {
                if (ball_x >= collector_x
                        && ball_x <= collector_x + COLLECTOR_WIDTH) {
                    textbackground(BLUE);
                    gotoxy(ball_x, ball_y - 1);
                    cprintf(" ");

                    ball_x = 1 + rand() % MAX_COLUMNS;
                    ball_y = 4;
                    ball_move_y = 0;
                    ++collector_points;
                } else {
                    state = GAMEOVER_STATE;

                    clear_color1 = BLACK;
                    clear_color2 = BLACK;
                }
            }

            if (redraw) {
                textcolor(BLACK);
                gotoxy(1, 1);
                cprintf("Pontos: %d", collector_points);

                textbackground(YELLOW);
                gotoxy(ball_x, ball_y);
                cprintf(" ");

                textbackground(BLUE);
                gotoxy(ball_x, ball_y - 1);
                cprintf(" ");

                textbackground(BLUE);
                if (collector_x > 1) {
                    gotoxy(collector_x - 1, collector_y);
                    cprintf(" ");
                }

                if (collector_x + COLLECTOR_WIDTH <= MAX_COLUMNS) {
                    gotoxy(collector_x + COLLECTOR_WIDTH, collector_y);
                    cprintf(" ");
                }

                textbackground(WHITE);
                for (i = 0; i < COLLECTOR_WIDTH; ++i) {
                    gotoxy(collector_x + i, collector_y);
                    cprintf(" ");
                }

                redraw = 0;
            }
        } else if (state == RULES_STATE) {
            if (key == 'b') {
                state = MENU_STATE;

                clear_color1 = BLACK;
                clear_color2 = BLACK;
            }

            if (redraw) {
                i = MAX_COLUMNS / 2 - 26;
                j = MAX_LINES / 2 - 4;

                textcolor(RED);

                gotoxy(MAX_COLUMNS / 2 - 3, j - 2);
                cprintf("REGRAS");

                textcolor(WHITE);

                gotoxy(i, j++);
                cprintf("1. Ao iniciar o jogo as teclas responsáveis por mover");
                gotoxy(i, j++);
                cprintf("   a barra (collector) serão a e b.");

                ++j;
                gotoxy(i, j++);
                cprintf("2. Quando o jogo for iniciado o objetivo será não");
                gotoxy(i, j++);
                cprintf("   deixar nenhum ítem cair no chão.");

                ++j;
                gotoxy(i, j++);
                cprintf("3. Para contabilizar os pontos será necessário guiar");
                gotoxy(i, j++);
                cprintf("   a barra (collector) até o ítem para coleta-lo.");

                gotoxy(MAX_COLUMNS / 2 - 2, j + 2);
                cprintf("<< b");

                redraw = 0;
            }
        } else if (state == GAMEOVER_STATE) {
            if (key == 'b') {
                state = MENU_STATE;

                clear_color1 = BLACK;
                clear_color2 = BLACK;

                collector_x = MAX_COLUMNS / 2 - COLLECTOR_WIDTH / 2;
                collector_points = 0;

                ball_x = 1 + rand() % MAX_COLUMNS;
                ball_y = 4;
                ball_move_y = 0;
            }

            if (redraw) {
                i = MAX_COLUMNS / 2 - 32;
                j = MAX_LINES / 2 - 3;

                textcolor(RED);

                gotoxy(i, j++);
                cprintf(" _______ _______ ___ ___ _______  _______ ___ ___ _______ _______ ");
                gotoxy(i, j++);
                cprintf("|   _   |   _   |   Y   |   _   ||   _   |   Y   |   _   |   _   \\");
                gotoxy(i, j++);
                cprintf("|.  |___|.  1   |.      |.  1___||.  |   |.  |   |.  1___|.  l   /");
                gotoxy(i, j++);
                cprintf("|.  |   |.  _   |. \\_/  |.  __)_ |.  |   |.  |   |.  __)_|.  _   1");
                gotoxy(i, j++);
                cprintf("|:  1   |:  |   |:  |   |:  1   ||:  1   |:  1   |:  1   |:  |   |");
                gotoxy(i, j++);
                cprintf("|::.. . |::.|:. |::.|:. |::.. . ||::.. . |\\:.. ./|::.. . |::.|:. |");
                gotoxy(i, j++);
                cprintf("`-------`--- ---`--- ---`-------'`-------' `---' `-------`--- ---'");

                textcolor(WHITE);

                gotoxy(MAX_COLUMNS / 2 - 5, ++j);
                cprintf("Pontos: %02d", collector_points);

                gotoxy(MAX_COLUMNS / 2 - 15, ++j + 1);
                cprintf("Precione b para voltar ao menu");

                redraw = 0;
            }
        }

        if (kbhit())
            key = getch();
        else
            key = 0;

        gotoxy(MAX_COLUMNS / 2, MAX_LINES);
    }

    return 0;
}


#ifdef __linux__

int Sleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

#endif /* __linux__ */

