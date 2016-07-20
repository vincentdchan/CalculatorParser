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
#include "svm_codes.h"
#include "svm.h"

int main()
{
	using namespace lex;
	using namespace parser;
	using namespace runtime::StackVM;

	static Instruction ins[] = 
	{ 
		{VM_CODE::PushInt, 1}, 
		{VM_CODE::PushInt, 1}, 
		{VM_CODE::Add, 0}, 
		{VM_CODE::PushInt, 2},
		{VM_CODE::Mul, 2},
		{VM_CODE::Out, 0},
		{VM_CODE::STOP, 0}
	};

	StackVM* vm = new StackVM(ins);
	vm->execute();

	fstream fs;
	fs.open("source.txt", fstream::in);
	Lexer lexer(fs);

	Parser parser(lexer);
	parser.parse();
	fs.close();

	string ch;
	cin >> ch;
    return 0;
}

