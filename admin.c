#include "header.h"

void tambahProduk() {
    FILE* f = fopen("produk.txt", "a");
    if (!f) {
        printf("Gagal membuka file produk.txt\n");
        pause();
        return;
    }

    int id, harga, stok;
    char nama[50];
    char idStr[20];

    while (1) {
        printf("ID Produk   : ");
        scanf("%s", idStr);

        int valid = 1;
        for (int i = 0; idStr[i] != '\0'; i++) {
            if (!isdigit(idStr[i])) {
                valid = 0;
                break;
            }
        }

        if (valid) {
            id = atoi(idStr);
            if (id > 0) {
                if (!cekIDTerpakai(id)) {
                    break;
                } else {
                    printf("ID sudah digunakan. Masukkan ID lain.\n");
                    continue;
                }
            }
        }
        printf("ID harus berupa angka positif! Coba lagi.\n");
    }

    getchar(); 

    printf("Nama Produk : ");
    fgets(nama, sizeof(nama), stdin);
    nama[strcspn(nama, "\n")] = 0;  

    while (1) {
        printf("Harga Produk: ");
        if (scanf("%d", &harga) != 1 || harga < 0) {
            printf("Harga harus berupa angka dan tidak negatif!\n");
            while (getchar() != '\n'); 
        } else {
            break;
        }
    }

    while (1) {
        printf("Stok Produk : ");
        if (scanf("%d", &stok) != 1 || stok < 0) {
            printf("Stok harus berupa angka dan tidak negatif!\n");
            while (getchar() != '\n'); 
        } else {
            break;
        }
    }

    fprintf(f, "%d;%s;%d;%d\n", id, nama, harga, stok);
    fclose(f);

    char logMsg[100];
    sprintf(logMsg, "Tambah Produk: %s (ID: %d, Stok: %d)", nama, id, stok);
    pushLog(logMsg);

    printf("\nProduk berhasil ditambahkan ke file!\n");
    pause();
}


void editProduk() {
    FILE* f = fopen("produk.txt", "r");
    if (!f) {
        printf("File produk.txt tidak ditemukan.\n");
        pause();
        return;
    }

    FILE* temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Gagal membuka file sementara.\n");
        fclose(f);
        pause();
        return;
    }

    int id, harga, stok, editId, ditemukan = 0;
    char nama[50], newNama[50];
    printf("\nMasukkan ID Produk yang ingin diedit: ");
    scanf("%d", &editId);
    getchar();

    while (fscanf(f, "%d;%[^;];%d;%d\n", &id, nama, &harga, &stok) != EOF) {
        if (id == editId) {
            ditemukan = 1;
            printf("Edit Nama (%s): ", nama);
            fgets(newNama, 50, stdin);
            newNama[strcspn(newNama, "\n")] = 0;

            printf("Edit Harga (%d): ", harga);
            scanf("%d", &harga);

            printf("Edit Stok (%d): ", stok);
            scanf("%d", &stok);

            fprintf(temp, "%d;%s;%d;%d\n", id, newNama, harga, stok);
            char logMsg[100];
            sprintf(logMsg, "Edit Produk ID %d", id);
            pushLog(logMsg);
        } else {
            fprintf(temp, "%d;%s;%d;%d\n", id, nama, harga, stok);
        }
    }

    fclose(f);
    fclose(temp);
    remove("produk.txt");
    rename("temp.txt", "produk.txt");

    if (ditemukan)
        printf("\nProduk berhasil diedit.\n");
    else
        printf("\nID tidak ditemukan.\n");
    
    pause();
}

