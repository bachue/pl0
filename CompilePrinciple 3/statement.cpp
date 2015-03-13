#include<string>
#include"LexicalAnalyzer.class.h"
#include"SyntacticAnalyzer.class.h"
#include"keywordlist.h"
#include"operatorlist.h"
#include"wordlist.h"

std::string SyntacticAnalyzer::statement(LexicalAnalyzer& lexical_analyzer)
{
	std::string word=lexical_analyzer.peeksymtype();
	if(word==IDENT) return ident(lexical_analyzer);
	if(word==CALLSYM) return callsym(lexical_analyzer);
	if(word==BEGINSYM) return beginsym(lexical_analyzer);
	if(word==IFSYM) return ifsym(lexical_analyzer);
	if(word==WHILESYM) return whilesym(lexical_analyzer);
	if(word==READSYM || word==WRITESYM) return readwritesym(lexical_analyzer);
	
	return "";
}

std::string SyntacticAnalyzer::ident(LexicalAnalyzer& lexical_analyzer)
{
    if(lexical_analyzer.getsymtype()!=IDENT) return "这里应该有一个标识符，参考赋值语句的语法：X:=2;";
    if(lexical_analyzer.getsymtype()!=BECOMES) return "这里应该是一个赋值符号 := ，参考赋值语句的语法：X:=2;";
    return expression(lexical_analyzer);
}

std::string SyntacticAnalyzer::callsym(LexicalAnalyzer& lexical_analyzer)
{
    if(lexical_analyzer.getsymtype()!=CALLSYM) return "这里应该是一个call语句，call语句的语法是这样紫滴：CALL NIMEI; 其中NIMEI是个过程的名字，事先用procedure语句定义它";
    if(lexical_analyzer.getsymtype()!=IDENT) return "这个应该是一个标识符，否则我怎么知道你要call谁？call语句的语法是这样紫滴：CALL NIMEI; 其中NIMEI是个过程的名字，事先用procedure语句定义它";
    return "";
}

std::string SyntacticAnalyzer::beginsym(LexicalAnalyzer& lexical_analyzer)
{
    std::string word;
    if(lexical_analyzer.getsymtype()!=BEGINSYM) return "这里应该有一个begin语句的吧";
    if((word=statement(lexical_analyzer))!="") return word;
    while(lexical_analyzer.peeksymtype()!=ENDSYM)
    {
        if(lexical_analyzer.getsymtype()!=SEMICOLON) return "这里必须有一个分号的啊，如果BEGIN..END语句中有多于一句语句的话，每句语句都必须用分号分隔";
        if((word=statement(lexical_analyzer))!="") return word;
    }
    lexical_analyzer.getsymtype();
    return "";
}

std::string SyntacticAnalyzer::ifsym(LexicalAnalyzer& lexical_analyzer)
{
    std::string word;
    if(lexical_analyzer.getsymtype()!=IFSYM) return "这里应该是一个if语句";
    if((word=condition(lexical_analyzer))!="") return word;
    if(lexical_analyzer.getsymtype()!=THENSYM) return "喂喂..if语句没有跟then，你这个if语句是干啥的？if语句参考语法：IF X=0 THEN X:=1;";
    return statement(lexical_analyzer);
}

std::string SyntacticAnalyzer::whilesym(LexicalAnalyzer& lexical_analyzer)
{
    std::string word;
    if(lexical_analyzer.getsymtype()!=WHILESYM) return "这里应该是一个while语句";
    if((word=condition(lexical_analyzer))!="") return word;
    if(lexical_analyzer.getsymtype()!=DOSYM) return "这里应该有一个do的，没有do，单单一个while有神马意义呢？";
    return statement(lexical_analyzer);
}

std::string SyntacticAnalyzer::readwritesym(LexicalAnalyzer& lexical_analyzer)
{
    std::string word=lexical_analyzer.getsymtype();
    if(word!=READSYM && word!=WRITESYM) return "这里应该是一个函数";
    if(lexical_analyzer.getsymtype()!=LPAREN) return "这是函数诶！他的语法应该是 READ(X); 或者 WRITE(Y); 吧，你貌似漏了左括号";
    do{
        if(lexical_analyzer.getsymtype()!=IDENT) return "函数里面要有标识符的啊，他的语法应该是 READ(X); 或是 WRITE(A,B,C);"; 
    }while((word=lexical_analyzer.getsymtype())==COMMA);
    return (word==RPAREN?"":"函数参数要用右括号扩起来的啊，你加右括号了吗");
}

