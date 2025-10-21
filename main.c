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

//quick sort report
int quick_comparation = 0;
int quick_partitions = 0;
int quick_swaps = 0;

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
    RowData *data = malloc(sizeof(RowData) * 13045);
    int count = 0;

    // Open the CSV
    file = fopen("../csv/focos_br_ms_ref_2024.csv", "r");

    if (file == NULL) {
        printf("Error in open the file!");
        return NULL;
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

void quick_sort(RowData *data, int size, const char *element) {
    if(size <=1) return;

    quick_partitions++;

    RowData *minors = malloc(sizeof(RowData) * size);
    RowData *equals = malloc(sizeof(RowData) * size);
    RowData *biggest = malloc(sizeof(RowData) * size);

    int minors_count = 0, equals_count = 0, biggest_count = 0;
    
    if (strcmp(element, "data_pas") == 0) {
        const char *pivo = data[0].data_pas;

        for (int i = 0; i < size; i++) {
            quick_comparation++;
            int cmp = strcmp(data[i].data_pas, pivo);
            if (cmp < 0) {
                minors[minors_count++] = data[i];
            } else if (cmp == 0) {
                equals[equals_count++] = data[i];
            } else {
                biggest[biggest_count++] = data[i];
            }
        }
    } else if (strcmp(element, "bioma") == 0) {
        const char *pivo = data[0].bioma;

        for (int i = 0; i < size; i++) {
            quick_comparation++;
            int cmp = strcmp(data[i].bioma, pivo);
            if (cmp < 0) {
                minors[minors_count++] = data[i];
            } else if (cmp == 0) {
                equals[equals_count++] = data[i];
            } else {
                biggest[biggest_count++] = data[i];
            }
        }
    } else {
        const char *pivo = data[0].municipio;

        for (int i = 0; i < size; i++) {
            quick_comparation++;
            int cmp = strcmp(data[i].municipio, pivo);
            if (cmp < 0) {
                minors[minors_count++] = data[i];
            } else if (cmp == 0) {
                equals[equals_count++] = data[i];
            } else {
                biggest[biggest_count++] = data[i];
            }
        }
    }

    quick_sort(minors, minors_count, element);
    quick_sort(biggest, biggest_count, element);

    int index = 0;
    for (int i = 0; i < minors_count; i++) {
        data[index++] = minors[i];
        quick_swaps++;
    }
    for (int i = 0; i < equals_count; i++) {
        data[index++] = equals[i];
        quick_swaps++;
    }
    for (int i = 0; i < biggest_count; i++) {
        data[index++] = biggest[i];
        quick_swaps++;
    }

    free(minors);
    free(equals);
    free(biggest);

    
    
}

void insertion_sort(RowData *data, int size, const char *element) {
    int comparation = 0;
    int swaps = 0;
    int operations = 0;
    
    for (int i = 1; i < size; i++) {
        operations++;
        RowData key = data[i];
        int j = i - 1;
        
        const char *key_value;
        if (strcmp(element, "data_pas") == 0) {
            key_value = key.data_pas;
        } else if (strcmp(element, "bioma") == 0) {
            key_value = key.bioma;
        } else if (strcmp(element, "municipio") == 0) {
            key_value = key.municipio;
        } else {
            printf("Erro ao achar o elemento %s\n", element);
            return;
        }
        
        while (j >= 0) {
            operations++;
            comparation++;
            
            const char *current_value;
            if (strcmp(element, "data_pas") == 0) {
                current_value = data[j].data_pas;
            } else if (strcmp(element, "bioma") == 0) {
                current_value = data[j].bioma;
            } else {
                current_value = data[j].municipio;
            }
            
            if (strcmp(current_value, key_value) > 0) {
                data[j + 1] = data[j];
                swaps++;
                j--;
            } else {
                break;
            }
        }
        
        data[j + 1] = key;
    }
    
    // File
    FILE *file = fopen("report.txt", "a");
    if (file == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }
    
    fprintf(file, "-------- %s - Insertion Sort --------\n", element);
    fprintf(file, "Número de operações: %d\nNúmero de Comparações: %d\nNúmero de Trocas: %d\nComplexidade: O(n^2)\nTotal de linhas: %d\n\n", operations, comparation, swaps, size);
    
    fclose(file);
    
    // Report
    printf("\n-------- %s - Insertion Sort --------\n", element);
    printf("Número de operações: %d\nNúmero de Comparações: %d\nNúmero de Trocas: %d\nComplexidade: O(n^2)\nTotal de linhas: %d\n\n", operations, comparation, swaps, size);
}

int binary_search(RowData *data, const char *item, int low, int high, const char *element, int *comparations) {
    const char *mid_value;
    int mid;
    int cmp;

    while (low <= high) {
        mid = low + (high - low) / 2;
        (*comparations)++;

        if (strcmp(element, "data_pas") == 0) {
            mid_value = data[mid].data_pas;
        } else if (strcmp(element, "bioma") == 0) {
            mid_value = data[mid].bioma;
        } else {
            mid_value = data[mid].municipio;
        }

        cmp = strcmp(item, mid_value);

        if (cmp == 0) {
            return mid +1;
        } else if (cmp > 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return low;
}

void binary_insertion_sort(RowData *data, int size, const char *element) {
    int comparations = 0;
    int swaps = 0;
    int operations = 0;
    int loc;
    int j;
    
    for (int i = 1; i < size; i++) {
        operations++;
        RowData selected = data[i];
        j = i - 1;

        const char *selected_value;
        if (strcmp(element, "data_pas") == 0) {
            selected_value = selected.data_pas;
        } else if (strcmp(element, "bioma") == 0) {
            selected_value = selected.bioma;
        } else if (strcmp(element, "municipio") == 0) {
            selected_value = selected.municipio;
        } else {
            printf("Erro ao achar o elemento %s\n", element);
            return;
        }

        loc = binary_search(data, selected_value, 0, j, element, &comparations);

        while (j >= loc) {
            operations++;
            data[j + 1] = data[j];
            swaps++;
            j--;
        }

        data[j + 1] = selected;
    }

    // File
    FILE *file = fopen("report.txt", "a");
    if (file == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }
    
    fprintf(file, "-------- %s - Binary Insertion Sort --------\n", element);
    fprintf(file, "Número de operações: %d\nNúmero de Comparações: %d\nNúmero de Trocas: %d\nComplexidade: O(n^2)\nTotal de linhas: %d\n\n", operations, comparations, swaps, size);
    
    fclose(file);
    
    // Report
    printf("\n-------- %s - Binary Insertion Sort --------\n", element);
    printf("Número de operações: %d\nNúmero de Comparações: %d\nNúmero de Trocas: %d\nComplexidade: O(n^2)\nTotal de linhas: %d\n\n", operations, comparations, swaps, size);
}

void selection_sort(RowData *data, int size, const char *element) {
    RowData temp;
    const char *v1, *v2;
    int min;
    int comparation = 0;
    int swaps = 0;
    int operations = 0;

    for (int i = 0; i < size; i++) {
        operations++;
        min = i;
        for (int j = i + 1; j < size; j++) {
            operations++;
            comparation++;

            if (strcmp(element, "data_pas") == 0) {
                v1 = data[j].data_pas;
                v2 = data[min].data_pas;
            } else if (strcmp(element, "bioma") == 0) {
                v1 = data[j].bioma;
                v2 = data[min].bioma;
            } else {
                v1 = data[j].municipio;
                v2 = data[min].municipio;
            }

            if (strcmp(v1, v2) < 0) {
                min = j;
            }
        }

        if (min != i) {
            temp = data[i];
            data[i] = data[min];
            data[min] = temp;
            swaps++;
        }
    }

    // File
    FILE *file = fopen("report.txt", "a");
    if (file == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }
    
    fprintf(file, "-------- %s - Selection Sort --------\n", element);
    fprintf(file, "Número de operações: %d\nNúmero de Comparações: %d\nNúmero de Trocas: %d\nComplexidade: O(n^2)\nTotal de linhas: %d\n\n", operations, comparation, swaps, size);
    
    fclose(file);
    
    // Report
    printf("\n-------- %s - Selection Sort --------\n", element);
    printf("Número de operações: %d\nNúmero de Comparações: %d\nNúmero de Trocas: %d\nComplexidade: O(n^2)\nTotal de linhas: %d\n\n", operations, comparation, swaps, size);
}

void heapify(RowData *data, int size, int i, const char *element, int *comparations, int *swaps) {
    const char *v_largest;
    const char *v_left;
    const char *v_right;
    RowData temp;
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size) {
        (*comparations)++;

        if (strcmp(element, "data_pas") == 0) {
            v_largest = data[largest].data_pas;
            v_left = data[left].data_pas;
        } else if (strcmp(element, "bioma") == 0) {
            v_largest = data[largest].bioma;
            v_left = data[left].bioma;
        } else {
            v_largest = data[largest].municipio;
            v_left = data[left].municipio;
        }

        if (strcmp(v_left, v_largest) > 0) {
            largest = left;
        }
    }

    if (right < size) {
        (*comparations)++;

        if (strcmp(element, "data_pas") == 0) {
            v_largest = data[largest].data_pas;
            v_right = data[right].data_pas;
        } else if (strcmp(element, "bioma") == 0) {
            v_largest = data[largest].bioma;
            v_right = data[right].bioma;
        } else {
            v_largest = data[largest].municipio;
            v_right = data[right].municipio;
        }

        if (strcmp(v_right, v_largest) > 0) {
            largest = right;
        }
    }

    if (largest != i) {
        (*swaps)++;
        temp = data[i];
        data[i] = data[largest];
        data[largest] = temp;
        

        heapify(data, size, largest, element, comparations, swaps);
    }
}

void heap_sort(RowData *data, int size, const char *element) {
    RowData temp;
    int comparations = 0;
    int swaps = 0;
    int operations = 0;
    
    for (int i = size / 2 -1; i >= 0; i--) {
        operations++;
        heapify(data, size, i, element, &comparations, &swaps);
    }

    for (int i = size -1; i > 0; i--) {
        operations++;

        temp = data[0];
        data[0] = data[i];
        data[i] = temp;
        swaps++;

        heapify(data, i, 0, element, &comparations, &swaps);
    }

    // File
    FILE *file = fopen("report.txt", "a");
    if (file == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }
    
    fprintf(file, "-------- %s - Heap Sort --------\n", element);
    fprintf(file, "Número de operações: %d\nNúmero de Comparações: %d\nNúmero de Trocas: %d\nComplexidade: O(n^2)\nTotal de linhas: %d\n\n", operations, comparations, swaps, size);
    
    fclose(file);
    
    // Report
    printf("\n-------- %s - Heap Sort --------\n", element);
    printf("Número de operações: %d\nNúmero de Comparações: %d\nNúmero de Trocas: %d\nComplexidade: O(n^2)\nTotal de linhas: %d\n\n", operations, comparations, swaps, size);
}

void merge(RowData *left, int len_left, RowData *right, int len_right, RowData *data, const char *element, int *comparations, int *operations) {
    int i = 0, j = 0, k = 0;
    
    while (i < len_left && j < len_right) {
        (*operations)++;
        (*comparations)++;
        
        const char *v_left, *v_right;
        
        if (strcmp(element, "data_pas") == 0) {
            v_left = left[i].data_pas;
            v_right = right[j].data_pas;
        } else if (strcmp(element, "bioma") == 0) {
            v_left = left[i].bioma;
            v_right = right[j].bioma;
        } else {
            v_left = left[i].municipio;
            v_right = right[j].municipio;
        }
        
        if (strcmp(v_left, v_right) <= 0) {
            data[k] = left[i];
            i++;
        } else {
            data[k] = right[j];
            j++;
        }
        k++;
    }
    
    while (i < len_left) {
        (*operations)++;
        data[k] = left[i];
        i++;
        k++;
    }
    
    while (j < len_right) {
        (*operations)++;
        data[k] = right[j];
        j++;
        k++;
    }
}

void merge_sort(RowData *data, int size, const char *element, int *comparations, int *operations) {
    int middle;
    RowData *left;
    RowData *right;
    
    if (size > 1) {
        middle = size / 2;
        left = malloc(sizeof(RowData) * middle);
        right = malloc(sizeof(RowData) * (size - middle));

        for (int i = 0; i < middle; i++) {
            left[i] = data[i];
        }

        for (int i = 0; i < size - middle; i++) {
            right[i] = data[middle + i];
        }

        merge_sort(left, middle, element, comparations, operations);
        merge_sort(right, size - middle, element, comparations, operations);

        merge(left, middle, right, size - middle, data, element, comparations, operations);

        free(left);
        free(right);
    }
}

void insertion_sort_bucket(RowData *arr, int n, const char *element, int *comparations, int *swaps) {
    RowData key;
    const char *key_value;
    int j;

    for (int i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        
        
        if (strcmp(element, "data_pas") == 0) {
            key_value = key.data_pas;
        } else if (strcmp(element, "bioma") == 0) {
            key_value = key.bioma;
        } else {
            key_value = key.municipio;
        }
        
        while (j >= 0) {
            (*comparations)++;

            const char *current_value;
            if (strcmp(element, "data_pas") == 0) {
                current_value = arr[j].data_pas;
            } else if (strcmp(element, "bioma") == 0) {
                current_value = arr[j].bioma;
            } else {
                current_value = arr[j].municipio;
            }
            
            if (strcmp(current_value, key_value) > 0) {
                arr[j + 1] = arr[j];
                (*swaps)++;
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
}

void bucket_sort(RowData *data, int size, const char *element) {
    int comparations = 0;
    int operations = 0;
    int swaps = 0;
    int numBuckets = 10;
    RowData **buckets = malloc(sizeof(RowData*) * numBuckets);
    int *bucketSizes = calloc(numBuckets, sizeof(int));
    int index = 0;

    if (size <= 1) {
        return;
    }

    for (int i = 0; i < numBuckets; i++) {
        buckets[i] = malloc(sizeof(RowData) * size);
        bucketSizes[i] = 0;
    }

    for (int i = 0; i < size; i++) {
        operations++;
        
        const char *value;
        if (strcmp(element, "data_pas") == 0) {
            value = data[i].data_pas;
        } else if (strcmp(element, "bioma") == 0) {
            value = data[i].bioma;
        } else {
            value = data[i].municipio;
        }
        
        int bucketIndex = ((unsigned char)value[0] * numBuckets) / 256;
        
        if (bucketIndex >= numBuckets) {
            bucketIndex = numBuckets - 1;
        }
        
        buckets[bucketIndex][bucketSizes[bucketIndex]++] = data[i];
    }

    for (int i = 0; i < numBuckets; i++) {
        if (bucketSizes[i] > 0) {
            operations++;
            insertion_sort_bucket(buckets[i], bucketSizes[i], element, &comparations, &swaps);
            comparations += bucketSizes[i];
        }
    }

    
    for (int i = 0; i < numBuckets; i++) {
        for (int j = 0; j < bucketSizes[i]; j++) {
            operations++;
            data[index++] = buckets[i][j];
        }
    }

    for (int i = 0; i < numBuckets; i++) {
        free(buckets[i]);
    }
    free(buckets);
    free(bucketSizes);

    // File
    FILE *file = fopen("report.txt", "a");
    if (file == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }
    
    fprintf(file, "-------- %s - Bucket Sort --------\n", element);
    fprintf(file, "Número de operações: %d\nNúmero de Comparações: %d\nNúmero de Trocas: %d\nComplexidade: O(n + k)\nTotal de linhas: %d\n\n", operations, comparations, swaps, size);
    
    fclose(file);
    
    // Report
    printf("\n-------- %s - Bucket Sort --------\n", element);
    printf("Número de operações: %d\nNúmero de Comparações: %d\nNúmero de Trocas: %d\nComplexidade: O(n + k)\nTotal de linhas: %d\n\n", operations, comparations, swaps, size);

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

    // Enable UTF-8
    SetConsoleOutputCP(CP_UTF8);  
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "pt_BR.UTF-8");

    //Start file
    FILE *file = fopen("report.txt", "w");
    if (file == NULL) {
        printf("Erro ao criar o arquivo.\n");
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
        printf("Escolha o algoritmo de ordenação: \n1.Bubble Sort\n2.Quick Sort\n3.Insertion Sort\n4.Binary Insertion Sort\n5.Selection Sort\n6.Heap Sort\n7.Merge Sort\n8.Bucket Sort (10)\n9.Sair\n");
        scanf("%d", &sort_option);

        if (sort_option >= 9) {
            break;
        }

        data = read_csv(&count);

        if (sort_option == 1) {
            bubble_sort(data, count, element[element_option-1]);
        } else if (sort_option == 2) {
            
            quick_sort(data, count, element[element_option-1]);

            //Write File
            FILE *file = fopen("report.txt", "a");
            if (file == NULL) {
                printf("Erro ao criar o arquivo.\n");
            }

            fprintf(file, "\n-------- %s - Quick Sort --------\n", element[element_option-1]);
            fprintf(file, "Número de operações: %d\nNúmero de Comparações: %d\nNúmero de Partições: %d\nNúmero de Trocas: %d\nComplexidade: O(n log n)\nTotal de linhas: %d\n\n", quick_comparation + quick_partitions + quick_swaps, quick_comparation, quick_partitions, quick_swaps, count);

            fclose(file);

            // show report
            printf("\n-------- %s - Quick Sort --------\n", element[element_option-1]);

            printf("Número de operações: %d\nNúmero de Comparações: %d\nNúmero de Partições: %d\nNúmero de Trocas: %d\nComplexidade: O(n log n)\nTotal de linhas: %d\n\n", quick_comparation + quick_partitions + quick_swaps, quick_comparation, quick_partitions, quick_swaps, count);
        } else if (sort_option == 3) {
            insertion_sort(data, count, element[element_option-1]);
        } else if (sort_option == 4) {
            binary_insertion_sort(data, count, element[element_option-1]);
        } else if (sort_option == 5) {
            selection_sort(data, count, element[element_option-1]);
        } else if (sort_option == 6) {
            heap_sort(data, count, element[element_option-1]);
        } else if (sort_option == 7) {
            int comparations = 0;
            int operations = 0;

            merge_sort(data, count, element[element_option-1], &comparations, &operations);

            // File
            FILE *file = fopen("report.txt", "a");
            if (file == NULL) {
                printf("Erro ao criar o arquivo.\n");
                return;
            }
            
            fprintf(file, "-------- %s - Merge Sort --------\n", element[element_option-1]);
            fprintf(file, "Número de operações: %d\nNúmero de Comparações: %d\nComplexidade: O(n log n)\nTotal de linhas: %d\n\n", operations, comparations, count);
            
            fclose(file);
            
            // Report
            printf("\n-------- %s - Merge Sort --------\n", element[element_option-1]);
            printf("Número de operações: %d\nNúmero de Comparações: %d\nComplexidade: O(n log n)\nTotal de linhas: %d\n\n", operations, comparations, count);
        } else if (sort_option == 8) {
            bucket_sort(data, count, element[element_option-1]);
        }

        //Show data
        showData(data, count);

        
    }

    
    return 0;

}