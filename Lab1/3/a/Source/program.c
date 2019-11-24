// Program that generates two random 3x3 matrices and multiplies them
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    // define variables
    int A[3][3], B[3][3], C[3][3], sum;
    int i, o, j, k;

    // generate two random matrices A and B (elements have values 0-14)
    srand(time(NULL));
    for(o = 0; o < 3; o++)
        for(i = 0; i < 3; i++) {
            A[o][i] = rand()%15;
            B[o][i] = rand()%15;
        }
    // multiply A and B
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            sum = 0;
            for (k = 0; k < 3; k++) {
                sum = sum + A[i][k] * B[k][j];
            }
            C[i][j] = sum;
         }
    }

    // print the results
    printf("A matrix is: \n");
    for (i = 0; i < 3; i++) {
      for (j = 0; j < 3; j++)
        printf("%d ", A[i][j]);
      printf("\n");
    }

    printf("\n\nB matrix is: \n");
    for (i = 0; i < 3; i++) {
      for (j = 0; j < 3; j++)
        printf("%d ", B[i][j]);
      printf("\n");
    }

    printf("\n\nC matrix is: \n");
    for (i = 0; i < 3; i++) {
      for (j = 0; j < 3; j++)
        printf("%d ", C[i][j]);
      printf("\n");
    }


    return 0;
}
