#include "stdafx.h"
#include "codegen.h"
#include "svm_codes.h"

namespace codegen
{

	using namespace parser;

	CodeGen::CodeGen(Parser& _p) : _parser(_p)
	{ }

	void CodeGen::generate()
	{
		visit(_parser.ast_root.get());
		pack.instructions.push_back(Instruction(VM_CODE::STOP, 0));
		pack.variablesSize = var_id.size();
	}

	void CodeGen::visit(Node* _node)
	{
		if (_node->asIdentifier())
			visit(_node->asIdentifier());
		else if (_node->asAssignment())
			visit(_node->asAssignment());
		else if (_node->asNumber())
			visit(_node->asNumber());
		else if (_node->asBinaryExpression())
			visit(_node->asBinaryExpression());
		else if (_node->asUnaryExpression())
			visit(_node->asUnaryExpression());
		else if (_node->asBlockExpression())
			visit(_node->asBlockExpression());
		else if (_node->asIfStmt())
			visit(_node->asIfStmt());
		else if (_node->asWhileStmt())
			visit(_node->asWhileStmt());
		else if (_node->asDefStmt())
			visit(_node->asDefStmt());
	}

	void CodeGen::visit(BlockExprNode* _node)
	{
		for (auto i = _node->children.begin(); i != _node->children.end(); ++i)
		{
			visit(i->get());
			if (i->get()->asBinaryExpression())
				pack.instructions.push_back(Instruction(VM_CODE::Out, 0));
		}
	}

	void CodeGen::visit(UnaryExprNode* _node)
	{

	}

	void CodeGen::visit(BinaryExprNode* _node)
	{
		visit(_node->left.get());
		visit(_node->right.get());
		switch (_node->op)
		{
		case OperatorType::ADD:
			pack.instructions.push_back(Instruction(VM_CODE::Add, 0));
			break;
		case OperatorType::SUB:
			pack.instructions.push_back(Instruction(VM_CODE::Add, 0));
			break;
		case OperatorType::MUL:
			pack.instructions.push_back(Instruction(VM_CODE::Mul, 0));
			break;
		case OperatorType::DIV:
			pack.instructions.push_back(Instruction(VM_CODE::Div, 0));
			break;
		case OperatorType::MOD:
			pack.instructions.push_back(Instruction(VM_CODE::Mod, 0));
			break;
		case OperatorType::POW:
			pack.instructions.push_back(Instruction(VM_CODE::Pow, 0));
			break;
		default:
			pack.instructions.push_back(Instruction(VM_CODE::Pop, 0));
			pack.instructions.push_back(Instruction(VM_CODE::Pop, 0));
		}
	}

	void CodeGen::visit(NumberNode* _node)
	{
		auto index = pack.constant.size();
		pack.constant.push_back(Object(_node->value));
		pack.instructions.push_back(Instruction(VM_CODE::LoadC, index));
	}

	void CodeGen::visit(IdentifierNode* _node)
	{
		// read the value of the memory
		// and push to the top of the stack
		auto index = var_id[_node->name];
		pack.instructions.push_back(Instruction(VM_CODE::LoadV, index));
	}

	void CodeGen::visit(AssignmentNode* _node)
	{
		// find if the var is exisits
		// if not exisits add a possition for it
		auto _name = _node->identifier;
		unsigned int _index;
		if (var_id.find(_name) == var_id.end()) // not find
		{
			_index = var_id.size();
			var_id[_name] = _index;
		}
		else
			_index = var_id[_name];

		visit(_node->expression.get());
		pack.instructions.push_back(Instruction(VM_CODE::StoreV, _index));
	}

	void CodeGen::visit(IfStmtNode* _node)
	{
		// wait to finsih;
	}

	void CodeGen::visit(WhileStmtNode* _node)
	{
		// wait to finish
	}

	void CodeGen::visit(DefStmtNode* _node)
	{
		// wait to finish
	}

}
