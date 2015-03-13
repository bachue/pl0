#include"CompilePrincipleInterpreter.class.h"
#include"../keywordlist.h"
#include"../operatorlist.h"
#include"../intermediatecodelist.h"
#include<string>
#include<iostream>
#include<fstream>
#include<deque>
#include<map>

#define COUNT_OPER	4

#define ISNUM(VARNAME) ((VARNAME[0]>='0' && VARNAME[0]<='9')||(VARNAME[0]=='-' && VARNAME[1]>='0' && VARNAME[1]<='9'))

#define SETOPERVAL int oper1_val,oper2_val;\
{\
	if(ISNUM(oper1)) sscanf(oper1.c_str(),"%d",&oper1_val);\
	else oper1_val=this->get_var(oper1);\
	\
	if(ISNUM(oper2)) sscanf(oper2.c_str(),"%d",&oper2_val);\
	else oper2_val=this->get_var(oper2);\
}

#define ARITHMETICS(OPER)\
{\
	SETOPERVAL\
	this->set_var(oper3,oper1_val OPER oper2_val);\
	++this->ip;\
}

#define CONDITION(OPER)\
{\
	SETOPERVAL\
	if(oper1_val OPER oper2_val)\
	{\
		++this->ip;\
	}\
	else\
	{\
		this->ip=this->point_map.find(point)->second;\
	}\
}\

void CompilePrincipleInterpreter::interpreter(const std::string& filename)
{
	std::ifstream file(filename.c_str(),std::ios::in);
	std::string file_row,oper[COUNT_OPER];
	int start,end,found,i;

	if(file.fail())
	{
		std::cerr << filename << ": 文件打开错误" << std::endl;
		return;
	}

	while(std::getline(file,file_row))
	{
		start=file_row.find_first_of(LPARAN);
		if(start==std::string::npos)
		{
			if((end=file_row.find_last_of(COLON))!=std::string::npos)
			{
				this->collect_point(file_row.substr(0,end),this->code_stack.size());
			}
			continue;
		}
		end=file_row.find_last_of(RPARAN);
		if(end==std::string::npos) continue;

		for(i=0;(found=file_row.find_first_of(DELIMITER,++start))!=std::string::npos && i<COUNT_OPER-1;++i)
		{
			oper[i]=file_row.substr(start,found-start);
			start=found;
		}
		oper[i]=file_row.substr(start,end-start);

		this->code_stack.push_back(CompilePrincipleInterpreter::IntermediateCodeNode(oper[0],oper[1],oper[2],oper[3]));
	}
	file.close();

	while(this->ip<this->code_stack.size())
	{
		this->command(this->code_stack[this->ip].command,
					  this->code_stack[this->ip].oper1,
					  this->code_stack[this->ip].oper2,
					  this->code_stack[this->ip].oper3);
	}
}

void CompilePrincipleInterpreter::collect_point(const std::string& row,const int ip)
{
	this->point_map.insert(std::make_pair(row,ip));
}

void CompilePrincipleInterpreter::command(const std::string& command, const std::string& oper1, const std::string& oper2, const std::string& oper3)
{
#ifdef DISPLAY
	std::cout << command << ' ' << oper1 << ' ' << oper2 << ' ' << oper3 << std::endl;
#endif
	if(command==VARSYM)
	{
		var(oper1);
	}
	else if(command==BECOMESOPER)
	{
		becomes(oper1,oper2);
	}
	else if(command==READSYM)
	{
		read(oper1);
	}
	else if(command==WRITESYM)
	{
		write(oper1);
	}
	else if(command==PLUSOPER)
	{
		plus(oper1,oper2,oper3);
	}
	else if(command==MINUSOPER)
	{
		minus(oper1,oper2,oper3);
	}
	else if(command==TIMESOPER)
	{
		times(oper1,oper2,oper3);
	}
	else if(command==SLASHOPER)
	{
		slash(oper1,oper2,oper3);
	}
	else if(command==ODDSYM)
	{
		odd(oper1,oper2);
	}
	else if(command==JUMP)
	{
		jump(oper1);
	}
	else if(command==GEQOPER)
	{
		geq(oper1,oper2,oper3);
	}
	else if(command==GTROPER)
	{
		gtr(oper1,oper2,oper3);
	}
	else if(command==LEQOPER)
	{
		leq(oper1,oper2,oper3);
	}
	else if(command==LSSOPER)
	{
		lss(oper1,oper2,oper3);
	}
	else if(command==EQLOPER)
	{
		eql(oper1,oper2,oper3);
	}
	else if(command==NEQOPER)
	{
		neq(oper1,oper2,oper3);
	}
	else if(command==CALLSYM)
	{
		call(oper1);
	}
	else if(command==RETURN)
	{
		ret();
	}
	else if(command==HALT)
	{
		this->ip=this->code_stack.size();
	}
}

