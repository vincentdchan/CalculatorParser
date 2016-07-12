#pragma once
#include <cinttypes>
#include <vector>
#include <memory>
#include "token.h"

#define NODE_LIST(V) \
	V(Number) \
	V(BinaryExpr) \
	V(UnaryExpr) \
	V(BlockExpr)

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
		virtual UnaryExprNode* asUnaryExpression() { return nullptr; }
		virtual BlockExprNode* asBlockExpression() { return nullptr; }
		virtual BinaryExprNode* asBinaryExpression() { return nullptr; }
		virtual ~Node() {}
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
		UnaryExprNode():
			_op(OperatorType::ILLEGAL_OP), _child(nullptr)
		{}
		UnaryExprNode(OperatorType o, Node* _after):
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
}
