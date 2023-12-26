#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>// Zaman islemleri için fonksiyonlar

#define MAX_N 5
#define MAX_K 1000

struct ThreadData {
    const char *infile;
    int processNumber;
    int k;
    int result;
};

void generateRandomNumbers(const char *filename, int count) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; ++i) {
        fprintf(file, "%d ", rand() % 100000 + 1);
    }

    fclose(file);
}

void *threadProcess(void *arg) {
    struct ThreadData *data = (struct ThreadData *)arg;

    FILE *inputFile = fopen(data->infile, "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    int *numbers = (int *)malloc(10000 * sizeof(int));
    if (numbers == NULL) {
        perror("Error allocating memory");
        fclose(inputFile);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 10000; ++i) {
        if (fscanf(inputFile, "%d", &numbers[i]) != 1) {
            fprintf(stderr, "Error reading from input file\n");
            free(numbers);
            fclose(inputFile);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < 10000 - 1; ++i) {
        for (int j = 0; j < 10000 - i - 1; ++j) {
            if (numbers[j] < numbers[j + 1]) {
                int temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }

    // Find the k-th largest number
    data->result = numbers[data->k - 1];

    free(numbers);
    fclose(inputFile);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 5 || argc > MAX_N + 4) {
        fprintf(stderr, "Usage: %s <k> <N> <infile1> ... <infileN> <outfile>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int k = atoi(argv[1]);
    int N = atoi(argv[2]);

    if (k < 1 || k > MAX_K || N < 1 || N > MAX_N) {
        fprintf(stderr, "Invalid values for k or N\n");
        exit(EXIT_FAILURE);
    }

    srand((unsigned int)time(NULL));

    pthread_t threads[MAX_N];
    struct ThreadData threadData[MAX_N];

    for (int i = 0; i < N; ++i) {
        generateRandomNumbers(argv[i + 3], 10000);
    }

    for (int i = 0; i < N; ++i) {
        threadData[i].infile = argv[i + 3];
        threadData[i].processNumber = i + 1;
        threadData[i].k = k;

        if (pthread_create(&threads[i], NULL, threadProcess, (void *)&threadData[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < N; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    FILE *outputFile = fopen(argv[N + 3], "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; ++i) {
        fprintf(outputFile, "%d\n", threadData[i].result);
    }

    fclose(outputFile);

    return 0;
}

