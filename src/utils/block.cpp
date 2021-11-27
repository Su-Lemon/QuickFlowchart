// Created by SuWei on 2021/11/24.

#include "block.h"

Block::Block(const Block::Type type): type(type) {}
Block::Block(const Block::Type type, std::pair<int, int> pos): type(type), pos(pos) {}
Block::Block(const Block::Type type, std::pair<int, int> pos, int state): type(type), pos(pos), state(state) {}

ProBlock::ProBlock(const std::string &pro_text, std::pair<int, int> pos) : Block(Type::PROCESS, pos), pro_text(pro_text){}

DecBlock::DecBlock(const std::string &dec_text, std::pair<int, int> pos) : Block(Type::DECISION, pos), dec_text(dec_text) {}

IfBlock::IfBlock(std::pair<int, int> pos) : Block(Type::IF, pos){}

ElseBlock::ElseBlock(std::pair<int, int> pos) : Block(Type::ELSE, pos){}

WhileBlock::WhileBlock(std::pair<int, int> pos) : Block(Type::WHILE, pos){}

BeginBlock::BeginBlock(std::pair<int, int> pos) : Block(Type::BEGIN, pos){}

EndBlock::EndBlock(std::pair<int, int> pos) : Block(Type::END, pos){}

LBraceBlock::LBraceBlock(std::pair<int, int> pos) : Block(Type::LBRACE, pos){}

RBraceBlock::RBraceBlock(std::pair<int, int> pos) : Block(Type::RBRACE, pos){}
