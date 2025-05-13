#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Produk* headProduk = NULL;
NodeStack* topLog = NULL;
NodeQueue* frontCart = NULL;
NodeQueue* rearCart = NULL;
NodeQueue* frontPesanan = NULL;
NodeQueue* rearPesanan = NULL;

int idOtomatis = 1;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    }

void pause() {
    printf("\nTekan Enter untuk melanjutkan...");
    while (getchar() != '\n'); // flush newline
    getchar(); // tunggu Enter
}


void pushLog(char aktifitas[]) {
    NodeStack* newLog = (NodeStack*)malloc(sizeof(NodeStack));
    strcpy(newLog->aktifitas, aktifitas);
    newLog->next = topLog;
    topLog = newLog;
}

void displayLog() {
    printf("+-----+---------------------------------------------+\n");
    printf("| %-3s | %-43s |\n", "No", "Aktifitas Admin");
    printf("+-----+---------------------------------------------+\n");

    NodeStack* current = topLog;
    int no = 1;
    while (current != NULL) {
        printf("| %-3d | %-43s |\n", no++, current->aktifitas);
        current = current->next;
    }

    if (no == 1) {
        printf("| %-3s | %-43s |\n", "-", "Belum ada aktifitas");
    }

    printf("+-----+---------------------------------------------+\n");
}

// Fungsi untuk cek apakah ID sudah pernah dipakai
int cekIDTerpakai(int id) {
    FILE* f = fopen("produk.txt", "r");
    if (!f) return 0;  // Jika file belum ada, ID dianggap belum dipakai

    int idBaca;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), f)) {
        sscanf(buffer, "%d", &idBaca);
        if (idBaca == id) {
            fclose(f);
            return 1;  // ID ditemukan, sudah dipakai
        }
    }
    fclose(f);
    return 0;  // ID belum dipakai
}


int getStokProduk(int idProduk) {
    FILE* f = fopen("produk.txt", "r");
    if (!f) return -1;

    char line[100];
    while (fgets(line, sizeof(line), f)) {
        int id, harga, stok;
        char nama[50];
        sscanf(line, "%d;%49[^;];%d;%d", &id, nama, &harga, &stok);
        if (id == idProduk) {
            fclose(f);
            return stok;
        }
    }

    fclose(f);
    return -1; // tidak ditemukan
}

void lihatProduk() {
    FILE *file = fopen("produk.txt", "r");
    if (!file) {
        printf("Gagal membuka file produk.txt\n");
        return;
    }
    
    // Baca produk ke dalam array
    Produk list[100];
    int count = 0;

    // Baca data produk dari file
    while (fscanf(file, "%d;%49[^;];%d;%d\n", &list[count].id, list[count].nama, &list[count].harga, &list[count].stok) == 4) {
        count++;
    }
    fclose(file);

    // Urutkan dari harga terbesar ke terkecil
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (list[i].harga < list[j].harga) {
                Produk temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }
    
    printf("+-----+------------------------------+--------------+-------+\n");
    printf("| ID  | Nama                         | Harga        | Stok  |\n");
    printf("+-----+------------------------------+--------------+-------+\n");
    for (int i = 0; i < count; i++) {
        printf("| %-3d | %-28s | Rp%-10d | %-5d |\n", list[i].id, list[i].nama, list[i].harga, list[i].stok);
    }
    printf("+-----+------------------------------+--------------+-------+\n");
}

void kurangiStokProduk(int idProduk, int jumlah) {
    FILE* f = fopen("produk.txt", "r");
    FILE* temp = fopen("temp.txt", "w");
    if (!f || !temp) {
        printf("Gagal membuka file saat mengurangi stok.\n");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), f)) {
        int id, harga, stok;
        char nama[50];
        sscanf(line, "%d;%49[^;];%d;%d", &id, nama, &harga, &stok);
        if (id == idProduk) {
            stok -= jumlah;
            if (stok < 0) stok = 0;
        }
        fprintf(temp, "%d;%s;%d;%d\n", id, nama, harga, stok);
    }

    fclose(f);
    fclose(temp);
    remove("produk.txt");
    rename("temp.txt", "produk.txt");
}
void login() {
    char username[50], password[50];
    char u[50], p[50], role[20];
    FILE *f = fopen("akun.txt", "r");

    if (!f) {
        printf("Gagal membuka file akun.txt\n");
        pause();
        return;
    }
    printf("\n=== LOGIN ===\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password); 
    
    while (fscanf(f, "%[^;];%[^;];%s\n", u, p, role) != EOF) {
        if (strcmp(username, u) == 0 && strcmp(password, p) == 0) {
            fclose(f);
            if (strcmp(role, "admin") == 0) {
                printf("\nLogin sebagai Admin.\n");
                pause();
                menuAdmin();
            } else {
                printf("\nLogin sebagai Customer.\n");
                pause();
                menuCustomer();
            }
            return;
        }
    }

    fclose(f);
    printf("\nLogin gagal. Username atau password salah.\n");
    pause();
}
