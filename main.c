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

    //Show all data
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

int main()
{
    RowData *data;
    int count = 0;

    // Enable UTF-8
    SetConsoleOutputCP(CP_UTF8);  
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "pt_BR.UTF-8");

    
    data = read_csv(&count);

    bubble_sort_data(data, count);

    showData(data, count);
    
    return 0;

}