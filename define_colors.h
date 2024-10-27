#ifndef PRINT_COLORS_H
#define PRINT_COLORS_H

#include <stdio.h>

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_PURPLE  "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#define RED(text)    COLOR_RED    text COLOR_RESET
#define GREEN(text)  COLOR_GREEN  text COLOR_RESET
#define YELLOW(text) COLOR_YELLOW text COLOR_RESET
#define BLUE(text)   COLOR_BLUE   text COLOR_RESET
#define PURPLE(text) COLOR_PURPLE text COLOR_RESET
#define CYAN(text)   COLOR_CYAN   text COLOR_RESET

#endif /* PRINT_COLORS_H */
