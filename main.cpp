#include "Menu.h"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc == 5 && string(argv[1]) == "-b") {
        batchMode(argv[2], argv[3], argv[4]);
        return 0;
    }
    return mainMenu();
}