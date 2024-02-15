#include "export.h"
#include "reports.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_USERS 100
#define MAX_NOTES 1000
#define FILENAME "notes.txt"
#define USERS_FILE "users.txt"

struct User {

  char username[50];

  char password[50];
};

struct Note {

  char title[100];

  char content[1000];

  char datetime[20];
};

struct User users[MAX_USERS];
int userCount = 0;
struct Note notes[MAX_NOTES];
int noteCount = 0;
char currentUser[50] = "";

// Function prototypes
void menu();
void getCurrentDateTime(char datetime[]);
void WriteNote();
void ShowNotes();
void SortNotesByDatetime();
int registerUser();
int loginUser();
int loadUsersFromFile();
void WriteUserToFile(const struct User *user);
int userExists(const char *username);
int loadNotesFromFile();
void WriteNotesToFile();
void PrintAllNotesFromFile();
void DeleteNote();
void EditNote();

int main() {

  char opt;

  // Load user data from the users.txt file

  if (!loadUsersFromFile()) {

    printf("Error loading user data. Exiting...\n");

    exit(1);
  }

  // Load note data from the notes.txt file

  if (!loadNotesFromFile()) {

    printf("Error loading notes data. Exiting...\n");

    exit(1);
  }

  do {
    printf("====================================================\n");
    printf("               Welcome to NOTESIFY                  \n");
    printf("====================================================\n\n\n");
    if (currentUser[0] == '\0') {
      printf("****************************************************\n");
      printf("*              You are not logged in               *\n");
      printf("****************************************************\n\n\n");

      printf("+--------------------------------------------------+\n");
      printf("|1. Login                                          |\n");
      printf("|2. Signup                                         |\n");
      printf("|3. Exit                                           |\n");
      printf("+--------------------------------------------------+\n");
      printf("Enter your choice: ");
      int choice;
      scanf("%d", &choice);
      switch (choice) {

      case 1:
        if (loginUser()) {
          printf("Login successful!\n");
        } else {
          printf("Login failed. Please try again.\n");
        }
        break;

      case 2:

        if (registerUser()) {

          printf("Registration successful!\n");

        } else {

          printf("Registration failed. Please try again.\n");
        }

        break;

      case 3:

        WriteNotesToFile();

        exit(0);

      default:

        printf("Invalid choice.\n");
      }

    } else {

      menu();

      fflush(stdin);

      scanf(" %c", &opt);

      switch (opt) {

      case 'n':

        WriteNote();
        break;

      case 's':

        SortNotesByDatetime();
        PrintAllNotesFromFile();
        break;

      case 'e':
        EditNote();
        break;

      case 'd':
        DeleteNote();
        break;

      case 'r':
        generateNotesReport(FILENAME);
        break;

      case 'u':
        generateUsersReport(USERS_FILE);
        break;

      case 'c':
        calculateAverageNoteLength(FILENAME);
        break;

      case 'f':
        findUserWithMostNotes(USERS_FILE, FILENAME);
        break;

      case 'j':
        exportNotesToJson(FILENAME);
        break;

      case 'x':

        exit(0);

      default:

        printf("ERROR! Try Again\n");
      }
    }

  } while (1);

  return 0;
}

void menu() {

  printf("++================================================++\n");
  printf("||                   NOTESIFY                     ||\n");
  printf("++================================================++\n\n\n");

  printf("+--------------------------------------------------+\n");
  printf("|                     MENU                         |\n");
  printf("|--------------------------------------------------|\n");
  printf("|NEW NOTE (n)                                      |\n");
  printf("|SHOW NOTE (s)                                     |\n");
  printf("|EDIT NOTE (e)                                     |\n");
  printf("|DELETE NOTE (d)                                   |\n");
  printf("|GENERATE NOTES REPORT (r)                         |\n");
  printf("|GENERATE USERS REPORT (u)                         |\n");
  printf("|CALCULATE AVG NOTE LENGTH (c)                     |\n");
  printf("|USERS WITH MOST NOTES (f)                         |\n");
  printf("|EXPORT NOTES AS JSON (j)                          |\n");
  printf("|EXIT (x)                                          |\n");
  printf("+--------------------------------------------------+\n");

  printf(" ENTER YOUR CHOICE => ");
}

void getCurrentDateTime(char datetime[20]) {

  time_t t;

  struct tm *tm_info;

  time(&t);

  tm_info = localtime(&t);

  strftime(datetime, 20, "%Y-%m-%d %H:%M:%S", tm_info);
}

void WriteNote() {

  if (noteCount >= MAX_NOTES) {
    printf("****************************************************\n");
    printf("*         Maximum number of notes reached          *\n");
    printf("****************************************************\n");
    return;
  }

  printf("Enter note title: ");

  scanf(" %[^\n]s", notes[noteCount].title);

  getchar();

  printf("Enter note content: \n");

  scanf(" %[^\n]s", notes[noteCount].content);

  getchar();

  getCurrentDateTime(notes[noteCount].datetime);

  noteCount++;

  WriteNotesToFile();
}

void ShowNotes() {

  if (noteCount == 0) {
    printf("****************************************************\n");
    printf("*               No notes to display                *\n");
    printf("****************************************************\n");
    return;
  }

  printf("==================================|\n");
  printf("|           NOTES                |\n");
  printf("==================================|\n");

  for (int i = 0; i < noteCount; i++) {
    printf("Note %d:\n", i + 1);
    printf("Title: %s\n", notes[i].title);
    printf("Content: %s\n", notes[i].content);
    printf("Datetime: %s\n", notes[i].datetime);
    printf("----------------------------------\n");
  }
}

