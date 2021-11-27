//
// Created by SuWei on 2021/11/24.
//

#ifndef QUICKFLOWCHART_NODE_H
#define QUICKFLOWCHART_NODE_H

class Node{
public:
    enum class Type : int { IF, WHILE, GENERAL, SEQ};
    Type type;

public:
    Node(const Type type);

};
#endif //QUICKFLOWCHART_NODE_H
