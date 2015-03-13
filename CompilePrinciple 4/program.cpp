/*
#include<string>
#include"LexicalAnalyzer.class.h"
#include"SyntacticAnalyzer.class.h"
#include"keywordlist.h"
#include"operatorlist.h"
#include"wordlist.h"

bool SyntacticAnalyzer::analyze(LexicalAnalyzer& lexical_analyzer)
{
	if(!subprogram(lexical_analyzer)) return false;
	if(lexical_analyzer.peeksymtype()!=PERIOD)
	{
	    if(lexical_analyzer.peeksymtype()!="") this->errorinfo="程序有错，这个程序到这里看上去应该已经结束了，但其实不是。是不是少了运算符或漏了左括号？";
		else this->errorinfo="必须在程序结尾加一个句号，不要问我为神马，这就是语法！";
		return false;
	}
	return true;
}

bool SyntacticAnalyzer::subprogram(LexicalAnalyzer& lexical_analyzer)
{
	std::string word=lexical_analyzer.peeksymtype();
	if(word==CONSTSYM)
	{
		if(!constsym(lexical_analyzer)) return false;
		word=lexical_analyzer.peeksymtype();
	}
	if(word==VARSYM)
	{
		if(!varsym(lexical_analyzer)) return false;
		word=lexical_analyzer.peeksymtype();
	}
	if(word==PROCSYM)
	{
		do
		{
			if(!procsym(lexical_analyzer)) return false;
		}while((word=lexical_analyzer.peeksymtype())==PROCSYM);
	}
	return statement(lexical_analyzer);
}

bool SyntacticAnalyzer::constsym(LexicalAnalyzer& lexical_analyzer)
{
	std::string word;
	if(lexical_analyzer.getsymtype()!=CONSTSYM)
	{
		this->errorinfo="这里应该是一个const语句";
		return false;
	}
	do
	{
		if(lexical_analyzer.getsymtype()!=IDENT) 
		{
			this->errorinfo="这里应该是一个标识符，例如：CONST X=6,Y=4;";
			return false;
		}
		if(lexical_analyzer.getsymtype()!=EQL)
		{
			this->errorinfo="这里应该是一个等于符号，例如：CONST X=6,Y=4;";
			return false;
		}
		if(lexical_analyzer.getsymtype()!=NUMBER)
		{
			this->errorinfo="这里应该是一个数字，例如：CONST X=6,Y=4;";
			return false;
		}
	}while((word=lexical_analyzer.getsymtype())==COMMA);
	
	if(word==SEMICOLON) return true;
	else
	{
		this->errorinfo="const语句必须以分号结尾，例如：CONST X=6,Y=4; 请留意末尾的分号";
		return false;
	}
}

bool SyntacticAnalyzer::varsym(LexicalAnalyzer& lexical_analyzer)
{
	std::string word;
	if(lexical_analyzer.getsymtype()!=VARSYM)
	{
		this->errorinfo="这里应该是一个var语句";
		return false;
	}
	do
	{
		if(lexical_analyzer.getsymtype()!=IDENT)
		{
			this->errorinfo="这里应该是一个标识符，例如：VAR X,Y,Z;";
			return false;
		}
	}while((word=lexical_analyzer.getsymtype())==COMMA);
	if(word==SEMICOLON) return true;
	else
	{
		this->errorinfo="var语句必须以分号结尾，例如：VAR X,Y,Z; 请留意末尾的分号";
		return false;
	}
}

bool SyntacticAnalyzer::procsym(LexicalAnalyzer& lexical_analyzer)
{
	if(lexical_analyzer.getsymtype()!=PROCSYM) 
	{
		this->errorinfo="这里应该是一个procedure语句";
		return false;
	}
	if(lexical_analyzer.getsymtype()!=IDENT) 
	{
		this->errorinfo="这里应该有一个标识符，作为procedure的名字，例如：PROCEDURE PROC;";
		return false;
	}
	if(lexical_analyzer.getsymtype()!=SEMICOLON)
	{
		this->errorinfo="这里应该有一个分号，作为procedure声明和内容的分隔符，例如：PROCEDURE PROC; 请留意末尾的分号";
		return false;
	}
	if(!subprogram(lexical_analyzer)) return false;
	if(lexical_analyzer.getsymtype()!=SEMICOLON)
	{
		this->errorinfo="这里应该有一个分号，作为一个procedure结束，我知道加一个分号很无聊也不能促进减肥，但是这是语法，是规定，只要你爸不是李刚，就必需遵守这个规定";
		return false;
	}
	return true;
}
*/