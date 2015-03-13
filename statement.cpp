#include<string>
#include"LexicalAnalyzer.class.h"
#include"SyntacticAnalyzer.class.h"
#include"keywordlist.h"
#include"operatorlist.h"
#include"wordlist.h"

#define VALUEBUFFERLENGTH 10

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::statement(LexicalAnalyzer& lexical_analyzer)
{
	int type=lexical_analyzer.peeksymtype();
	switch(type)
	{
	case IDENTTYPE: return ident(lexical_analyzer);
	case CALLTYPE: return callsym(lexical_analyzer);
	case BEGINTYPE: return beginsym(lexical_analyzer);
	case IFTYPE: return ifsym(lexical_analyzer);
	case WHILETYPE: return whilesym(lexical_analyzer);
	case READTYPE: return readsym(lexical_analyzer);
	case WRITETYPE: return writesym(lexical_analyzer);
	}
	
	return RIGHT;
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::ident(LexicalAnalyzer& lexical_analyzer)
{
	int value;
	SyntacticAnalyzer::SyntacticAnalyzerResult res;
	SyntacticAnalyzer::IntermediateCodeNode newcodenode(BECOMESOPER);
	std::deque<SyntacticAnalyzer::IdentStackNode>::const_iterator iter;
	std::string varname;
    if(lexical_analyzer.getsymtype()!=IDENTTYPE)
	{
		this->errorinfo="这里应该有一个标识符，参考赋值语句的语法：X:=2;";
		return ERROR;
	}
	else if((iter=this->ident_stack_find(lexical_analyzer.getprevsymword().c_str(),QUERY_ALL_PROC))==this->ident_stack.end())
	{
		this->errorinfo="找不到这个标识符哦，它确实已经定义了吗？";
		return ERROR;
	}
	else if(iter->flag!=VAR)
	{
		this->errorinfo="喂喂喂！这不是变量，是常量吧！你怎么能对常量赋值呢？？";
		return ERROR;
	}
	else varname=iter->ident;
	
    if(lexical_analyzer.getsymtype()!=BECOMESTYPE)
	{
		this->errorinfo="这里应该是一个赋值符号 := ，参考赋值语句的语法：X:=2;";
		return ERROR;
	}
    if((res=expression(lexical_analyzer,value))==ERROR)
	{
		return ERROR;
	}
	else
	{
		newcodenode.oper1=this->tempvar.prev_temp_varname;
		newcodenode.oper2=varname;
		this->intermediate_code_deque_vector[this->cur_procedure_index].push_back(newcodenode);
	}
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::callsym(LexicalAnalyzer& lexical_analyzer)
{
	std::string word;
    if(lexical_analyzer.getsymtype()!=CALLTYPE)
	{
		this->errorinfo="这里应该是一个call语句，call语句的语法是这样紫滴：CALL NIMEI; 其中NIMEI是个过程的名字，事先用procedure语句定义它";
		return ERROR;
	}

    if(lexical_analyzer.getsymtype()!=IDENTTYPE)
    {
		this->errorinfo="这个应该是一个标识符，否则我怎么知道你要call谁？call语句的语法是这样紫滴：CALL NIMEI; 其中NIMEI是个过程的名字，事先用procedure语句定义它";
		return ERROR;
	}
	else if(!this->procedure_find(word=lexical_analyzer.getprevsymword()))
	{
		this->errorinfo="这神马过程啊，我真的不认识这货！";
		return ERROR;
	}
	else
	{
		this->intermediate_code_deque_vector[this->cur_procedure_index].push_back(SyntacticAnalyzer::IntermediateCodeNode(CALLSYM,word));
	}
	
	return RIGHT;
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::beginsym(LexicalAnalyzer& lexical_analyzer)
{
    if(lexical_analyzer.getsymtype()!=BEGINTYPE)
	{
		this->errorinfo="这里应该有一个begin语句的吧";
		return ERROR;
	}
    if(!statement(lexical_analyzer)) return ERROR;
    while(lexical_analyzer.peeksymtype()!=ENDTYPE)
    {
        if(lexical_analyzer.getsymtype()!=SEMICOLONTYPE)
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
	std::string point;
	SyntacticAnalyzer::SyntacticAnalyzerResult res;
    if(lexical_analyzer.getsymtype()!=IFTYPE)
	{
		this->errorinfo="这里应该是一个if语句";
		return ERROR;
	}

	point=this->temppoint.next();

    if(!(res=condition(lexical_analyzer,point,value))) return ERROR;

	//这里不生成任何条件语句，因为condition会为我们完成的

    if(lexical_analyzer.getsymtype()!=THENTYPE)
    {
		this->errorinfo="喂喂..if语句没有跟then，你这个if语句是干啥的？if语句参考语法：IF X=0 THEN X:=1;";
		return ERROR;
	}
	if(!statement(lexical_analyzer)) return ERROR;
	else if(res!=VALUE || !value) //和生成point的逻辑条件完全一致
	{
		this->intermediate_code_deque_vector[this->cur_procedure_index].push_back(
			SyntacticAnalyzer::IntermediateCodeNode(DECLARE,point));
	}
	return RIGHT;
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::whilesym(LexicalAnalyzer& lexical_analyzer)
{
	bool value;
	std::string point1,point2;
	SyntacticAnalyzer::SyntacticAnalyzerResult res;
    if(lexical_analyzer.getsymtype()!=WHILETYPE)
	{
		this->errorinfo="这里应该是一个while语句";
		return ERROR;
	}

	point1=this->temppoint.next();
	point2=this->temppoint.next();

	this->intermediate_code_deque_vector[this->cur_procedure_index].push_back(
			SyntacticAnalyzer::IntermediateCodeNode(DECLARE,point1));

    if(!(res=condition(lexical_analyzer,point2,value))) return ERROR;
    if(lexical_analyzer.getsymtype()!=DOTYPE)
	{
		this->errorinfo="这里应该有一个do的，没有do，单单一个while有神马意义呢？";
		return ERROR;
	}
	if(!statement(lexical_analyzer)) return ERROR;
	
	this->intermediate_code_deque_vector[this->cur_procedure_index].push_back(
			SyntacticAnalyzer::IntermediateCodeNode(JUMP,point1));

	if(res!=VALUE || !value) //和生成point的逻辑条件完全一致
	{
		this->intermediate_code_deque_vector[this->cur_procedure_index].push_back(
			SyntacticAnalyzer::IntermediateCodeNode(DECLARE,point2));
	}
	return RIGHT;
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::readsym(LexicalAnalyzer& lexical_analyzer)
{
    int type1=lexical_analyzer.getsymtype(),type2;
	std::deque<SyntacticAnalyzer::IdentStackNode>::const_iterator iter;
	SyntacticAnalyzer::IntermediateCodeNode newcodenode(READSYM);
	
    if(type1!=READTYPE)
	{
		this->errorinfo="这里应该是一个函数";
		return ERROR;
	}
    if(lexical_analyzer.getsymtype()!=LPARENTYPE)
	{
		 this->errorinfo="这是函数诶！他的语法应该是 READ(X); 你貌似漏了左括号";
		 return ERROR;
	}
    do{
        if(lexical_analyzer.getsymtype()!=IDENTTYPE)
		{
			 this->errorinfo="函数里面要有标识符的啊，他的语法应该是 READ(X,Y,Z);";
			 return ERROR;
		}
		else if((iter=this->ident_stack_find(lexical_analyzer.getprevsymword().c_str(),QUERY_ALL_PROC))==this->ident_stack.end())
		{
			this->errorinfo="找不到这个标识符哦，它确实已经定义了吗？";
			return ERROR;
		}
		else if(iter->flag!=VAR)
		{
			this->errorinfo="喂喂喂！这不是变量，是常量吧！你怎么能对常量赋值呢？？";
			return ERROR;
		}
		else
		{
			newcodenode.oper1=iter->ident;
			this->intermediate_code_deque_vector[this->cur_procedure_index].push_back(newcodenode);
		}
    }while((type2=lexical_analyzer.getsymtype())==COMMATYPE);
	if(type2==RPARENTYPE) return RIGHT;
	else
	{
		this->errorinfo="函数参数要用右括号扩起来的啊，你加右括号了吗";
		return ERROR;
	}
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::writesym(LexicalAnalyzer& lexical_analyzer)
{
	int value,type1=lexical_analyzer.getsymtype(),type2;
	SyntacticAnalyzer::SyntacticAnalyzerResult res;
	SyntacticAnalyzer::IntermediateCodeNode newcodenode(WRITESYM);
	char buffer[VALUEBUFFERLENGTH];

    if(type1!=WRITETYPE)
	{
		this->errorinfo="这里应该是一个函数";
		return ERROR;
	}
    if(lexical_analyzer.getsymtype()!=LPARENTYPE)
	{
		 this->errorinfo="这是函数诶！他的语法应该是 WRITE(Y); 吧，你貌似漏了左括号";
		 return ERROR;
	}
    do{
        if((res=expression(lexical_analyzer,value))==ERROR)
		{
			return ERROR;
		}
		else
		{
			if(res==RIGHT) newcodenode.oper1=this->tempvar.prev_temp_varname;
			else /*res==VALUE*/
			{
				sprintf(buffer,"%d",value);
				newcodenode.oper1=buffer;
			}
			this->intermediate_code_deque_vector[this->cur_procedure_index].push_back(newcodenode);
		}
    }while((type2=lexical_analyzer.getsymtype())==COMMATYPE);
	if(type2==RPARENTYPE) return RIGHT;
	else
	{
		this->errorinfo="函数参数要用右括号扩起来的啊，你加右括号了吗";
		return ERROR;
	}
}
