#include<string>
#include"LexicalAnalyzer.class.h"
#include"SyntacticAnalyzer.class.h"
#include"keywordlist.h"
#include"operatorlist.h"
#include"wordlist.h"

std::string SyntacticAnalyzer::analyze(LexicalAnalyzer& lexical_analyzer)
{
    std::string word;
	if((word=subprogram(lexical_analyzer))!="") return word;
	if(lexical_analyzer.peeksymtype()!=PERIOD)
	{
	    if(lexical_analyzer.peeksymtype()!="") return "程序有错，这个程序到这里看上去应该已经结束了，但其实不是。是不是少了运算符或漏了左括号？";
	    else return "必须在程序结尾加一个句号，不要问我为神马，这就是语法！";
	}
	return "";
}

std::string SyntacticAnalyzer::subprogram(LexicalAnalyzer& lexical_analyzer)
{
	std::string word=lexical_analyzer.peeksymtype();
	if(word==CONSTSYM)
	{
		if((word=constsym(lexical_analyzer))!="") return word;
		word=lexical_analyzer.peeksymtype();
	}
	if(word==VARSYM)
	{
		if((word=varsym(lexical_analyzer))!="") return word;
		word=lexical_analyzer.peeksymtype();
	}
	if(word==PROCSYM)
	{
		do
		{
			if((word=procsym(lexical_analyzer))!="") return word;
		}while((word=lexical_analyzer.peeksymtype())==PROCSYM);
	}
	return statement(lexical_analyzer);
}

std::string SyntacticAnalyzer::constsym(LexicalAnalyzer& lexical_analyzer)
{
	std::string word;
	if(lexical_analyzer.getsymtype()!=CONSTSYM) return "这里应该是一个const语句";
	do
	{
		if(lexical_analyzer.getsymtype()!=IDENT) return "这里应该是一个标识符，例如：CONST X=6,Y=4;";
		if(lexical_analyzer.getsymtype()!=EQL) return "这里应该是一个等于符号，例如：CONST X=6,Y=4;";
		if(lexical_analyzer.getsymtype()!=NUMBER) return "这里应该是一个数字，例如：CONST X=6,Y=4;";
	}while((word=lexical_analyzer.getsymtype())==COMMA);
	
	return (word==SEMICOLON?"":"const语句必须以分号结尾，例如：CONST X=6,Y=4; 请留意末尾的分号");
}

std::string SyntacticAnalyzer::varsym(LexicalAnalyzer& lexical_analyzer)
{
	std::string word;
	if(lexical_analyzer.getsymtype()!=VARSYM) return "这里应该是一个var语句";
	do
	{
		if(lexical_analyzer.getsymtype()!=IDENT) return "这里应该是一个标识符，例如：VAR X,Y,Z;";
	}while((word=lexical_analyzer.getsymtype())==COMMA);

	return (word==SEMICOLON?"":"var语句必须以分号结尾，例如：VAR X,Y,Z; 请留意末尾的分号");
}

std::string SyntacticAnalyzer::procsym(LexicalAnalyzer& lexical_analyzer)
{
	std::string word="";
	if(lexical_analyzer.getsymtype()!=PROCSYM) return "这里应该是一个procedure语句";
	if(lexical_analyzer.getsymtype()!=IDENT) return "这里应该有一个标识符，作为procedure的名字，例如：PROCEDURE PROC;";
	if(lexical_analyzer.getsymtype()!=SEMICOLON) return "这里应该有一个分号，作为procedure声明和内容的分隔符，例如：PROCEDURE PROC; 请留意末尾的分号";
	if((word=subprogram(lexical_analyzer))!="") return word;
	if(lexical_analyzer.getsymtype()!=SEMICOLON) return "这里应该有一个分号，作为一个procedure结束，我知道加一个分号很无聊也不能促进减肥，但是这是语法，是规定，只要你爸不是李刚，就必需遵守这个规定";
	return "";
}
