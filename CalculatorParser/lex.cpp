#include "stdafx.h"
#include "lex.h"

int getPrecedence(OperatorType ot)
{
	if (ot >= OperatorType::OP_NUM || ot < 0) return -1;
	return OpreatorPrecedence[ot];
}

namespace lex
{
	using namespace std;

	Lexer::Lexer(istream& s): 
		_end(false), linenumber(0), _beginpos(0), _endpos(0), ist(s)
	{
		readline();
	}

	Token Lexer::read()
	{
		if (!token_q.empty())
		{
			Token t = token_q.front();
			token_q.pop();
			return t;
		}
		else
		{
			if (readline())
				return read();
			else
				return Token(Token::TYPE::ENDFILE, Location(linenumber));
		}
	}

	bool Lexer::readline()
	{
		if (ist.eof()) return false;
		/*
		Token _token;
		*/

		string line;
		std::getline(ist, line);
		unsigned int i = 0;
		while (i < line.size())
		{
			Location loc;
			loc.line = linenumber;
			loc.begin = loc.end = i;
			switch (line[i])
			{
			case '\t':
			case '\n':
			case ' ': ++i; break;
			case '(':
				token_q.push(Token(Token::TYPE::OPEN_PAREN, move(loc)));
				++i; break;
			case ')':
				token_q.push(Token(Token::TYPE::CLOSE_PAREN, move(loc)));
				++i; break;
			case '+':
				token_q.push(Token(Token::TYPE::ADD, move(loc)));
				++i; break;
			case '-':
				token_q.push(Token(Token::TYPE::SUB, move(loc)));
				++i; break;
			case '*':
				if (i + 1 != line.size() && line[i + 1] != '*')
					token_q.push(Token(Token::TYPE::MUL, move(loc)));
				else
				{
					token_q.push(Token(Token::TYPE::POW, move(loc)));
					++i;
				}
				++i; break;
			case '/':
				token_q.push(Token(Token::TYPE::DIV, move(loc)));
				++i; break;
			case '%':
				token_q.push(Token(Token::TYPE::MOD, move(loc)));
				++i; break;
			default:
				if (line[i] >= '0' && line[i] <= '9')
					scanNumber(line, i);
				else
					// TODO: error
					// throw exception here
					break;
			}
		}
		token_q.push(Token(Token::Token::ENDLINE, Location(linenumber)));
		++linenumber;
		return true;
	}
	void Lexer::scanNumber(string& str, unsigned int& ic)
	{
		Token t;
		Location &loc = t.location;
		loc.begin = loc.end = ic;
		loc.line = linenumber;

		string num;
		if (isDigit(str[ic]))
		{
			t.set_type(Token::TYPE::NUMBER);
			num.push_back(str[ic++]);
			while (isDigit(str[ic]) || str[ic] == '.')
			{
				if (str[ic] == '.')
				{
					num.push_back(str[ic++]);
					if (isDigit(str[ic]))
					{
						while (isDigit(str[ic]))
							num.push_back(str[ic++]);
						loc.end = ic - 1;
						token_q.push(move(t));
						literal_q.push(move(num));
						return;
					}
					else
					{
						// error
					}
				}
				else // digit
				{
					num.push_back(str[ic++]);
				}
			}
			loc.end = ic - 1;
			token_q.push(move(t));
			literal_q.push(move(num));
		}
		else
		{
			// TODO: throw Exception
		}
	}

	void Lexer::finish()
	{
		token_q.push(Token(Token::TYPE::ENDFILE, Location(linenumber)));
		_end = true;
	}

	bool Lexer::isDigit(uc32 ch)
	{
		return ch >= '0' && ch <= '9' ? true : false;
	}

	string Lexer::consumeLiteral()
	{
		if (literal_q.empty())
			return nullptr;
		else
		{
			string t = literal_q.front();
			literal_q.pop();
			return t;
		}
	}
}