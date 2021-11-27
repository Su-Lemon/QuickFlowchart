// Created by SuWei on 2021/11/24.

#include "srcfile_parser.h"

static const std::map<char, char> kw_char = {
        std::map<char, char>::value_type('(', '('),
        std::map<char, char>::value_type(')', ')'),
        std::map<char, char>::value_type('{', '{'),
        std::map<char, char>::value_type('}', '}'),
        std::map<char, char>::value_type(' ', ' '),
        std::map<char, char>::value_type(';', ';'),
};

static const std::map<std::string, std::string> kw_word = {
        std::map<std::string, std::string>::value_type("while", "while"),
        std::map<std::string, std::string>::value_type("if", "if"),
        std::map<std::string, std::string>::value_type("else", "else"),
};

static const std::set<char> delimiter = {'(', ')', '{', '}', ';'};
static const std::set<char> blank = {' ', '\n', '\t'};

// Load source file
FileParser::FileParser(const std::string &file_path) {
    this->file_path = file_path;
    this->fs.open(file_path, std::ios::in);
    dbg(this->file_path);
    if(!this->fs.is_open()){
        std::cout << "Error: File not open!" << std::endl;
        exit(0);
    }
}

void FileParser::ReadPerBlock() {
    char scan;
    std::string str;
    std::pair<int, int> curr_pos = std::make_pair(1, 0);

    while (this->fs.get(scan)){
        // Current read position
        if (scan == '\n'){
            curr_pos.first++;
            curr_pos.second = 0;
        } else{
            curr_pos.second++;
        }

        if (delimiter.find(scan) == delimiter.end()){
            // Read non-delimited characters until delimiter found
            if (blank.find(scan) == blank.end()){
                // Read a non-blank character
                str += scan;
            } else if (scan == kw_char.at(' ') && !str.empty()) {
                str += scan;
            }
        } else{
            // Handling the string before delimiter

            // Clear the space at the end
            str.erase(str.find_last_not_of(' ') + 1);

            if (str == kw_word.at("if")){
                this->block_list.push_back(new IfBlock(curr_pos));
            } else if (str == kw_word.at("else")){
                this->block_list.push_back(new ElseBlock(curr_pos));
            } else if (str == kw_word.at("while")){
                this->block_list.push_back(new WhileBlock(curr_pos));
            } else if (!str.empty()){
                // "str" is a non-conditional control keyword
                if (scan == kw_char.at(')')){
                    // Decision Block
                    this->block_list.push_back(new DecBlock(str, curr_pos));
                } else{
                    // Process Block
                    this->block_list.push_back(new ProBlock(str, curr_pos));
                }
            }

            if(scan == kw_char.at('{')){
                this->block_list.push_back(new LBraceBlock(curr_pos));
            } else if (scan == kw_char.at('}')){
                this->block_list.push_back(new RBraceBlock(curr_pos));
            }
            str.clear();
        }
    }
    this->block_list.push_back(new EndBlock());
}

