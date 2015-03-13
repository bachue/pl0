#include<string>
#include"LexicalAnalyzer.class.h"
#include"SyntacticAnalyzer.class.h"
#include"operatorlist.h"
#include"keywordlist.h"
#include"wordlist.h"

#define VALUEBUFFERLENGTH 10

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::condition(LexicalAnalyzer& lexical_analyzer,bool& value)
{
	SyntacticAnalyzer::SyntacticAnalyzerResult res=RIGHT,tmp_res=RIGHT,tmp_res2=RIGHT;
	int tmp_value,tmp_value2;
    std::string type=lexical_analyzer.peeksymtype();
    if(type==ODDSYM)
	{
		lexical_analyzer.getsymtype();
		if(!(res=expression(lexical_analyzer,tmp_value))) return ERROR;
		else if(res==VALUE) value=tmp_value%2==1;
		
		return res;
	}
    else
    {
        if(!(tmp_res=expression(lexical_analyzer,tmp_value))) return ERROR;
        if(type=lexical_analyzer.getsymtype(),
           type!=EQL && type!=NEQ && type!=LSS && type!=LEQ && type!=GTR && type!=GEQ)
		{
			this->errorinfo="这里应该有一个比较运算符，例如等于 = ，不等于 # ，小于 < ，小于等于 <= ，大于 > ，大于等于 >= ";	
			return ERROR;
		}
		else
		{
			if(!(tmp_res2=expression(lexical_analyzer,tmp_value2))) return ERROR;
			else if(tmp_res==VALUE && tmp_res2==VALUE)
			{
				if(type==EQL) value=tmp_value==tmp_value2;
				else if(type==NEQ) value=tmp_value!=tmp_value2;
				else if(type==LSS) value=tmp_value<tmp_value2;
				else if(type==LEQ) value=tmp_value<=tmp_value2;
				else if(type==GTR) value=tmp_value>tmp_value2;
				else if(type==GEQ) value=tmp_value>=tmp_value2;
				return VALUE;
			}
			return RIGHT;
		}
    }
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::expression(LexicalAnalyzer& lexical_analyzer,int& value)
{
	SyntacticAnalyzer::SyntacticAnalyzerResult res=RIGHT,tmp_res=RIGHT;
	SyntacticAnalyzer::IntermediateCodeNode newcodenode;
	int fac=1,tmp_value,some_value_to_optimize=0/*加减法这里是0,乘除法是1*/;
	char buffer[VALUEBUFFERLENGTH];
    std::string type=lexical_analyzer.peeksymtype(),oper1;

	for(;;)
	{
		type=lexical_analyzer.peeksymtype();
		if(type==PLUS || type==MINUS)
		{
			if(type==MINUS) fac=-1;
			lexical_analyzer.getsymtype();
		}
		if(!(res=item(lexical_analyzer,value))) return ERROR;
		else if(res==VALUE && value==0 && (lexical_analyzer.peeksymtype()==PLUS || lexical_analyzer.peeksymtype()==MINUS)) continue;
		else if(res==VALUE)//既然是值，我们保留值的样子，不生成中间代码，以期待接下来的操作数都以值的形式出现，这样就可以实现优化
		{
			value*=fac;//之前是符号，让它乘以符号因子1或-1
			sprintf(buffer,"%d",value);
			this->tempvar.prev_temp_varname=buffer;//这里如果item返回值，替换掉最后一个临时变量
		}
		else if(fac==-1 && res==RIGHT)//如果这是个变量且在前面加过负号，在这里将出现（times -1 var tempvar）的中间代码
		{
			newcodenode.command=TIMESOPER;
			sprintf(buffer,"%d",-1);
			newcodenode.oper1=buffer;
			newcodenode.oper2=this->tempvar.prev_temp_varname;//prev_temp_varname里存储item函数中的最后结果
			newcodenode.oper3=this->tempvar.next();
			this->intermediate_code_deque.push_back(newcodenode);
			this->tempvar.prev_temp_varname=newcodenode.oper3;
		}
		//如果fac==1 && res==RIGHT，那么前一个结果的最后变量还在this->tempvar.prev_temp_varname不动
		//这样一来，this->tempvar.prev_temp_varname就始终代表前一个变量或是值了
		//然后，我们把this->tempvar.prev_temp_varname保存在oper1里，因为等会还要调用item函数，this->tempvar.prev_temp_varname将会被替换
		oper1=this->tempvar.prev_temp_varname;
		break;
	}
	
    while(type=lexical_analyzer.peeksymtype(),type==PLUS || type==MINUS)
    {
        if(lexical_analyzer.getsymtype(),!(tmp_res=item(lexical_analyzer,tmp_value))) return ERROR;

		else if(tmp_res==VALUE && tmp_value==0)
		{
			this->tempvar.prev_temp_varname=oper1;//抛弃0这个结果
			continue;//对于加减法，如果 加/减 数为0,不再继续做下去，直接下一轮迭代。
		}

		else if(res==VALUE && tmp_res==VALUE)
		{
			if(type==PLUS) value+=tmp_value;
			else /*if(type==MINUS)*/ value-=tmp_value;
			//我们必须继续保持oper1为前一个操作数不变，必须继续保持，希望不要在哪里忘记掉
			sprintf(buffer,"%d",value);
			this->tempvar.prev_temp_varname=oper1=buffer;//我们还要让this->tempvar.prev_temp_varname也为最新的value，
														//万一在这里结束，this->tempvar.prev_temp_varname里能够保存本函数最后的结果值
		}
		else
		{
			//对于一旦出现res还是VALUE而tmp_res已不是VALUE的情况，将会保存res为一临时变量，然后让这个临时变量参与接下来一切计算
			if(res==VALUE /* && tmp_res==RIGHT*/)
			{
				res=RIGHT;
				
				newcodenode.command=(type==PLUS?PLUSOPER:MINUSOPER);//PLUS OR MINUS
				newcodenode.oper1=oper1;
				newcodenode.oper2=this->tempvar.prev_temp_varname;
				if(SyntacticAnalyzer::TempVar::isTempVar(this->tempvar.prev_temp_varname)) newcodenode.oper3=this->tempvar.prev_temp_varname;
				else newcodenode.oper3=this->tempvar.next();

				this->intermediate_code_deque.push_back(newcodenode);
				this->tempvar.prev_temp_varname=oper1=newcodenode.oper3;
			}
			else if(tmp_res==VALUE /*&& res==RIGHT*/)
			{
				if(type==PLUS)
				{
					some_value_to_optimize+=tmp_value;
				}
				else /*if(type==MINUS)*/
				{
					some_value_to_optimize-=tmp_value;
				}
			}
			else
			{
				newcodenode.command=(type==PLUS?PLUSOPER:MINUSOPER);//不是PLUS就是MINUS
				newcodenode.oper1=oper1;
				newcodenode.oper2=this->tempvar.prev_temp_varname;	//item的结果，可能是值。注意，即使是返回值，this->tempvar.prev_temp_varname内也必须是结果值的字符串
																	//调用他的函数不应为此负责，简化下代码
				//由于非常复杂，可能出现的情况太多，这里仅仅依靠前缀来判断是否是临时变量，如果是临时变量就是可以修改的。这里关键是不能修改掉用户定义的变量常量或是值
				if(SyntacticAnalyzer::TempVar::isTempVar(newcodenode.oper1)) newcodenode.oper3=newcodenode.oper1;
				else if(SyntacticAnalyzer::TempVar::isTempVar(newcodenode.oper2)) newcodenode.oper3=newcodenode.oper2;
				else newcodenode.oper3=this->tempvar.next();

				this->intermediate_code_deque.push_back(newcodenode);
				this->tempvar.prev_temp_varname=oper1=newcodenode.oper3;  //oper1和this->tempvar.prev_temp_varname同时保存前一个结果，一个给函数返回以后使用，一个给下一个循环使用
			}
		}
    }

	if(some_value_to_optimize!=0)
	{
		newcodenode.command=PLUSOPER;
		newcodenode.oper1=oper1;
		sprintf(buffer,"%d",some_value_to_optimize);
		newcodenode.oper2=buffer;
		if(SyntacticAnalyzer::TempVar::isTempVar(oper1)) newcodenode.oper3=oper1;
		else newcodenode.oper3=this->tempvar.next();

		this->intermediate_code_deque.push_back(newcodenode);
		this->tempvar.prev_temp_varname=newcodenode.oper3;
	}


    return res;
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::item(LexicalAnalyzer& lexical_analyzer,int& value)
{
	SyntacticAnalyzer::SyntacticAnalyzerResult res=RIGHT,tmp_res=RIGHT;
	SyntacticAnalyzer::IntermediateCodeNode newcodenode;
	int tmp_value,some_value_to_optimize=1/*乘除法这里是1,加减法是0*/;
	bool stop_optimizing=false;//当出现第一个除法时，立即停止优化。
    std::string type,oper1;
	char buffer[VALUEBUFFERLENGTH];

	for(;;)
	{
		if(!(res=factor(lexical_analyzer,value))) return ERROR;
		else if(res==VALUE && value==1 && lexical_analyzer.peeksymtype()==TIMES)
		{
			lexical_analyzer.getsymtype();
			continue;
		}
		else/*if(res==VALUE || res==RIGHT)*/
		{//和前面一样，我们把factor的结果放在oper1里，小心翼翼的维护他。
		 //但这里还比前面简单，完全不需要对res的处理，好爽好爽~~
			oper1=this->tempvar.prev_temp_varname;
		}
		//如果能走到这一步，res只会是RIGHT或VALUE
		//和前面一样，oper1和this->tempvar.prev_temp_varname同时保持最新结果，一个给返回后使用，一个给接下来的迭代使用
		break;
	}

    while(type=lexical_analyzer.peeksymtype(),type==TIMES || type==SLASH)
    {
        if(lexical_analyzer.getsymtype(),!(tmp_res=factor(lexical_analyzer,tmp_value))) return ERROR;
		//如果能走到这一步，tmp_res只会是RIGHT或VALUE
		else if(tmp_res==VALUE && tmp_value==1)
		{
			this->tempvar.prev_temp_varname=oper1;//抛弃1这个结果
			continue;//对于乘除法，如果 乘/除 数为1,不再继续做下去，直接下一轮迭代。
		}

		else if(res==VALUE && tmp_res==VALUE)
		{
			if(type==TIMES) value*=tmp_value;
			else/* if(type==SLASH)*/ value/=tmp_value;
			sprintf(buffer,"%d",value);
			this->tempvar.prev_temp_varname=oper1=buffer;//继续保持 偶也~~
		}
		else
		{
			if(type==SLASH)
			{
				stop_optimizing=true;
				if(some_value_to_optimize!=1)
				{
					newcodenode.command=TIMESOPER;
					newcodenode.oper1=oper1;
					sprintf(buffer,"%d",some_value_to_optimize);
					newcodenode.oper2=buffer;
					if(SyntacticAnalyzer::TempVar::isTempVar(oper1)) newcodenode.oper3=oper1;
					else newcodenode.oper3=this->tempvar.next();

					this->intermediate_code_deque.push_back(newcodenode);
					some_value_to_optimize=1;
				}
			}
			//破处~~
			if(res==VALUE /*&& tmp_res==RIGHT*/)
			{
				res=RIGHT;//只要有一次tmp_res不是VALUE，那么对value的计算毫无意义，使res=RIGHT来避免此计算
				
				newcodenode.command=(type==TIMES?TIMESOPER:SLASHOPER);//TIMES OR SLASH
				newcodenode.oper1=oper1;
				newcodenode.oper2=this->tempvar.prev_temp_varname;
				if(SyntacticAnalyzer::TempVar::isTempVar(this->tempvar.prev_temp_varname)) newcodenode.oper3=this->tempvar.prev_temp_varname;
				else newcodenode.oper3=this->tempvar.next();

				this->intermediate_code_deque.push_back(newcodenode);
				this->tempvar.prev_temp_varname=oper1=newcodenode.oper3;//保持保持~~
			}
			else if(tmp_res==VALUE && stop_optimizing==false /* && res=RIGHT*/)
			{//既然tmp_res是个值，证明还有优化的潜能，我们把值放进some_value_to_optimize里，并在最后把它作为一条指令放进结果中
				some_value_to_optimize*=tmp_value;
			}
			else
			{
				newcodenode.command=(type==TIMES?TIMESOPER:SLASHOPER);//TIMES or SLASH
				newcodenode.oper1=oper1;
				newcodenode.oper2=this->tempvar.prev_temp_varname;

				if(SyntacticAnalyzer::TempVar::isTempVar(newcodenode.oper1)) newcodenode.oper3=newcodenode.oper1;
				else if(SyntacticAnalyzer::TempVar::isTempVar(newcodenode.oper2)) newcodenode.oper3=newcodenode.oper2;
				else newcodenode.oper3=this->tempvar.next();

				this->intermediate_code_deque.push_back(newcodenode);
				this->tempvar.prev_temp_varname=oper1=newcodenode.oper3;//保持保持~~
			}
		}
    }

	if(some_value_to_optimize!=1)
	{
		newcodenode.command=TIMESOPER;
		newcodenode.oper1=oper1;
		sprintf(buffer,"%d",some_value_to_optimize);
		newcodenode.oper2=buffer;
		if(SyntacticAnalyzer::TempVar::isTempVar(oper1)) newcodenode.oper3=oper1;
		else newcodenode.oper3=this->tempvar.next();

		this->intermediate_code_deque.push_back(newcodenode);
		this->tempvar.prev_temp_varname=newcodenode.oper3;
	}

    return res;
}

SyntacticAnalyzer::SyntacticAnalyzerResult SyntacticAnalyzer::factor(LexicalAnalyzer& lexical_analyzer,int& value)
{
	SyntacticAnalyzer::SyntacticAnalyzerResult res=RIGHT;
    std::string type=lexical_analyzer.getsymtype();
	std::string word=lexical_analyzer.getprevsymword();
	std::deque<SyntacticAnalyzer::IdentStackNode>::const_iterator iter;
	char buffer[VALUEBUFFERLENGTH];
	
    if(type!=IDENT && type!=NUMBER && type!=LPAREN)
	{
		this->errorinfo="这里应该是一个标识符或是一个数字或是一个表达式";
		return ERROR;
	}
	
	else if(type==NUMBER)
	{
		this->tempvar.prev_temp_varname=word;//this->tempvar.prev_temp_varname必须保持最新结果，无论接下来是不是返回值
		sscanf(word.c_str(),"%d",&value);
		return VALUE;
	}

	else if(type==IDENT)
	{
		if((iter=this->ident_stack_find(word.c_str()))==this->ident_stack.end())
		{
			this->errorinfo="找不到这个标识符哦，它确实已经定义了吗？";
			return ERROR;
		}
		else if(iter->flag==CONST)
		{
			value=iter->value;
			sprintf(buffer,"%d",value);
			this->tempvar.prev_temp_varname=buffer;
			return VALUE;
		}
		else this->tempvar.prev_temp_varname=word;
	}
	
	else /*if(type==LPAREN)*/
	{
		if(!(res=expression(lexical_analyzer,value))) return ERROR;
		if(lexical_analyzer.getsymtype()!=RPAREN)
		{
			this->errorinfo="这里应该有一个右括号，表达式必须用一对括号扩起来";
			return ERROR;
		}
	}
    return res;
}

