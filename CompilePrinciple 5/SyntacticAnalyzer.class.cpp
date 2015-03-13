#include"SyntacticAnalyzer.class.h"
#include<string>
#include<deque>

std::deque<SyntacticAnalyzer::IdentStackNode>::const_iterator SyntacticAnalyzer::ident_stack_find(const std::string _ident)
{
	for(std::deque<SyntacticAnalyzer::IdentStackNode>::iterator iter=this->ident_stack.begin();iter!=this->ident_stack.end();++iter)
	{
		if(iter->ident==_ident)
		{
			return iter;
		}
	}
	return this->ident_stack.end();
}

void SyntacticAnalyzer::ident_stack_clear()
{
	while(this->ident_stack.back().flag!=BOUND)
	{
		this->ident_stack.pop_back();
	}
	this->ident_stack.pop_back();
}

std::string SyntacticAnalyzer::TempVar::next()
{
	char buffer[10];
	sprintf(buffer,"%d",this->seqnum++);
	return std::string(TEMP_VARNAME_PREFIX)+buffer;
}

bool SyntacticAnalyzer::TempVar::isTempVar(const std::string varname)
{
	const std::string temp_varname_prefix=TEMP_VARNAME_PREFIX;
	return temp_varname_prefix==varname.substr(0,temp_varname_prefix.size());
}