#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // POSIX isletim sistemine özgü sistem çagrilari
#include <sys/types.h> // Temel veri türleri ve sistem çagrilari için gerekli tanimlamalar
#include <sys/wait.h> // Çocuk süreçlerin bitisini beklemek için
#include <sys/stat.h>  // Dosya ve dizin durumu ile ilgili fonksiyonlar
#include <fcntl.h> // Dosya kontrolü ve manipülasyonu için islevler
#include <mqueue.h> // POSIX message queue (mesaj kuyrugu) islemleri için kütüphane
#include <string.h>
#include <errno.h> // Hata numaralari için tanimlamalar
#include <time.h>  // Zaman islemleri için fonksiyonlar

#define MAX_N 5
#define MAX_K 1000

#define MAX_MSG_SIZE 256

struct Message {
    long mtype; // mesajin turu
    int data;   //veri
};

//random sayi ureten fonksiyon
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

//cocuk surec
void childProcess(const char *infile, int processNumber, int k, mqd_t parentQueue) {
    FILE *inputFile = fopen(infile, "r"); //dosya acma
    if (inputFile == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

	//bellekte yer ayir ve dosyadan sayilari oku
    int *numbers = (int *)malloc(10000 * sizeof(int));
    if (numbers == NULL) {
        perror("Error allocating memory");
        fclose(inputFile);
        exit(EXIT_FAILURE);
    }

	// dosyadan sayilari okuyup hata durumunu kontrol etme
    for (int i = 0; i < 10000; ++i) {
        if (fscanf(inputFile, "%d", &numbers[i]) != 1) {
            fprintf(stderr, "Error reading from input file\n");
            free(numbers);
            fclose(inputFile);
            exit(EXIT_FAILURE);
        }
    }

	//bubble sort ile siralama
    for (int i = 0; i < 10000 - 1; ++i) {
        for (int j = 0; j < 10000 - i - 1; ++j) {
            if (numbers[j] < numbers[j + 1]) {
                int temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }

    struct Message msg;
    msg.mtype = processNumber;
    msg.data = numbers[k - 1];

    // Ssonucu parent prosese gonderme
    if (mq_send(parentQueue, (const char *)&msg, sizeof(struct Message), 0) == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }

    free(numbers);
    fclose(inputFile);
}

int main(int argc, char *argv[]) {
    if (argc < 5 || argc > MAX_N + 4) {
        fprintf(stderr, "Usage: %s <k> <N> <infile1> ... <infileN> <outfile>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int k = atoi(argv[1]); //kacinci buyuk sayiyi bulacaksak o parametre
    int N = atoi(argv[2]); //kac dosya olacak
    
    //verilen araliklarin kontrolu
    if (k < 1 || k > MAX_K || N < 1 || N > MAX_N) {
        fprintf(stderr, "Invalid values for k or N\n");
        exit(EXIT_FAILURE);
    }
	//rastgele sayi üreteci için baslangiç zamanini kullan
    srand((unsigned int)time(NULL));

    // Parent ve child süreçler arasindaki iletisim için bir POSIX mesaj kuyrugu olusturma
    mqd_t parentQueue;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;  // kuyruktaki mesajlarin maksimum sayisi
    attr.mq_msgsize = sizeof(struct Message);
    attr.mq_curmsgs = 0;

    parentQueue = mq_open("/topk_mqueue", O_CREAT | O_RDWR, 0666, &attr);
    if (parentQueue == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

	// her bir giris dosyasi için rastgele sayilar üret
    for (int i = 0; i < N; ++i) {
        generateRandomNumbers(argv[i + 3], 10000);
    }
	
	 // cocuk süreçler olustur ve islevlerini çagir
    pid_t pid;
    for (int i = 0; i < N; ++i) {
        pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            childProcess(argv[i + 3], i + 1, k, parentQueue);
            exit(EXIT_SUCCESS);
        }
    }

	// tum çocuk süreçlerin bitmesini bekle
    int status;
    while (wait(&status) > 0);

	// mesaj kuyrugundan gelen sonuçlari çikis dosyasina yaz
    struct Message msg;
    FILE *outputFile = fopen(argv[N + 3], "w");
    
    if (outputFile == NULL) {
        perror("Error opening output file");
        mq_close(parentQueue);
        mq_unlink("/topk_mqueue");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; ++i) {
        if (mq_receive(parentQueue, (char *)&msg, sizeof(struct Message), NULL) == -1) {
            perror("mq_receive");
            fclose(outputFile);
            mq_close(parentQueue);
            mq_unlink("/topk_mqueue");
            exit(EXIT_FAILURE);
        }

        fprintf(outputFile, "%d\n", msg.data);
    }

    fclose(outputFile);

    //mesaj kuyrugunu kapat ve sil
    mq_close(parentQueue);
    mq_unlink("/topk_mqueue");

    return 0;
}

