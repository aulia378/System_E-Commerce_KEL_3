#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Produk {
    int id;
    char nama[50];
    int harga;
    int stok;
    struct Produk* next;
} Produk;

typedef struct NodeStack {
    char aktifitas[100];
    struct NodeStack* next;
} NodeStack;

typedef struct NodeQueue {
    int id_produk;
    char nama_produk[50];
    int harga;
    int jumlah;  
    char status[30];
    struct NodeQueue* next;
} NodeQueue;

extern Produk* headProduk;
extern NodeStack* topLog;
extern NodeQueue* frontCart;
extern NodeQueue* rearCart;
extern NodeQueue* frontPesanan;
extern NodeQueue* rearPesanan;

extern int idOtomatis;

void menuAdmin();
void menuProduk();
void tambahProduk();
void editProduk();
void hapusProduk();
void prosesPesanan();

void menuCustomer();
void enqueueCartFromFile(int idCari, int jumlahBeli);
void tampilCart();
void checkoutCart();
void lacakPesanan();

void clearScreen();
void pause();
void lihatProduk();
void pushLog(char aktifitas[]);
void displayLog();
int cekIDTerpakai(int id);
int getStokProduk(int idProduk);
void kurangiStokProduk(int idProduk, int jumlah);
void login();

#endif
