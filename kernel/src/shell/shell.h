#include <stdint.h>
#include <ramdisk/ramdisk.h>

char scancode_to_ascii(uint8_t scancode);
void shell_input_char(char c);
void shell_input_backspace();
void shell_execute_command();
void shell_prompt();