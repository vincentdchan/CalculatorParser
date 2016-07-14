// CalculatorParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <utility>
#include "lex.h"
#include "scanner.h"
#include "ast.h"
#include "parser.h"
#include "SimpleRuntime.h"

int main()
{
	using namespace lex;
	using namespace parser;
	fstream fs;
	fs.open("source.txt", fstream::in);
	Lexer lexer(fs);

	Parser parser(lexer);
	parser.parse();
	runtime::Environment env;
	runtime::eval(parser.getAstRoot(), env);
	/*
	Token t;
	while (t = lexer.read(), t != Token::TYPE::ENDFILE)
	{
		if (t.type() == Token::NUMBER)
		{
			string s = move(lexer.consumeLiteral());
			cout << t.type() << ": " << s 
				<< " line: " << t.location.line 
				<< " begin: " << t.location.begin
				<< " end: " << t.location.end << endl;
		}
		else
		{
			cout << t.type() << endl;
		}
	}
	*/
	fs.close();
	string ch;
	cin >> ch;
    return 0;
}

