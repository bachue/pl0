#include<string>
#include<map>
#define OPERATOR_LENGTH 16

const std::pair<std::string,std::string> OPERATORS_ARRAY[]={ std::make_pair(":=","becomes"),
                                                              std::make_pair(">=","geq"),
                                                              std::make_pair(">","gtr"),
                                                              std::make_pair("<=","leq"),
                                                              std::make_pair("<","lss"),
                                                              std::make_pair("+","plus"),
                                                              std::make_pair("-","minus"),
                                                              std::make_pair("*","times"),
                                                              std::make_pair("/","slash"),
                                                              std::make_pair("=","eql"),
                                                              std::make_pair("#","neq"),
                                                              std::make_pair(",","comma"),
                                                              std::make_pair(".","period"),
                                                              std::make_pair(";","semicolon"),
                                                              std::make_pair("(","lparen"),
                                                              std::make_pair(")","rparen")};

const std::map<std::string,std::string> OPERATORS(OPERATORS_ARRAY,OPERATORS_ARRAY+OPERATOR_LENGTH);

std::string get_operator_name(const std::string& str)
{
    std::map<std::string,std::string>::const_iterator iter=OPERATORS.find(str);
    if(iter!=OPERATORS.end()) return iter->second;
    else return "";
}

