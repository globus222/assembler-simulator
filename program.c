#include "compiler.h"

int main(int argc, char *argv[]) {
    int index;
    for (index = 1; index < argc; index++) {
        compile(argv[index]);
    }
    return 0;
}