
#include "stdafx.h"
#include "parser.h"
#include "ast.h"

namespace parser
{

	Parser::Parser(Lexer& _lex) : 
		lexer(_lex), ok(true), ast_root(nullptr)
	{
		nextToken();
	}

	Parser::Parser(Parser&& _parser) :
		_MessageContainer(_parser),
		lexer(_parser.lexer), ok(_parser.ok), 
		ast_root(move(_parser.ast_root))
	{ }

	void Parser::parse()
	{
		this->ast_root = this->parseBlock();
	}

	/*
	void Parser::next()
	{
		lookahead = lexer.read();
	}
	*/

	/*
	void Parser::ReportMessage(const string& _Str, Parser::MESSAGE_TYPE _mt)
	{
		messages.push_back(make_pair(_Str, _mt));
	}

	void Parser::ReportError(const string& _Str)
	{
		ok = false;
		messages.push_back(make_pair(_Str, Parser::MESSAGE_TYPE::ERROR));
	}
	*/

	Token Parser::nextToken()
	{
		Token tmp = move(lookahead);
		lookahead = lexer.read();
		return tmp;
	}

	/*
	string Parser::consumeLiteral()
	{
		return lexer.consumeLiteral();
	}
	*/

	/**********************************/
	/*   Paring                       */
	/**********************************/

	unique_ptr<Node> Parser::parseBlock()
	{
		unique_ptr<BlockExprNode> _block(new BlockExprNode());
		while (!match(Token::TYPE::ENDFILE))
		{
			auto _line = parseLine();
			if (_line != nullptr)
			{
				_block->children.push_back(move(_line));
			}
		}
		return move(_block);
	}

	unique_ptr<Node> Parser::parseLine()
	{
		// Line:: BinaryExpr ENDLINE
		unique_ptr<Node> node;
		if (match(Token::TYPE::LET))
			node = parseLetExpr();
		else
			node = parseBinaryExpr();
		if (!match(Token::TYPE::ENDLINE))
			ReportError("Exprected end of line");
		nextToken();
		return move(node);
	}

	unique_ptr<Node> Parser::parseLetExpr()
	{
		expect(Token::TYPE::LET);
		nextToken();
		return parseAssignment();
	}

	unique_ptr<Node> Parser::parseAssignment()
	{
		expect(Token::TYPE::IDENTIFIER);

		string _id = *lookahead.pLiteral;
		nextToken();
		expect(Token::TYPE::ASSIGN);
		nextToken();
		unique_ptr<Node> _exp = parseBinaryExpr();
		if (_exp)
			return unique_ptr<Node>(new AssignmentNode(_id, move(_exp)));
		else
			return nullptr;
	}

	unique_ptr<Node> Parser::parseUnaryExpr()
	{
		// UnaryExpr: ( + | - )? Int
		if (match(Token::TYPE::ADD))
		{
			nextToken();
			if (match(Token::TYPE::NUMBER))
			{
				double _data = std::stod(*lookahead.pLiteral);
				auto numberNode = unique_ptr<Node>(new NumberNode(_data));
				nextToken();
				return move(numberNode);
			}
			else if (match(Token::TYPE::IDENTIFIER))
			{
				string s = *lookahead.pLiteral;
				nextToken();
				return unique_ptr<Node>(new IdentifierNode(s));
			}
			else
			{
				ReportError("Expected number.");
				return nullptr;
			}
		}
		else if (match(Token::TYPE::SUB))
		{
			nextToken();
			if (match(Token::Token::NUMBER))
			{
				double _data = std::stod(*lookahead.pLiteral);
				nextToken();
				return unique_ptr<Node>(new NumberNode(_data * -1));
			}
			else if (match(Token::TYPE::IDENTIFIER))
			{
				Token _t = move(nextToken());
				auto _node = unique_ptr<Node>(new IdentifierNode(*_t.pLiteral));
				return unique_ptr<Node>(new UnaryExprNode(OperatorType::SUB, move(_node)));
			}
			else
			{
				ReportError("Expected number");
				return nullptr;
			}
		}
		else if (match(Token::TYPE::NUMBER))
		{
			Token _t = move(nextToken());
			return unique_ptr<Node>(new NumberNode(stod(*_t.pLiteral)));
		}
		else if (match(Token::TYPE::IDENTIFIER))
		{
			Token _t = move(nextToken());
			return unique_ptr<Node>(new IdentifierNode(*_t.pLiteral));
		}
		else
		{
			ReportError("Unexpected token, expected +, - or Integer.");
			return nullptr;
		}
	}

	unique_ptr<Node> Parser::parseBinaryExpr()
	{
		// BinaryExpr: Unary { OP Unary }
		stack<OperatorType> opStack;
		stack<unique_ptr<Node> > nodeStack;
		auto _begin = parseUnaryExpr();
		if (_begin == nullptr)
			return nullptr;
		nodeStack.push(move(_begin));

		while (Token::isOperator(lookahead))
		{
			int prec = getPrecedence(Token::toOperator(lookahead));
			if (opStack.empty() || prec > getPrecedence(opStack.top()))
			{
				opStack.push(Token::toOperator(lookahead));
				nextToken();
				auto l = parseUnaryExpr();
				nodeStack.push(move(l));
			}
			else
			{
				while (!opStack.empty() && prec <= getPrecedence(opStack.top()))
				{
					unique_ptr<Node> l1, l2;
					l1 = move(nodeStack.top()); nodeStack.pop();
					l2 = move(nodeStack.top()); nodeStack.pop();
					nodeStack.push(unique_ptr<Node>(new BinaryExprNode(opStack.top(), move(l2), move(l1))));
					opStack.pop();
				}
			}
		}
		while (!opStack.empty())
		{
			unique_ptr<Node> l1, l2;
			l1 = move(nodeStack.top()); nodeStack.pop();
			l2 = move(nodeStack.top()); nodeStack.pop();
			nodeStack.push(unique_ptr<Node>(new BinaryExprNode(opStack.top(), move(l2), move(l1))));
			opStack.pop();
		}
		return move(nodeStack.top());
	}

	unique_ptr<Node> Parser::parseWhileStmt()
	{
		expect(Token::TYPE::WHILE);
		nextToken();
		auto _condition = parseBinaryExpr();
		auto _block = parseBlock();
		return unique_ptr<Node>(new WhileStmtNode(move(_condition), move(_block)));
	}

}
