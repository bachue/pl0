#ifndef __SYNTACTICANALYZER_CLASS_H
#define __SYNTACTICANALYZER_CLASS_H
#include"LexicalAnalyzer.class.h"

class SyntacticAnalyzer
{
public:
	std::string get_error_info()
	{
		return this->errorinfo;
	}
//仅仅给第4题用
/*
    bool analyze(LexicalAnalyzer&);
private:
    bool subprogram(LexicalAnalyzer&);
    bool constsym(LexicalAnalyzer&);
    bool varsym(LexicalAnalyzer&);
    bool procsym(LexicalAnalyzer&);
    bool statement(LexicalAnalyzer&);
    bool callsym(LexicalAnalyzer&);
    bool beginsym(LexicalAnalyzer&);
    bool ifsym(LexicalAnalyzer&);
    bool whilesym(LexicalAnalyzer&);
    bool readwritesym(LexicalAnalyzer&);
    bool ident(LexicalAnalyzer&);*/
    int condition(LexicalAnalyzer&,bool&);
    int expression(LexicalAnalyzer&,int&);
//仅仅给第4题用
private:
    int item(LexicalAnalyzer&,int&);
    int factor(LexicalAnalyzer&,int&);
private:
	std::string errorinfo;
};

#endif

