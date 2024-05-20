/*EL2208 Praktikum Pemecahan Masalah dengan C 2023/2024
*Modul            : Tugas Besar
*Batas Pengumpulan: Selasa, 21 Mei 2024
*Pembuat kode     : Naufal Abrar (13222023)
*Kelompok         : B2
*Pemilihan soal   : Soal 2
*Algoritma        : Algoritma Ant Colony Optimization
*Deskripsi        : Program untuk mencari rute terpendek untuk traveling salesman menggunakan algoritma ant colony optimization. Algoritma ini menggunakan konsep feromon untuk mewakilkan probabilitas suatu jalur dilewati.

/* Perintah Kompilasi  
    gcc ACO.c -o aco
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <string.h>

#define MAX_CITIES 15
#define MAX_ANTS 50
#define MAX_ITER 1000
#define ALPHA 1.0
#define BETA 2.0
#define EVAPORATION 0.5
#define Q 100
#define PI 3.14159265358979323846

typedef struct Ant{
    int route[MAX_CITIES];
    double route_length;
} Ant;

// Struct untuk menyimpan data kota
typedef struct DataKota{
    char Kota[20];
    int indeks;
    double latitude;
    double longitude;
} DataKota;

// Menghitung jarak antar kota menggunakan rumus haversine
double haversine(double Lati1, double Lati2, double Long1, double Long2) 
{
    
    double R = 6371;
    double DLati = (Lati2-Lati1) * PI / 180; //Mengkonversi ke radian
    double DLong = (Long2-Long1) * PI / 180;

    double distance = 2 * R * asin(sqrt(pow(sin(DLati / 2), 2) + cos(Lati1 * PI / 180) * cos(Lati2 * PI / 180) * pow(sin(DLong / 2), 2)));
    return distance;
}

void printMatrix(int n, double MatriksKota[n][n]) 
{
    for (int i = 0; i < n; i++) 
    {
        printf("[");
        for (int j = 0; j < n; j++)
        {
            printf("%.2lf ",MatriksKota[i][j]);
        }
        printf("]\n");
    }
}

double calculaterouteCost(int n, double MatriksKota[n][n], int route[n]) 
{
    double total_cost = 0;
    for (int i = 0; i < n - 1; ++i) 
    {
        total_cost += MatriksKota[route[i]][route[i + 1]];
    }
    total_cost += MatriksKota[route[n - 1]][route[0]];
    return total_cost;
}

void initializePheromoneMatrix(int n, double pheromone[n][n], double initial_pheromone) 
{
    for (int i = 0; i < n; ++i) 
    {
        for (int j = 0; j < n; ++j) 
        {
            pheromone[i][j] = initial_pheromone;
        }
    }
}

void initializeAnts(int n, int num_ants, Ant ants[MAX_ANTS], int start_city) 
{
    for (int i = 0; i < num_ants; ++i)
    {
        ants[i].route[0] = start_city;
        for (int j = 1; j < n; ++j) 
        {
            ants[i].route[j] = -1;
        }
        ants[i].route_length = 0;
    }
}

int selectNextCity(int n, double MatriksKota[n][n], double pheromone[n][n], int current_city, int visited[n]) 
{
    double total = 0;
    double probabilities[n];

    // Menghitung probabilitas suatu kota dikunjungi
    for (int i = 0; i < n; ++i) 
    {
        if (visited[i] == 0)
        {
            probabilities[i] = pow(pheromone[current_city][i], ALPHA) * pow(1.0 / MatriksKota[current_city][i], BETA);
            total += probabilities[i];
        } 
        else 
        {
            probabilities[i] = 0;
        }
    }

    double r = ((double)rand() / (double)RAND_MAX) * total;
    double sum = 0;

    for (int i = 0; i < n; ++i) 
    {
        if (visited[i] == 0) 
        {
            sum += probabilities[i];
            if (sum >= r) 
            {
                return i;
            }
        }
    }

    // Fallback in case of rounding errors
    for (int i = 0; i < n; ++i) 
    {
        if (visited[i] == 0) 
        {
            return i;
        }
    }

    return -1;
}

void updatePheromones(int n, int num_ants, Ant ants[MAX_ANTS], double pheromone[n][n]) {
    //Evaporasi feromon
    for (int i = 0; i < n; ++i) 
    {
        for (int j = 0; j < n; ++j) 
        {
            pheromone[i][j] *= (1.0 - EVAPORATION);
        }
    }

    //Menambahkan feromon baru
    for (int i = 0; i < num_ants; ++i) 
    {
        for (int j = 0; j < n - 1; ++j) 
        {
            int city1 = ants[i].route[j];
            int city2 = ants[i].route[j + 1];
            pheromone[city1][city2] += Q / ants[i].route_length;
            pheromone[city2][city1] += Q / ants[i].route_length;
        }
        //Balik ke kota awal
        int city1 = ants[i].route[n - 1];
        int city2 = ants[i].route[0];
        pheromone[city1][city2] += Q / ants[i].route_length;
        pheromone[city2][city1] += Q / ants[i].route_length;
    }
}

void antColonyOptimization(int n, double MatriksKota[n][n], int start_city, DataKota *Kota) 
{
    Ant ants[MAX_ANTS];
    double pheromone[n][n];
    double initial_pheromone = 1.0;
    int num_ants = MAX_ANTS;
    double best_route_length = DBL_MAX;
    int best_route[n];

    initializePheromoneMatrix(n, pheromone, initial_pheromone);

    for (int iter = 0; iter < MAX_ITER; ++iter) 
    {
        initializeAnts(n, num_ants, ants, start_city);

        for (int i = 0; i < num_ants; ++i) 
        {
            int visited[MAX_CITIES] = {0};
            visited[start_city] = 1;

            for (int j = 1; j < n; ++j) 
            {
                int current_city = ants[i].route[j - 1];
                int next_city = selectNextCity(n, MatriksKota, pheromone, current_city, visited);
                ants[i].route[j] = next_city;
                visited[next_city] = 1;
                ants[i].route_length += MatriksKota[current_city][next_city];
            }
            //Rute selesai
            ants[i].route_length += MatriksKota[ants[i].route[n - 1]][ants[i].route[0]];

            if (ants[i].route_length < best_route_length) 
            {
                best_route_length = ants[i].route_length;
                for (int j = 0; j < n; ++j) 
                {
                    best_route[j] = ants[i].route[j];
                }
            }
        }
        updatePheromones(n, num_ants, ants, pheromone);
    }

    //Menampilkan route terpendek dan panjangnya
    printf("Best route found:\n");
    for (int i = 0; i < n; ++i) 
    {
        printf("%s -> ", Kota[best_route[i]].Kota);
    }
    printf("%s\n", Kota[best_route[0]].Kota);

    printf("Best route distance: %.5f km\n", best_route_length);
}

int main() 
{
    clock_t mulai, selesai;
    char namafile[100];
    printf("Enter list of cities file name: "); //Input nama file
    scanf("%s",namafile);

    FILE* file = fopen(namafile, "r"); //Membuka file
    if (file == NULL) 
    {
        printf("Error opening file\n"); //Program akan berhenti jika file tidak ditemukan
        return 1;
    }

    DataKota* Kota = NULL; //Deklarasi struct untuk menyimpan data kota
    int baris = 0; //Banyaknya kota pada file
    char isifile[256]; //Variabel untuk menyimpan string dari file

    //Membaca file baris per baris
    while (fgets(isifile, sizeof(isifile), file) != NULL) { 
        Kota = realloc(Kota, sizeof(DataKota) * (baris + 1));
        if (Kota == NULL) 
        {
            perror("Error allocating memory"); //Program akan berhenti jika isifile kosong
            fclose(file);
            return 1;
        }
        //Mengambil nama kota, latitude, dan longitude dari file yang dipisahkan oleh ","
        sscanf(isifile, "%[^,],%lf,%lf", Kota[baris].Kota, &Kota[baris].latitude, &Kota[baris].longitude);
        Kota[baris].indeks = baris;
        baris++;
    }

    double MatriksKota[baris][baris]; //Deklarasi matriks untuk jarak antar kota
    for (int i = 0; i < baris; i++) 
    {
        for (int j = 0; j < baris; j++) 
        {
            MatriksKota[i][j] = haversine(Kota[i].latitude, Kota[j].latitude, Kota[i].longitude, Kota[j].longitude);
        }
    }

    //printMatrix(baris,MatriksKota);

    if (baris < 1 || baris > 15) 
    {
        printf("Number of cities must be between 6 and 15.\n");
        return 1;
    }
    
    char KotaAwal[20];
    int indeksAwal = 99;    
    printf("Enter starting point: "); //Input kota awal
    scanf("%s",KotaAwal);

    //Mencari indeks kota awal
    for (int i = 0; i < baris; i++) 
    { 
        if (strcmp(Kota[i].Kota, KotaAwal) == 0) 
        {
            indeksAwal = Kota[i].indeks;
        }
    }

    if (indeksAwal < 0 || indeksAwal >= baris) 
    {
        printf("Invalid starting city.\n");
        return 1;
    }

    srand(time(NULL));
    mulai = clock(); 

    antColonyOptimization(baris, MatriksKota, indeksAwal, Kota);
    selesai = clock(); //Selesai menghitung waktu
    double waktu = (double)(selesai - mulai) / CLOCKS_PER_SEC; 
    printf("Time elapsed: %f s\n", waktu);

    fclose(file);
    free(Kota);
    return 0;
}
