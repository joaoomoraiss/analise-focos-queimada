#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include <ctype.h>

// data formatted
typedef struct {
    char id[15];
    char foco_id[40];
    float lat;
    float lon;
    char data_pas[25];
    char pais[20];
    char estado[30];
    char municipio[50];
    char bioma[30];
} RowData;

void showData(RowData* allData, int size) {

    //Show some data
    printf("Os primeiros 5 resultados:\n");
    for (int i = 0; i < size && i < 5; i++) {
        printf("%s | %s | %.4f | %.4f | %s | %s | %s | %s | %s\n",
               allData[i].id,
               allData[i].foco_id,
               allData[i].lat,
               allData[i].lon,
               allData[i].data_pas,
               allData[i].pais,
               allData[i].estado,
               allData[i].municipio,
               allData[i].bioma);
    }
}

// Read all the lines of a csv file
// O(n^2)
RowData* read_csv(int *total_count) {

    // Variable
    FILE *file;
    char line[1024];
    RowData *data = malloc(sizeof(RowData) * 13043);
    int count = 0;

    // Open the CSV
    file = fopen("../csv/focos_br_ms_ref_2024.csv", "r");

    if (file == NULL) {
        printf("Error in open the file!");
        return 1;
    }

    //Read the csv
    printf("Content of file: \n");

    //Header of file
    if (fgets(line, sizeof(line), file)) {
        printf("%s\n", line);
    }

    while (fgets(line, sizeof(line), file)) {

        //separate each data in a row breaking the string using strtok
        char *token = strtok(line, ",");
        int col = 0;
        RowData row;

        while (token != NULL) {
            
            //place the data inside CsvDataFormatted
            switch (col) {
            case 0:
                strncpy(row.id, token, sizeof(row.id));
                break;
            case 1:
                strncpy(row.foco_id, token, sizeof(row.foco_id));
                break;
            case 2:
                row.lat = atof(token);
                break;
            case 3:
                row.lon = atof(token);
                break;
            case 4:
                strncpy(row.data_pas, token, sizeof(row.data_pas));
                break;
            case 5:
                strncpy(row.pais, token, sizeof(row.pais));
                break;
            case 6:
                strncpy(row.estado, token, sizeof(row.estado));
                break;
            case 7:
                strncpy(row.municipio, token, sizeof(row.municipio));
                break;
            case 8:
                strncpy(row.bioma, token, sizeof(row.bioma));
                break;
            }

            token = strtok(NULL, ",");
            col++;

        }

        //place the row in array
        data[count++] = row;

        printf("line %d \n", count);
    }

    //close all file
    fclose(file);

    *total_count = count;

    return data;
}

void bubble_sort_data(RowData *data, int size) {
    int change = 1;

    for(int i = 0; i < size-1; i++) {
        
        while (change == 1) {
            change = 0;
            for (int j = 0; j < size - i - 1; j++) {
                if (strcmp(data[j].data_pas, data[j + 1].data_pas) > 0) {
                    RowData temp = data[j];
                    data[j] = data[j + 1];
                    data[j+1] = temp;

                    change = 1;
                }
            }

            //Show the progress
            if((i + 1) % 1000 == 0) {
                printf("Progresso: %d/%d iterações\n", i + 1, size - 1);
            }
        }
    }
}

void bubble_sort(RowData *data, int size, const char *element) {
    int operation = 0;
    int comparation = 0;
    int change = 0;
    int line = 0;

    for(int i = 0; i < size-1; i++) {
        operation++;
            for (int j = 0; j < size - i - 1; j++) {
                operation++;
                
                const char *v1;
                const char *v2;

                if (element == "data_pas") {
                    v1 = data[j].data_pas;
                    v2 = data[j + 1].data_pas;
                } else if (strcmp(element, "bioma") == 0) {
                    v1 = data[j].bioma;
                    v2 = data[j + 1].bioma;
                } else if (strcmp(element, "municipio") == 0) {
                    v1 = data[j].municipio;
                    v2 = data[j + 1].municipio;
                } else {
                    printf("Erro ao achar o elemento &s\n", element);
                    return;
                }
                comparation++;

                if (strcmp(v1, v2) > 0) {
                    RowData temp = data[j];
                    data[j] = data[j + 1];
                    data[j+1] = temp;
                    change++;
                }
            }

        
    }

    //Open File
    FILE *file = fopen("report.txt", "a");
    if (file == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }

    fprintf(file, "-------- %s - Bubble Sort --------\n", element);
    fprintf(file, "Número de operações: %d\nNúmero de Comparações: %d\nNúmero de Trocas: %d\nComplexidade: O(n^2)\nTotal de linhas: %d\n\n", operation, comparation, change, size);

    fclose(file);


    // show report
    printf("\n-------- %s - Bubble Sort --------\n", element);
    printf("Número de operações: %d\nNúmero de Comparações: %d\nNúmero de Trocas: %d\nComplexidade: O(n^2)\nTotal de linhas: %d\n\n", operation, comparation, change, size);
}

void quick_sort(RowData *data, int size) {

}

void insertion_sort(RowData *data, int size) {

}

void binary_insertion_sort(RowData *data, int size) {

}

void selection_sort(RowData *data, int size) {

}

void heap_sort(RowData *data, int size) {

}

void merge_sort(RowData *data, int size) {

}

void bucket_sort(RowData *data, int size) {

}

void show_report() {

}

int main()
{
    RowData *data;
    int count = 0;
    int element_option;
    int sort_option;

    const char *element[3];
    element[0] = "data_pas";
    element[1] = "bioma";
    element[2] = "municipio";

    const char *sort[8];
    sort[0] = "Bubble Sort";
    sort[1] = "Quick Sort";
    sort[2] = "Insertion Sort";
    sort[3] = "Binary Insertion Sort";
    sort[4] = "Selection Sort";
    sort[5] = "Heap Sort";
    sort[6] = "Merge Sort";
    sort[7] = "Bucket Sort";
    


    // Enable UTF-8
    SetConsoleOutputCP(CP_UTF8);  
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "pt_BR.UTF-8");

    //Start file
    FILE *file = fopen("report.txt", "w");
    if (file == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }

    while (1) {
        printf("\nEscolha qual dado você deseja ordenar: \n1.Data\n2.Bioma\n3.Munícipio\n4.Ver Relatório\n5.Sair\n");
        scanf("%d", &element_option);

        if (element_option == 4) {
            show_report();
        } else if (element_option >= 5) {
            break;
        }

        // Select the sort algoritmo
        printf("Escolha o algoritmo de ordenação: \n1.Bubble Sort\n2.Quick Sort\n3.Insertion Sort\n4.Binary Insertion Sort\n5.Selection Sort\n6.Heap Sort\n7.Merge Sort\n8.Bucket Sort\n9.Sair\n");
        scanf("%d", &sort_option);

        if (sort_option >= 4) {
            break;
        }

        data = read_csv(&count);

        if (sort_option == 1) {
            bubble_sort(data, count, element[element_option-1]);
        } else if (sort_option == 2) {
            //quick_sort();
        } else if (sort_option == 3) {
            //insertion_sort();
        } else if (sort_option == 4) {
            //binary_insertion_sort
        } else if (sort_option == 5) {
            //selection_sort
        } else if (sort_option == 6) {
            //heap_sort();
        } else if (sort_option == 7) {
            //merge_sort();
        } else if (sort_option == 8) {
            //bucket_sort();
        }

        //Show data
        showData(data, count);

        
    }

    
    return 0;

}