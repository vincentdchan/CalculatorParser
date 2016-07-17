#pragma once
#include <cinttypes>
#include <vector>
#include <memory>
#include "token.h"

#define NODE_LIST(V) \
	V(Identifier) \
	V(Assignment) \
	V(Number) \
	V(BinaryExpr) \
	V(UnaryExpr) \
	V(BlockExpr) \
	V(IfStmt) \
	V(WhileStmt) \
	V(DefStmt) \

namespace parser
{
	class Node;
#define E(Name) class Name##Node;
	NODE_LIST(E)
#undef E


	class Node
	{
	public:
		Node() {}
		virtual NumberNode* asNumber() { return nullptr; }
		virtual IdentifierNode* asIdentifier() { return nullptr; }
		virtual AssignmentNode* asAssignment() { return nullptr; }
		virtual UnaryExprNode* asUnaryExpression() { return nullptr; }
		virtual BlockExprNode* asBlockExpression() { return nullptr; }
		virtual BinaryExprNode* asBinaryExpression() { return nullptr; }
		virtual IfStmtNode* asIfStmt() { return nullptr; }
		virtual WhileStmtNode* asWhileStmt() { return nullptr; }
		virtual DefStmtNode* asDefStmt() { return nullptr; }
		virtual ~Node() {}
	};

	class IdentifierNode : public Node
	{
	public:
		IdentifierNode(const std::string& _str) : _name(_str)
		{}
		virtual IdentifierNode* asIdentifier() override { return this; }
		std::string Name() { return _name; }
		void setName(std::string _str) { _name = _str; }
	private:
		std::string _name;
	};

	class AssignmentNode : public Node
	{
	public:
		AssignmentNode() {}
		AssignmentNode(const std::string& _id, Node* _exp):
			_identifier(_id), _expression(_exp)
		{}
		virtual AssignmentNode* asAssignment() override { return this; }
		void setIdentifier(const std::string& _str) { _identifier = _str; }
		Node* setExpression(Node* _exp) 
		{ 
			Node* tmp = _expression;
			_expression = _exp;
			return tmp;
		}
		std::string Identifier() { return _identifier; }
		Node* Expression() { return _expression; }
		virtual ~AssignmentNode()
		{
			if (_expression)
				delete _expression;
		}
	private:
		std::string _identifier;
		Node* _expression;
	};

	class BlockExprNode : public Node
	{
	public:
		BlockExprNode() {}
		std::vector<Node* > children;
		virtual BlockExprNode* asBlockExpression() override { return this; }
		virtual ~BlockExprNode()
		{
			for (auto i = children.begin(); i != children.end(); ++i)
			{
				if (*i)
					delete *i;
			}
		}
	};

	class UnaryExprNode : public Node
	{
	public:
		UnaryExprNode(OperatorType o = OperatorType::ILLEGAL_OP, Node* _after = nullptr):
			_op(o), _child(_after)
		{}
		OperatorType op() const { return _op; }
		Node* child() const { return _child; }
		void setNode(Node* _ptr) { _child = _ptr; }
		void setOp(OperatorType ot) { _op = ot; }
		virtual UnaryExprNode* asUnaryExpression() override
		{
			return this;
		}
		virtual ~UnaryExprNode()
		{
			if (_child)
				delete _child;
		}
	private:
		OperatorType _op;
		Node* _child;
	};

	class BinaryExprNode : public Node
	{
	public:
		BinaryExprNode(OperatorType op = OperatorType::ILLEGAL_OP, Node *_l = nullptr, Node *_r = nullptr):
			_op(op), _left(_l), _right(_r)
		{}
		virtual BinaryExprNode* asBinaryExpression() override { return this; }
		inline OperatorType op() const { return _op; }
		Node* left() const { return _left; }
		Node* right() const { return _right; }
		virtual ~BinaryExprNode()
		{
			delete _left;
			delete _right;
		}
	private:
		OperatorType _op;
		Node *_left, *_right;
	};

	class NumberNode : public Node
	{
	public:
		NumberNode(double _decimal = 0.0):
			_value(_decimal)
		{}
		virtual NumberNode* asNumber() override
		{
			return this;
		}
		inline double value() const
		{
			return _value;
		}
	private:
		double _value;
	};

	class IfStmtNode : public Node
	{
	public:
		IfStmtNode(Node* _con = nullptr, Node* _cont = nullptr):
			_condition(_con), _content(_cont)
		{}
		Node* Condition() { return _condition; }
		Node* Content() { return _content; }
		void setCondition(Node* _con) { _condition = _con; }
		void setContent(Node* _con) { _content = _con; }
 		virtual IfStmtNode* asIfStmt() override { return this; }
		virtual ~IfStmtNode()
		{
			delete _condition;
			delete _content;
		}
	private:
		Node* _condition;
		Node* _content;
	};

	class WhileStmtNode : public Node
	{
	public:
		WhileStmtNode(Node* _con = nullptr, Node* _cont = nullptr) :
			_condition(_con), _content(_cont)
		{}
		Node* Condition() { return _condition; }
		Node* Content() { return _content; }
		void setCondition(Node* _con) { _condition = _con; }
		void setContent(Node* _con) { _content = _con; }
 		virtual WhileStmtNode* asWhileStmt() override { return this; }
		virtual ~WhileStmtNode()
		{
			delete _condition;
			delete _content;
		}
	private:
		Node* _condition;
		Node* _content;
	};

	class DefStmtNode : public Node
	{
	public:
		DefStmtNode() {}
	private:
		std::string _name;
		std::unique_ptr<std::vector<std::string> > _parameters;
		Node* _content;
	};

}
