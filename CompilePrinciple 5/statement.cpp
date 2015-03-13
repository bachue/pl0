#include<string>
#include"LexicalAnalyzer.class.h"
#include"SyntacticAnalyzer.class.h"
#include"keywordlist.h"
#include"operatorlist.h"
#include"wordlist.h"

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::statement(LexicalAnalyzer& lexical_analyzer)
{
	std::string type=lexical_analyzer.peeksymtype();
	if(type==IDENT) return ident(lexical_analyzer);
	if(type==CALLSYM) return callsym(lexical_analyzer);
	if(type==BEGINSYM) return beginsym(lexical_analyzer);
	if(type==IFSYM) return ifsym(lexical_analyzer);
	if(type==WHILESYM) return whilesym(lexical_analyzer);
	if(type==READSYM || type==WRITESYM) return readwritesym(lexical_analyzer);
	
	return RIGHT;
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::ident(LexicalAnalyzer& lexical_analyzer)
{
	int value;
	std::deque<SyntacticAnalyzer::IdentStackNode>::const_iterator iter;
    if(lexical_analyzer.getsymtype()!=IDENT)
	{
		this->errorinfo="这里应该有一个标识符，参考赋值语句的语法：X:=2;";
		return ERROR;
	}
	else if((iter=this->ident_stack_find(lexical_analyzer.getprevsymword().c_str()))==this->ident_stack.end())
	{
		this->errorinfo="找不到这个标识符哦，它确实已经定义了吗？";
		return ERROR;
	}
	else if(iter->flag!=VAR)
	{
		this->errorinfo="喂喂喂！这不是变量，是常量吧！你怎么能对常量赋值呢？？";
		return ERROR;
	}

    if(lexical_analyzer.getsymtype()!=BECOMES) 
	{
		this->errorinfo="这里应该是一个赋值符号 := ，参考赋值语句的语法：X:=2;";
		return ERROR;
	}
    return expression(lexical_analyzer,value);
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::callsym(LexicalAnalyzer& lexical_analyzer)
{
    if(lexical_analyzer.getsymtype()!=CALLSYM) 
	{
		this->errorinfo="这里应该是一个call语句，call语句的语法是这样紫滴：CALL NIMEI; 其中NIMEI是个过程的名字，事先用procedure语句定义它";
		return ERROR;
	}

    if(lexical_analyzer.getsymtype()!=IDENT)
    {
		this->errorinfo="这个应该是一个标识符，否则我怎么知道你要call谁？call语句的语法是这样紫滴：CALL NIMEI; 其中NIMEI是个过程的名字，事先用procedure语句定义它";
		return ERROR;
	}
	else if(this->procedure_set.find(lexical_analyzer.getprevsymword())==this->procedure_set.end())
	{
		this->errorinfo="这神马过程啊，我真的不认识这货！";
		return ERROR;
	}
	
	return RIGHT;
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::beginsym(LexicalAnalyzer& lexical_analyzer)
{
    if(lexical_analyzer.getsymtype()!=BEGINSYM) 
	{
		this->errorinfo="这里应该有一个begin语句的吧";
		return ERROR;
	}
    if(!statement(lexical_analyzer)) return ERROR;
    while(lexical_analyzer.peeksymtype()!=ENDSYM)
    {
        if(lexical_analyzer.getsymtype()!=SEMICOLON)
        {
			this->errorinfo="这里必须有一个分号的啊，如果BEGIN..END语句中有多于一句语句的话，每句语句都必须用分号分隔";
			return ERROR;
		}
		if(!statement(lexical_analyzer)) return ERROR;
    }
    lexical_analyzer.getsymtype();
    return RIGHT;
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::ifsym(LexicalAnalyzer& lexical_analyzer)
{
	bool value;
    if(lexical_analyzer.getsymtype()!=IFSYM) 
	{
		this->errorinfo="这里应该是一个if语句";
		return ERROR;
	}
    if(!condition(lexical_analyzer,value)) return ERROR;
    if(lexical_analyzer.getsymtype()!=THENSYM)
    {
		this->errorinfo="喂喂..if语句没有跟then，你这个if语句是干啥的？if语句参考语法：IF X=0 THEN X:=1;";
		return ERROR;
	}
	return statement(lexical_analyzer);
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::whilesym(LexicalAnalyzer& lexical_analyzer)
{
	bool value;
    if(lexical_analyzer.getsymtype()!=WHILESYM) 
	{
		this->errorinfo="这里应该是一个while语句";
		return ERROR;
	}
    if(!condition(lexical_analyzer,value)) return ERROR;
    if(lexical_analyzer.getsymtype()!=DOSYM) 
	{
		this->errorinfo="这里应该有一个do的，没有do，单单一个while有神马意义呢？";
		return ERROR;
	}
    return statement(lexical_analyzer);
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::readwritesym(LexicalAnalyzer& lexical_analyzer)
{
    std::string type1=lexical_analyzer.getsymtype(),type2;
	std::deque<SyntacticAnalyzer::IdentStackNode>::const_iterator iter;
    if(type1!=READSYM && type1!=WRITESYM)
	{
		this->errorinfo="这里应该是一个函数";
		return ERROR;
	}
    if(lexical_analyzer.getsymtype()!=LPAREN)
	{
		 this->errorinfo="这是函数诶！他的语法应该是 READ(X); 或者 WRITE(Y); 吧，你貌似漏了左括号";
		 return ERROR;
	}
    do{
        if(lexical_analyzer.getsymtype()!=IDENT)
		{
			 this->errorinfo="函数里面要有标识符的啊，他的语法应该是 READ(X); 或是 WRITE(A,B,C);";
			 return ERROR;
		}
		else if((iter=this->ident_stack_find(lexical_analyzer.getprevsymword().c_str()))==this->ident_stack.end())
		{
			this->errorinfo="找不到这个标识符哦，它确实已经定义了吗？";
			return ERROR;
		}
		else if(type1==READSYM && iter->flag!=VAR)
		{
			this->errorinfo="喂喂喂！这不是变量，是常量吧！你怎么能对常量赋值呢？？";
			return ERROR;
		}
    }while((type2=lexical_analyzer.getsymtype())==COMMA);
	if(type2==RPAREN) return RIGHT;
	else
	{
		this->errorinfo="函数参数要用右括号扩起来的啊，你加右括号了吗";
		return ERROR;
	}
}
