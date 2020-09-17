#include "scanner.h"


void scanner_init(Scanner* scanner, const char* src)
{
    scanner->start = src;
    scanner->current = src;
    scanner->line = 1;
}