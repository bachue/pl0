#ifndef __DECLARE_H
#define __DECLARE_H
#include<string>

class LexicalAnalyzer;
std::string program(LexicalAnalyzer&);
std::string subprogram(LexicalAnalyzer&);
std::string constsym(LexicalAnalyzer&);
std::string varsym(LexicalAnalyzer&);
std::string procsym(LexicalAnalyzer&);
std::string statement(LexicalAnalyzer&);
std::string ident(LexicalAnalyzer&);
std::string callsym(LexicalAnalyzer&);
std::string beginsym(LexicalAnalyzer&);
std::string ifsym(LexicalAnalyzer&);
std::string whilesym(LexicalAnalyzer&);
std::string readwritesym(LexicalAnalyzer&);
std::string condition(LexicalAnalyzer&);
std::string ident(LexicalAnalyzer&);
std::string condition(LexicalAnalyzer&);
std::string expression(LexicalAnalyzer&);
std::string item(LexicalAnalyzer&);
std::string factor(LexicalAnalyzer&);

#endif