void SortNotesByDatetime() {
  for (int i = 0; i < noteCount - 1; i++) {
    for (int j = 0; j < noteCount - i - 1; j++) {
      if (strcmp(notes[j].datetime, notes[j + 1].datetime) > 0) {
        struct Note temp = notes[j];
        notes[j] = notes[j + 1];
        notes[j + 1] = temp;
      }
    }
  }
}

void WriteNotesToFile() {
  FILE *file = fopen(FILENAME, "w");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }

  for (int i = 0; i < noteCount; i++) {
    fprintf(file, "Title: %s\n", notes[i].title);
    fprintf(file, "Content: %s\n", notes[i].content);
    fprintf(file, "Datetime: %s\n", notes[i].datetime);
    fprintf(file, "----------------------------------\n");
  }
  fclose(file);
}

void PrintAllNotesFromFile() {
  FILE *file = fopen(FILENAME, "r");
  if (file == NULL) {
    printf("Notes file not found.\n");
    return;
  }
  printf("==================================|\n");
  printf("|           NOTES                |\n");
  printf("==================================|\n");

  char line[1000];
  while (fgets(line, sizeof(line), file)) {
    if (strcmp(line, "----------------------------------\n") == 0) {
      printf("----------------------------------\n");
    } else {
      printf("%s", line);
    }
  }
  fclose(file);
}

int registerUser() {
  if (userCount >= MAX_USERS) {
    printf("****************************************************\n");
    printf("*          Maximum number of users reached         *\n");
    printf("****************************************************\n");
    return 0;
  }

  struct User newUser;

  printf("Enter a new username: ");
  scanf(" %s", newUser.username);
  if (userExists(newUser.username)) {
    printf("****************************************************\n");
    printf("*            Username already exists               *\n");
    printf("****************************************************\n");
    return 0;
  }

  printf("Enter a password: ");
  scanf(" %s", newUser.password);
  users[userCount] = newUser;
  userCount++;
  WriteUserToFile(&newUser);
  return 1;
}

int loginUser() {
  char username[50];
  char password[50];

  printf("Enter your username: ");
  scanf(" %s", username);
  printf("Enter your password: ");
  scanf(" %s", password);

  for (int i = 0; i < userCount; i++) {
    if (strcmp(users[i].username, username) == 0 &&
        strcmp(users[i].password, password) == 0) {
      strcpy(currentUser, username);
      return 1;
    }
  }
  return 0;
}

int loadUsersFromFile() {
  FILE *file = fopen(USERS_FILE, "r");
  if (file == NULL) {
    printf("User file not found. Creating a new one...\n");
    return 1;
  }
  while (fscanf(file, "%s %s", users[userCount].username,
                users[userCount].password) != EOF) {
    userCount++;
  }
  fclose(file);
  return 1;
}

void WriteUserToFile(const struct User *user) {
  FILE *file = fopen(USERS_FILE, "a");
  if (file == NULL) {
    perror("Error opening users file");
    return;
  }
  fprintf(file, "%s %s\n", user->username, user->password);
  fclose(file);
}

int userExists(const char *username) {
  for (int i = 0; i < userCount; i++) {
    if (strcmp(users[i].username, username) == 0) {
      return 1;
    }
  }
  return 0;
}

int loadNotesFromFile() {
  FILE *file = fopen(FILENAME, "r");
  if (file == NULL) {
    printf("Notes file not found. Creating a new one...\n");
    return 1;
  }
  while (fscanf(file, "Title: %[^\n]\nContent: %[^\n]\nDatetime: %[^\n]\n",
                notes[noteCount].title, notes[noteCount].content,
                notes[noteCount].datetime) == 3) {
    noteCount++;
  }
  fclose(file);
  return 1;
}
void DeleteNote() {
  if (noteCount == 0) {
    printf("No notes to delete.\n");
    return;
  }
  char titleToDelete[100];
  printf("Enter the title of the note you want to delete: ");
  scanf(" %[^\n]s", titleToDelete);
  getchar();
  int found = 0;
  for (int i = 0; i < noteCount; i++) {
    if (strcmp(notes[i].title, titleToDelete) == 0) {
      for (int j = i; j < noteCount - 1; j++) {
        notes[j] = notes[j + 1];
      }
      noteCount--;
      found = 1;
      break;
    }
  }
  if (found) {
    printf("Note with title \"%s\" has been deleted.\n", titleToDelete);
    WriteNotesToFile();
  } else {
    printf("Note with title \"%s\" not found.\n", titleToDelete);
  }
}
void EditNote() {
  if (noteCount == 0) {
    printf("No notes to edit.\n");
    return;
  }
  char titleToEdit[100];
  printf("Enter the title of the note you want to edit: ");
  scanf(" %[^\n]s", titleToEdit);
  getchar();

  int found = 0;
  for (int i = 0; i < noteCount; i++) {
    if (strcmp(notes[i].title, titleToEdit) == 0) {
      printf("Enter new content for the note:\n");
      scanf(" %[^\n]s", notes[i].content);
      getchar();

      getCurrentDateTime(notes[i].datetime);

      found = 1;
      break;
    }
  }
  if (found) {
    printf("Note with title \"%s\" has been edited.\n", titleToEdit);
    WriteNotesToFile();
  } else {
    printf("Note with title \"%s\" not found.\n", titleToEdit);
  }
}