std::shared_ptr<Node> FileParser::ParseBlock() {
    std::shared_ptr<Node> nodes_ret;
    std::vector<Node *> node_vec;
    std::vector<Block *> b_stack;

    b_stack.push_back(new BeginBlock());
    int next_block = 0;

    while (true){
        ConnectInfo ci = this->QueryEncodingSheet(b_stack.back()->state, this->block_list[next_block]->type, b_stack.back()->pos);
        if (ci.action == ConnectInfo::Action::SHIFT){
            this->block_list[next_block]->state = ci.num;
            b_stack.push_back(this->block_list[next_block]);
            // Transfer to a process node
            if (this->block_list[next_block]->type == Block::Type::PROCESS){
                auto *g_node = new GeneralNode(((ProBlock *)(this->block_list[next_block]))->pro_text);
                node_vec.push_back(g_node);
            }
            next_block++;
        } else if (ci.action == ConnectInfo::Action::REDUCE){
            Node *tmp_node;
            int back = node_vec.size() - 1;
            if (ci.num == 4){
                assert(node_vec[back]->type != Node::Type::SEQ);
                Node *node = new SeqNode(node_vec[back]);
                node_vec.pop_back();
                node_vec.push_back(node);
            } else if (ci.num == 5){
                assert(node_vec[back]->type == Node::Type::SEQ);
                assert(node_vec[back-1]->type != Node::Type::SEQ);
                Node *seq = node_vec[back];
                ((SeqNode *)seq)->seq.insert(((SeqNode *)seq)->seq.begin(), node_vec[back - 1]);
                node_vec.pop_back();
                node_vec.pop_back();
                node_vec.push_back(seq);
            } else if (ci.num == 3){
                assert(node_vec[back]->type == Node::Type::SEQ);
                assert(b_stack[b_stack.size() - 4]->type == Block::Type::DECISION);
                std::string dec_text = ((DecBlock *)(b_stack[b_stack.size() - 4]))->dec_text;
                Node *node = new WhileNode(dec_text, node_vec[back]);
                node_vec.pop_back();
                node_vec.push_back(node);
            } else if (ci.num == 2){
                assert(node_vec[back]->type == Node::Type::SEQ);
                assert(node_vec[back-1]->type == Node::Type::SEQ);
                assert(b_stack[b_stack.size() - 8]->type == Block::Type::DECISION);
                std::string dec_text = ((DecBlock *)(b_stack[b_stack.size() - 8]))->dec_text;
                Node *node = new IfNode(dec_text, node_vec[back - 1], node_vec[back]);
                node_vec.pop_back();
                node_vec.pop_back();
                node_vec.push_back(node);
            } else if (ci.num == 1){
                assert(node_vec[back]->type == Node::Type::SEQ);
                assert(b_stack[b_stack.size() - 4]->type == Block::Type::DECISION);
                std::string dec_text = ((DecBlock *)(b_stack[b_stack.size() - 4]))->dec_text;
                Node *node = new IfNode(dec_text, node_vec[back]);
                node_vec.pop_back();
                node_vec.push_back(node);
            } else{
                assert(0);
            }

            ReductionInfo rf = this->GetReductionInfo(ci.num);
            std::pair<int, int> new_pos = b_stack.back()->pos;
            for (int i = 0; i < rf.pop_num; i++)
            {
                b_stack.pop_back();
            }
            int new_state = this->QueryEncodingSheet(b_stack.back()->state, rf.new_block_type, b_stack.back()->pos).num;
            b_stack.push_back(new Block(rf.new_block_type, new_pos, new_state));

        } else if (ci.action == ConnectInfo::Action::ACCEPT){
            assert(node_vec.size() == 1);
            nodes_ret.reset(node_vec[0]);
            break;
        } else{
            assert(0);
        }
    }
    return nodes_ret;
}

FileParser::ReductionInfo FileParser::GetReductionInfo(int productionNum){
    switch (productionNum) {
        case 1:
            return {Block::Type::PROCESS, 5};
        case 2:
            return {Block::Type::PROCESS, 9};
        case 3:
            return {Block::Type::PROCESS, 5};
        case 4:
            return {Block::Type::SEQ, 1};
        case 5:
            return {Block::Type::SEQ, 2};
        default:
            assert(0);
    }
}

