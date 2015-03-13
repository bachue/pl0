#include<string>
#include<map>
#include"LexicalAnalyzer.class.h"
#include"operatorlist.h"
#define OPERATOR_LENGTH 16

std::string LexicalAnalyzer::get_operator_name(const std::string& str) const
{
	static const std::pair<std::string,std::string> OPERATORS_ARRAY[]={ std::make_pair(":=",BECOMES),
                                                              std::make_pair(">=",GEQ),
                                                              std::make_pair(">",GTR),
                                                              std::make_pair("<=",LEQ),
                                                              std::make_pair("<",LSS),
                                                              std::make_pair("+",PLUS),
                                                              std::make_pair("-",MINUS),
                                                              std::make_pair("*",TIMES),
                                                              std::make_pair("/",SLASH),
                                                              std::make_pair("=",EQL),
                                                              std::make_pair("#",NEQ),
                                                              std::make_pair(",",COMMA),
                                                              std::make_pair(".",PERIOD),
                                                              std::make_pair(";",SEMICOLON),
                                                              std::make_pair("(",LPAREN),
                                                              std::make_pair(")",RPAREN)};

	static const std::map<std::string,std::string> OPERATORS(OPERATORS_ARRAY,OPERATORS_ARRAY+OPERATOR_LENGTH);

    std::map<std::string,std::string>::const_iterator iter=OPERATORS.find(str);
    if(iter!=OPERATORS.end()) return iter->second;
    else return "";
}
