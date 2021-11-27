// Created by SuWei on 2021/11/24.

#ifndef QUICKFLOWCHART_SRCFILE_PARSER_H
#define QUICKFLOWCHART_SRCFILE_PARSER_H

#include <fstream>
#include <vector>
#include <cstring>

#include "common.h"
#include "flowline.h"
#include "block.h"

class FileParser{
private:
    std::string file_path;
    std::fstream fs;
    std::vector<Block *> keyword_list;

    struct EncodingInfo{
        enum class Action:int{ SHIFT, GOTO, REDUCE, ACCEPT };
        Action action;
        int num;

        EncodingInfo(Action action) : action(action){};
        EncodingInfo(Action action, int num) : action(action), num(num){};
    };

public:
    FileParser(const std::string &file_path);
    void ReadPerBlock();
    EncodingInfo QueryEncodingSheet(int num, Block::Type block_type, std::pair<int, int> pos);
    std::shared_ptr<Node> ParseBlock();

    ~FileParser();
};

#endif //QUICKFLOWCHART_SRCFILE_PARSER_H
