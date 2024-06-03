#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

void multMatBlock(int n, double* A, double* B, double* C, int block_size) {
    int i, j, k, ii, jj, kk;
    for (i = 0; i < n; i += block_size) {
        for (j = 0; j < n; j += block_size) {
            for (k = 0; k < n; k += block_size) {
                for (ii = i; ii < i + block_size && ii < n; ++ii) {
                    for (jj = j; jj < j + block_size && jj < n; ++jj) {
                        double sum = 0.0;
                        for (kk = k; kk < k + block_size && kk < n; ++kk) {
                            sum += A[ii + kk * n] * B[kk + jj * n];
                        }
                        C[ii + jj * n] += sum;
                    }
                }
            }
        }
    }
}

void PrintMat(int n, double* M) {
    int i, j;
    for (j = 0; j < n; j++) {
        for (i = 0; i < n; i++)
            printf("%.3f ", M[i + j * n]);
        printf("; \n");
    }
    printf("\n\n");
}

int main(int argc, char *argv[]) {
    FILE *fp;
    errno_t err = fopen_s(&fp, "C:/lab_105/Final_Report_double.txt", "a+");
    if (err != 0) {
        printf("The file 'Final_Report_double' was not opened\n");
        return 0;
    }
   
    if (argc > 1) {
        int n = atoi(argv[1]);
        int block_size = atoi(argv[2]);
        int samples = 10;

        double* A = (double*)malloc(n * n * sizeof(double));
        double* B = (double*)malloc(n * n * sizeof(double));
        double* C = (double*)malloc(n * n * sizeof(double));
         
        for (int i = 0; i < n * n; i++) A[i] = (double)rand() / 100;
        for (int i = 0; i < n * n; i++) B[i] = (double)rand() / 100;
        for (int i = 0; i < n * n; i++) C[i] = 0.0;

        printf("version TypeData   #sample    n    time(s)        Normalized(ns)\n");
        for (int s = 0; s < samples; s++) {
            clock_t start = clock();
            multMatBlock(n, A, B, C, block_size);
            clock_t end = clock();

            double seconds = (double)(end - start) / CLOCKS_PER_SEC;
            double N = n;
            double timeNormalized = (seconds * 1.0e9) / (N * N * N);

            printf("verBlock \t d %03d\t %05d\t  %2.4f\t   %2.4f \n", s, n, seconds, timeNormalized);
            fprintf_s(fp, "verBlock \t d %03d\t %05d  %2.4f\t   %2.4f \n", s, n, seconds, timeNormalized);
        }
        free(A);
        free(B);
        free(C);
        fclose(fp);
    }
    printf("\n");
    return 0;
}