// Queries the connection mode between the current node and the next node
FileParser::ConnectInfo FileParser::QueryEncodingSheet(int curr_state, Block::Type block_type, std::pair<int, int> pos) {
    switch (curr_state) {
        case 1: {
            switch (block_type) {
                case Block::Type::PROCESS:
                    return {ConnectInfo::Action::SHIFT, 4};
                case Block::Type::IF:
                    return {ConnectInfo::Action::SHIFT, 2};
                case Block::Type::WHILE:
                    return {ConnectInfo::Action::SHIFT, 3};
                case Block::Type::SEQ:
                    return {ConnectInfo::Action::GOTO, 18};
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
        case 2: {
            switch (block_type) {
                case Block::Type::DECISION:
                    return {ConnectInfo::Action::SHIFT, 6};
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
        case 3: {
            switch (block_type) {
                case Block::Type::DECISION:
                    return {ConnectInfo::Action::SHIFT, 14};
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
        case 4: {
            switch (block_type) {
                case Block::Type::PROCESS:
                    return {ConnectInfo::Action::SHIFT, 4};
                case Block::Type::IF:
                    return {ConnectInfo::Action::SHIFT, 2};
                case Block::Type::WHILE:
                    return {ConnectInfo::Action::SHIFT, 3};
                case Block::Type::RBRACE:
                    return {ConnectInfo::Action::REDUCE, 4};
                case Block::Type::END:
                    return {ConnectInfo::Action::REDUCE, 4};
                case Block::Type::SEQ:
                    return {ConnectInfo::Action::GOTO, 5};
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
        case 5: {
            switch (block_type) {
                case Block::Type::RBRACE:
                    return {ConnectInfo::Action::REDUCE, 5};
                case Block::Type::END:
                    return {ConnectInfo::Action::REDUCE, 5};
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
        case 6: {
            switch (block_type) {
                case Block::Type::LBRACE:
                    return {ConnectInfo::Action::SHIFT, 7};
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
        case 7: {
            switch (block_type) {
                case Block::Type::PROCESS:
                    return {ConnectInfo::Action::SHIFT, 4};
                case Block::Type::IF:
                    return {ConnectInfo::Action::SHIFT, 2};
                case Block::Type::WHILE:
                    return {ConnectInfo::Action::SHIFT, 3};
                case Block::Type::SEQ:
                    return {ConnectInfo::Action::GOTO, 8};
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
        case 8: {
            switch (block_type) {
                case Block::Type::RBRACE:
                    return {ConnectInfo::Action::SHIFT, 9};
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
        case 9: {
            switch (block_type) {
                case Block::Type::PROCESS:
                    return {ConnectInfo::Action::REDUCE, 1};
                case Block::Type::IF:
                    return {ConnectInfo::Action::REDUCE, 1};
                case Block::Type::ELSE:
                    return {ConnectInfo::Action::SHIFT, 10};
                case Block::Type::WHILE:
                    return {ConnectInfo::Action::REDUCE, 1};
                case Block::Type::RBRACE:
                    return {ConnectInfo::Action::REDUCE, 1};
                case Block::Type::END:
                    return {ConnectInfo::Action::REDUCE, 1};
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
        case 10: {
            switch (block_type) {
                case Block::Type::LBRACE:
                    return {ConnectInfo::Action::SHIFT, 11};
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
        case 11: {
            switch (block_type) {
                case Block::Type::PROCESS:
                    return {ConnectInfo::Action::SHIFT, 4};
                case Block::Type::IF:
                    return {ConnectInfo::Action::SHIFT, 2};
                case Block::Type::WHILE:
                    return {ConnectInfo::Action::SHIFT, 3};
                case Block::Type::SEQ:
                    return {ConnectInfo::Action::GOTO, 12};
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
        case 12: {
            switch (block_type) {
                case Block::Type::RBRACE:
                    return {ConnectInfo::Action::SHIFT, 13};
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
        case 13: {
            switch (block_type) {
                case Block::Type::PROCESS:
                    return {ConnectInfo::Action::REDUCE, 2};
                case Block::Type::IF:
                    return {ConnectInfo::Action::REDUCE, 2};
                case Block::Type::WHILE:
                    return {ConnectInfo::Action::REDUCE, 2};
                case Block::Type::RBRACE:
                    return {ConnectInfo::Action::REDUCE, 2};
                case Block::Type::END:
                    return {ConnectInfo::Action::REDUCE, 2};
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
        case 14: {
            switch (block_type) {
                case Block::Type::LBRACE:
                    return {ConnectInfo::Action::SHIFT, 15};
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
        case 15: {
            switch (block_type) {
                case Block::Type::PROCESS:
                    return {ConnectInfo::Action::SHIFT, 4};
                case Block::Type::IF:
                    return {ConnectInfo::Action::SHIFT, 2};
                case Block::Type::WHILE:
                    return {ConnectInfo::Action::SHIFT, 3};
                case Block::Type::SEQ:
                    return {ConnectInfo::Action::GOTO, 16};
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
        case 16: {
            switch (block_type) {
                case Block::Type::RBRACE:
                    return {ConnectInfo::Action::SHIFT, 17};
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
        case 17: {
            switch (block_type) {
                case Block::Type::PROCESS:
                    return {ConnectInfo::Action::REDUCE, 3};
                case Block::Type::IF:
                    return {ConnectInfo::Action::REDUCE, 3};
                case Block::Type::WHILE:
                    return {ConnectInfo::Action::REDUCE, 3};
                case Block::Type::RBRACE:
                    return {ConnectInfo::Action::REDUCE, 3};
                case Block::Type::END:
                    return {ConnectInfo::Action::REDUCE, 3};
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
        case 18: {
            switch (block_type) {
                case Block::Type::END:
                    return ConnectInfo(ConnectInfo::Action::ACCEPT);
                default:
                    std::cout << "Compile error near (" << pos.first << ", " << pos.second << ")." << std::endl;
                    exit(-1);
            }
        }
    }
    assert(0);
}

FileParser::~FileParser() {
    this->fs.close();
}
