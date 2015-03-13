#include<string>
#include"LexicalAnalyzer.class.h"
#include"declare.h"

std::string program(LexicalAnalyzer& lexical_analyzer)
{
    std::string word;
	if((word=subprogram(lexical_analyzer))!="") return word;
	if(lexical_analyzer.peeksymtype()!="period") return "必须在程序结尾加一个句号，不要问我为神马，这就是语法！";
	return "";
}

std::string subprogram(LexicalAnalyzer& lexical_analyzer)
{
	std::string word=lexical_analyzer.peeksymtype();
	if(word=="constsym")
	{
		if((word=constsym(lexical_analyzer))!="") return word;
		word=lexical_analyzer.peeksymtype();
	}
	if(word=="varsym")
	{
		if((word=varsym(lexical_analyzer))!="") return word;
		word=lexical_analyzer.peeksymtype();
	}
	if(word=="procsym")
	{
		do
		{
			if((word=procsym(lexical_analyzer))!="") return word;
		}while((word=lexical_analyzer.peeksymtype())=="procsym");
	}
	return statement(lexical_analyzer);
}

std::string constsym(LexicalAnalyzer& lexical_analyzer)
{
	std::string word;
	if(lexical_analyzer.getsymtype()!="constsym") return "这里应该是一个const语句";
	do
	{
		if(lexical_analyzer.getsymtype()!="ident") return "这里应该是一个标识符，例如：CONST X=6,Y=4;";
		if(lexical_analyzer.getsymtype()!="eql") return "这里应该是一个等于符号，例如：CONST X=6,Y=4;";
		if(lexical_analyzer.getsymtype()!="number") return "这里应该是一个数字，例如：CONST X=6,Y=4;";
	}while((word=lexical_analyzer.getsymtype())=="comma");
	
	return (word=="semicolon"?"":"const语句必须以分号结尾，例如：CONST X=6,Y=4; 请留意末尾的分号");
}

std::string varsym(LexicalAnalyzer& lexical_analyzer)
{
	std::string word;
	if(lexical_analyzer.getsymtype()!="varsym") return "这里应该是一个var语句";
	do
	{
		if(lexical_analyzer.getsymtype()!="ident") return "这里应该是一个标识符，例如：VAR X,Y,Z;";
	}while((word=lexical_analyzer.getsymtype())=="comma");

	return (word=="semicolon"?"":"var语句必须以分号结尾，例如：VAR X,Y,Z; 请留意末尾的分号");
}

std::string procsym(LexicalAnalyzer& lexical_analyzer)
{
	std::string word="";
	if(lexical_analyzer.getsymtype()!="procsym") return "这里应该是一个procedure语句";
	if(lexical_analyzer.getsymtype()!="ident") return "这里应该有一个标识符，作为procedure的名字，例如：PROCEDURE PROC;";
	if(lexical_analyzer.getsymtype()!="semicolon") return "这里应该有一个分号，作为procedure声明和内容的分隔符，例如：PROCEDURE PROC; 请留意末尾的分号";
	if((word=subprogram(lexical_analyzer))!="") return word;
	if(lexical_analyzer.getsymtype()!="semicolon") return "这里应该有一个分号，作为一个procedure结束，我知道加一个分号很无聊也不能促进减肥，但是这是语法，是规定，只要你爸不是李刚，就必需遵守这个规定";
	return "";
}
