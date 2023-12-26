#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> // Çocuk süreçlerin bitisini beklemek için
#include <time.h> // Zaman islemleri için fonksiyonlar

//Programdaki sabit degerleri tanimlar.
#define MAX_N 5
#define MAX_K 1000


//input dosyalarina 1-100000 araliginda 10000 tane rastgele sayi yazan fonksiyon
void generateRandomNumbers(const char *filename, int count) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");//hata tespiti
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; ++i) {
        fprintf(file, "%d ", rand() % 100000 + 1);
    }

    fclose(file);
}

void childProcess(const char *infile, int processNumber, int k) {
    char intermediateFilename[20];
    snprintf(intermediateFilename, sizeof(intermediateFilename), "intermediate%d.txt", processNumber);

    FILE *inputFile = fopen(infile, "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

//son output dosyasi
    FILE *outputFile = fopen(intermediateFilename, "a");
    if (outputFile == NULL) {
        perror("Error opening output file");
        fclose(inputFile);
        exit(EXIT_FAILURE);
    }

    int *numbers = (int *)malloc(10000 * sizeof(int));
    if (numbers == NULL) {
        perror("Error allocating memory");
        fclose(inputFile);
        fclose(outputFile);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 10000; ++i) {
    	//sayiyi okuyup dosyaya atar
        if (fscanf(inputFile, "%d", &numbers[i]) != 1) {
            fprintf(stderr, "Error reading from input file\n");
            free(numbers);
            fclose(inputFile);
            fclose(outputFile);
            exit(EXIT_FAILURE);
        }
    }

	 // En büyük k. sayiyi bulmak için bubble sort algoritmasi kullanildi	
    for (int i = 0; i < 10000 - 1; ++i) {
        for (int j = 0; j < 10000 - i - 1; ++j) {
            if (numbers[j] < numbers[j + 1]) {
                int temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }

    // En buyuk k. sayi output dosyasina yazdirildi
    fprintf(outputFile, "%d\n", numbers[k - 1]);

    free(numbers);
    fclose(inputFile);
    fclose(outputFile);
}

//child sureclerinin olusturdugu ara dosyalari birlestiren fonksiyon
void mergeFiles(int n, const char *outfile) {
    FILE *outputFile = fopen(outfile, "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; ++i) {
        char intermediateFilename[20];
        snprintf(intermediateFilename, sizeof(intermediateFilename), "intermediate%d.txt", i + 1);

        FILE *intermediateFile = fopen(intermediateFilename, "r");
        if (intermediateFile == NULL) {
            perror("Error opening intermediate file");
            fclose(outputFile);
            exit(EXIT_FAILURE);
        }

        int number;
        
        //ara dosyalardaki sayilari output dosyasina yaziyoruz
        while (fscanf(intermediateFile, "%d", &number) == 1) {
            fprintf(outputFile, "%d\n", number);
        }

        fclose(intermediateFile);
    }

    fclose(outputFile);
}

int main(int argc, char *argv[]) {
	
	//minimum arguman sayisi kontrol ediliyor, programi cagirma seklimize uygun
    if (argc < 5 || argc > MAX_N + 4) {
        fprintf(stderr, "Usage: %s <k> <N> <infile1> ... <infileN> <outfile>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int k = atoi(argv[1]); //k degeri
    int N = atoi(argv[2]); //N degeri


	//bu blokda k ve N nin degerleri kontrol ediliyor
    if (k < 1 || k > MAX_K || N < 1 || N > MAX_N) {
        fprintf(stderr, "Invalid values for k or N\n");
        exit(EXIT_FAILURE);
    }  

    srand((unsigned int)time(NULL));

    for (int i = 0; i < N; ++i) {
        generateRandomNumbers(argv[i + 3], 10000); //i+3 giris dosyasinin program cagirilmasindaki sirasi, input dosyasna 10000 sayi yazdiriliyor
    }

    // child processler olusturuluyor 
    pid_t pid;
    for (int i = 0; i < N; ++i) {
        pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {  // cocuk prosesdir
            childProcess(argv[i + 3], i + 1, k);
            exit(EXIT_SUCCESS);
        }
    }

    // tum child proceslerin bitmesini bekliyoruz.
    int status;
    while (wait(&status) > 0);

    // ara dosyalari birlestiriyoruz
    mergeFiles(N, argv[N + 3]);

    // ara dosyalari kaldiriyoruz
    for (int i = 0; i < N; ++i) {
        char intermediateFilename[20];
        snprintf(intermediateFilename, sizeof(intermediateFilename), "intermediate%d.txt", i + 1);
        remove(intermediateFilename);
    }

    return 0;
}

