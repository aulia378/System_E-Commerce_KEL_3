#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void enqueueCartFromFile(int idCari, int jumlahBeli) {
    FILE* f = fopen("produk.txt", "r");
    if (!f) {
        printf("Gagal membuka file produk.txt\n");
        return;
    }

    char line[100];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        int id, harga, stok;
        char nama[50];
        sscanf(line, "%d;%49[^;];%d;%d", &id, nama, &harga, &stok);
        if (id == idCari) {
            found = 1;
            if (stok < jumlahBeli) {
                printf("\nStok produk '%s' tidak mencukupi. Stok tersedia: %d\n", nama, stok);
                break;
            }

            NodeQueue* baru = (NodeQueue*)malloc(sizeof(NodeQueue));
            baru->id_produk = id;
            strcpy(baru->nama_produk, nama);
            baru->harga = harga;
            baru->jumlah = jumlahBeli;
            strcpy(baru->status, "Di Keranjang");
            baru->next = NULL;

            if (rearCart == NULL) {
                frontCart = rearCart = baru;
            } else {
                rearCart->next = baru;
                rearCart = baru;
            }

            printf("\nProduk '%s' sebanyak %d berhasil ditambahkan ke Cart!\n", nama, jumlahBeli);
            break;
        }
    }

    if (!found) {
        printf("\nProduk dengan ID %d tidak ditemukan.\n", idCari);
    }

    fclose(f);
}

void tampilCart() {
    NodeQueue* temp = frontCart;
    printf("\n=== Isi Cart ===\n");
    if (!temp) {
        printf("Cart kosong.\n");
        return;
    }

    printf("+----+----------------------------+---------+--------+\n");
    printf("| ID | Nama Produk                 | Harga   | stok  |\n");
    printf("+----+----------------------------+---------+--------+\n");
    while (temp) {
        printf("| %-2d | %-26s | Rp%-6d | %-6d |\n",
               temp->id_produk, temp->nama_produk, temp->harga, temp->jumlah);
        temp = temp->next;
    }
    printf("+----+----------------------------+---------+--------+\n");
}

void checkoutCart() {
    if (!frontCart) {
        printf("\nCart kosong, tidak bisa checkout.\n");
        return;
    }

    NodeQueue* temp = frontCart;
    int processedIds[100]; 
    int processedCount = 0; 
  

    while (temp) {
        int alreadyProcessed = 0;

        for (int i = 0; i < processedCount; i++) {
            if (processedIds[i] == temp->id_produk) {
                alreadyProcessed = 1;
                break;
            }
        }

        if (!alreadyProcessed) {
            kurangiStokProduk(temp->id_produk, temp->jumlah);

            NodeQueue* baru = (NodeQueue*)malloc(sizeof(NodeQueue));
            baru->id_produk = temp->id_produk;
            strcpy(baru->nama_produk, temp->nama_produk);
            baru->harga = temp->harga;
            baru->jumlah = temp->jumlah;
            strcpy(baru->status, "Sedang Diproses");
            baru->next = NULL;

            if (rearPesanan == NULL) {
                frontPesanan = rearPesanan = baru;
            } else {
                rearPesanan->next = baru;
                rearPesanan = baru;
            }
            processedIds[processedCount++] = temp->id_produk;
        }

        temp = temp->next;
    }

    printf("\nCheckout berhasil! Pesanan masuk antrian.\n");
  
    while (frontCart) {
        NodeQueue* hapus = frontCart;
        frontCart = frontCart->next;
        free(hapus);
    }
    rearCart = NULL;

    pause();
}

void lacakPesanan() {
    NodeQueue* temp = frontPesanan;
    printf("\n=== Lacak Pesanan ===\n");
    if (!temp) {
        printf("Tidak ada pesanan dalam antrian.\n");
        return;
    }
    printf("+----+----------------------------+---------+------------------+\n");
    printf("| ID | Nama Produk                 | Harga   | Status           |\n");
    printf("+----+----------------------------+---------+------------------+\n");
    while (temp) {
        printf("| %-2d | %-26s | Rp%-6d | %-16s |\n", temp->id_produk, temp->nama_produk, temp->harga, temp->status);
        temp = temp->next;
    }
    printf("+----+----------------------------+---------+------------------+\n");
}

void menuCustomer() {
    int pilihan;
    do {
        printf("+---------------------------------------+\n");
        printf("|            MENU Costumer              |\n");
        printf("+----+----------------------------------+\n");
        printf("| 1  | Lihat Produk                     |\n");
        printf("| 2  | Tambah ke Cart                   |\n");
        printf("| 3  | Lihat Cart                       |\n");
        printf("| 4  | Checkout                         |\n");
        printf("| 5  | Lacak Pesanan                    |\n");
        printf("| 0  | Logout                           |\n");
        printf("+----+----------------------------------+\n");
        printf("Pilih menu : ");
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1: lihatProduk(); pause(); break;
            case 2: {
                lihatProduk(); 
                printf("Masukkan ID Produk yang ingin dibeli: ");
                int idCari;
                scanf("%d", &idCari);
            
                printf("Masukkan jumlah yang ingin dibeli: ");
                int jumlahBeli;
                scanf("%d", &jumlahBeli);
            
                enqueueCartFromFile(idCari, jumlahBeli);
                pause();
                break;
            }            
            case 3: tampilCart(); pause(); break;
            case 4: checkoutCart(); pause(); break;
            case 5: lacakPesanan(); pause(); break;
            case 0: printf("Logout...\n"); break;
            default: printf("Pilihan tidak valid.\n"); pause(); break;
        }
    } while (pilihan != 0);
}
