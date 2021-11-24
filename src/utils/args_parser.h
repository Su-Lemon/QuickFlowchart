// Created by SuWei on 2021/11/24.

#ifndef QUICKFLOWCHART_ARGS_PARSER_H
#define QUICKFLOWCHART_ARGS_PARSER_H

#include <iostream>
#include <utility>
#include <cstring>

// Hold info from cmd
struct CmdArgs{
    bool isre_dir;
    std::string srcfile_path;
    std::string chart_path;

    explicit CmdArgs(std::string srcfile_path) : isre_dir(false), srcfile_path(std::move(srcfile_path)) {};
    CmdArgs(std::string srcfile_path, std::string chart_path) : isre_dir(false), srcfile_path(std::move(srcfile_path)), chart_path(std::move(chart_path)) {};
};

void ShowHelp();
// Parse cmd info
CmdArgs ParseArgs(int argc, char *argv[]);

#endif //QUICKFLOWCHART_ARGS_PARSER_H
