#include "export.h"
#include <stdio.h>
#include <string.h>

void exportNotesToJson(const char *notesFileName) {
  FILE *notesFile = fopen(notesFileName, "r");
  if (notesFile == NULL) {
    printf("Notes file not found.\n");
    return;
  }

  FILE *jsonFile = fopen("notes.json", "w");
  if (jsonFile == NULL) {
    printf("Error opening JSON file for writing.\n");
    fclose(notesFile);
    return;
  }

  fprintf(jsonFile, "[\n");

  char line[1000];
  int isFirstNote = 1;

  while (fgets(line, sizeof(line), notesFile)) {
    if (strncmp(line, "Title: ", 7) == 0) {
      char noteTitle[100];
      char noteContent[1000];
      char noteDatetime[20];

      sscanf(line, "Title: %[^\n]\n", noteTitle);

      fgets(line, sizeof(line), notesFile);
      sscanf(line, "Content: %[^\n]\n", noteContent);
      fgets(line, sizeof(line), notesFile);
      sscanf(line, "Datetime: %[^\n]\n", noteDatetime);

      char escapedTitle[100];
      char escapedContent[1000];
      escapeJsonString(noteTitle, escapedTitle);
      escapeJsonString(noteContent, escapedContent);

      if (!isFirstNote) {
        fprintf(jsonFile, ",\n");
      }
      fprintf(jsonFile, "  {\n");
      fprintf(jsonFile, "    \"title\": \"%s\",\n", escapedTitle);
      fprintf(jsonFile, "    \"content\": \"%s\",\n", escapedContent);
      fprintf(jsonFile, "    \"datetime\": \"%s\"\n", noteDatetime);
      fprintf(jsonFile, "  }");

      isFirstNote = 0;
    }
  }

  fprintf(jsonFile, "\n]\n");

  fclose(notesFile);
  fclose(jsonFile);

  printf("Notes exported to 'notes.json' in JSON format.\n");
}

void escapeJsonString(const char *input, char *output) {
  char *o = output;
  for (const char *i = input; *i; i++) {
    switch (*i) {
    case '"':
      *o++ = '\\';
      *o++ = '"';
      break;
    case '\\':
      *o++ = '\\';
      *o++ = '\\';
      break;
    case '\b':
      *o++ = '\\';
      *o++ = 'b';
      break;
    case '\f':
      *o++ = '\\';
      *o++ = 'f';
      break;
    case '\n':
      *o++ = '\\';
      *o++ = 'n';
      break;
    case '\r':
      *o++ = '\\';
      *o++ = 'r';
      break;
    case '\t':
      *o++ = '\\';
      *o++ = 't';
      break;
    default:
      *o++ = *i;
      break;
    }
  }
  *o = 0;
}
