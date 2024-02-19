#include <nm_functions.h>

void exit_error(const char *msg)
{
    printf(C_G_RED"Error: "C_RES"nm: %s: %s\n", msg, strerror(errno));
    free_all_malloc();
    exit(1);
}

void exit_corrupted(const char *msg)
{
    printf(C_G_BLUE"Error: "C_RES"nm: %s\n", msg);
    free_all_malloc();
    exit(1);
}

void exit_msg(const char *msg)
{
    printf(C_G_RED"Error: "C_RES"nm: %s\n", msg);
    free_all_malloc();
    exit(1);
}

void exit_error_close(const char *msg, int fd)
{
    printf(C_G_RED"Error: "C_RES"nm: %s: %s\n", msg, strerror(errno));
    free_all_malloc();
    close(fd);
    exit(1);
}
