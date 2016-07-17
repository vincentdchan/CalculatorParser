#pragma once

#include <cinttypes>
#include "lex.h"
#include "ast.h"
#include <memory>
#include <stack>
#include <list>
#include <iostream>

namespace parser
{
	using namespace lex;

	class Parser final
	{
	public:
		enum struct MESSAGE_TYPE
		{
			NORMAL,
			WARNING,
			ERROR
		};
		Parser(Lexer& _lex);
		Parser(Parser&& _parser);
		// void next();
		Token nextToken();
		Token lookahead;
		void parse();
		string consumeLiteral();
		void ReportMessage(const string&, MESSAGE_TYPE _mt);
		void ReportError(const string&);
		bool isOk() const { return ok; }
		Node* const getAstRoot();
		Node* popAstRoot();
		~Parser();
	private:
		bool ok;
		Node* ast_root;
		Node* parseBlock();
		Node* parseLine();
		Node* parseLetExpr();
		Node* parseAssignment();
		Node* parseUnaryExpr();
		Node* parseBinaryExpr();
		// Node* parseIfStmt();
		Node* parseWhileStmt();
		Node* parseDefStmt();
		Parser() = delete;
		Parser(const Parser&) = delete;
		Parser& operator=(const Parser&) = delete;
		Lexer& lexer;
		list<pair<string, MESSAGE_TYPE> > _messages;
		inline bool match(Token::TYPE t) { return lookahead.type() == t; } // just judge
		inline bool expect(Token::TYPE t)
		{
			if (lookahead.type() == t)
				return true;
			else
			{
				auto num = static_cast<int>(lookahead.type());
				// warning: here may cause crash if num is not in range
				ReportError(string("Unexpected token: ") + TokenName[num]);
				ok = false;
				return false;
			}
		}
	};

}
