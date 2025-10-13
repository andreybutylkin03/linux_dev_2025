#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int beg, end, inc;

    switch (argc) {
        case 2:
            beg = 0;
            end = atoi(argv[1]);
            inc = 1;
            break;
        case 3:
            beg = atoi(argv[1]);
            end = atoi(argv[2]);
            inc = 1;
            break;
        case 4:
            beg = atoi(argv[1]);
            end = atoi(argv[2]);
            inc = atoi(argv[3]);
            break;
    }

    for (int val = beg; (inc > 0 ? val < end : val > end); val += inc) {
        printf("%d\n", val);
    }

    return 0;
}

