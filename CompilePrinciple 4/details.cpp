#include<string>
#include"LexicalAnalyzer.class.h"
#include"SyntacticAnalyzer.class.h"
#include"operatorlist.h"
#include"keywordlist.h"
#include"wordlist.h"

int SyntacticAnalyzer::condition(LexicalAnalyzer& lexical_analyzer,bool& value)
{
	int res=1,tmp_value,tmp_value2,tmp_res=1,tmp_res2=1;
    std::string word=lexical_analyzer.peeksymtype();
    if(word==ODDSYM)
	{
		lexical_analyzer.getsymtype();
		if(!(res=expression(lexical_analyzer,tmp_value))) return false;
		else if(res==2) value=tmp_value%2==1;
		
		return res;
	}
    else
    {
        if(!(tmp_res=expression(lexical_analyzer,tmp_value))) return false;
        if(word=lexical_analyzer.getsymtype(),
           word!=EQL && word!=NEQ && word!=LSS && word!=LEQ && word!=GTR && word!=GEQ)
		{
			this->errorinfo="这里应该有一个比较运算符，例如等于 = ，不等于 # ，小于 < ，小于等于 <= ，大于 > ，大于等于 >= ";	
			return false;
		}
		else
		{
			if(!(tmp_res2=expression(lexical_analyzer,tmp_value2))) return false;
			else if(tmp_res==2 && tmp_res2==2)
			{
				if(word==EQL) value=tmp_value==tmp_value2;
				else if(word==NEQ) value=tmp_value!=tmp_value2;
				else if(word==LSS) value=tmp_value<tmp_value2;
				else if(word==LEQ) value=tmp_value<=tmp_value2;
				else if(word==GTR) value=tmp_value>tmp_value2;
				else if(word==GEQ) value=tmp_value>=tmp_value2;
				return 2;
			}
			return 1;
		}
    }
}

int SyntacticAnalyzer::expression(LexicalAnalyzer& lexical_analyzer,int& value)
{
	int fac=1,res=1,tmp_res=1,tmp_value;
    std::string type=lexical_analyzer.peeksymtype();
    if(type==PLUS || type==MINUS)
	{
		if(type==MINUS) fac=-1;
		lexical_analyzer.getsymtype();
	}
    if(!(res=item(lexical_analyzer,value))) return false;
	else if(res==2) value*=fac;//之前是符号，让它乘以符号因子1或-1
	
    while(type=lexical_analyzer.peeksymtype(),type==PLUS || type==MINUS)
    {
        if(lexical_analyzer.getsymtype(),!(tmp_res=item(lexical_analyzer,tmp_value))) return false;
		if(res==2 && tmp_res==2)
		{
			if(type==PLUS) value+=tmp_value;
			else /*if(type==MINUS)*/ value-=tmp_value;
		}
		else res=1;
    }
 
    return res;
}

int SyntacticAnalyzer::item(LexicalAnalyzer& lexical_analyzer,int& value)
{
	int res=1,tmp_res=1,tmp_value;
    std::string type;
    if(!(res=factor(lexical_analyzer,value))) return false;
	//如果能走到这一步，res只会是1或2
    while(type=lexical_analyzer.peeksymtype(),type==TIMES || type==SLASH)
    {
        if(lexical_analyzer.getsymtype(),!(tmp_res=factor(lexical_analyzer,tmp_value))) return false;
		//如果能走到这一步，tmp_res只会是1或2
		else if(res==2 && tmp_res==2)
		{
			if(type==TIMES) value*=tmp_value;
			else/* if(type==SLASH)*/ value/=tmp_value;
		}
		else res=1;//只要有一次tmp_res不是2，那么对value的计算毫无意义，使res=1来避免此计算
    }
    return res;
}

int SyntacticAnalyzer::factor(LexicalAnalyzer& lexical_analyzer,int& value)
{
	int res=1;//0: 语法错 1: 语法对 不会返回语义值 2: 语法对 会返回语义值 
    std::string type=lexical_analyzer.getsymtype();
	std::string word=lexical_analyzer.getprevsymword();
    if(type!=IDENT && type!=NUMBER && type!=LPAREN)
	{
		this->errorinfo="这里应该是一个标识符或是一个数字或是一个表达式";
		return false;
	}
    if(type==NUMBER)
	{
		sscanf(word.c_str(),"%d",&value);
		return 2;
	}
    if(type==LPAREN)
	{
		if(!(res=expression(lexical_analyzer,value))) return false;
		if(lexical_analyzer.getsymtype()!=RPAREN)
		{
			this->errorinfo="这里应该有一个右括号，表达式必须用一对括号扩起来";
			return false;
		}
	}
    return res;
}

