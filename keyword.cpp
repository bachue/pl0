#include<string>
#include<ctype.h>
#include<map>
#include"LexicalAnalyzer.class.h"
#include"keywordlist.h"
#define KEYWORD_LENGTH 13

int LexicalAnalyzer::get_keyword(std::string str) const
{
	static const std::pair<std::string,int> KEYWORDS_ARRAY[]={	std::make_pair(IFSYM,IFTYPE),
																std::make_pair(THENSYM,THENTYPE),
																std::make_pair(WHILESYM,WHILETYPE),
																std::make_pair(DOSYM,DOTYPE),
																std::make_pair(READSYM,READTYPE),
																std::make_pair(WRITESYM,WRITETYPE),
																std::make_pair(CALLSYM,CALLTYPE),
																std::make_pair(BEGINSYM,BEGINTYPE),
																std::make_pair(ENDSYM,ENDTYPE),
																std::make_pair(CONSTSYM,CONSTTYPE),
																std::make_pair(VARSYM,VARTYPE),
																std::make_pair(PROCSYM,PROCTYPE),
																std::make_pair(ODDSYM,ODDTYPE)};

	static const std::map<std::string,int> KEYWORDS(KEYWORDS_ARRAY,KEYWORDS_ARRAY+KEYWORD_LENGTH);

    for(int i=0;i<str.size();++i) str[i]=tolower(str[i]);
    std::map<std::string,int>::const_iterator iter=KEYWORDS.find(str);
    if(iter!=KEYWORDS.end()) return iter->second;
    else return 0;
}
