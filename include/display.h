#ifndef __DISPLAY_H_
#define __DISPLAY_H_
#include "data.h"
void initDisplay();
void displayLoop();

line *displayTable(const table &target,int x,int y);

#endif
