#ifndef __LEXICALANALYZER_CLASS_H
#define __LEXICALANALYZER_CLASS_H
#include<iostream>
#include<fstream>
#include<string>
#include<deque>

class LexicalAnalyzer
{
public:
    class InputReader
	{
	protected:
		int curline,curchar,curindex,prevline,prevchar;
	public:
		InputReader():curline(1),curchar(0),curindex(0)
		{
		}
		virtual char peekchar() const=0;
		virtual char getchar()=0;
		virtual int get_curline() const
		{
			return this->curline;
		}
		virtual int get_curchar() const
		{
			return this->curchar;
		}
		virtual int get_prevline() const
		{
			return this->prevline;
		}
		virtual int get_prevchar() const
		{
			return this->prevchar;
		}
		virtual bool eof() const=0;
	};

    class ConsoleInputReader:public InputReader
	{
	public:
		char peekchar() const
		{
			if(eof()) return -1;
			return std::cin.peek();
		}
		char getchar()
		{
			if(eof()) return -1;
			char c=std::cin.get();
			this->prevline=this->curline;
			this->prevchar=this->curchar;
			++this->curindex;
			if(c=='\n')
			{
				++this->curline;
				this->curchar=0;
			}
			else ++this->curchar;        
			return c;
		}
		bool eof() const
		{
			return std::cin.eof();    
		}
	};

    class FileReader:public InputReader
	{
	private:
		std::string buffer;
	public:
		bool open(const std::string& path)
		{
			std::ifstream file(path.c_str(),std::ios::in);
			if(file.fail()) return false;
			std::getline(file,this->buffer,(char)-1);
			file.close();
			curline=1;
			curchar=curindex=0;
			return true;
		}
		char peekchar() const
		{
			if(eof()) return -1;
			return this->buffer[this->curindex];
		}
		char getchar()
		{
			char c=peekchar();
			this->prevline=this->curline;
			this->prevchar=this->curchar;
			++this->curindex;
			if(c=='\n')
			{
				++this->curline;
				this->curchar=0;
			}
			else ++this->curchar;
			return c;
		}
		bool eof() const
		{
			return this->curindex >= this->buffer.size();
		}
	};
private:
    ConsoleInputReader consoleinputreader;
    FileReader filereader;
    InputReader *inputreader;
	std::string cursymword,prevsymword;
	int cursymtype,prevsymtype;
	int cursymline,cursymchar,prevsymline,prevsymchar;
public:
    bool open(const std::string &path="");
    void next();
	int peeksymtype() const;
    int getsymtype();
    std::string peeksymword() const;
    std::string getsymword();
    int getprevsymtype() const;
    std::string getprevsymword() const;
    bool eof() const;
    int get_curline() const;
    int get_curchar() const;
    int get_prevline() const;
    int get_prevchar() const;
private:
    int get_keyword(std::string) const;
    int get_operator(const std::string&) const;
	bool getsym(std::string&,int&);
};

#endif
