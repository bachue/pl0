#include<string>
#include"LexicalAnalyzer.class.h"
#include"SyntacticAnalyzer.class.h"
#include"operatorlist.h"
#include"keywordlist.h"
#include"wordlist.h"

std::string SyntacticAnalyzer::condition(LexicalAnalyzer& lexical_analyzer)
{
    std::string word=lexical_analyzer.peeksymtype();
    if(word==ODDSYM) return lexical_analyzer.getsymtype().size(),expression(lexical_analyzer);
    else
    {
        if((word=expression(lexical_analyzer))!="") return word;
        if(word=lexical_analyzer.getsymtype(),
           word!=EQL && word!=NEQ && word!=LSS && word!=LEQ && word!=GTR && word!=GEQ) return "这里应该有一个比较运算符，例如等于 = ，不等于 # ，小于 < ，小于等于 <= ，大于 > ，大于等于 >= ";
        return expression(lexical_analyzer);
    }
}

std::string SyntacticAnalyzer::expression(LexicalAnalyzer& lexical_analyzer)
{
    std::string word=lexical_analyzer.peeksymtype();
    if(word==PLUS || word==MINUS) lexical_analyzer.getsymtype();
    if((word=item(lexical_analyzer))!="") return word;
    while(word=lexical_analyzer.peeksymtype(),word==PLUS || word==MINUS)
    {
        if(lexical_analyzer.getsymtype(),(word=item(lexical_analyzer))!="") return word;
    }
 
    return "";
}

std::string SyntacticAnalyzer::item(LexicalAnalyzer& lexical_analyzer)
{
    std::string word;
    if((word=factor(lexical_analyzer))!="") return word;
    while(word=lexical_analyzer.peeksymtype(),word==TIMES || word==SLASH)
    {
        if(lexical_analyzer.getsymtype(),(word=item(lexical_analyzer))!="") return word;
    }
    return "";
}

std::string SyntacticAnalyzer::factor(LexicalAnalyzer& lexical_analyzer)
{
    std::string word=lexical_analyzer.getsymtype();
    if(word!=IDENT && word!=NUMBER && word!=LPAREN) return "这里应该是一个标识符或是一个数字或是一个表达式";
    
    if(word==LPAREN)
	{
		if((word=expression(lexical_analyzer))!="") return word;
		if(lexical_analyzer.getsymtype()!=RPAREN) return "这里应该有一个右括号，表达式必须用一对括号扩起来";
	}
    return "";
}