void CompilePrincipleInterpreter::var(const std::string& varname)
{
	this->create_var(varname);
	++this->ip;
}

void CompilePrincipleInterpreter::becomes(const std::string& valuename,const std::string& varname)
{
	int val;
	if(ISNUM(valuename)) //这里不进行任何验证性判断，如果中间代码非法，将会段错误
	{
		sscanf(valuename.c_str(),"%d",&val);
		this->set_var(varname,val);
	}
	else
	{
		this->set_var(varname,this->get_var(valuename));
	}
	++this->ip;
}

void CompilePrincipleInterpreter::read(const std::string& varname)
{
	int buf;
	std::cin >> buf;
	this->set_var(varname,buf);
	++this->ip;
}

void CompilePrincipleInterpreter::write(const std::string& valuename)
{
	int val;
	if(ISNUM(valuename)) //这里不进行任何验证性判断，如果中间代码非法，将会段错误
	{
		sscanf(valuename.c_str(),"%d",&val);
		std::cout << val << std::endl;
	}
	else
	{
		std::cout << this->get_var(valuename) << std::endl;
	}
	++this->ip;
}

void CompilePrincipleInterpreter::plus(const std::string& oper1, const std::string& oper2, const std::string& oper3)
{
	ARITHMETICS(+)
}

void CompilePrincipleInterpreter::minus(const std::string& oper1, const std::string& oper2, const std::string& oper3)
{
	ARITHMETICS(-)
}

void CompilePrincipleInterpreter::times(const std::string& oper1, const std::string& oper2, const std::string& oper3)
{
	ARITHMETICS(*)
}

void CompilePrincipleInterpreter::slash(const std::string& oper1, const std::string& oper2, const std::string& oper3)
{
	ARITHMETICS(/)
}

void CompilePrincipleInterpreter::call(const std::string& point)
{
	this->runtime_stack.push_back(CompilePrincipleInterpreter::RuntimeStackNode(this->ip+1));
	this->ip=this->point_map.find(point)->second;
}

void CompilePrincipleInterpreter::ret()
{
	this->ip=this->runtime_stack.back().ip;
	this->runtime_stack.pop_back();
}

void CompilePrincipleInterpreter::odd(const std::string& oper1, const std::string& point)
{
	if(this->get_var(oper1)%2==1)//奇数
	{
		++this->ip;
	}
	else/*偶数*/
	{
		this->ip=this->point_map.find(point)->second;
	}
}

void CompilePrincipleInterpreter::jump(const std::string& point)
{
	this->ip=this->point_map.find(point)->second;
}

void CompilePrincipleInterpreter::geq(const std::string& oper1, const std::string& oper2, const std::string& point)
{
	CONDITION(>=)
}

void CompilePrincipleInterpreter::gtr(const std::string& oper1, const std::string& oper2, const std::string& point)
{
	CONDITION(>)
}

void CompilePrincipleInterpreter::leq(const std::string& oper1, const std::string& oper2, const std::string& point)
{
	CONDITION(<=)
}

void CompilePrincipleInterpreter::lss(const std::string& oper1, const std::string& oper2, const std::string& point)
{
	CONDITION(<)
}

void CompilePrincipleInterpreter::eql(const std::string& oper1, const std::string& oper2, const std::string& point)
{
	CONDITION(==)
}

void CompilePrincipleInterpreter::neq(const std::string& oper1, const std::string& oper2, const std::string& point)
{
	CONDITION(!=)
}

int CompilePrincipleInterpreter::get_var(const std::string& varname) const
{
	std::map<std::string,int>::const_iterator found_iter;
	for(std::deque<CompilePrincipleInterpreter::RuntimeStackNode>::const_reverse_iterator iter=this->runtime_stack.rbegin();
			iter!=this->runtime_stack.rend();++iter)
	{
		if((found_iter=iter->var_map.find(varname))!=iter->var_map.end())
		{
			return found_iter->second;
		}
	}//if not found, error
}

void CompilePrincipleInterpreter::set_var(const std::string& varname,const int value)
{
	std::map<std::string,int>::iterator found_iter;
	for(std::deque<CompilePrincipleInterpreter::RuntimeStackNode>::reverse_iterator iter=this->runtime_stack.rbegin();
			iter!=this->runtime_stack.rend();++iter)
	{
		if((found_iter=iter->var_map.find(varname))!=iter->var_map.end())
		{
			found_iter->second=value;
			return;
		}
	}//if not found, error
}

void CompilePrincipleInterpreter::create_var(const std::string& varname)
{
	this->runtime_stack.back().var_map.insert(std::make_pair(varname,0));
}