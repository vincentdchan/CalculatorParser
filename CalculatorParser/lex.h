#pragma once
#include "token.h"
#include <queue>
#include <istream>
#include <sstream>
#include <memory>

namespace lex
{
	using namespace std;

	class Lexer
	{
	public:
		typedef shared_ptr<string> pString;
		Lexer(istream& s);
		Token read();
		inline void finish();
		bool isFinish()
		{
			return _end;
		}
		static bool isDigit(uc32 t);
		string consumeLiteral();
	private:
		Lexer();
		bool readline();
		void scanNumber(string&, unsigned int&);
		queue<Token> token_q;
		queue<string> literal_q;
		bool _end;
		unsigned int linenumber, _beginpos, _endpos;
		// stream
		istream &ist;
	};

}
