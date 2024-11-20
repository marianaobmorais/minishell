#ifndef COLORS_H
# define COLORS_H

// Reset
#define RESET       "\001\033[0m\002"

// Text Styles
#define BOLD        "\001\033[1m\002"
#define DIM         "\001\033[2m\002"
#define ITALIC      "\001\033[3m\002"
#define UNDERLINE   "\001\033[4m\002"
#define BLINK       "\001\033[5m\002"
#define REVERSE     "\001\033[7m\002"
#define HIDDEN      "\001\033[8m\002"

// Foreground Colors
#define FG_BLACK    "\001\033[30m\002"
#define FG_RED      "\001\033[31m\002"
#define FG_GREEN    "\001\033[32m\002"
#define FG_YELLOW   "\001\033[33m\002"
#define FG_BLUE     "\001\033[34m\002"
#define FG_MAGENTA  "\001\033[35m\002"
#define FG_CYAN     "\001\033[36m\002"
#define FG_WHITE    "\001\033[37m\002"

// Bright Foreground Colors
#define FG_BRIGHT_BLACK   "\001\033[90m\002"
#define FG_BRIGHT_RED     "\001\033[91m\002"
#define FG_BRIGHT_GREEN   "\001\033[92m\002"
#define FG_BRIGHT_YELLOW  "\001\033[93m\002"
#define FG_BRIGHT_BLUE    "\001\033[94m\002"
#define FG_BRIGHT_MAGENTA "\001\033[95m\002"
#define FG_BRIGHT_CYAN    "\001\033[96m\002"
#define FG_BRIGHT_WHITE   "\001\033[97m\002"

// Background Colors
#define BG_BLACK    "\001\033[40m\002"
#define BG_RED      "\001\033[41m\002"
#define BG_GREEN    "\001\033[42m\002"
#define BG_YELLOW   "\001\033[43m\002"
#define BG_BLUE     "\001\033[44m\002"
#define BG_MAGENTA  "\001\033[45m\002"
#define BG_CYAN     "\001\033[46m\002"
#define BG_WHITE    "\001\033[47m\002"

// Bright Background Colors
#define BG_BRIGHT_BLACK   "\001\033[100m\002"
#define BG_BRIGHT_RED     "\001\033[101m\002"
#define BG_BRIGHT_GREEN   "\001\033[102m\002"
#define BG_BRIGHT_YELLOW  "\001\033[103m\002"
#define BG_BRIGHT_BLUE    "\001\033[104m\002"
#define BG_BRIGHT_MAGENTA "\001\033[105m\002"
#define BG_BRIGHT_CYAN    "\001\033[106m\002"
#define BG_BRIGHT_WHITE   "\001\033[107m\002"

// Usage Example Macros
#define ERROR_TEXT   FG_RED BOLD
#define SUCCESS_TEXT FG_GREEN BOLD
#define WARNING_TEXT FG_YELLOW BOLD

#endif // COLORS_H
