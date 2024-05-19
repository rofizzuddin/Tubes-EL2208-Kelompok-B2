/*EL2208 Praktikum Pemecahan Masalah dengan C 2023/2024
*Modul            : Tugas Besar
*Batas Pengumpulan: Selasa, 21 Mei 2024
*Pembuat kode     : I Made Medika Surya W.M. (13222021)
*Kelompok         : B2
*Pemilihan soal   : Soal 2
*Algoritma        : Algoritma BFS (Breadth First Search) untuk TSP
*Deskripsi        : Program ini mencari rute terpendek dengan algoritma BFS yang dimodifikasi
*                   untuk Travelling Salesman Problem (TSP). Program ini akan memuat kota-kota
*                   dari file eksternal, kemudian mencari rute terpendek yang mengunjungi semua
*                   kota tepat satu kali dan kembali ke kota awal. Program akan mencetak rute
*                   terpendek, jarak terpendek, dan waktu eksekusi program.*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>



int main(void) 
{   
    //Mendefinisikan konstanta
    #define R 6371
    #define MAX_CITIES 1000
    #define MAX_NAME_LEN 50
    #define PI 3.14159265358979323846

    //Definisikan tipe data struct City dan Route
    typedef struct 
    {
        char name[MAX_NAME_LEN];
        double lat;
        double lon;
    } City;

    typedef struct 
    {
        City path[MAX_CITIES];
        int num_cities;
        double total_distance;
    } Route;

    //Deklarasi variabel global
    City cities[MAX_CITIES];
    int num_cities = 0;
    Route best_route;
    int valid = 1;

    //Fungsi untuk mengubah derajat ke radian
    double deg2rad(double deg) 
    {
        return deg * (PI/180);
    }

    //Fungsi untuk menghitung jarak antara dua kota
    double haversine_distance(City a, City b) 
    {
        double dLat = deg2rad(b.lat - a.lat);
        double dLon = deg2rad(b.lon - a.lon);
        double lat1 = deg2rad(a.lat);
        double lat2 = deg2rad(b.lat);

        double a_val = pow(sin(dLat/2), 2) + cos(lat1) * cos(lat2) * pow(sin(dLon/2), 2);
        double c = 2 * asin(sqrt(a_val));

        return R * c;
    }

    //Prosedur untuk memuat kota dari file
    void load_cities(char *filename) 
    {
        FILE *file = fopen(filename, "r");
        if (file == NULL) 
        {
            printf("Invalid file name.\n");
            exit(1);
        }

        char line[256];
        int i = 1;
        while (fgets(line, sizeof(line), file)) 
        {
            char name[MAX_NAME_LEN];
            double lat, lon;

            //Mengecek apakah format benar : <nama_kota>,<latitude>,<longitude>
            
            if (sscanf(line, "%[^,],%lf ,%lf ", name, &lat, &lon) != 3) 
            {
                printf("Invalid data format in line %d: %s\n", i, line);
                i = i + 1;
                valid = 0;
                continue;
            }

            //Mengecek apakah latitude dan longitude valid
            if (lat < -90 || lat > 90 || lon < -180 || lon > 180) 
            {
                printf("Invalid latitude or longitude in line %d: %s\n", i, line);
                i = i + 1;
                valid = 0;
                continue;
            }

            //Jika valid maka simpan kota ke dalam array cities
            strncpy(cities[num_cities].name, name, MAX_NAME_LEN);
            cities[num_cities].lat = lat;
            cities[num_cities].lon = lon;
            num_cities++;
            i = i + 1;
        }

        fclose(file);
    }

    //Prosedur untuk menukar dua kota
    void swap(City *a, City *b) 
    {
        City temp = *a;
        *a = *b;
        *b = temp;
    }

    //Prosedur untuk menghasilkan semua permutasi kota sebagai graph BFS
    void permute(City *array, int start, int end) 
    {
        //Jika hanya ada satu kota, maka permutasi sudah selesai
        if (start == end) 
        {
            //Menghitung total permutasi total graph kemudian mencari jarak terpendek
            double total_distance = 0;
            for (int i = 0; i < end; i++) 
            {
                total_distance += haversine_distance(array[i], array[i+1]);
            }

            total_distance += haversine_distance(array[end], array[0]); //Menambahkan jarak dari hasil akhir permutasi ke kota awal

            //Jika rute saat ini lebih pendek dari rute terbaik yang ditemukan sejauh ini, simpan rute saat ini sebagai rute terbaik
            if (best_route.num_cities == 0 || total_distance <= best_route.total_distance) 
            {
                for (int i = 0; i <= end; i++) 
                {
                    best_route.path[i] = array[i];
                }
                best_route.num_cities = end + 1;
                best_route.total_distance = total_distance;
            }
        } 

        //Jika lebih dari 1 kota, lakukan permutasi
        else 
        {
            //Menghasilkan semua permutasi kota
            for (int i = start; i <= end; i++) 
            {   
                //Menukar kota
                swap(&array[start], &array[i]);
                
                //Rekursif untuk kota lainnya
                permute(array, start + 1, end);

                //Menukar kembali kota untuk backtracking
                swap(&array[start], &array[i]);
            }
        }
    }


    //Prosedur untuk melakukan BFS yang dimodifikasi untuk TSP dengan permutasi
    void bfs(City start) 
    {
        //Mencari indeks kota awal
        int start_index;
        for (int i = 0; i < num_cities; i++) 
        {
            if (strcmp(cities[i].name, start.name) == 0) 
            {
                start_index = i;
                break;
            }
        }

        //Menukar kota awal dengan kota kota indeks pertama
        swap(&cities[0], &cities[start_index]);

        //Menghasilkan graph permutasi kota yang jarak terpendeknya
        permute(cities, 1, num_cities - 1);
    }

    //Prosedur untuk mencetak semua jarak antar kota
    void print_all_distance(City *array, int num_cities)
    {
        int i, j;
        for (i = 0 ; i < num_cities ; i = i + 1)
        {
            for (j = 0 ; j < num_cities ; j = j + 1)
            {
                double distance = haversine_distance(best_route.path[i], best_route.path[j]);
        
                printf("%s -> %s = %.5f km\n", best_route.path[i].name, best_route.path[j].name, distance);
            }
            printf("\n");
        }
    }

    //Prosedur untuk mencetak jarak antar kota per step
    void print_step_distance(City *array, int num_cities, City start)
    {   
        int i;
        for (int i = 0; i < best_route.num_cities - 1; i++) 
        {
            double distance = haversine_distance(best_route.path[i], best_route.path[i+1]);
            printf("%d. %s -> %s = %.5f km\n", i + 1, best_route.path[i].name, best_route.path[i+1].name, distance);
        }

        double distance = haversine_distance(best_route.path[best_route.num_cities - 1], start);
        printf("%d. %s -> %s = %.5f km\n", num_cities, best_route.path[best_route.num_cities - 1].name, start.name, distance);
        
    }
    //Deklarasi variabel di main
    char filename[100];
    char start_name[MAX_NAME_LEN];

    //Input file
    printf("Enter list of cities file name: ");
    scanf("%s", filename);
    
    load_cities(filename);


    //Jika format file tidak valid, program berhenti
    if (valid == 0)
    {
        printf("Invalid data format, program stopped\n");
        exit(1);
    }


    //Input kota awal
    printf("Enter starting point: ");
    scanf("%s", start_name);


    City start;
    int found = 0;
    
    //Mencari kota awal di array cities
    for (int i = 0; i < num_cities; i++) 
    {
        if (strcmp(cities[i].name, start_name) == 0) 
        {
            start = cities[i];
            found = 1;
            break;
        }
    }

    //Jika kota tidak ditemukan, program berhenti
    if (!found)
    {
        printf("City not found\n");
        exit(1);
    }

    //Menghitung waktu mulai program
    clock_t begin = clock();

    //Melakukan BFS untuk TSP
    bfs(start);

    //Menghitung waktu selesai program
    clock_t end = clock();

    //Mencetak hasil rute terpendek
    printf("Best route found:\n");
    for (int i = 0; i < best_route.num_cities; i++) 
    {
        printf("%s -> ", best_route.path[i].name);
    }
    printf("%s\n", start.name);
    
    //Mencetak jarak terpendek
    printf("Best route distance: %.5f km\n", best_route.total_distance);


    //Mencetak waktu eksekusi
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time elapsed: %.10f s\n", time_spent);
    return 0;
}
