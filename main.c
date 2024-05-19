/*EL2208 Praktikum Pemecahan Masalah dengan C 2023/2024
*Modul            : Tugas Besar PPMC
*Batas Pengumpulan: Selasa, 21 Mei 2024
*Pembuat kode     : Matthew Wijaya Chandra (13222020)
*Kelompok         : B2
*Pemilihan soal   : Soal 2
*Algoritma        : Algoritma Genetika
*Deskripsi        : Program untuk mencari rute terpendek untuk the traveling salesman problem yang menggunakan
                    algoritma genetika dimana akan dicari seluruh kemungkinan rute yang dapat dilalui dan jarak terpendeknya.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846

// Struct CityData
typedef struct CityData{
    char name[100];
    int index;
    double latitude;
    double longitude;
} CityData;


double calculate_distance(struct CityData city1, struct CityData city2) {
    double R = 6371; // Earth's radius in km

    // convert deg -> rad
    double Lat1 = city1.latitude * PI / 180;
    double Lat2 = city2.latitude * PI / 180;
    double Lon1 = city1.longitude * PI / 180;
    double Lon2 = city2.longitude * PI / 180;

    double radLat = Lat2 - Lat1;
    double radLon = Lon2 - Lon1;

    // Haversine formula
    double temp = pow(sin(radLat / 2), 2) + cos(Lat1) * cos(Lat2) * pow(sin(radLon / 2), 2);
    double distance = 2 * R * atan2(sqrt(temp), sqrt(1 - temp));

    return distance;
}


void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}


void generate_random_permutation(int* array, int n){
    for(int i = n - 1; i > 0; i--){
        int j = rand() % (i + 1);
        swap(&array[i], &array[j]);
    }
}


int** generate_population(int n, int population_size){
    int** population = (int**)malloc(population_size * sizeof(int*));
    if(population == NULL){
        perror("Memory allocation for population failed");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < population_size; i++){
        population[i] = (int*)malloc(n * sizeof(int));
        if(population[i] == NULL){
            perror("Memory allocation for population failed");
            exit(EXIT_FAILURE);
        }

        for(int j = 0; j < n; j++){
            population[i][j] = j;
        }
        generate_random_permutation(population[i], n);
    }
    return population;
}


double calculate_route_distance(int *path, int n, struct CityData* cities){
    double total = 0;

    for(int i = 0; i < n - 1; i++){
        total += calculate_distance(cities[path[i]], cities[path[i + 1]]);
    }
    total += calculate_distance(cities[path[n - 1]], cities[path[0]]);

    return total;
}


void evaluate_population(int **population, int population_size, int n, struct CityData* cities, double *fitness_value){
    for(int i = 0; i < population_size; i++){
        fitness_value[i] = 1 / calculate_route_distance(population[i], n, cities);
    }
}


void select_parents(int **population, int population_size, double *fitness_value, int *parent1, int *parent2){
    *parent1 = rand() % population_size;
    *parent2 = rand() % population_size;

    // Tournament selection
    int tournament_size = 5;
    for(int i = 0; i < tournament_size; i++){
        int candidate = rand() % population_size;

        if(fitness_value[candidate] > fitness_value[*parent1]){
            *parent1 = candidate;
        }else if(fitness_value[candidate] > fitness_value[*parent2]){
            *parent2 = candidate;
        }
    }
}


void crossover(int *parent1, int *parent2, int *offspring, int n){
    // Generate 2 random points to define a segment from the parent routes
    int start  = rand() % n;
    int end = rand() % n;

    if(start > end){
        int temp = start;
        start = end;
        end = temp;
    }


    int *temp = (int*)malloc(n * sizeof(int));
    memset(temp, -1, n * sizeof(int));

    // Copy the segment from parent1 to the offspring
    for(int i = start; i <= end; i++){
        temp[parent1[i]] = parent1[i];
    }

    // Fill the rest of the offspring with cities from parent2
    int k = 0;
    for(int i = 0; i < n; i++){
        if(temp[parent2[i]] == -1){ // city from parent2 not in the segment copied from parent1
            while(temp[k] != -1){
                k++;
            }
            offspring[i] = k;
            k++;
        }else{  // city from parent2 is in the segment copied from parent1
            offspring[i] = parent2[i];
        }
    }
    free(temp);
}


void mutate(int *offspring, int n){
    int temp1 = rand() % n;
    int temp2 = rand() % n;

    swap(&offspring[temp1], &offspring[temp2]);
}


void free_population(int **population, int population_size){
    for(int i = 0; i < population_size; i++){
        free(population[i]);
    }
    free(population);
}


int main()
{
    clock_t start, finish;

    // Input file external name
    char file_name[100];
    printf("Enter list of cities file name: ");
    scanf("%s", file_name);

    // Add .csv to file_name
    strcat(file_name, ".csv");

    // Open file
    FILE* file = fopen(file_name, "r");
    if(file == NULL){   // Isi file kosong
        printf("Failed to open file\n");
        return 1;
    }


    // Initialize struct
    struct CityData* City = NULL;
    int n = 0; // Jumlah kota pada file
    char city_name[100]; // variabel untuk menyimpan nama kota

    // Read file per line
    while(fgets(city_name, sizeof(city_name), file) != NULL){
        City = realloc(City, sizeof(struct CityData) * (n + 1));
        if(City == NULL){
            perror("Memory allocation failed!");
            fclose(file);
            return 1;
        }
        sscanf(city_name, "%[^,],%lf,%lf", City[n].name, &City[n].latitude, &City[n].longitude);
        City[n].index = n;
        n++;
    }

    // Close file
    fclose(file);

    // Input starting city
    char starting_city[100];
    printf("Enter starting point: ");
    scanf("%s", starting_city);

    // Search starting city index
    int starting_index = -1;
    for(int i = 0; i < n; i++){
        if(strcmp(City[i].name, starting_city) == 0){
            starting_index = City[i].index;
            break;
        }
    }

    if(starting_index == -1){   // Input starting city wrong
        printf("City not found");
        return 1;
    }


    // Initialize genetic algorithm parameters
    int population_size = 50;
    int num_generation = 1000;
    double mutation_rate = 0.1;

    // Create initial population
    int **population = generate_population(n, population_size);

    // Evaluate initial population
    double *fitness_value = malloc(population_size * sizeof(double));
    if(fitness_value == NULL){
        perror("Memory allocation for fitness_value failed");
        return 1;
    }

    evaluate_population(population, population_size, n, City, fitness_value);

    // Start of genetic algorithm
    srand(time(NULL));
    start = clock();

    // Main loop for genetic algorithm
    for(int generation = 0; generation < num_generation; generation++){
        // Create next generation
        int** next_generation = (int**)malloc(population_size * sizeof(int*));
        if(next_generation == NULL){
            perror("Memory allocation for next_generation failed");
            return 1;
        }

        for(int i = 0; i < population_size; i++){
            // Memory allocation for next_generation
            next_generation[i] = (int*)malloc(n * sizeof(int));
            if(next_generation[i] == NULL){
                perror("Memory allocation for next_generation[i] failed");
                return 1;
            }

            // Initialize parents
            int parent1;
            int parent2;

            select_parents(population, population_size, fitness_value, &parent1, &parent2);

            // Crossover with 100% chance
            crossover(population[parent1], population[parent2], next_generation[i], n);


            // Mutation with 10% chance
            if((double)rand() / RAND_MAX < mutation_rate){
                mutate(next_generation[i], n);
            }
        }

        // Replace old population with new generation
        free_population(population, population_size);
        population = next_generation;

        // Evaluate new population
        evaluate_population(population, population_size, n, City, fitness_value);
    }

    // Genetics algorithm is finished
    finish = clock();


    // Finding the best route
    int best_index = -1;
    double shortest_distance = -1;
    int array[100];

    for(int i = 0; i < population_size; i++){
        if(population[i][0] == starting_index){
            double distance = calculate_route_distance(population[i], n, City);
            if(best_index == -1 || distance < shortest_distance){
                best_index = i;
                shortest_distance = distance;
            }

        }
    }

    // Output print the shortest route
    printf("Best route starting at %s:\n", starting_city);
    for(int i = 0; i < n; i++){
        printf("%s -> ", City[population[best_index][i]].name);
    }
    printf("%s\n", City[population[best_index][0]].name);
    printf("Best Route Distance : %.2f km\n", shortest_distance);

    // Output total time elapsed
    double time = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("Time elapsed : %f s", time);

    // Free memory
    free_population(population, population_size);
    free(fitness_value);
    free(City);

    return 0;
}
