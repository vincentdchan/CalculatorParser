#pragma once
#include <queue>
#include <istream>
#include <sstream>
#include <memory>
#include <list>
#include "token.h"
#include "util.h"

namespace lex
{
	using namespace std;

	class Lexer final: utils::_MessageContainer
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
		static bool isAlphabet(uc32 t);
		// string consumeLiteral();
	private:
		Lexer();
		bool readline();
		void scanNumber(string&, unsigned int&);
		void scanLet(string&, unsigned int&);
		void scanIf(string&, unsigned int&);
		void scanElse(string&, unsigned int&);
		void scanWhile(string&, unsigned int&);
		void scanDef(string&, unsigned int&);
		void scanIdentifier(string&, unsigned int&);
		queue<Token> token_q;
		// queue<string> literal_q;
		bool _end;
		unsigned int linenumber, _beginpos, _endpos;
		// stream
		istream &ist;
	};

}
