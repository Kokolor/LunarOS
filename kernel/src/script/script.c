#include "script.h"

void create_lunar_script(const char *name, const char *script)
{
    size_t size = strlen(script) + 1;
    char *file = create_file(name, size);

    memcpy(file, script, size);

    // printk_success("Successfully created Lunar Script: %s\n", name);
}

void execute_lunar_script(const char *name)
{
    char buffer[4096];
    read_file(name, buffer, sizeof(buffer));

    char *line = strtok(buffer, "\n");
    while (line)
    {
        if (strncmp(line, "print", 5) == 0)
        {
            char *text = strchr(line, '[') + 1;
            *strchr(text, ']') = '\0';
            printk("%s\n", text);
        }
        else if (strncmp(line, "list_files", 10) == 0)
        {
            printk("List files:\n");
            list_files();
        }

        line = strtok(NULL, "\n");
    }
}
