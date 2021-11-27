// Created by SuWei on 2021/11/24.


#include "common.h"
#include "args_parser.h"
#include "srcfile_parser.h"

int main(int argc, char *argv[]) {
//    CmdArgs cmd_args = ParseArgs(argc, argv);

    std::string srcfile_path = "../samples/srcfile/process.txt";
    std::string chart_path = "../samples/chart";
    CmdArgs cmd_args = CmdArgs(srcfile_path, chart_path);

    std::unique_ptr<FileParser> file_parser = std::make_unique<FileParser>(cmd_args.srcfile_path);

    file_parser->ReadPerBlock();
    std::shared_ptr<Node> node = file_parser->ParseBlock();
    node->Print(0);

    return 0;
}
