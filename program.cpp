#include<string>
#include"LexicalAnalyzer.class.h"
#include"SyntacticAnalyzer.class.h"
#include"keywordlist.h"
#include"operatorlist.h"
#include"wordlist.h"

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::analyze(LexicalAnalyzer& lexical_analyzer)
{
	if(!subprogram(lexical_analyzer)) return ERROR;
	if(lexical_analyzer.peeksymtype()!=PERIODTYPE)
	{
	    if(lexical_analyzer.peeksymtype()==0) this->errorinfo="程序有错，这个程序到这里看上去应该已经结束了，但其实不是。是不是少了运算符或漏了左括号？";
		else this->errorinfo="必须在程序结尾加一个句号，不要问我为神马，这就是语法！";
		return ERROR;
	}
	else
	{
		this->intermediate_code_deque_vector[this->cur_procedure_index].push_back(SyntacticAnalyzer::IntermediateCodeNode(HALT));
	}
	return RIGHT;
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::subprogram(LexicalAnalyzer& lexical_analyzer)
{
	int type=lexical_analyzer.peeksymtype();
	
	if(type==CONSTTYPE)
	{
		if(!constsym(lexical_analyzer)) return ERROR;
		type=lexical_analyzer.peeksymtype();
	}
	if(type==VARTYPE)
	{
		if(!varsym(lexical_analyzer)) return ERROR;
		type=lexical_analyzer.peeksymtype();
	}
	if(type==PROCTYPE)
	{
		do
		{
			if(!procsym(lexical_analyzer)) return ERROR;
		}while((type=lexical_analyzer.peeksymtype())==PROCTYPE);
	}
	
	return statement(lexical_analyzer);
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::constsym(LexicalAnalyzer& lexical_analyzer)
{
	int type;
	SyntacticAnalyzer::IdentStackNode newnode(CONST);
	if(lexical_analyzer.getsymtype()!=CONSTTYPE)
	{
		this->errorinfo="这里应该是一个const语句";
		return ERROR;
	}
	do
	{
		if(lexical_analyzer.getsymtype()!=IDENTTYPE)
		{
			this->errorinfo="这里应该是一个标识符，例如：CONST X=6,Y=4;";
			return ERROR;
		}
		else if(this->ident_stack_find(lexical_analyzer.getprevsymword().c_str(),QUERY_CUR_PROC)!=this->ident_stack.end())
		{
			this->errorinfo="我说，常量名好像有重复啊，你检查下，看看本级的，上一级的常量或者变量有没有叫这名儿的";
			return ERROR;
		}
		else newnode.ident=lexical_analyzer.getprevsymword();

		if(lexical_analyzer.getsymtype()!=EQLTYPE)
		{
			this->errorinfo="这里应该是一个等于符号，例如：CONST X=6,Y=4;";
			return ERROR;
		}
		if(lexical_analyzer.getsymtype()!=NUMBERTYPE)
		{
			this->errorinfo="这里应该是一个数字，例如：CONST X=6,Y=4;";
			return ERROR;
		}
		else
		{
			sscanf(lexical_analyzer.getprevsymword().c_str(),"%d",&newnode.value);
			this->ident_stack.push_back(newnode);
		}
	}while((type=lexical_analyzer.getsymtype())==COMMATYPE);
	
	if(type==SEMICOLONTYPE) return RIGHT;
	else
	{
		this->errorinfo="const语句必须以分号结尾，例如：CONST X=6,Y=4; 请留意末尾的分号";
		return ERROR;
	}
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::varsym(LexicalAnalyzer& lexical_analyzer)
{
	int type;
	SyntacticAnalyzer::IdentStackNode newnode(VAR);
	if(lexical_analyzer.getsymtype()!=VARTYPE)
	{
		this->errorinfo="这里应该是一个var语句";
		return ERROR;
	}
	do
	{
		if(lexical_analyzer.getsymtype()!=IDENTTYPE)
		{
			this->errorinfo="这里应该是一个标识符，例如：VAR X,Y,Z;";
			return ERROR;
		}
		else if(this->ident_stack_find(lexical_analyzer.getprevsymword().c_str(),QUERY_CUR_PROC)!=this->ident_stack.end())
		{
			this->errorinfo="我说，变量名好像有重复啊，你检查下，看看本级的，上一级的变量或者常量有没有叫这名儿的";
			return ERROR;
		}
		else
		{
			newnode.ident=lexical_analyzer.getprevsymword();
			this->ident_stack.push_back(newnode);
			this->declare_new_var(newnode.ident);
		}
	}while((type=lexical_analyzer.getsymtype())==COMMATYPE);
	if(type==SEMICOLONTYPE) return RIGHT;
	else
	{
		this->errorinfo="var语句必须以分号结尾，例如：VAR X,Y,Z; 请留意末尾的分号";
		return ERROR;
	}
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::procsym(LexicalAnalyzer& lexical_analyzer)
{
	std::string word;
	if(lexical_analyzer.getsymtype()!=PROCTYPE)
	{
		this->errorinfo="这里应该是一个procedure语句";
		return ERROR;
	}
	
	if(lexical_analyzer.getsymtype()!=IDENTTYPE)
	{
		this->errorinfo="这里应该有一个标识符，作为procedure的名字，例如：PROCEDURE PROC;";
		return ERROR;
	}
	else if(this->procedure_find(word=lexical_analyzer.getprevsymword()))
	{
		this->errorinfo="过程名好像重复了耶！要记住哦，PL/0不能出现重复的过程名哦！一定不能重复哦！乖！";
		return ERROR;
	}
	else
	{
		this->ident_stack.push_back(SyntacticAnalyzer::IdentStackNode(BOUND,this->cur_procedure_name,this->cur_procedure_index));
		this->cur_procedure_name=word;
		this->cur_procedure_index=intermediate_code_deque_vector.size();
		this->intermediate_code_deque_vector.push_back(std::deque<IntermediateCodeNode>());
		this->procedure_vector.push_back(word);
	}

	if(lexical_analyzer.getsymtype()!=SEMICOLONTYPE)
	{
		this->errorinfo="这里应该有一个分号，作为procedure声明和内容的分隔符，例如：PROCEDURE PROC; 请留意末尾的分号";
		return ERROR;
	}
	if(!subprogram(lexical_analyzer)) return ERROR;
	else
	{
		this->ident_stack_clear();
	}

	if(lexical_analyzer.getsymtype()!=SEMICOLONTYPE)
	{
		this->errorinfo="这里应该有一个分号，作为一个procedure结束，我知道加一个分号很无聊也不能促进减肥，但是这是语法，是规定，只要你爸不是李刚，就必需遵守这个规定";
		return ERROR;
	}
	return RIGHT;
}
