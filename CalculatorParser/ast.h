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
		IdentifierNode(const std::string& _str) : name(_str)
		{}
		virtual IdentifierNode* asIdentifier() override { return this; }

		std::string name;
	};

	class AssignmentNode : public Node
	{
	public:
		AssignmentNode() {}
		AssignmentNode(const std::string& _id, std::unique_ptr<Node> _exp):
			identifier(_id), expression(std::move(_exp))
		{}

		std::string identifier;
		std::unique_ptr<Node> expression;

		virtual AssignmentNode* asAssignment() override { return this; }
	};

	class BlockExprNode : public Node
	{
	public:
		BlockExprNode() {}
		std::vector<std::unique_ptr<Node> > children;
		virtual BlockExprNode* asBlockExpression() override { return this; }
	};

	class UnaryExprNode : public Node
	{
	public:
		UnaryExprNode(OperatorType _op = OperatorType::ILLEGAL_OP, 
			std::unique_ptr<Node> _after = nullptr):
			op(_op), child(std::move(_after))
		{}
		virtual UnaryExprNode* asUnaryExpression() override { return this; }
		OperatorType op;
		std::unique_ptr<Node> child;
	};

	class BinaryExprNode : public Node
	{
	public:
		BinaryExprNode(OperatorType _op = OperatorType::ILLEGAL_OP, 
			std::unique_ptr<Node> _l = nullptr, 
			std::unique_ptr<Node> _r = nullptr):
			op(_op), left(std::move(_l)), right(std::move(_r))
		{}
		virtual BinaryExprNode* asBinaryExpression() override { return this; }

		OperatorType op;
		std::unique_ptr<Node> left;
		std::unique_ptr<Node> right;
	};

	class NumberNode : public Node
	{
	public:
		NumberNode(double _decimal = 0.0):
			value(_decimal)
		{}
		virtual NumberNode* asNumber() override { return this; }

		double value;
	};

	class IfStmtNode : public Node
	{
	public:
		IfStmtNode(std::unique_ptr<Node> _exp = nullptr, 
			std::unique_ptr<Node> _true = nullptr, 
			std::unique_ptr<Node> _false = nullptr):
			expression(std::move(_exp)), 
			true_branch(std::move(_true)), 
			false_branch(std::move(_false))
		{}
 		virtual IfStmtNode* asIfStmt() override { return this; }

		std::unique_ptr<Node> expression;
		std::unique_ptr<Node> true_branch;
		std::unique_ptr<Node> false_branch;
	};

	class WhileStmtNode : public Node
	{
	public:
		WhileStmtNode(std::unique_ptr<Node> _con = nullptr, 
			std::unique_ptr<Node> _cont = nullptr) :
			expression(std::move(_con)), child(std::move(_cont))
		{}
 		virtual WhileStmtNode* asWhileStmt() override { return this; }

		std::unique_ptr<Node> expression;
		std::unique_ptr<Node> child;
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
