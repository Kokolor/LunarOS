#include <framebuffer/font.h>
#include "shell.h"

#define MAX_CMD_LENGTH 256

char command_buffer[MAX_CMD_LENGTH];
int command_length = 0;

char scancode_to_ascii(uint8_t scancode)
{
    static const char scancode_ascii[128] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',
        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
        '2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    return scancode_ascii[scancode];
}

void shell_input_char(char c)
{
    if (command_length < MAX_CMD_LENGTH - 1)
    {
        command_buffer[command_length++] = c;
    }
}

void shell_input_backspace()
{
    if (command_length > 0)
    {
        command_length--;
        put_rectangle(term.cursor_x - 8, term.cursor_y, 8, 16, 0x000000);
    }
}

void shell_execute_command()
{
    command_buffer[command_length] = '\0';

    if (strcmp(command_buffer, "div0") == 0)
    {
        int panic = 0 / 0;
    }
    else if (strcmp(command_buffer, "help") == 0)
    {
        put_string("div0 - Create a division per zero\nclear - Clear the screen\nhelp - Show the comamnd list\nshutdown - Shutdown the os\nfetch - Fetch Lunar information\nls - List ramdisk files\n");
    }
    else if (strcmp(command_buffer, "") == 0)
    {
    }
    else if (strcmp(command_buffer, "fetch") == 0)
    {
        put_string("OS: LunarOS\nKernel: Lunaris\nVersion: 0.1Dev\nDeveloppers: Kokolor, LeGamer\n");
    }
    else if (strcmp(command_buffer, "clear") == 0)
    {
        clear_screen(0x000000);
    }
    else if (strcmp(command_buffer, "shutdown") == 0)
    {
        printk_info("Now, you can safety shutdown your computer.");
        asm("hlt");
    }
    else if (strcmp(command_buffer, "ls") == 0)
    {
        list_files();
    }
    else if (strncmp(command_buffer, "cat ", 4) == 0)
    {
        char *filename = command_buffer + 4;
        char buffer[1024];

        void *file_start = read_file(filename, buffer, sizeof(buffer) - 1);

        if (file_start != NULL)
        {
            buffer[sizeof(buffer) - 1] = '\0';
            put_string(buffer);
        }
        printk("\n");
    }
    else if (strncmp(command_buffer, "touch ", 6) == 0)
    {
        char *filename = command_buffer + 6;
        size_t size = 0;

        void *file_start = create_file(filename, size);

        if (file_start != NULL)
        {
            put_string("Created file: ");
            put_string(filename);
            put_string("\n");
        }
    }
    else
    {
        put_string("Command not found: ");
        put_string(command_buffer);
        put_string("\n");
    }

    command_length = 0;
}

void shell_prompt()
{
    term.color = 0xf00fff;
    put_string("admin@shell-# ");
    term.color = 0xffffff;
}