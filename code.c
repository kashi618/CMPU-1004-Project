#include <stdio.h>
#include <string.h>


// Symbolic Names
#define SIZE 50         // Sets maximum length for strings
#define LINESIZE 10     // Sets size of each line
#define BUFFERSIZE 200  // Maximum size of a line in a file


// Structure Tags
struct batchDate {
    int day;
    int hour;
    int minute;
};
struct product {
    int lineCode;
    int batchCode;
    int batchDate; // day, hour, minute
    int productId;
    char productName[SIZE];
    char targetEngineCode[SIZE];
    int binNumber;
    float weight;
    float price;
};


// Function Signatures
void readCSV(FILE*);

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
    readCSV(fpLine1);
   


    /*
    // Cycles through each line, storing values into its respective structure
    for (int line=1; line<4; line++) {
        for (int i=0; i<LINESIZE; i++) {
            switch (line) {
                case 1:
                    //...
                    while (i) {


                    }


                    break;
        
                case 2:
                    //...
                    break;
                
                case 3:
                    //...
                    break;

                case 4:
                    //...
                    break;
            } // END SWITCH
        } // END FOR (i)
    } // END FOR (line)
    */

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
void readCSV(FILE *fpLine) {
    char buffer[BUFFERSIZE];
    char* elementPtr;

    /*
    fgets(buffer, 100, fpLine);
    buffer[strlen(buffer)-1] = '\0';

    printf("%s",buffer);
    */

    // Read each line in file
    while (fgets(buffer, BUFFERSIZE, fpLine) != NULL) {
        buffer[strlen(buffer)-1] = '\0'; // Remove '\n' added by fgets
        printf("%s\n",buffer);

        elementPtr = strtok(buffer,",");
        printf("%s\n", elementPtr);
        for (int i=0; i<11; i++) {
            elementPtr = strtok(NULL,",");
            printf("%s\n", elementPtr);
        }

    }
}