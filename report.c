#include "reports.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generateNotesReport(const char *notesFileName) {
  FILE *file = fopen(notesFileName, "r");
  if (file == NULL) {
    printf("Notes file not found.\n");
    return;
  }

  printf("==================================|\n");
  printf("|       NOTES REPORT              |\n");
  printf("==================================|\n");

  char line[1000];
  int noteCount = 0;
  while (fgets(line, sizeof(line), file)) {
    if (strcmp(line, "----------------------------------\n") == 0) {
      noteCount++;
    }
  }

  printf("Total number of notes: %d\n", noteCount);

  fclose(file);
}

void generateUsersReport(const char *usersFileName) {
  FILE *file = fopen(usersFileName, "r");
  if (file == NULL) {
    printf("Users file not found.\n");
    return;
  }

  printf("==================================|\n");
  printf("|       USERS REPORT              |\n");
  printf("==================================|\n");

  char line[100];
  int userCount = 0;
  while (fgets(line, sizeof(line), file)) {
    userCount++;
  }

  printf("Total number of users: %d\n", userCount);

  fclose(file);
}

void countNotesPerDay(const char *notesFileName) {
  FILE *file = fopen(notesFileName, "r");
  if (file == NULL) {
    printf("Notes file not found.\n");
    return;
  }

  printf("==================================|\n");
  printf("| NOTES COUNT PER DAY REPORT      |\n");
  printf("==================================|\n");

  char line[1000];
  char prevDate[20] = "";
  int notesCount = 0;

  while (fgets(line, sizeof(line), file)) {
    if (strncmp(line, "Datetime: ", 10) == 0) {
      char noteDate[20];
      sscanf(line, "Datetime: %s", noteDate);

      if (strcmp(noteDate, prevDate) == 0) {
        notesCount++;
      } else {
        if (notesCount > 0) {
          printf("Date: %s, Notes Count: %d\n", prevDate, notesCount);
        }
        strcpy(prevDate, noteDate);
        notesCount = 1;
      }
    }
  }

  if (notesCount > 0) {
    printf("Date: %s, Notes Count: %d\n", prevDate, notesCount);
  }

  fclose(file);
}

void calculateAverageNoteLength(const char *notesFileName) {
  FILE *file = fopen(notesFileName, "r");
  if (file == NULL) {
    printf("Notes file not found.\n");
    return;
  }

  printf("==================================|\n");
  printf("|   AVERAGE NOTE LENGTH REPORT    |\n");
  printf("==================================|\n");

  char line[1000];
  int totalLength = 0;
  int noteCount = 0;
  while (fgets(line, sizeof(line), file)) {
    if (strncmp(line, "Content: ", 9) == 0) {
      int contentLength = strlen(line) - 9; // Calculate content length
      totalLength += contentLength;
      noteCount++;
    }
  }

  if (noteCount > 0) {
    double averageLength = (double)totalLength / noteCount;
    printf("Average note content length: %.2lf characters\n", averageLength);
  } else {
    printf("No notes found.\n");
  }

  fclose(file);
}

void findUserWithMostNotes(const char *usersFileName,
                           const char *notesFileName) {
  FILE *usersFile = fopen(usersFileName, "r");
  if (usersFile == NULL) {
    printf("Users file not found.\n");
    return;
  }

  char mostNotesUser[50] = "";
  int mostNotesCount = 0;

  while (fscanf(usersFile, "%s", mostNotesUser) != EOF) {

    int currentUserNotesCount = 0;

    FILE *notesFile = fopen(notesFileName, "r");
    if (notesFile == NULL) {
      printf("Notes file not found.\n");
      return;
    }

    char line[1000];
    while (fgets(line, sizeof(line), notesFile)) {
      if (strncmp(line, "Title: ", 7) == 0) {

        char noteTitle[100];
        sscanf(line, "Title: %[^\n]", noteTitle);

        if (strcmp(noteTitle, mostNotesUser) == 0) {
          currentUserNotesCount++;
        }
      }
    }

    fclose(notesFile);

    if (currentUserNotesCount > mostNotesCount) {
      mostNotesCount = currentUserNotesCount;
    }
  }

  fclose(usersFile);

  if (mostNotesCount > 0) {
    printf("User with the most notes: %s (Total Notes: %d)\n", mostNotesUser,
           mostNotesCount);
  } else {
    printf("No notes found.\n");
  }
}