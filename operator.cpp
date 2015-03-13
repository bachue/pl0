#include<string>
#include<map>
#include"LexicalAnalyzer.class.h"
#include"operatorlist.h"
#define OPERATOR_LENGTH 16

int LexicalAnalyzer::get_operator(const std::string& str) const
{
	static const std::pair<std::string,int> OPERATORS_ARRAY[]={	std::make_pair(BECOMESOPER,BECOMESTYPE),
																std::make_pair(GEQOPER,GEQTYPE),
																std::make_pair(GTROPER,GTRTYPE),
																std::make_pair(LEQOPER,LEQTYPE),
																std::make_pair(LSSOPER,LSSTYPE),
																std::make_pair(PLUSOPER,PLUSTYPE),
																std::make_pair(MINUSOPER,MINUSTYPE),
																std::make_pair(TIMESOPER,TIMESTYPE),
																std::make_pair(SLASHOPER,SLASHTYPE),
																std::make_pair(EQLOPER,EQLTYPE),
																std::make_pair(NEQOPER,NEQTYPE),
																std::make_pair(COMMAOPER,COMMATYPE),
																std::make_pair(PERIODOPER,PERIODTYPE),
																std::make_pair(SEMICOLONOPER,SEMICOLONTYPE),
																std::make_pair(LPARENOPER,LPARENTYPE),
																std::make_pair(RPARENOPER,RPARENTYPE)};

	static const std::map<std::string,int> OPERATORS(OPERATORS_ARRAY,OPERATORS_ARRAY+OPERATOR_LENGTH);

    std::map<std::string,int>::const_iterator iter=OPERATORS.find(str);
    if(iter!=OPERATORS.end()) return iter->second;
	else return 0;
}
