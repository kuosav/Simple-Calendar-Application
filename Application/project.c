#include "project.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


int main(void) {
    
    int listLen = 0;
    char buffer[80];
    int repeat = 1;
    char *ptr;

    Entry *calendar = malloc(sizeof(NULL));
    if (calendar==NULL) {
        printf("Malloc fail.\n");
        repeat = 0;
    }

    printf("Welcome!\nAccepted commands:\n"
            "ADD A NEW ENTRY:                A description month day hour\n"
            "REMOVE AN EXISTING ENTRY:       D month day hour\n"
            "SAVE CALENDAR TO A FILE:        W filename\n"
            "DOWNLOAD CALENDAR FROM A FILE:  O filename\n"
            "PRINT CALENDAR:                 L\n"
            "QUIT:                           Q\n"
            "Please give time and date as numbers.\n");

    while (repeat) {

        char *ret = fgets(buffer, 80, stdin);
        if (ret==NULL) {
            printf("End of file.\n");
            freeList(calendar, listLen);
            break;
        }
        
        buffer[strlen(buffer)-1] = '\0';

        if (strlen(buffer)>1 && buffer[1]!=' ') {
            printf("Invalid input, more than one character.\n");
            continue;
        }
        
        ptr = &buffer[2];

        switch(buffer[0]) {
            case 'A': ;
                Entry *newList_A = addEntry(calendar, listLen, ptr);
                if (newList_A!=NULL) {
                    listLen++;
                    calendar = newList_A;
                }
                break;
            case 'D': ;
                Entry *newList_D = removeEntry(calendar, listLen, ptr);
                if (newList_D != NULL) {
                    listLen--;
                    calendar = newList_D;
                }
                break;
            case 'L':
                if (strlen(buffer)>2) {
                    printf("Invalid input, too many letters.\n");
                    break;
                }
                printCalendar(calendar, listLen);
                break;

            case 'W': ;
                saveCalendar(calendar, listLen, ptr);
                break;
            case 'O': ;
                int prevListLen = listLen;
                Entry *newList_O = loadCalendar(ptr, &listLen);
                if (newList_O!=NULL) {
                    freeList(calendar, prevListLen);
                    calendar = newList_O;
                }
                break;
            case 'Q':
                if (strlen(buffer)>2) {
                    printf("Invalid input, too many characters.\n");
                    break;
                }
                repeat = 0;
                freeList(calendar, listLen);
                printf("Thank you for using calendar.\n");
                break;
            default:
                printf("Unknown command, please try again.\n");
        }
    }
    return 0;
}


/*
Adds a new entry to the calendar. Parameter 'input' is
the user's command, without the letter A.
If successful, returns the updated list.
Else, returns NULL.
*/
Entry *addEntry(Entry *calendar, int listLen, char *input) {

    char *ptr = strtok(input, " ");
    char *pieces[4];
    int i = 0;
    while (i<4 && ptr!=NULL) {
        pieces[i] = ptr;
        ptr = strtok(NULL, " ");
        i++;
    }
    if (ptr!=NULL || i!=4) {
        printf("Invalid input, wrong number of parameters.\n");
        return NULL;
    }
    
    int date[3];
    for (i = 1; i<4; i++) {
        int asNumber = toNumber(pieces[i]);
        if (asNumber==-1) {
            printf("Invalid parameters for adding a new entry.\n");
            return NULL;
        }
        date[i-1] = asNumber;
    }

    if (date[0]<1 || date[0]>12 || date[1]<1 || date[1]>31 || date[2]<0 || date[2]>23) {
        printf("Given date is invalid.\n");
        return NULL;
    }

    for (int i = 0; i<listLen; i++) {
        if (calendar[i].month==date[0] && calendar[i].day==date[1] && calendar[i].hour==date[2]) {
            printf("Given date is already taken!\n");
            return NULL;
        }
    }
    Entry newEntry;
    newEntry.description = malloc((strlen(pieces[0])+1)*sizeof(char));
    if (newEntry.description==NULL) {
        printf("Malloc fail.\n");
        return NULL;
    }
    strcpy(newEntry.description, pieces[0]);
    newEntry.month = date[0];
    newEntry.day = date[1];
    newEntry.hour = date[2];
    Entry *tmp = realloc(calendar, (listLen+1)*sizeof(Entry));
    if (tmp == NULL) {
        printf("Realloc fail.\n");
        return NULL;
    }
    tmp[listLen] = newEntry;
    printf("Entry added successfully.\n");
    return tmp;
}


