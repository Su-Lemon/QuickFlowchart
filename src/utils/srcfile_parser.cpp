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
                this->keyword_list.push_back(new IfBlock(curr_pos));
            } else if (str == kw_word.at("else")){
                this->keyword_list.push_back(new ElseBlock(curr_pos));
            } else if (str == kw_word.at("while")){
                this->keyword_list.push_back(new WhileBlock(curr_pos));
            } else if (!str.empty()){
                // "str" is a non-conditional control keyword
                if (scan == kw_char.at(')')){
                    // Decision Block
                    this->keyword_list.push_back(new DecBlock(str, curr_pos));
                } else{
                    // Process Block
                    this->keyword_list.push_back(new ProBlock(str, curr_pos));
                }
            }

            if(scan == kw_char.at('{')){
                this->keyword_list.push_back(new LBraceBlock(curr_pos));
            } else if (scan == kw_char.at('}')){
                this->keyword_list.push_back(new RBraceBlock(curr_pos));
            }
            str.clear();
        }
    }
    this->keyword_list.push_back(new EndBlock(curr_pos));
}

FileParser::EncodingInfo FileParser::QueryEncodingSheet(int num, Block::Type block_type, std::pair<int, int> pos) {

}

std::shared_ptr<Node> FileParser::ParseBlock() {

}

FileParser::~FileParser() {
    this->fs.close();
}
