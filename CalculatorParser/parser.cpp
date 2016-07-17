
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
		lexer(_parser.lexer), ok(_parser.ok), _messages(_parser._messages), ast_root(nullptr)
	{
		ast_root = _parser.ast_root;
		_parser.ast_root = nullptr;
	}

	void Parser::parse()
	{
		Node* _root = parseBlock();
		if (ok)
			this->ast_root = _root;
		else
			if (_root != nullptr)
				delete _root;
	}

	/*
	void Parser::next()
	{
		lookahead = lexer.read();
	}
	*/

	void Parser::ReportMessage(const string& _Str, Parser::MESSAGE_TYPE _mt)
	{
		_messages.push_back(make_pair(_Str, _mt));
	}

	void Parser::ReportError(const string& _Str)
	{
		ok = false;
		_messages.push_back(make_pair(_Str, Parser::MESSAGE_TYPE::ERROR));
	}

	Token Parser::nextToken()
	{
		Token tmp = lookahead;
		lookahead = move(lexer.read());
		return tmp;
	}

	string Parser::consumeLiteral()
	{
		return lexer.consumeLiteral();
	}

	Node* Parser::parseBlock()
	{
		auto _block = new BlockExprNode();
		while (!match(Token::TYPE::ENDFILE))
		{
			auto _line = parseLine();
			if (_line != nullptr)
			{
				_block->children.push_back(_line);
			}
		}
		return _block;
	}

	Node* Parser::parseLine()
	{
		// Line:: BinaryExpr ENDLINE
		Node *node;
		if (match(Token::TYPE::LET))
			node = parseLetExpr();
		else
			node = parseBinaryExpr();
		if (!match(Token::TYPE::ENDLINE))
			ReportError("Exprected end of line");
		nextToken();
		return node;
	}

	Node* Parser::parseLetExpr()
	{
		expect(Token::TYPE::LET);
		nextToken();
		return parseAssignment();
	}

	Node* Parser::parseAssignment()
	{
		expect(Token::TYPE::IDENTIFIER);

		string _id = consumeLiteral();
		nextToken();
		expect(Token::TYPE::ASSIGN);
		nextToken();
		Node* _exp = parseBinaryExpr();
		if (_exp)
			return new AssignmentNode(_id, _exp);
		else
			return nullptr;
	}

	Node* Parser::parseUnaryExpr()
	{
		// UnaryExpr: ( + | - )? Int
		if (match(Token::TYPE::ADD))
		{
			nextToken();
			if (match(Token::TYPE::NUMBER))
			{
				nextToken();
				string s = consumeLiteral();
				double _data = std::stod(s);
				auto numberNode = new NumberNode(_data);
				return numberNode;
			}
			else if (match(Token::TYPE::IDENTIFIER))
			{
				nextToken();
				string s = consumeLiteral();
				return new IdentifierNode(s);
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
				nextToken();
				auto s = consumeLiteral();
				double _data = std::stod(s);
				NumberNode* numberNode = new NumberNode(_data * -1);
				return numberNode;
			}
			else if (match(Token::TYPE::IDENTIFIER))
			{
				nextToken();
				string s = consumeLiteral();
				auto _node = new IdentifierNode(s);
				return new UnaryExprNode(OperatorType::SUB, _node);
			}
			else
			{
				ReportError("Expected number");
				return nullptr;
			}
		}
		else if (match(Token::TYPE::NUMBER))
		{
			nextToken();
			return new NumberNode(stod(consumeLiteral()));
		}
		else if (match(Token::TYPE::IDENTIFIER))
		{
			nextToken();
			string s = consumeLiteral();
			return new IdentifierNode(s);
		}
		else
		{
			ReportError("Unexpected token, expected +, - or Integer.");
			return nullptr;
		}
	}

	Node* Parser::parseBinaryExpr()
	{
		// BinaryExpr: Unary { OP Unary }
		stack<OperatorType> opStack;
		stack<Node* > nodeStack;
		Node* _begin = parseUnaryExpr();
		if (_begin == nullptr)
			return nullptr;
		nodeStack.push(_begin);

		while (Token::isOperator(lookahead))
		{
			int prec = getPrecedence(Token::toOperator(lookahead));
			if (opStack.empty() || prec > getPrecedence(opStack.top()))
			{
				opStack.push(Token::toOperator(lookahead));
				nextToken();
				Node* l = parseUnaryExpr();
				nodeStack.push(l);
			}
			else
			{
				while (!opStack.empty() && prec <= getPrecedence(opStack.top()))
				{
					Node *l1, *l2;
					l1 = nodeStack.top(); nodeStack.pop();
					l2 = nodeStack.top(); nodeStack.pop();
					nodeStack.push(new BinaryExprNode(opStack.top(), l2, l1));
					opStack.pop();
				}
			}
		}
		while (!opStack.empty())
		{
			Node *l1, *l2;
			l1 = nodeStack.top(); nodeStack.pop();
			l2 = nodeStack.top(); nodeStack.pop();
			nodeStack.push(new BinaryExprNode(opStack.top(), l2, l1));
			opStack.pop();
		}
		return nodeStack.top();
	}

	Node* Parser::parseWhileStmt()
	{
		expect(Token::TYPE::WHILE);
		nextToken();
		Node* _condition = parseBinaryExpr();
		Node* _block = parseBlock();
		return new WhileStmtNode(_condition, _block);
	}

	Node* Parser::parseDefStmt()
	{
		expect(Token::TYPE::DEF); nextToken();
		expect(Token::TYPE::OPEN_PAREN); nextToken();
		while (match(Token::TYPE::IDENTIFIER))
		{

		}
		return nullptr;
	}

	Node* const Parser::getAstRoot() 
	{ 
		return ast_root; 
	}

	Node* Parser::popAstRoot()
	{
		Node* _Root = this->ast_root;
		this->ast_root = nullptr;
		return _Root;
	}

	Parser::~Parser()
	{
		if (!ast_root)
			delete ast_root;
	}
}
