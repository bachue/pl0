#include"SyntacticAnalyzer.class.h"
#include"keywordlist.h"
#include"intermediatecodelist.h"
#include<string>
#include<deque>

#define TEMP_VARNAME_PREFIX "tempvar_"
#define TEMP_POINT_PREFIX ""

std::deque<SyntacticAnalyzer::IdentStackNode>::const_iterator SyntacticAnalyzer::ident_stack_find(const std::string& _ident,const QUERYWAY query_way) const
{
	for(std::deque<SyntacticAnalyzer::IdentStackNode>::const_reverse_iterator iter=this->ident_stack.rbegin();iter!=this->ident_stack.rend();++iter)
	{
		if(iter->ident==_ident)
		{
			return iter.base()-1;
		}
		else if(iter->flag==BOUND && query_way==QUERY_CUR_PROC) break;
	}
	return this->ident_stack.end();
}

bool SyntacticAnalyzer::procedure_find(const std::string& _ident) const
{
	for(std::vector<std::string>::const_iterator iter=this->procedure_vector.begin();
												 iter!=this->procedure_vector.end();++iter)
	{
		if(*iter==_ident)
		{
			return true;
		}
	}
	return false;
}

void SyntacticAnalyzer::ident_stack_clear()
{
	this->intermediate_code_deque_vector[this->cur_procedure_index].push_back(SyntacticAnalyzer::IntermediateCodeNode(RETURN));
	
	while(this->ident_stack.back().flag!=BOUND)
	{
		this->ident_stack.pop_back();
	}
	this->cur_procedure_name=this->ident_stack.back().ident;
	this->cur_procedure_index=this->ident_stack.back().value;
	this->ident_stack.pop_back();
}

std::string SyntacticAnalyzer::TempVar::next()
{
	char buffer[10];
	sprintf(buffer,"%d",this->seqnum++);
	return std::string(TEMP_VARNAME_PREFIX)+buffer;
}

std::string SyntacticAnalyzer::TempPoint::next()
{
	char buffer[10];
	sprintf(buffer,"%d",this->seqnum++);
	return std::string(TEMP_POINT_PREFIX)+buffer;
}

void SyntacticAnalyzer::declare_new_var(const std::string& varname)
{
	this->intermediate_code_deque_vector[this->cur_procedure_index].push_back(SyntacticAnalyzer::IntermediateCodeNode(VARSYM,varname));
}

bool SyntacticAnalyzer::TempVar::isTempVar(const std::string& varname)
{
	const std::string temp_varname_prefix=TEMP_VARNAME_PREFIX;
	return temp_varname_prefix==varname.substr(0,temp_varname_prefix.size());
}

void SyntacticAnalyzer::print_intermediate_code(std::ostream& out) const
{
	int i;
	std::deque<IntermediateCodeNode>::const_iterator iter;
	for(i=0;i<this->intermediate_code_deque_vector.size();++i)
	{
		out << this->procedure_vector[i] << ':' << '\n';
		for(iter=this->intermediate_code_deque_vector[i].begin();
												iter!=this->intermediate_code_deque_vector[i].end();++iter)
		{//必须对DECLARE做特别判断，这仅仅是个妥协，不是合法指令
			if(iter->command==DECLARE)
			{
				out << iter->oper1 << COLON << std::endl;
			}
			else
			{
				out << LPARAN << iter->command;
				if(iter->oper1!="") out << DELIMITER << iter->oper1;
				if(iter->oper2!="") out << DELIMITER << iter->oper2;
				if(iter->oper3!="") out << DELIMITER << iter->oper3;
				out << RPARAN << std::endl;
			}
		}
	}
}