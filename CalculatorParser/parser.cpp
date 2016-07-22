
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

	Node* Parser::parseBlock()
	{
		auto _block = make_node<BlockExprNode>();
		while (!match(Token::TYPE::ENDFILE))
		{
			auto _line = parseLine();
			if (_line != nullptr)
			{
				_block->children.push_back(move(_line));
			}
		}
		return _block;
	}

	Node* Parser::parseLine()
	{
		// Line:: BinaryExpr ENDLINE
		Node* node;
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

		string _id = *lookahead.pLiteral;
		nextToken();
		expect(Token::TYPE::ASSIGN);
		nextToken();
		Node* _exp = parseBinaryExpr();
		return make_node<AssignmentNode>(_id, _exp);
	}

	Node* Parser::parseUnaryExpr()
	{
		// UnaryExpr: ( + | - )? Int
		if (match(Token::TYPE::ADD))
		{
			nextToken();
			if (match(Token::TYPE::NUMBER))
			{
				double _data = std::stod(*lookahead.pLiteral);
				auto numberNode = make_node<NumberNode>(_data);
				nextToken();
				return numberNode;
			}
			else if (match(Token::TYPE::IDENTIFIER))
			{
				string s = *lookahead.pLiteral;
				nextToken();
				return make_node<IdentifierNode>(s);
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
				return make_node<NumberNode>(_data * -1);
			}
			else if (match(Token::TYPE::IDENTIFIER))
			{
				Token _t = move(nextToken());
				auto _node = make_node<IdentifierNode>(*_t.pLiteral);
				return make_node<UnaryExprNode>(OperatorType::SUB, _node);
			}
			else
			{
				ReportError("Expected number or identifier");
				return nullptr;
			}
		}
		else if (match(Token::TYPE::NUMBER))
		{
			Token _t = move(nextToken());
			return make_node<NumberNode>(stod(*_t.pLiteral));
		}
		else if (match(Token::TYPE::IDENTIFIER))
		{
			Token _t = move(nextToken());
			return make_node<IdentifierNode>(*_t.pLiteral);
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
		stack<Node*> nodeStack;
		auto _begin = parseUnaryExpr();
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
				auto l = parseUnaryExpr();
				nodeStack.push(l);
			}
			else
			{
				while (!opStack.empty() && prec <= getPrecedence(opStack.top()))
				{
					Node *l1, *l2;
					l1 = nodeStack.top(); nodeStack.pop();
					l2 = nodeStack.top(); nodeStack.pop();
					nodeStack.push(make_node<BinaryExprNode>(opStack.top(), l2, l1));
					opStack.pop();
				}
			}
		}
		while (!opStack.empty())
		{
			Node *l1, *l2;
			l1 = nodeStack.top(); nodeStack.pop();
			l2 = nodeStack.top(); nodeStack.pop();
			nodeStack.push(make_node<BinaryExprNode>(opStack.top(), l2, l1));
			opStack.pop();
		}
		return nodeStack.top();
	}

	Node* Parser::parseWhileStmt()
	{
		expect(Token::TYPE::WHILE);
		nextToken();
		auto _condition = parseBinaryExpr();
		auto _block = parseBlock();
		return make_node<WhileStmtNode>(_condition, _block);
	}

	Parser::~Parser()
	{
		for (auto i = _node_list.begin(); i != _node_list.end(); ++i)
			delete *i;
	}

}
