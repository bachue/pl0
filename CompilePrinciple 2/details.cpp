#include<string>
#include"LexicalAnalyzer.class.h"
#include"declare.h"

std::string condition(LexicalAnalyzer& lexical_analyzer)
{
    std::string word=lexical_analyzer.peeksymtype();
    if(word=="oddsym") return lexical_analyzer.getsymtype().size(),expression(lexical_analyzer);
    else
    {
        if((word=expression(lexical_analyzer))!="") return word;
        if(word=lexical_analyzer.getsymtype(),
           word!="eql" && word!="neq" && word!="lss" && word!="leq" && word!="gtr" && word!="geq") return "这里应该有一个比较运算符，例如等于 = ，不等于 # ，小于 < ，小于等于 <= ，大于 > ，大于等于 >= ";
        return expression(lexical_analyzer);
    }
}

std::string expression(LexicalAnalyzer& lexical_analyzer)
{
    std::string word=lexical_analyzer.peeksymtype();
    if(word=="plus" || word=="minus") lexical_analyzer.getsymtype();
    if((word=item(lexical_analyzer))!="") return word;
    while(word=lexical_analyzer.peeksymtype(),word=="plus" || word=="minus")
    {
        if(lexical_analyzer.getsymtype(),(word=item(lexical_analyzer))!="") return word;
    }
 
    return "";
}

std::string item(LexicalAnalyzer& lexical_analyzer)
{
    std::string word;
    if((word=factor(lexical_analyzer))!="") return word;
    while(word=lexical_analyzer.peeksymtype(),word=="times" || word=="slash")
    {
        if(lexical_analyzer.getsymtype(),(word=item(lexical_analyzer))!="") return word;
    }
    return "";
}

std::string factor(LexicalAnalyzer& lexical_analyzer)
{
    std::string word=lexical_analyzer.getsymtype();
    if(word!="ident" && word!="number" && word!="lparen") return "这里应该是一个标识符或是一个数字或是一个表达式";
    
    if(word=="lparen")
	{
		if((word=expression(lexical_analyzer))!="") return word;
		if(lexical_analyzer.getsymtype()!="rparen") return "这里应该有一个右括号，表达式必须用一对括号扩起来";
	}
    return "";
}

