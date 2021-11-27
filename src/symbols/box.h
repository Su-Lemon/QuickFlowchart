//
// Created by SuWei on 2021/11/24.
//

#ifndef QUICKFLOWCHART_BOX_H
#define QUICKFLOWCHART_BOX_H

struct AttachInfo{

};

struct DrawInfo{

};

class Box{
public:
    enum class Type : int{  IF, WHILE, GENERAL, SEQ };
    Type type;
    int l_width, r_width;
    int width, height;

public:
    Box(const Type type);
    virtual AttachInfo Attach() = 0;

};

class SeqBox : public Box{

};

class GeneralBox : public Box{

};

class IfBox : public Box{

};

class WhileBox : public Box{

};

#endif //QUICKFLOWCHART_BOX_H
