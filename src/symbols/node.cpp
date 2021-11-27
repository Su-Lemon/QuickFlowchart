//
// Created by SuWei on 2021/11/24.
//

#include "node.h"

static void indentHelper(int num) {
    for (int i = 0; i < num; i++) {
        std::cout << ' ';
    }
}

Node::Node(const Node::Type type) : type(type){}

// ---------------------------------------------------------------------------------------------------------------------
SeqNode::SeqNode() : Node(Node::Type::SEQ){}

SeqNode::SeqNode(Node *const node) : Node(Node::Type::SEQ){
    this->seq.push_back(node);
}

void SeqNode::Print(int d) const {
    for (Node *node: this->seq) {
        node->Print(d);
    }
}

Box *SeqNode::Build() {
    return nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
GeneralNode::GeneralNode(const std::string &gen_text) : Node(Node::Type::GENERAL), gen_text(gen_text){}

void GeneralNode::Print(int d) const {
    indentHelper(2*d);
    std::cout << this->gen_text << std::endl;
}

Box *GeneralNode::Build() {
    return nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
IfNode::IfNode(const std::string &dec_text, Node *const if_then, Node *const if_else) : Node(Node::Type::IF), dec_text(dec_text), if_then(if_then), if_else(if_else) {}

void IfNode::Print(int d) const {
    indentHelper(2 * d);
    std::cout << "if " << this->dec_text << std::endl;
    this->if_then->Print(d + 1);
    if (this->if_else) {
        indentHelper(2 * d);
        std::cout << "else" << std::endl;
        this->if_else->Print(d + 1);
    }
}

Box *IfNode::Build() {
    return nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
WhileNode::WhileNode(const std::string &dec_text, Node *const body) : Node(Node::Type::WHILE), dec_text(dec_text), body(body){}

void WhileNode::Print(int d) const {
    indentHelper(2 * d);
    std::cout << "while " << this->dec_text << std::endl;
    this->body->Print(d + 1);
}

Box *WhileNode::Build() {
    return nullptr;
}
