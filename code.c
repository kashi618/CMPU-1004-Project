#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// Symbolic Names
#define SIZE 50         // Sets maximum length for strings
#define LINESIZE 10     // Sets size of each line
#define BUFFERSIZE 200  // Maximum size of a line in a file


// Structure Tags
struct date {
    int day;
    int hour;
    int minute;
};
struct product {
    int lineCode;
    int batchCode;
    struct date batchDate;
    int productId;
    char productName[SIZE];
    char targetEngineCode[SIZE];
    int binNumber;
    float weight;
    float price;
};


// Function Signatures
void readCSV(FILE*, struct product[]);

// Main function 
int main(void) {
    struct product L1[LINESIZE], L2[LINESIZE], L3[LINESIZE], L4[LINESIZE], masterLine[LINESIZE*4];

    // Create file poiinter, and open files for reading
    FILE* fpLine1 = fopen("line1.csv","r");
    FILE* fpLine2 = fopen("line2.csv","r");
    FILE* fpLine3 = fopen("line3.csv","r");
    FILE* fpLine4 = fopen("line4.csv","r");

    // Check if all files has been opened successfully
    if (fpLine1==NULL || fpLine2==NULL || fpLine3==NULL || fpLine4==NULL) {
        printf("\nError opening files");
        printf("\nExiting program...");
        return 0;
    }
    else {
        printf("Successfully opened files for reading\n");
    }

    // Skip past header, for the CSV files
    fseek(fpLine1, 97, SEEK_SET);
    fseek(fpLine2, 97, SEEK_SET);
    fseek(fpLine3, 97, SEEK_SET);
    fseek(fpLine4, 97, SEEK_SET);


    // Read CSV
    readCSV(fpLine1, L1);

    // Closes files
    printf("Closing files\n");
    fclose(fpLine1);
    fclose(fpLine2);
    fclose(fpLine3);
    fclose(fpLine4);

    printf("Exiting program...\n");
    return 0;
} // END main()

// Stores data from csv to structure
void readCSV(FILE *fpLine, struct product line[LINESIZE]) {
    char buffer[BUFFERSIZE];
    char* elementPtr;
    int loop=0;

    // Read each line in file
    while (fgets(buffer, BUFFERSIZE, fpLine) != NULL) {
        buffer[strlen(buffer)-1] = '\0'; // Remove '\n' added by fgets

        // Show current working line
        //printf("%s\n",buffer);

        // Find first character up to the comma ','
        elementPtr = strtok(buffer,",");
        line[loop].lineCode = atoi(elementPtr); // lineCode
        
        elementPtr = strtok(NULL,",");
        line[loop].batchCode = atoi(elementPtr); // batchCode

        elementPtr = strtok(NULL,",");
        line[loop].batchDate.day = atoi(elementPtr); // day

        elementPtr = strtok(NULL,",");
        line[loop].batchDate.minute = atoi(elementPtr); // hour

        elementPtr = strtok(NULL,",");
        line[loop].batchDate.hour = atoi(elementPtr); // minute

        elementPtr = strtok(NULL,",");
        line[loop].productId = atoi(elementPtr); // productId

        elementPtr = strtok(NULL,",");
        strcpy(line[loop].productName, elementPtr); // productName

        elementPtr = strtok(NULL,",");
        strcpy(line[loop].targetEngineCode, elementPtr); // targetEngineCode

        elementPtr = strtok(NULL,","); // binNumber
        line[loop].binNumber = atoi(elementPtr);

        elementPtr = strtok(NULL,","); // weight
        line[loop].binNumber = atof(elementPtr);

        elementPtr = strtok(NULL,","); // price
        line[loop].binNumber = atof(elementPtr);

        loop++;
    }

}

/*
struct product {
    int lineCode;
    int batchCode;
    struct batchDate {
        int day;
        int hour;
        int minute;
    };
    int productId;
    char productName[SIZE];
    char targetEngineCode[SIZE];
    int binNumber;
    float weight;
    float price;
};
*/