/*
Removes an existing entry from the list. Parameter 'input'
is the user's command without the letter 'D'.
If successful, returns the updated list.
Else, returns NULL.
*/
Entry *removeEntry(Entry *calendar, int listLen, char *input) {

    char *ptr = strtok(input, " ");
    int i = 0;
    int date[3];
    while (i<3 && ptr!=NULL) {
        int asNumber = toNumber(ptr);
        if (asNumber==-1) {
            printf("Invalid parameters for adding a new entry.\n");
            return NULL;
        }
        date[i] = asNumber;
        ptr = strtok(NULL, " ");
        i++;
    }
    if (ptr!=NULL || i!=3) {
        printf("Invalid input, wrong number of parameters.\n");
        return NULL;
    }
    
    if (date[0]<1 || date[0]>12 || date[1]<1 || date[1]>31 || date[2]<0 || date[2]>23) {
        printf("Given date is invalid.\n");
        return NULL;
    }
    int removeIndex = -1;
    for (int i = 0; i<listLen; i++) {
        if (calendar[i].month==date[0] && calendar[i].day==date[1] && calendar[i].hour==date[2]) {
            free(calendar[i].description);
            removeIndex = i;
            break;
        }
    }
    if (removeIndex == -1) {
        printf("Given date is already free.\n");
        return NULL;
    }

    for (int j = removeIndex; j<listLen-1; j++) {
        memcpy(&calendar[j], &calendar[j+1], sizeof(Entry));
    }
    Entry *tmp;

    if (listLen==1) {
        tmp = realloc(calendar, sizeof(NULL));
    } else {
        tmp = realloc(calendar, (listLen-1)*sizeof(Entry));
    }
    printf("Entry removed successfully.\n");
    return tmp;
}

int printCalendar(Entry* calendar, int listLen) {
    if (listLen==0) {
        printf("Your calendar is empty.\n");
        return -1;
    }

    qsort(calendar, listLen, sizeof(Entry), compareDates);

    for (int i = 0; i<listLen; i++) {
        printf("%s %.2d.%.2d. at %.2d\n", calendar[i].description, calendar[i].day, calendar[i].month, calendar[i].hour);
    }
    return 0;
}

/*
Saves the calendar to a file.
If the file is not empty, its contents will be overwritten.
*/
int saveCalendar(Entry *calendar, int listLen, char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Cannot open file \"%s\".\n", filename);
        return -1;
    }
    for (int i = 0; i<listLen; i++) {
        if (fprintf(f, "%s %d %d %d\n", calendar[i].description, calendar[i].month, calendar[i].day, calendar[i].hour)==EOF) {
            printf("Saving calendar failed.");
            break;
        }
    }
    printf("Calendar saved!\n");
    fclose(f);
    return 0;
}

/*
Downloads the calendar from file. If there are any entries in
the current calendar, they will be replaced.Q
If successful, returns a new calendar (i.e. a list of entries).
Otherwise, returns NULL.
*/
Entry *loadCalendar(char *filename, int *listLen) {

    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("cannot open file \"%s\".\n", filename);
        return NULL;
    }

    char currentLine[80];
    Entry *newCalendar = malloc(sizeof(NULL));
    int length = 0;
    int counter = 0;

    while (fgets(currentLine, 80, f) != NULL) {
        if (currentLine[strlen(currentLine)-1] == '\n') {
            currentLine[strlen(currentLine)-1] = '\0';
        }
        counter++;
        printf("%d... ", counter);
        Entry *tmpCalendar = addEntry(newCalendar, length, currentLine);
        if (tmpCalendar!= NULL) {
            newCalendar = tmpCalendar;
            length++;
        }
    }
    *listLen = length;
    printf("%d out of %d entries added to your calendar.\n", length, counter);
    fclose(f);
    return newCalendar;
}

//frees all allocated memory
void freeList(Entry *calendar, int listLen) {
    for (int i = 0; i<listLen; i++) {
        free(calendar[i].description);
    }
    free(calendar);
}

/*
Helper function that converts strings to integers.
Returns the input string as integer if the input is valid (i.e. input is 1-2 digits,
anything more than that is unnecessary since this function is used for converting date numbers to integers).
Otherwise returns -1.
*/
int toNumber(char *numberAsString) {
    if (strlen(numberAsString)==2 && isdigit(numberAsString[0]) && isdigit(numberAsString[1])) {
        return 10*(numberAsString[0]-'0') + (numberAsString[1]-'0');
    } else if (strlen(numberAsString)==1 && isdigit(numberAsString[0])) {
        return numberAsString[0]-'0';
    } else {
        return -1;
    }
}

//compare function for sorting entries by date
int compareDates(const void* a, const void* b) {
    
    const Entry *entry_a = a;
    const Entry *entry_b = b;

    if (entry_a->month>entry_b->month) {
        return 1;
    } else if (entry_a->month<entry_b->month) {
        return -1;
    }
    
    if (entry_a->day>entry_b->day) {
        return 1;
    } else if (entry_a->day<entry_b->day) {
        return -1;
    }
    
    if (entry_a->hour>entry_b->day) {
        return 1;
    } else if (entry_a->hour<entry_b->day) {
        return -1;
    }

    return strcmp(entry_a->description, entry_b->description);
}