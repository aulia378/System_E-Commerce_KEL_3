#include "header.h"

int main() {
    int pilihan;
    do {
        clearScreen();
        printf("+===============================================+\n");
        printf("|      SELAMAT DATANG DI SISTEM E-COMMERCE      |\n");
        printf("+----------------------+------------------------+\n");
        printf("|                    1. Login                   |\n");
        printf("|                    0. Keluar                  |\n");
        printf("+----------------------+------------------------+\n");
        printf("Pilih: ");
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1: login(); break;
            case 0: printf("\nTerima kasih telah menggunakan sistem!\n"); break;
            default: printf("Pilihan tidak valid.\n"); pause(); break;
        }
    } while (pilihan != 0);
    return 0;
}
