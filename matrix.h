#include <stdlib.h>
#include <time.h>


typedef struct {
    int row;
    int column;
    int** matrix;
} Mat;

int check_around(Mat* m, int row_index, int column_index);
Mat* game_of_life(Mat* mat);

int check_around(Mat* m, int row_index, int column_index) {
    int n_live = 0;

    for (int i = row_index - 1; i <= row_index + 1; ++i) {
        for (int j = column_index - 1; j <= column_index + 1; ++j) {
            if (i != row_index || j != column_index) {
                if (i == -1 || j == -1 || i == m -> row || j == m -> column) {continue;}
                if (m -> matrix[i][j] == 1) {n_live += 1;}
            }
        }
    }
    return n_live;
}

Mat* game_of_life(Mat* mat) {
    Mat* next_g = (Mat*) malloc(sizeof(Mat));
    next_g -> matrix = (int**) malloc(mat -> row * sizeof(int*));
    next_g -> row = mat -> row;
    next_g -> column = mat -> column;
    int live = 1;
    int dead = 0;

    for (int i = 0; i < mat -> row; ++i) {
        next_g -> matrix[i] = (int*) calloc(mat -> column, sizeof(int));
        for (int j = 0; j < mat -> column; ++j) {
            int n_live = check_around(mat, i, j);
            
            if (mat -> matrix[i][j] == live) {
                if (n_live == 2 || n_live == 3) {
                    next_g -> matrix[i][j] = live;
                    continue;
                } 
            } else {
                if (n_live == 3) {
                    next_g -> matrix[i][j] = live;
                    continue;
                }
            }

            next_g -> matrix[i][j] = dead;
        }
    }

    return next_g;
}


Mat* get_random_matrix(int row, int column) {
    Mat* matrix = (Mat*) malloc(sizeof(Mat*));
    matrix -> matrix = (int**) malloc(row * sizeof(int*));
    matrix -> row = row;
    matrix -> column = column;
    srand(time(NULL));
    
    for (int i = 0; i < row; ++i) {
        matrix -> matrix[i] = (int*) calloc(column, sizeof(int));
        for (int j = 0; j < column; ++j) {
            matrix -> matrix[i][j] = rand() % 2;
        }
    }

    return matrix;
}