void hapusProduk() {
    FILE* f = fopen("produk.txt", "r");
    if (!f) {
        printf("File produk.txt tidak ditemukan.\n");
        pause();
        return;
    }

    FILE* temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Gagal membuka file sementara.\n");
        fclose(f);
        pause();
        return;
    }

    int id, harga, stok, hapusId, ditemukan = 0;
    char nama[50];
    printf("\nMasukkan ID Produk yang ingin dihapus: ");
    scanf("%d", &hapusId);

    while (fscanf(f, "%d;%[^;];%d;%d\n", &id, nama, &harga, &stok) != EOF) {
        if (id == hapusId) {
            ditemukan = 1;
            char logMsg[100];
            sprintf(logMsg, "Hapus Produk: %s (ID: %d)", nama, id);
            pushLog(logMsg);
        } else {
            fprintf(temp, "%d;%s;%d;%d\n", id, nama, harga, stok);
        }
    }

    fclose(f);
    fclose(temp);
    remove("produk.txt");
    rename("temp.txt", "produk.txt");

    if (ditemukan)
        printf("\nProduk berhasil dihapus.\n");
    else
        printf("\nID tidak ditemukan.\n");

    pause();
}



void prosesPesanan() {
    if (!frontPesanan) {
        printf("\nTidak ada pesanan yang diproses.\n");
        return;
    }

    printf("\n=== Proses Pesanan ===\n");
    NodeQueue* temp = frontPesanan;
    printf("ID Produk : %d\n", temp->id_produk);
    printf("Nama Produk : %s\n", temp->nama_produk);
    printf("Status Sebelumnya: %s\n", temp->status);

    printf("\nUpdate Status:\n");
    printf("1. Sedang Diproses\n");
    printf("2. Dikirim\n");
    printf("3. Selesai\n");
    printf("Pilih Status (1/2/3): ");
    int pilih;
    scanf("%d", &pilih);

    switch (pilih) {
        case 1: strcpy(temp->status, "Sedang Diproses"); break;
        case 2: strcpy(temp->status, "Dikirim"); break;
        case 3: strcpy(temp->status, "Selesai"); break;
        default: printf("Pilihan tidak valid.\n"); return;
    }

    printf("\nStatus berhasil diperbarui ke '%s'.\n", temp->status);

    char logMsg[100];
    sprintf(logMsg, "Proses Pesanan: %s (Status: %s)", temp->nama_produk, temp->status);
    pushLog(logMsg);

    if (pilih == 3) {
        NodeQueue* hapus = frontPesanan;
        frontPesanan = frontPesanan->next;
        free(hapus);
        if (!frontPesanan) rearPesanan = NULL;
    }
}

void menuProduk() {
    int pilihan;
    do {
        printf("+---------------------------------------+\n");
        printf("|            MENU PRODUK                |\n");
        printf("+----+----------------------------------+\n");
        printf("| 1  | Tambah Produk                    |\n");
        printf("| 2  | Edit Produk                      |\n");
        printf("| 3  | Hapus Produk                     |\n");
        printf("| 0  | Keluar                           |\n");
        printf("+----+----------------------------------+\n");
        printf("Pilih menu : ");
        scanf("%d", &pilihan);
        getchar(); 

        switch (pilihan) {
            case 1: tambahProduk(); break;
            case 2: editProduk(); break;
            case 3: hapusProduk(); break;
            case 0: break;
            default: printf("Pilihan tidak valid.\n"); pause(); break;
        }
    } while (pilihan != 0);
}

void menuAdmin() {
    int pilihan;
    do {
        printf("+---------------------------------------+\n");
        printf("|              MENU ADMIN               |\n");
        printf("+----+----------------------------------+\n");
        printf("| 1  | Menu Produk                      |\n");
        printf("| 2  | Lihat Produk                     |\n");
        printf("| 3  | Riwayat Sistem                   |\n");
        printf("| 4  | Proses Pesanan                   |\n");
        printf("| 0  | Logout                           |\n");
        printf("+----+----------------------------------+\n");
        printf("Pilih menu : ");
        scanf("%d", &pilihan);
        getchar(); 

        switch (pilihan) {
            case 1: menuProduk(); break;
            case 2: lihatProduk(); pause(); break;
            case 3: displayLog(); pause(); break;
            case 4: prosesPesanan(); pause(); break;
            case 0: printf("Logout...\n"); break;
            default: printf("Pilihan tidak valid.\n"); pause(); break;
        }
    } while (pilihan != 0);
}
