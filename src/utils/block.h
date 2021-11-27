// Created by SuWei on 2021/11/24.

#ifndef QUICKFLOWCHART_BLOCK_H
#define QUICKFLOWCHART_BLOCK_H

#include <utility>
#include <map>
#include <set>

#include "common.h"

class Block{
public:
    enum class Type:int{ PROCESS, DECISION, LBRACE, RBRACE, IF, ELSE, WHILE, END, SEQ, BEGIN };

    Type type;
    std::pair<int, int> pos;
    int state;

public:
    Block(const Type type);
    Block(const Type type, std::pair<int, int> pos);
    Block(const Type type, std::pair<int, int> pos, int state);
};

// Process Block
class ProBlock : public Block{
public:
    std::string pro_text;

    ProBlock(const std::string &pro_text, std::pair<int, int> pos);
};

class DecBlock : public Block{
public:
    std::string dec_text;

    DecBlock(const std::string &dec_text, std::pair<int, int> pos);
};

class IfBlock : public Block{
public:
    IfBlock(std::pair<int, int> pos);
};

class ElseBlock : public Block{
public:
    ElseBlock(std::pair<int, int> pos);
};

class WhileBlock : public Block{
public:
    WhileBlock(std::pair<int, int> pos);
};

class BeginBlock : public Block{
public:
    BeginBlock();
};

class EndBlock : public Block{
public:
    EndBlock();
};

class LBraceBlock : public Block{
public:
    LBraceBlock(std::pair<int, int> pos);
};

class RBraceBlock : public Block{
public:
    RBraceBlock(std::pair<int, int> pos);
};



#endif //QUICKFLOWCHART_BLOCK_H
