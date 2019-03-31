#pragma once

void DisplayFunction(const char *buttonA, const char *buttonB, const char *buttonC);
void DisplayBackground();
bool ChooseAnImage(int sx,int sy, char *incoming_dir, int slot, char **filename, bool *isdir, int *index_file);

