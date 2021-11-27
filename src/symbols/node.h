//
// Created by SuWei on 2021/11/24.
//

#ifndef QUICKFLOWCHART_NODE_H
#define QUICKFLOWCHART_NODE_H

#include "common.h"
#include "box.h"

class Node{
public:
    enum class Type : int { IF, WHILE, GENERAL, SEQ};
    Type type;

public:
    explicit Node(const Type type);
    virtual void Print(int d) const = 0;
    virtual Box *Build() = 0;

};

class SeqNode : public Node{
public:
    std::vector<Node *> seq;
public:
    SeqNode();
    SeqNode(Node *const node);
    void Print(int d) const override;
    Box * Build() override;
};

class GeneralNode : public Node{
public:
    std::string gen_text;
public:
    GeneralNode(const std::string &gen_text);
    void Print(int d) const override;
    Box * Build() override;
};

class IfNode : public Node{
public:
    std::string dec_text;
    Node *if_then;
    Node *if_else;
public:
    IfNode(const std::string &dec_text, Node *const if_then, Node *const if_else = nullptr);
    void Print(int d) const override;
    Box * Build() override;
};

class WhileNode : public Node{
public:
    std::string dec_text;
    Node *body;
public:
    WhileNode(const std::string &dec_text, Node *const body);
    void Print(int d) const override;
    Box * Build() override;
};

#endif //QUICKFLOWCHART_NODE_H
