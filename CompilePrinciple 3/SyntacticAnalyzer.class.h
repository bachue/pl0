#ifndef __SYNTACTICANALYZER_CLASS_H
#define __SYNTACTICANALYZER_CLASS_H
#include"LexicalAnalyzer.class.h"

class SyntacticAnalyzer
{
public:
    static std::string analyze(LexicalAnalyzer&);
private:
    static std::string subprogram(LexicalAnalyzer&);
    static std::string constsym(LexicalAnalyzer&);
    static std::string varsym(LexicalAnalyzer&);
    static std::string procsym(LexicalAnalyzer&);
    static std::string statement(LexicalAnalyzer&);
    static std::string callsym(LexicalAnalyzer&);
    static std::string beginsym(LexicalAnalyzer&);
    static std::string ifsym(LexicalAnalyzer&);
    static std::string whilesym(LexicalAnalyzer&);
    static std::string readwritesym(LexicalAnalyzer&);
    static std::string ident(LexicalAnalyzer&);
    static std::string condition(LexicalAnalyzer&);
    static std::string expression(LexicalAnalyzer&);
    static std::string item(LexicalAnalyzer&);
    static std::string factor(LexicalAnalyzer&);
};

#endif

