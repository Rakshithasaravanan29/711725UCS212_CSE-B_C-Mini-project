#include <stdio.h>
#include <stdlib.h>

// structure
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
    int pin; // PIN added
};

// function prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void checkBalance(FILE *fPtr); // NEW

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("File could not be opened.\n");
        exit(1);
    }

    while ((choice = enterChoice()) != 6)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;
        case 2:
            updateRecord(cfPtr);
            break;
        case 3:
            newRecord(cfPtr);
            break;
        case 4:
            deleteRecord(cfPtr);
            break;
        case 5:
            checkBalance(cfPtr); //  NEW FEATURE
            break;
        default:
            printf("Invalid choice\n");
        }
    }

    fclose(cfPtr);
    return 0;
}

// MENU
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n====== BANK MENU ======\n");
    printf("1 - Save data to text file\n");
    printf("2 - Update account (PIN required)\n");
    printf("3 - Add new account\n");
    printf("4 - Delete account (PIN required)\n");
    printf("5 - Check Balance (PIN required)\n");
    printf("6 - Exit\n");
    printf("Enter choice: ");

    scanf("%u", &choice);
    return choice;
}

// TEXT FILE
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("File could not be opened.\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr, "%-6s%-16s%-11s%10s\n",
            "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr))
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr, "%-6d%-16s%-11s%10.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);
}

// UPDATE WITH PIN
void updateRecord(FILE *fPtr)
{
    unsigned int acc;
    int enteredPin;
    double amount;
    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter PIN: ");
    scanf("%d", &enteredPin);

    if (enteredPin != client.pin)
    {
        printf("Incorrect PIN! Access denied.\n");
        return;
    }

    printf("Current Balance: %.2f\n", client.balance);
    printf("Enter amount (+ deposit / - withdraw): ");
    scanf("%lf", &amount);

    client.balance += amount;

    if (client.balance < 1000)
    {
        printf("Warning: Balance is below minimum limit!\n");
    }

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Updated Balance: %.2f\n", client.balance);
}

// DELETE WITH PIN
void deleteRecord(FILE *fPtr)
{
    unsigned int acc;
    int enteredPin;
    struct clientData client, blank = {0, "", "", 0, 0};

    printf("Enter account number to delete: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter PIN: ");
    scanf("%d", &enteredPin);

    if (enteredPin != client.pin)
    {
        printf(" Incorrect PIN! Cannot delete.\n");
        return;
    }

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&blank, sizeof(struct clientData), 1, fPtr);

    printf("Account deleted successfully.\n");
}

// ADD NEW WITH PIN
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0, 0};
    unsigned int acc;

    printf("Enter account number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
    }
    else
    {
        printf("Enter lastname firstname balance: ");
        scanf("%14s %9s %lf",
              client.lastName,
              client.firstName,
              &client.balance);

        printf("Set 4-digit PIN: ");
        scanf("%d", &client.pin);

        client.acctNum = acc;

        fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Account created successfully.\n");
    }
}

// CHECK BALANCE WITH PIN
void checkBalance(FILE *fPtr)
{
    unsigned int acc;
    int enteredPin;
    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter PIN: ");
    scanf("%d", &enteredPin);

    if (enteredPin != client.pin)
    {
        printf("Incorrect PIN! Access denied.\n");
        return;
    }

    printf("\n Account Verified!\n");
    printf("Account Number: %d\n", client.acctNum);
    printf("Name: %s %s\n", client.firstName, client.lastName);
    printf("Balance: %.2f\n", client.balance);
}