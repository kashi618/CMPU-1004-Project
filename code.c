#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// Symbolic Names
#define SIZE        50  // Maximum string length

#define LINESIZE    10  // Sets size of each line
#define MASTERSIZE  40  // Sets size for masterline, containing all lines
#define BUFFERSIZE  200 // Maximum size of a single line in a file

#define NUMVAN      5   // How many vans are available
#define WEIGHTLIMIT 400 // Maximum weight for one van

// Structure Tags
struct date
{
    int day;
    int hour;
    int minute;
};
struct product
{
    int lineCode;
    int batchCode;
    struct date batchDate;
    int productId;
    char productName[SIZE];
    char targetEngineCode[SIZE];
    int binNumber;
    int weight;
    float price;
};

// Function Signatures
void readCSV(FILE *, struct product[], int);
void mergesort(struct product[], int, int);
void merge(struct product[], int, int, int);
void mergeLines(struct product[], struct product[], struct product[], struct product[], struct product[]);
void binarySearch(struct product[], int);
void showStructVar(struct product[], int, int, int);
void vanReport(struct product[], struct product[], struct product[], struct product[], struct product[], struct product[], int[], int[]);

// Main function 
int main(void) {
    struct product L1[LINESIZE], L2[LINESIZE], L3[LINESIZE], L4[LINESIZE], masterLine[LINESIZE * 4];
    struct product van1[LINESIZE], van2[LINESIZE], van3[LINESIZE], van4[LINESIZE], van5[LINESIZE];
    struct product temp;

    // Create file poiinter, and open files for reading
    FILE *fpLine1 = fopen("line1.csv", "r");
    FILE *fpLine2 = fopen("line2.csv", "r");
    FILE *fpLine3 = fopen("line3.csv", "r");
    FILE *fpLine4 = fopen("line4.csv", "r");

    // Check if all files has been opened successfully
    if (fpLine1 == NULL || fpLine2 == NULL || fpLine3 == NULL || fpLine4 == NULL) {
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

    // Read and store data into structures
    readCSV(fpLine1, L1, 1);
    readCSV(fpLine2, L2, 2);
    readCSV(fpLine3, L3, 3);
    readCSV(fpLine4, L4, 4);

    // Show contents of CSV
    showStructVar(L1, 1, LINESIZE, 0);
    showStructVar(L2, 2, LINESIZE, 0);
    showStructVar(L3, 3, LINESIZE, 0);
    showStructVar(L4, 4, LINESIZE, 0);

    printf("\nPress (enter) to continue: "); while(getchar() != '\n');

    // Sort each line using mergesort O(log(n))
    printf("\nSorting each line...");
    mergesort(L1,0,LINESIZE-1);
    mergesort(L2,0,LINESIZE-1);
    mergesort(L3,0,LINESIZE-1);
    mergesort(L4,0,LINESIZE-1);
    printf("\nFinished sorting\n");

    //showStructVar(struct product line[], int index, int size, int showAll)
    printf("\nPress (enter) to show weights:\n"); while(getchar() != '\n');
    showStructVar(L1, 1, LINESIZE, 1);
    showStructVar(L2, 2, LINESIZE, 1);
    showStructVar(L3, 3, LINESIZE, 1);
    showStructVar(L4, 4, LINESIZE, 1);

    // Merge 4 sorted lines
    printf("\nMerging lines...");
    mergeLines(L1,L2,L3,L4,masterLine);
    printf("\nFinished merging lines\n");

    printf("\nPress (enter) to show merged weights"); while(getchar() != '\n');
    showStructVar(masterLine, 0, MASTERSIZE, 1);

    // Ask user to search for product by weight
    while (1) {
        char find;

        printf("\nDo you want to find a product? (Y/N)\n");
        printf("-> ");
        scanf("%c", &find);

        if (find == 'y' || find == 'Y') {
            binarySearch(masterLine, MASTERSIZE);
        }
        else if (find == 'n' || find == 'N') {
            while(getchar() != '\n');
            break;
        }
        else {
            printf("Input not recognised. Please try again.\n\n");
        }
    }

    // Generate van report
    int vanCount[NUMVAN];
    int totalWeight[NUMVAN];
    vanReport(masterLine, van1, van2, van3, van4, van5, vanCount, totalWeight);

    // Prints report if weight limit hasn't been hit (when all products have been asssigned)
    if ((vanCount[0]+vanCount[1]+vanCount[2]+vanCount[3]+vanCount[4])==MASTERSIZE) {
        printf("\nPress (enter) to view van report (van 1): "); while(getchar() != '\n');
        showStructVar(van1, 0, vanCount[0], 0);
        printf("Number of Items: %d", vanCount[0]);
        printf("\nTotal Weight:    %d", totalWeight[0]);
    
        printf("\nPress (enter) to view van report (van 2): "); while(getchar() != '\n');
        showStructVar(van2, 0, vanCount[1], 0);
        printf("Number of Items: %d", vanCount[1]);
        printf("\nTotal Weight:    %d", totalWeight[1]);
    
        printf("\nPress (enter) to view van report (van 3): "); while(getchar() != '\n');
        showStructVar(van3, 0, vanCount[2], 0);
        printf("Number of Items: %d", vanCount[2]);
        printf("\nTotal Weight:    %d", totalWeight[2]);
    
        printf("\nPress (enter) to view van report (van 4): "); while(getchar() != '\n');
        showStructVar(van4, 0, vanCount[3], 0);
        printf("Number of Items: %d", vanCount[3]);
        printf("\nTotal Weight:    %d", totalWeight[3]);
    
        printf("\nPress (enter) to view van report (van54): "); while(getchar() != '\n');
        showStructVar(van5, 0, vanCount[4], 0);
        printf("Number of Items: %d", vanCount[4]);
        printf("\nTotal Weight:    %d", totalWeight[4]);
    }


    // Closes files
    fclose(fpLine1);
    fclose(fpLine2);
    fclose(fpLine3);
    fclose(fpLine4);

    printf("\nExiting program...\n");
    return 0;
} // END main()

// Stores data from csv to structure
void readCSV(FILE *fpLine, struct product line[], int x)
{
    char buffer[BUFFERSIZE];
    char *elementPtr;
    int loop = 0;

    // Read each line in file
    while (fgets(buffer, BUFFERSIZE, fpLine) != NULL) {
        buffer[strlen(buffer) - 1] = '\0'; // Remove '\n' added by fgets

        // Find first character up to the comma ','
        elementPtr = strtok(buffer, ",");
        line[loop].lineCode = atoi(elementPtr); // lineCode

        elementPtr = strtok(NULL, ",");
        line[loop].batchCode = atoi(elementPtr); // batchCode

        elementPtr = strtok(NULL, ",");
        line[loop].batchDate.day = atoi(elementPtr); // day

        elementPtr = strtok(NULL, ",");
        line[loop].batchDate.hour = atoi(elementPtr); // hour

        elementPtr = strtok(NULL, ",");
        line[loop].batchDate.minute = atoi(elementPtr); // minute

        elementPtr = strtok(NULL, ",");
        line[loop].productId = atoi(elementPtr); // productId

        elementPtr = strtok(NULL, ",");
        strcpy(line[loop].productName, elementPtr); // productName

        elementPtr = strtok(NULL, ",");
        strcpy(line[loop].targetEngineCode, elementPtr); // targetEngineCode

        elementPtr = strtok(NULL, ","); // binNumber
        line[loop].binNumber = atoi(elementPtr);

        elementPtr = strtok(NULL, ","); // weight
        line[loop].weight = atoi(elementPtr);

        elementPtr = strtok(NULL, ","); // price
        line[loop].price = atof(elementPtr);

        loop++;
    } // END while
}

// Sorts the 4 lines
void mergesort(struct product line[LINESIZE], int left, int right) {   
    int middle;

    // Find the middle
    if (left < right) {
        middle = left + (right - left) / 2;

        // Sort first and second half
        mergesort(line, left, middle);
        mergesort(line, middle+1, right);

        // Merge the two halves
        merge(line, left, middle, right);
    } // END if
} // END function

void merge(struct product line[LINESIZE], int left, int middle, int right) {
    int i;
    int j;
    int k;

    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Create temporary structures for storing values
    struct product tempLeft[LINESIZE];
    struct product tempRight[LINESIZE];

    // Copy data from lines to temp structs
    for (i=0; i<n1; i++) {
        tempLeft[i] = line[left + i];
    }
    for (i=0; i<n2; i++) { 
        tempRight[i] = line[middle + i + 1];
    }

    // Merge temp structs
    i = 0;
    j = 0;
    k = left;


    while (i<n1 && j<n2) {
        if (tempLeft[i].weight <= tempRight[j].weight) {
            line[k] = tempLeft[i];
            i++;
        }
        else {
            line[k] = tempRight[j];
            j++;
        }
        k++;
    } // END while

    while (i<n1) {
        line[k] = tempLeft[i];
        i++;
        k++;
    } // END while

    while (j<n2) {
        line[k] = tempRight[j];
        j++;
        k++;
    } // END while
} // END function

// Merges all 4 lines
void mergeLines(struct product L1[LINESIZE], struct product L2[LINESIZE], struct product L3[LINESIZE], struct product L4[LINESIZE], struct product masterLine[MASTERSIZE]) {
    int n1 = 0;
    int n2 = 0;
    int n3 = 0;
    int n4 = 0;
    int min;
    int minList;

    // Loop through each the nth element of each line, and find the min
    for (int i=0; i<MASTERSIZE; i++) {
        min = INT_MAX;

        // Compare current elements of all 4 lines
        if (L1[n1].weight < min && n1 < LINESIZE) {
            min = L1[n1].weight;
            minList = 1;
        }
        if (L2[n2].weight < min && n2 < LINESIZE) {
            min = L2[n2].weight;
            minList = 2;
        }
        if (L3[n3].weight < min && n3 < LINESIZE) {
            min = L3[n3].weight;
            minList = 3;
        }
        if (L4[n4].weight < min && n4 < LINESIZE) {
            min = L4[n4].weight;
            minList = 4;
        }

        // Append that min value to the master line, then move that line up by 1
        if (minList == 1) {
            masterLine[i] = L1[n1];
            n1++;
        }
        else if (minList == 2) {
            masterLine[i] = L2[n2];
            n2++;
        }
        else if (minList == 3) {
            masterLine[i] = L3[n3];
            n3++;
        }
        else if (minList == 4) {
            masterLine[i] = L4[n4];
            n4++;
        }
    } // END for
} // END function

// Search for car part by weight
void binarySearch(struct product line[], int size) {
    int left;
    int right;
    int mid;
    int target;
    int targetIndex;

    // Ask user to enter a weight
    printf("\nEnter weight to find\n");
    printf("-> ");
    scanf("%d", &target);
    while(getchar() != '\n'); //clear input buffer

    // Set boundaries for binary search
    left = 0;
    right = size-1;
    targetIndex = -1;

    while (left <= right) {
        mid = left + (right-left)/2;

        // Regular binary search
        if (line[mid].weight == target) {
            targetIndex = mid;
            break;
        }
        else if (line[mid].weight < target) {
            left = mid+1; // Search the right half
        }
        else {
            right = mid-1; // Search the left half
        }
    } // END while

    // Tell user if weight has been found or not
    if (targetIndex == -1) {
        printf("\nTarget weight %d not found\n", target);
    }
    else {
        printf("\nTarget weight %d FOUND\n",target);
        showStructVar(line, targetIndex, MASTERSIZE, 2); 
    }
} // END function

void showStructVar(struct product line[], int index, int size, int type) {
    // This shows structures in CSV format
    if (type == 0) {
        printf("\nlineCode,batchCode,Day,Hour,minute,productId,productName,targetEngineCode,binNumber,weight,price\n");
        for (int i=0; i<size; i++) {
            printf("%d,",  line[i].lineCode);
            printf("%d,",  line[i].batchCode);
            printf("%d,",  line[i].batchDate.day);
            printf("%d,",  line[i].batchDate.hour);
            printf("%d,",  line[i].batchDate.minute);
            printf("%d,",  line[i].productId);
            printf("%s,",  line[i].productName);
            printf("%s,",  line[i].targetEngineCode);
            printf("%d,",  line[i].binNumber);
            printf("%d,",  line[i].weight);
            printf("%.2f\n",line[i].price);
        }
    }
    // This shows the weights for a line
    else if (type == 1) {
        if (index==0) {
            printf("Weights for masterLine (all 4 lines merged):\n");
        }
        else {
            printf("Weights for line %d: ", index);    
        }
        for (int i=0; i<size; i++) {
            printf("%d ",  line[i].weight);
        }
        printf("\n");
    }
    // Show info for specific index in structure array
    else if (type == 2) {
        printf("Line code:          %d\n",  line[index].lineCode);
        printf("Batch code:         %d\n",  line[index].batchCode);
        printf("Day:                %d\n",  line[index].batchDate.day);
        printf("Hour:               %d\n",  line[index].batchDate.hour);
        printf("Minute:             %d\n",  line[index].batchDate.minute);
        printf("Product ID:         %d\n",  line[index].productId);
        printf("Product name:       %s\n",  line[index].productName);
        printf("Target engine code: %s\n",  line[index].targetEngineCode);
        printf("Bin number:         %d\n",  line[index].binNumber);
        printf("Weight:             %d\n",  line[index].weight);
        printf("price:              %.2f\n",line[index].price);
    }
} // END function

// Create report for storing products inside the vans
void vanReport(struct product line[], struct product van1[], struct product van2[], struct product van3[], struct product van4[], struct product van5[], int vanCount[], int totalWeight[]) {
    int vanIndex[NUMVAN] = {0};

    printf("\nGenerating van report...\n");

    // Goes through each product in the masterline
    for (int i=0; i<MASTERSIZE;) {
        // Adds the current product to van 1 if weight limit has not been exceeded
        if (totalWeight[0]+line[i].weight <= WEIGHTLIMIT) {
            van1[vanIndex[0]] = line[i];
            totalWeight[0] += line[i].weight;

            vanIndex[0]++;
            vanCount[0]++;
            i++;
        }
        // Adds the current product to van 2 if weight limit has not been exceeded
        else if (totalWeight[1]+line[i].weight < WEIGHTLIMIT) {
            van2[vanIndex[1]] = line[i];
            totalWeight[1] += line[i].weight;

            vanIndex[1]++;
            vanCount[1]++;
            i++;
        }
        // Adds the current product to van 3 if weight limit has not been exceeded
        else if (totalWeight[2]+line[i].weight < WEIGHTLIMIT) {
            van3[vanIndex[2]] = line[i];
            totalWeight[2] += line[i].weight;

            vanIndex[2]++;
            vanCount[2]++;
            i++;
        }
        // Adds the current product to van 4 if weight limit has not been exceeded
        else if (totalWeight[3]+line[i].weight < WEIGHTLIMIT) {
            van4[vanIndex[3]] = line[i];
            totalWeight[3] += line[i].weight;

            vanIndex[3]++;
            vanCount[3]++;
            i++;
        }
        // Adds the current product to van 5 if weight limit has not been exceeded
        else if (totalWeight[4]+line[i].weight < WEIGHTLIMIT) {
            van5[vanIndex[4]] = line[i];
            totalWeight[4] += line[i].weight;

            vanIndex[4]++;
            vanCount[4]++;
            i++;
        }
        // Alternative case for when no van can store product without exceeding weight limit
        else { 
            printf("\nWeight limit %d is too small, no possible van report possible");
            printf("Please increase weight limit or number of vans\n", WEIGHTLIMIT);
            return;
        }
    } // END for

    printf("Van report generated!\n");    
} // END function