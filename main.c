#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CHALLANS 1000

// --- Data Structures ---
typedef struct {
    int offenceCode;
    char offenceName[50];
    int baseFine;
} Offence;

typedef struct {
    int challanID;
    char vehicleNumber[15];
    int offenceCode;
    char offenceName[50];
    int fineAmount;
    int isPaid; // 0 = Pending, 1 = Paid
    char area[50]; 
} Challan;

// --- Global Variables ---
Challan database[MAX_CHALLANS];
int totalChallans = 0;
int nextChallanID = 1;

// Core Feature 1: Maintain offences with fine amounts
Offence offenceList[3] = {
    {1, "Speeding", 1000},
    {2, "No_Helmet", 500},
    {3, "Signal_Jump", 1500}
};

// --- File Handling Functions ---
void saveToFile() {
    FILE *fp = fopen("database.txt", "w");
    if (!fp) return;
    fprintf(fp, "%d\n", nextChallanID);
    for (int i = 0; i < totalChallans; i++) {
        fprintf(fp, "%d %s %d %s %d %d %s\n",
            database[i].challanID, database[i].vehicleNumber,
            database[i].offenceCode, database[i].offenceName,
            database[i].fineAmount, database[i].isPaid, database[i].area);
    }
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen("database.txt", "r");
    if (!fp) return;
    if (fscanf(fp, "%d", &nextChallanID) != 1) {
        fclose(fp);
        return;
    }
    while (fscanf(fp, "%d %14s %d %49s %d %d %49s",
            &database[totalChallans].challanID, database[totalChallans].vehicleNumber,
            &database[totalChallans].offenceCode, database[totalChallans].offenceName,
            &database[totalChallans].fineAmount, &database[totalChallans].isPaid,
            database[totalChallans].area) == 7) {
        totalChallans++;
    }
    fclose(fp);
}

// --- Helper Functions ---
int isValidVehicleNumber(char *vehicle) {
    int len = strlen(vehicle);
    if (len < 6 || len > 10) return 0;
    
    for (int i = 0; i < len; i++) {
        vehicle[i] = toupper(vehicle[i]); // Convert to uppercase to prevent case mismatch
        if (!isalnum(vehicle[i])) return 0;
    }
    if (!isalpha(vehicle[0]) || !isalpha(vehicle[1])) return 0; // Must start with state code (e.g., KA)
    
    return 1;
}

int countPastOffences(char *vNum) {
    int count = 0;
    for (int i = 0; i < totalChallans; i++) {
        if (strcmp(database[i].vehicleNumber, vNum) == 0) {
            count++;
        }
    }
    return count;
}

// --- Core Feature Functions ---
void issueChallan() {
    if (totalChallans >= MAX_CHALLANS) {
        printf("Database full!\n");
        return;
    }
    
    char vNum[15];
    printf("\nEnter Vehicle Number (e.g., KA07AB1234): ");
    scanf("%14s", vNum);

    // Core Feature 2: Validation
    if (!isValidVehicleNumber(vNum)) {
        printf("Error: Invalid format. Must be 6-10 alphanumeric characters starting with state code.\n");
        return;
    }

    printf("\nAvailable Offences:\n");
    for (int i = 0; i < 3; i++) {
        printf("%d. %s (Base Fine: Rs %d)\n", offenceList[i].offenceCode, offenceList[i].offenceName, offenceList[i].baseFine);
    }
    
    int code;
    printf("Enter Offence Code: ");
    scanf("%d", &code);

    int baseFine = 0;
    char offName[50] = "";
    for (int i = 0; i < 3; i++) {
        if (offenceList[i].offenceCode == code) {
            baseFine = offenceList[i].baseFine;
            strcpy(offName, offenceList[i].offenceName);
            break;
        }
    }
    if (baseFine == 0) {
        printf("Invalid offence code!\n");
        return;
    }

    char area[50];
    printf("Enter Area of Offence (no spaces): ");
    scanf("%49s", area);

    // Core Feature 3: Repeat Offender Fine Increase
    int pastCount = countPastOffences(vNum);
    int finalFine = baseFine;
    if (pastCount > 0) {
        finalFine = baseFine + (baseFine * 0.5 * pastCount); // 50% increase for each past offence
        printf("\n** Notice: Repeat offender! Fine multiplied based on history. **\n");
    }

    // Save record
    Challan newC;
    newC.challanID = nextChallanID++;
    strcpy(newC.vehicleNumber, vNum);
    newC.offenceCode = code;
    strcpy(newC.offenceName, offName);
    newC.fineAmount = finalFine;
    newC.isPaid = 0;
    strcpy(newC.area, area);

    database[totalChallans++] = newC;
    saveToFile();

    printf("\nChallan issued successfully!\nID: %d | Vehicle: %s | Fine: Rs %d\n", newC.challanID, newC.vehicleNumber, newC.fineAmount);

    // Stretch Feature 1: Suspend Licence
    if (pastCount + 1 >= 5) {
        printf("\nURGENT ALERT: This vehicle has 5 or more offences. Licence immediately suspended!\n");
    }
}

