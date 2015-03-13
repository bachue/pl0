#ifndef __FUNC_LIB_H
#define __FUNC_LIB_H

#define CHAR_CHARACTER 1
#define CHAR_NUMBER 2
#define CHAR_OTHER 3
#define CHAR_NULL 4

inline
int judge_char_type(char c)
{
    if(c==' '||c=='\t'||c=='\n') return CHAR_NULL;
    else if( (c>='A'&&c<='Z') || (c>='a'&&c<='z') ) return CHAR_CHARACTER;
    else if( c>='0'&&c<='9' ) return CHAR_NUMBER;
    else return CHAR_OTHER;
}

#endif

