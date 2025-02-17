#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define BUF_SIZE 124
#define WW 80

int l_nums = 0;
int meow_on_off = 0;
int w_wrap = 0;

void print_file(FILE *file) {
    char buf[BUF_SIZE];
    int line_count = 1;

    while (fgets(buf, sizeof(buf), file)) {
        if (l_nums) {
            printf("%2d  ", line_count++);
        }

        if (w_wrap) {
            int len = strlen(buf);
            int i = 0;
            while (i < len) {
                int w = WW;
                while (w > 0 && !isspace(buf[i + w]) && i + w < len) {
                    w--;
                }
                if (w <= 0) w = WW;
                printf("%.*s\n", w, &buf[i]);
                i += w;
                while (isspace(buf[i])) i++;
            }
        } else {
            printf("%s", buf);
        }

        if (meow_on_off) {
            printf(" meow!\n");
        }
    }
}

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "hnmw")) != -1) {
        switch (opt) {
            case 'h':
                printf("Usage: %s [-h] [-n] [-m] [-w] [file1 file2 ...]\n", argv[0]);
                printf("Options:\n");
                printf("  -h    Show this list.\n");
                printf("  -n    Add line numbers\n");
                printf("  -m    Add meow at the end of each line.\n");
                printf("  -w    Wrap words for easier reading\n");
                return 0;
            case 'n':
                l_nums = 1;
                break;
            case 'm':
                meow_on_off = 1;
                break;
            case 'w':
                w_wrap = 1;
                break;
        }
    }

    if (optind == argc) {
        print_file(stdin);
    } else {
        for (int i = optind; i < argc; i++) {
            FILE *file;

            if (strcmp(argv[i], "-") == 0) {
                file = stdin;
            } else {
                file = fopen(argv[i], "r");
            }

            print_file(file);

            if (file != stdin) {
                fclose(file);
            }
        }
    }

    return 0;
}
