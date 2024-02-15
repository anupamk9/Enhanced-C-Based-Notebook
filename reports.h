#ifndef REPORTS_H
#define REPORTS_H

void generateNotesReport(const char *notesFileName);
void generateUsersReport(const char *usersFileName);
void countNotesPerDay(const char *notesFileName);
void calculateAverageNoteLength(const char *notesFileName);
void findUserWithMostNotes(const char *usersFileName,
                           const char *notesFileName);

#endif