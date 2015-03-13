/*
#include<string>
#include"LexicalAnalyzer.class.h"
#include"SyntacticAnalyzer.class.h"
#include"keywordlist.h"
#include"operatorlist.h"
#include"wordlist.h"

bool SyntacticAnalyzer::statement(LexicalAnalyzer& lexical_analyzer)
{
	std::string word=lexical_analyzer.peeksymtype();
	if(word==IDENT) return ident(lexical_analyzer);
	if(word==CALLSYM) return callsym(lexical_analyzer);
	if(word==BEGINSYM) return beginsym(lexical_analyzer);
	if(word==IFSYM) return ifsym(lexical_analyzer);
	if(word==WHILESYM) return whilesym(lexical_analyzer);
	if(word==READSYM || word==WRITESYM) return readwritesym(lexical_analyzer);
	
	return true;
}

bool SyntacticAnalyzer::ident(LexicalAnalyzer& lexical_analyzer)
{
    if(lexical_analyzer.getsymtype()!=IDENT)
	{
		this->errorinfo="这里应该有一个标识符，参考赋值语句的语法：X:=2;";
		return false;
	}
    if(lexical_analyzer.getsymtype()!=BECOMES) 
	{
		this->errorinfo="这里应该是一个赋值符号 := ，参考赋值语句的语法：X:=2;";
		return false;
	}
    return expression(lexical_analyzer);
}

bool SyntacticAnalyzer::callsym(LexicalAnalyzer& lexical_analyzer)
{
    if(lexical_analyzer.getsymtype()!=CALLSYM) 
	{
		this->errorinfo="这里应该是一个call语句，call语句的语法是这样紫滴：CALL NIMEI; 其中NIMEI是个过程的名字，事先用procedure语句定义它";
		return false;
	}
    if(lexical_analyzer.getsymtype()!=IDENT)
    {
		this->errorinfo="这个应该是一个标识符，否则我怎么知道你要call谁？call语句的语法是这样紫滴：CALL NIMEI; 其中NIMEI是个过程的名字，事先用procedure语句定义它";
		return false;
	}
	return true;
}

bool SyntacticAnalyzer::beginsym(LexicalAnalyzer& lexical_analyzer)
{
    std::string word;
    if(lexical_analyzer.getsymtype()!=BEGINSYM) 
	{
		this->errorinfo="这里应该有一个begin语句的吧";
		return false;
	}
    if(!statement(lexical_analyzer)) return false;
    while(lexical_analyzer.peeksymtype()!=ENDSYM)
    {
        if(lexical_analyzer.getsymtype()!=SEMICOLON)
        {
			this->errorinfo="这里必须有一个分号的啊，如果BEGIN..END语句中有多于一句语句的话，每句语句都必须用分号分隔";
			return false;
		}
		if(!statement(lexical_analyzer)) return false;
    }
    lexical_analyzer.getsymtype();
    return true;
}

bool SyntacticAnalyzer::ifsym(LexicalAnalyzer& lexical_analyzer)
{
    std::string word;
    if(lexical_analyzer.getsymtype()!=IFSYM) 
	{
		this->errorinfo="这里应该是一个if语句";
		return false;
	}
    if(!condition(lexical_analyzer)) return false;
    if(lexical_analyzer.getsymtype()!=THENSYM)
    {
		this->errorinfo="喂喂..if语句没有跟then，你这个if语句是干啥的？if语句参考语法：IF X=0 THEN X:=1;";
		return false;
	}
	return statement(lexical_analyzer);
}

bool SyntacticAnalyzer::whilesym(LexicalAnalyzer& lexical_analyzer)
{
    std::string word;
    if(lexical_analyzer.getsymtype()!=WHILESYM) 
	{
		this->errorinfo="这里应该是一个while语句";
		return false;
	}
    if(!condition(lexical_analyzer)) return false;
    if(lexical_analyzer.getsymtype()!=DOSYM) 
	{
		this->errorinfo="这里应该有一个do的，没有do，单单一个while有神马意义呢？";
		return false;
	}
    return statement(lexical_analyzer);
}

bool SyntacticAnalyzer::readwritesym(LexicalAnalyzer& lexical_analyzer)
{
    std::string word=lexical_analyzer.getsymtype();
    if(word!=READSYM && word!=WRITESYM)
	{
		this->errorinfo="这里应该是一个函数";
		return false;
	}
    if(lexical_analyzer.getsymtype()!=LPAREN)
	{
		 this->errorinfo="这是函数诶！他的语法应该是 READ(X); 或者 WRITE(Y); 吧，你貌似漏了左括号";
		 return false;
	}
    do{
        if(lexical_analyzer.getsymtype()!=IDENT)
		{
			 this->errorinfo="函数里面要有标识符的啊，他的语法应该是 READ(X); 或是 WRITE(A,B,C);";
			 return false;
		}
    }while((word=lexical_analyzer.getsymtype())==COMMA);
	if(word==RPAREN) return true;
	else
	{
		this->errorinfo="函数参数要用右括号扩起来的啊，你加右括号了吗";
		return false;
	}
}

*/