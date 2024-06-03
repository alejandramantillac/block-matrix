#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

void multMatBlock(int n, float* A, float* B, float* C, int block_size) {
    int i, j, k, ii, jj, kk;
    for (i = 0; i < n; i += block_size) {
        for (j = 0; j < n; j += block_size) {
            for (k = 0; k < n; k += block_size) {
                for (ii = i; ii < i + block_size && ii < n; ++ii) {
                    for (jj = j; jj < j + block_size && jj < n; ++jj) {
                        float sum = 0.0f;
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

void PrintMat(int n, float* M) {
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
    errno_t err = fopen_s(&fp, "C:/lab_105/Final_Report_float.txt", "a+");
    if (err != 0) {
        printf("The file 'Final_Report_float' was not opened\n");
        return 0;
    }
   
    if (argc > 1) {
        int n = atoi(argv[1]);
        int block_size = atoi(argv[2]);
        int samples = 10;

        float* A = (float*)malloc(n * n * sizeof(float));
        float* B = (float*)malloc(n * n * sizeof(float));
        float* C = (float*)malloc(n * n * sizeof(float));
         
        for (int i = 0; i < n * n; i++) A[i] = (float)rand() / 100;
        for (int i = 0; i < n * n; i++) B[i] = (float)rand() / 100;
        for (int i = 0; i < n * n; i++) C[i] = 0.0f;

        printf("version TypeData   #sample    n    time(s)        Normalized(ns)\n");
        for (int s = 0; s < samples; s++) {
            clock_t start = clock();
            multMatBlock(n, A, B, C, block_size);
            clock_t end = clock();

            double seconds = (double)(end - start) / CLOCKS_PER_SEC;
            double N = n;
            double timeNormalized = (seconds * 1.0e9) / (N * N * N);

            printf("verBlock \t f %03d\t %05d\t  %2.4f\t   %2.4f \n", s, n, seconds, timeNormalized);
            fprintf_s(fp, "verBlock \t f %03d\t %05d  %2.4f\t   %2.4f \n", s, n, seconds, timeNormalized);
        }
        free(A);
        free(B);
        free(C);
        fclose(fp);
    }
    printf("\n");
    return 0;
}
