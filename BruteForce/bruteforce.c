/*EL2208 Praktikum Pemecahan Masalah dengan C 2023/2024
*Modul            : Tugas Besar
*Batas Pengumpulan: Selasa, 21 Mei 2024
*Pembuat kode     : Naufal Afiq Muzaffar (13222025)
*Kelompok         : B2
*Pemilihan soal   : Soal 2
*Algoritma        : Algoritma Brute Force (Exhaustive Search)
*Deskripsi        : Program untuk mencari rute terpendek untuk traveling salesman menggunakan algoritma brute force
                    yang mana mencari seluruh kemungkinan rute yang dapat dilalui dan mendapatkan jarak terpendeknya

/* Perintah Kompilasi  
    gcc bruteforce.c -o bruteforce
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define PI 3.14159265358979323846

// Struct untuk menyimpan data kota
typedef struct DataKota{
    char Kota[20];
    int indeks;
    double latitude;
    double longitude;
} DataKota;

// Menghapus Kota dari struct
DataKota* HapusKota(DataKota* Kota, int baris, char NamaKota[]) {
    struct DataKota* temp = malloc(sizeof(DataKota) * baris-1);
    int indeks=0;
    for (int i = 0; i < baris; i++) {
        if (strcmp(Kota[i].Kota, NamaKota) != 0) {
            temp[indeks] = Kota[i];
            indeks++;
        }
    }
    return temp;
}

// Algoritma Brute Force
void BruteForce(struct DataKota* Kota, int BanyakKota, double* jarakTerpendek, double * jarakTerjauh,
               char (*ruteTerpendek), char (*ruteTerjauh), char hasil[100], char KotaAwal[20],int indeksAwal,
               int indeksNext, double jarak, double *MatriksKota[], char printRute[6]) {

    if (BanyakKota == 0) {    // Kasus basis jika sudah tidak ada kota yang bisa dikunjungi
        jarak = jarak + MatriksKota[indeksAwal][indeksNext]; //Menambahkan jarak kembali ke kota awal
        char HasilAkhir[300]="";
        strcat(HasilAkhir,KotaAwal);
        strcat(HasilAkhir,hasil);
        strcat(HasilAkhir,"->");
        strcat(HasilAkhir,KotaAwal);
        if (jarak < *jarakTerpendek) { 
            *jarakTerpendek = jarak; //Menyimpan jarak terpendek ke main
            strcpy(ruteTerpendek,HasilAkhir); //Menyimpan rute terpendek ke main
        }
        if (jarak > *jarakTerjauh) { 
            *jarakTerjauh = jarak; //Menyimpan jarak terpendek ke main
            strcpy(ruteTerjauh,HasilAkhir); //Menyimpan rute terpendek ke main
        }
        if (strcmp(printRute,"ya")==0 || strcmp(printRute,"Ya")==0 || strcmp(printRute,"YA")==0){
            printf("%s%s->%s %lf km\n",KotaAwal,hasil,KotaAwal,jarak); //Menampilkan rute dan jarak
        }
    }

    // Kasus rekursif jika masih ada kota yang bisa dikunjungi
    for (int i = 0; i < BanyakKota; i++) {
        char JalurKota[300];
        struct DataKota* temp = HapusKota(Kota, BanyakKota, Kota[i].Kota); //Menghapus kota yang sudah dikunjungi
        strcpy(JalurKota, hasil); //Mendapatkan jalur kota yang sudah dikunjungi
        strcat(JalurKota,"->");
        strcat(JalurKota, Kota[i].Kota); //Menambahkan kota yang sudah dikunjungi ke jalur
        BruteForce(temp, BanyakKota - 1, jarakTerpendek, jarakTerjauh,ruteTerpendek,ruteTerjauh,
        JalurKota,KotaAwal, indeksAwal, Kota[i].indeks, jarak + MatriksKota[indeksNext][Kota[i].indeks], MatriksKota, printRute);
        free(temp); 
    }
}

unsigned long long factorial(int n) {
    if (n == 0) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

// Menghitung jarak antar kota menggunakan rumus haversine
double haversine(double Lati1, double Lati2, double Long1, double Long2) {
    
    double DLati = (Lati2-Lati1) * PI / 180; //Mengkonversi ke radian
    double DLong = (Long2-Long1) * PI / 180;

    double distance = 2 * 6371 * asin(sqrt(pow(sin(DLati / 2), 2) + cos(Lati1 * PI / 180) 
                      * cos(Lati2 * PI / 180) * pow(sin(DLong / 2), 2)));
    return distance;
}

int main() {

    clock_t mulai, selesai;
    char namafile[100];
    printf("Masukkan nama file: "); //Input nama file
    scanf("%s",namafile);

    FILE* file = fopen(namafile, "r"); //Membuka file
    if (file == NULL) {
        printf("Gagal membuka file\n"); //Program akan berhenti jika file tidak ditemukan
        return 1;
    }

    struct DataKota* Kota = NULL; //Deklarasi struct untuk menyimpan data kota
    int baris = 0; //Banyaknya kota pada file
    char isifile[256]; //Variabel untuk menyimpan string dari file

    while (fgets(isifile, sizeof(isifile), file) != NULL) { //Membaca file baris per baris
        Kota = realloc(Kota, sizeof(struct DataKota) * (baris + 1));
        //Mengambil nama kota, latitude, dan longitude dari file yang dipisahkan oleh ","
        if(strcmp(isifile,"\n")!=0){
            sscanf(isifile, "%[^,],%lf,%lf", Kota[baris].Kota, &Kota[baris].latitude, &Kota[baris].longitude);
            Kota[baris].indeks = baris;
            baris++;
        }
    }

    double *MatriksKota[baris]; //Deklarasi matriks untuk jarak antar kota
    for (int i = 0; i < baris; i++) {
        MatriksKota[i] = malloc(sizeof(double) * baris);
        for (int j = 0; j < baris; j++) {
            MatriksKota[i][j] = haversine(Kota[i].latitude, Kota[j].latitude, Kota[i].longitude, Kota[j].longitude);
        }
    }

    double jarakTerpendek = 99999; //Deklarasi data dummy
    char Ruteterpendek[300] ="";
    double jarakTerjauh = 0;
    char Ruteterjauh[300] ="";
    char KotaAwal[20];
    int indeksAwal = 99;
    char printRute[6]="";

    printf("Masukkan kota awal: "); //Input kota awal
    scanf("%s",KotaAwal);

    for (int i = 0; i < baris; i++) { //Mencari indeks kota awal
        if (strcmp(Kota[i].Kota, KotaAwal) == 0) {
            indeksAwal = Kota[i].indeks;
        }
    }

    if (indeksAwal == 99) {
        printf("Kota tidak ditemukan\n"); //Program akan berhenti jika input kota salah
        return 1;
    }
    
    //Program utama
    printf("\nKombinasi seluruh rute yang mungkin sebanyak: %llu rute\n", factorial(baris-1));
    printf("Tampilkan seluruh rute? (ya/tidak): ");
    scanf("%s", printRute);

    srand(time(NULL)); //Mulai menghitung waktu
    mulai = clock(); 
    Kota = HapusKota(Kota, baris, KotaAwal);
    BruteForce(Kota, baris-1,&jarakTerpendek,&jarakTerjauh,Ruteterpendek,Ruteterjauh,"",KotaAwal, indeksAwal,
    indeksAwal, 0, MatriksKota, printRute);

    selesai = clock(); //Selesai menghitung waktu
    double waktu = (double)(selesai - mulai) / CLOCKS_PER_SEC; 

    printf("\nRute terpendek: %s\n", Ruteterpendek);
    printf("Jarak terpendek: %lf km \n", jarakTerpendek);
    printf("Rute terjauh: %s\n", Ruteterjauh);
    printf("Jarak terjauh: %lf km \n", jarakTerjauh);
    printf("\nWaktu yang dibutuhkan program untuk mencari seluruh kemungkinan rute : %f detik\n", waktu);

    fclose(file);
    free(Kota);
    return 0;
}