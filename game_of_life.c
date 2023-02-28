#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define X 80
#define Y 25
#define SPEED_UP 'w'
#define SPEED_DOWN 's'
#define LIVE '*'
#define DEAD ' '
#define STOP 'x'

void output(char **matrix, int y, int x);
void input(char **matrix, int y, int x);
void evolution(char **matrix, int y, int x);
void copy(char **matrix, char **help_matrix, int y, int x);
void live_or_not(char **matrix, char **matrix_help, int i, int j);
int count_live(char **matrix_help, int y_, int x_);
int read_file(char **matrix);

int main() {
    int eror = 0;
    char **p_matrix = (char **)malloc((Y - 2) * sizeof(char *));
    for (int i = 0; i < Y - 2; i++) {
        p_matrix[i] = (char *)malloc((X - 2) * sizeof(char));
    }
    input(p_matrix, (Y - 2), (X - 2));
    initscr();
    noecho();
    curs_set(0);
    printw("Write number: ");
    refresh();
    eror = read_file(p_matrix);
    if (eror == 1) {
        int speed = 10;
        while (1) {
            char c;
            clear();
            halfdelay(speed);
            output(p_matrix, Y, X);
            evolution(p_matrix, (Y - 2), (X - 2));
            refresh();
            c = getch();
            switch (c) {
                case SPEED_UP:
                    speed -= 1;
                    break;
                case SPEED_DOWN:
                    speed += 1;
                    break;
            }
            if (c == STOP) {
                break;
            }
            halfdelay(speed);
            refresh();
        }
    }
    for (int i = 0; i < Y - 2; i++) {
        free(p_matrix[i]);
    }
    free(p_matrix);
    endwin();
    return 0;
}

void output(char **matrix, int y, int x) {  // ouput our matrix
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            if (i != 0 && j != 0 && j != x - 1 && i != y - 1) {
                printw("%c", matrix[i - 1][j - 1]);
            } else {
                printw(".");
            }
        }
        printw("\n");
    }
    printw("                            TO EXITE PRESS X");
}

void input(char **matrix, int y, int x) {  // input out matrix
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            matrix[i][j] = DEAD;
        }
    }
}

void evolution(char **matrix, int y, int x) {         // main function of the logic of game
    char **matrix_help = malloc(y * sizeof(char *));  // create new matrix
    for (int i = 0; i < y; i++) {
        matrix_help[i] = malloc(x * sizeof(char));
    }
    copy(matrix, matrix_help, y, x);  // copy our matrix to new matrix
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            live_or_not(matrix, matrix_help, i, j);  // check is this element live or not
        }
    }
    for (int i = 0; i < y; i++) {  // delete new matrix
        free(matrix_help[i]);
    }
    free(matrix_help);
}

void copy(char **matrix, char **matrix_help, int y, int x) {  // create copy of our matrix
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            matrix_help[i][j] = matrix[i][j];
        }
    }
}

void live_or_not(char **matrix, char **matrix_help, int i, int j) {
    if (matrix_help[i][j] == DEAD) {
        if (count_live(matrix_help, i, j) == 3) {
            matrix[i][j] = LIVE;
        }
    } else {
        if (count_live(matrix_help, i, j) != 2 && count_live(matrix_help, i, j) != 3) {
            matrix[i][j] = DEAD;
        }
    }
}

int count_live(char **matrix_help, int y_, int x_) {  // function counts live elements near
    int count = 0;
    for (int i = y_ - 1; i <= y_ + 1; i++) {
        int b = i;
        i = i < 0 ? Y - 3 : i;
        i = i >= Y - 2 ? 0 : i;
        for (int j = x_ - 1; j <= x_ + 1; j++) {
            int a = j;
            j = j < 0 ? X - 3 : j;
            j = j >= X - 2 ? 0 : j;
            if (i == y_ && j == x_) continue;
            if (matrix_help[i][j] == LIVE) {
                count++;
            }
            j = a;
        }
        i = b;
    }
    return count;
}

int read_file(char **matrix) {  // function for read files
    FILE *file;
    file = fopen("1.txt", "r");
    int i, j, check = 1;
    char c;
    i = scanw("%d", &j);
    if (i == 1) {
        switch (j) {
            case (1):
                file = fopen("1.txt", "r");
                break;
            case (2):
                file = fopen("2.txt", "r");
                break;
            case (3):
                file = fopen("3.txt", "r");
                break;
            case (4):
                file = fopen("4.txt", "r");
                break;
            case (5):
                file = fopen("5.txt", "r");
                break;
            case (6):
                file = fopen("6.txt", "r");
                break;
            default:
                check = 0;
                break;
        }
        if (check == 1) {
            refresh();
            while (c != '\n') {
                fscanf(file, "%d %d%c", &i, &j, &c);
                matrix[i][j] = LIVE;
            }
        }
    }
    fclose(file);
    return check;
}