void recordPayment() {
    int id;
    printf("\nEnter Challan ID to pay: ");
    scanf("%d", &id);
    
    // Core Feature 4: Record Payment
    for (int i = 0; i < totalChallans; i++) {
        if (database[i].challanID == id) {
            if (database[i].isPaid) {
                printf("Error: This challan is already paid.\n");
                return;
            }
            database[i].isPaid = 1;
            saveToFile();
            printf("Payment of Rs %d recorded successfully for Challan ID %d.\n", database[i].fineAmount, id);
            return;
        }
    }
    printf("Challan ID not found.\n");
}

void listPending() {
    char vNum[15];
    printf("\nEnter Vehicle Number: ");
    scanf("%14s", vNum);
    for (int i = 0; vNum[i]; i++) vNum[i] = toupper(vNum[i]);

    // Core Feature 5: List pending and total due
    int totalDue = 0;
    int found = 0;
    printf("\n--- Pending Challans for %s ---\n", vNum);
    
    for (int i = 0; i < totalChallans; i++) {
        if (strcmp(database[i].vehicleNumber, vNum) == 0 && database[i].isPaid == 0) {
            printf("ID: %d | Offence: %s | Fine: Rs %d\n", database[i].challanID, database[i].offenceName, database[i].fineAmount);
            totalDue += database[i].fineAmount;
            found = 1;
        }
    }
    
    if (!found) {
        printf("No pending challans found for this vehicle.\n");
    } else {
        printf("----------------------------------\n");
        printf("TOTAL AMOUNT DUE: Rs %d\n", totalDue);
    }
}

void reportByArea() {
    char searchArea[50];
    printf("\nEnter Area Name to generate report: ");
    scanf("%49s", searchArea);

    // Stretch Feature 2: Report most common offences by area
    int sCount = 0, hCount = 0, jCount = 0;
    for (int i = 0; i < totalChallans; i++) {
        if (strcmp(database[i].area, searchArea) == 0) {
            if (database[i].offenceCode == 1) sCount++;
            else if (database[i].offenceCode == 2) hCount++;
            else if (database[i].offenceCode == 3) jCount++;
        }
    }
    printf("\n--- Offence Report for %s ---\n", searchArea);
    printf("Speeding: %d times\n", sCount);
    printf("No Helmet: %d times\n", hCount);
    printf("Signal Jump: %d times\n", jCount);
}

// --- Main Menu ---
int main() {
    loadFromFile();
    int choice;
    
    while (1) {
        printf("\n=== ChallanTrack: Traffic Management System ===\n");
        printf("1. Maintain Offences (View Standard Fines)\n");
        printf("2. Issue New Challan\n");
        printf("3. Record Payment\n");
        printf("4. List Pending Challans by Vehicle\n");
        printf("5. Area Offence Report (Stretch Feature)\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        
        // Input validation to prevent infinite loops on char inputs
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); 
            continue;
        }

        switch (choice) {
            case 1:
                printf("\n--- Predefined Offences ---\n");
                for (int i = 0; i < 3; i++) {
                    printf("%d. %s - Rs %d\n", offenceList[i].offenceCode, offenceList[i].offenceName, offenceList[i].baseFine);
                }
                break;
            case 2: issueChallan(); break;
            case 3: recordPayment(); break;
            case 4: listPending(); break;
            case 5: reportByArea(); break;
            case 6:
                printf("Saving database and exiting...\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}