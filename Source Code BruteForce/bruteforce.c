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

// Mencetak seluruh data kota dari struct
void CetakStruct(DataKota* Kota, int baris) {
    struct DataKota* temp = malloc(sizeof(DataKota) * baris);
    temp = Kota;
    for (int i = 0; i < baris; i++) {
        printf("Indeks %d: %s, %lf, %lf\n", temp[i].indeks, temp[i].Kota, temp[i].latitude, temp[i].longitude);
    }
}

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
void BruteForce(struct DataKota* Kota, int BanyakKota, double* jarakTerpendek, char (*ruteTerpendek),
               char hasil[100], char KotaAwal[20],int indeksAwal,int indeksNext, double jarak, 
               double MatriksKota[4][4]) {

    // Kasus basis jika sudah tidak ada kota yang bisa dikunjungi
    if (BanyakKota == 0) {
        jarak = jarak + MatriksKota[indeksAwal][indeksNext]; //Menambahkan jarak kembali ke kota awal
        if (jarak < *jarakTerpendek) { 
            char HasilAkhir[1000]="";
            *jarakTerpendek = jarak; //Menyimpan jarak terpendek ke main
            strcat(HasilAkhir,KotaAwal);
            strcat(HasilAkhir,hasil);
            strcat(HasilAkhir,"->");
            strcat(HasilAkhir,KotaAwal);
            strcpy(ruteTerpendek,HasilAkhir); //Menyimpan rute terpendek ke main
        }
        printf("%s%s->%s %lf km\n",KotaAwal,hasil,KotaAwal,jarak); //Menampilkan rute dan jarak
    }

    // Kasus rekursif jika masih ada kota yang bisa dikunjungi
    for (int i = 0; i < BanyakKota; i++) {
        char JalurKota[1000];
        struct DataKota* temp = HapusKota(Kota, BanyakKota, Kota[i].Kota); //Menghapus kota yang sudah dikunjungi
        strcpy(JalurKota, hasil); //Mendapatkan jalur kota yang sudah dikunjungi
        strcat(JalurKota,"->");
        strcat(JalurKota, Kota[i].Kota); //Menambahkan kota yang sudah dikunjungi ke jalur
        BruteForce(temp, BanyakKota - 1, jarakTerpendek,ruteTerpendek, JalurKota,KotaAwal,indeksAwal, 
        Kota[i].indeks, jarak + MatriksKota[indeksNext][Kota[i].indeks], MatriksKota);
        free(temp); 
    }
}

int factorial(int n) {
    if (n == 0) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

// Menghitung jarak antar kota menggunakan rumus haversine
double haversine(double Lati1, double Lati2, double Long1, double Long2) {
    
    double R = 6371;
    double DLati = (Lati2-Lati1) * PI / 180; //Mengkonversi ke radian
    double DLong = (Long2-Long1) * PI / 180;

    double distance = 2 * R * asin(sqrt(pow(sin(DLati / 2), 2) + cos(Lati1 * PI / 180) 
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
        if (Kota == NULL) {
            perror("Error alokasi memori"); //Program akan berhenti jika isifile kosong
            fclose(file);
            return 1;
        }
        //Mengambil nama kota, latitude, dan longitude dari file yang dipisahkan oleh ","
        sscanf(isifile, "%[^,],%lf,%lf", Kota[baris].Kota, &Kota[baris].latitude, &Kota[baris].longitude);
        Kota[baris].indeks = baris;
        baris++;
    }

    double MatriksKota[baris][baris]; //Deklarasi matriks untuk jarak antar kota
    for (int i = 0; i < baris; i++) {
        for (int j = 0; j < baris; j++) {
            MatriksKota[i][j] = haversine(Kota[i].latitude, Kota[j].latitude, Kota[i].longitude, Kota[j].longitude);
        }
    }

    double jarakTerpendek = 99999; //Deklarasi data dummy
    char Ruteterpendek[1000] ="";
    char KotaAwal[20];
    int indeksAwal = 99;

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
    
    srand(time(NULL)); //Mulai menghitung waktu
    mulai = clock(); 
    
    //Program utama
    printf("\nKombinasi seluruh rute yang mungkin sebanyak: %d rute\n", factorial(baris-1));
    Kota = HapusKota(Kota, baris, KotaAwal);
    BruteForce(Kota, baris-1,&jarakTerpendek,Ruteterpendek,"",KotaAwal, indeksAwal, 0, 0, MatriksKota);

    selesai = clock(); //Selesai menghitung waktu
    double waktu = (double)(selesai - mulai) / CLOCKS_PER_SEC; 

    printf("\n");
    printf("Rute terpendek: %s\n", Ruteterpendek);
    printf("Jarak terpendek: %lf\n", jarakTerpendek);
    printf("Waktu yang dibutuhkan: %f detik\n", waktu);

    fclose(file);
    free(Kota);
    return 0;
}