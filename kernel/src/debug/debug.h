#include <stddef.h>
#include <lib/lib.h>

#define DEBUG_COLOR_RED "\033[0;31m"
#define DEBUG_COLOR_GREEN "\033[0;32m"
#define DEBUG_COLOR_YELLOW "\033[0;33m"
#define DEBUG_COLOR_BLUE "\033[0;34m"
#define DEBUG_COLOR_MAGENTA "\033[0;35m"
#define DEBUG_COLOR_CYAN "\033[0;36m"
#define DEBUG_COLOR_RESET "\033[0m"

void debug_print(const char *str);