// Created by SuWei on 2021/11/24.

#ifndef QUICKFLOWCHART_SRCFILE_PARSER_H
#define QUICKFLOWCHART_SRCFILE_PARSER_H

#include <fstream>
#include <cstring>

#include "common.h"
#include "node.h"
#include "block.h"

class FileParser{
private:
    std::string file_path;
    std::fstream fs;
    std::vector<Block *> block_list;

    struct ConnectInfo{
        enum class Action:int{ SHIFT, GOTO, REDUCE, ACCEPT };
        Action action;
        int num;

        explicit ConnectInfo(Action action) : action(action){};
        ConnectInfo(Action action, int num) : action(action), num(num){};
    };

    struct ReductionInfo{
        Block::Type new_block_type;
        int pop_num;

        ReductionInfo(Block::Type new_block_type, int pop_num) : new_block_type(new_block_type), pop_num(pop_num) {};
    };

public:
    explicit FileParser(const std::string &file_path);
    void ReadPerBlock();
    ConnectInfo QueryEncodingSheet(int curr_state, Block::Type block_type, std::pair<int, int> pos);
    ReductionInfo GetReductionInfo(int productionNum);
    std::shared_ptr<Node> ParseBlock();

    ~FileParser();
};

#endif //QUICKFLOWCHART_SRCFILE_PARSER_H
