// Created by SuWei on 2021/11/24.

#include "args_parser.h"

void ShowHelp() {
    std::cout << "Quick Flowchart: A tool to quickly generate small flowcharts using pseudo-code.\n";
    std::cout << std::string(60, '-') << std::endl;
    std::cout << "| -h  | Help info.\n\n";
    std::cout << "| -f  | The path of the user-written pseudo-code file.\n";
    std::cout << "| -o  | The path of the generated flowchart file.\n";
    std::cout << std::string(60, '-') << std::endl;
}

CmdArgs ParseArgs(int argc, char *argv[]) {
    switch (argc) {
        case 3:
            if (!strcmp(argv[1], "-f")) {return CmdArgs(argv[2]);}
            else if (!strcmp(argv[1], "-h")){break;}
            break;
        case 5:
            if (!strcmp(argv[1], "-f") && !strcmp(argv[3], "-o")) {
                return CmdArgs(argv[2], argv[4]);
            } else if (!strcmp(argv[3], "-f") && !strcmp(argv[1], "-o")) {
                return CmdArgs(argv[4], argv[2]);
            }
            break;
        default:
            break;
    }
    ShowHelp();
    exit(0);
}

