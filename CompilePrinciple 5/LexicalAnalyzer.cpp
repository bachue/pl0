#include<string>
#include<fstream>
#include<deque>
#include"LexicalAnalyzer.class.h"

bool LexicalAnalyzer::open(const std::string &path)
{
    std::string word,type;
    this->symwords.clear();
    this->symtypes.clear();
    this->symnumchars.clear();
    this->symnumlines.clear();

    if(path.size()>0)//文件操作
    {
        this->inputreader=&this->filereader;
        if(!this->filereader.open(path)) return false;
    }
    else//控制台输入操作
    {
        this->inputreader=&this->consoleinputreader;
    }
    
    if(!this->getsym(word,type)) return false;
    this->symwords.push_back(word);
    this->symtypes.push_back(type);
    this->curdequeindex=0;
    this->resetmode=0;
    return true;
}

void LexicalAnalyzer::next()
{
    std::string type,word;
    if(!this->resetmode)
    {
        this->getsym(word,type);
        this->symwords.push_back(word);
        this->symtypes.push_back(type);
    }
    ++this->curdequeindex;
}

std::string LexicalAnalyzer::peeksymtype() const
{
    return this->symtypes[this->curdequeindex];
}

std::string LexicalAnalyzer::getsymtype()
{
    if(!this->eof())
	{
		this->next();    
		return this->getprevsymtype();
	}
	else return this->peeksymtype();
}

std::string LexicalAnalyzer::peeksymword() const
{
    return this->symwords[this->curdequeindex];
}

std::string LexicalAnalyzer::getsymword()
{
	if(!this->eof())
	{
		this->next();    
		return this->getprevsymword();
	}
	else return this->peeksymword();
}

std::string LexicalAnalyzer::getprevsymtype() const
{
    return this->symtypes[this->curdequeindex-1];
}

std::string LexicalAnalyzer::getprevsymword() const
{
    return this->symwords[this->curdequeindex-1];
}

bool LexicalAnalyzer::eof() const
{
    if(!this->resetmode) return this->inputreader->eof();
    else return this->curdequeindex==this->symtypes.size();
}

int LexicalAnalyzer::get_curline() const
{
    return this->symnumlines[this->curdequeindex];
}

int LexicalAnalyzer::get_curchar() const
{
    return this->symnumchars[this->curdequeindex];
}

int LexicalAnalyzer::get_prevline() const
{
    return this->symnumlines[this->curdequeindex-1];
}

int LexicalAnalyzer::get_prevchar() const
{
    return this->symnumchars[this->curdequeindex-1];
}

void LexicalAnalyzer::reset()
{
    this->resetmode=1;
    this->curdequeindex=0;
}
