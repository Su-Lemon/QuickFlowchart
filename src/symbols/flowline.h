//
// Created by SuWei on 2021/11/24.
//

#ifndef QUICKFLOWCHART_FLOWLINE_H
#define QUICKFLOWCHART_FLOWLINE_H

class Node{
public:
    enum class Type : int { IF, WHILE, GENERAL, SEQ};
    Type type;

public:
    Node(const Type type);

};
#endif //QUICKFLOWCHART_FLOWLINE_H
