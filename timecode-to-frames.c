#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
struct termios oldt;
#endif

void set_raw_mode() {
#ifndef _WIN32
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
#endif
}

void reset_mode() {
#ifndef _WIN32
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
}

int main() {
    printf("Enter time in [minutes:]seconds:frames (24 FPS): ");
    fflush(stdout);

    char input[100];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("\nError reading input.\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    char *parts[3];
    int num_parts = 0;
    char *tok = strtok(input, ":");
    while (tok && num_parts < 3) {
        parts[num_parts++] = tok;
        tok = strtok(NULL, ":");
    }

    if (num_parts < 2) {
        printf("\nInvalid input. Minimum required: seconds:frames\n");
    } else {
        int m = 0, s = 0, f = 0;
        if (num_parts == 2) {
            s = atoi(parts[0]);
            f = atoi(parts[1]);
        } else if (num_parts == 3) {
            m = atoi(parts[0]);
            s = atoi(parts[1]);
            f = atoi(parts[2]);
        }

        if (m < 0 || s < 0 || f < 0) {
            printf("\nInvalid input: values must be non-negative.\n");
        } else {
            long total_frames = ((long)m * 60 + s) * 24 + f;
            printf("\nTotal frames at 24 FPS: %ld\n", total_frames);
        }
    }

    printf("Press any key to close...\n");
    set_raw_mode();
#ifdef _WIN32
    getch();
#else
    getchar();
#endif
    reset_mode();

    return 0;
}
