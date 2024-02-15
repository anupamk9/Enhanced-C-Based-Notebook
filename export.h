#ifndef EXPORT_H
#define EXPORT_H

#include "reports.h"
#include <stdio.h>

void exportNotesToJson(const char *notesFileName);

void escapeJsonString(const char *input, char *output);

#endif
