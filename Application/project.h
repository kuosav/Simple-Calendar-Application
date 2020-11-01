#ifndef PROJECT_HEADER
#define PROJECT_HEADER

typedef struct {
    char *description;
    int month, day, hour;
} Entry;

Entry *addEntry(Entry *calendar, int listLen, char *input);

Entry *removeEntry(Entry *calendar, int listLen, char *input);

Entry *loadCalendar(char *filename, int *listLen);

int saveCalendar(Entry *calendar, int listLen, char *filename);

int printCalendar(Entry *calendar, int listLen);

void freeList(Entry *calendar, int listLen);

int toNumber(char *numberAsString);

int compareDates(const void *a, const void *b);

#endif