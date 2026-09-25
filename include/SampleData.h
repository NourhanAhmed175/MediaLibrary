#pragma once
#include "Library.h"

// Fills the library with 7 sample items (3 songs, 2 videos, 2 podcasts).
// Titles that already exist are skipped. Returns how many items were added.
int loadSampleData(Library& library);
