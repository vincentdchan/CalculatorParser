#pragma once
#include <iostream>
#include <map>
#include <cmath>
#include "ast.h"

namespace runtime
{
	using namespace parser;

	class Environment
	{
	public:
		void setValue(const string& _key, Node* _node)
		{
			_content[_key] = _node;
		}
		Node* getValue(const string& _key)
		{
			if (_content.find(_key) == _content.end())
				return nullptr;
			else
				return _content[_key];
		}
	private:	
		std::map<std::string, Node*> _content;
	};

	double eval(Node* root, Environment& env)
	{
		if (root == nullptr)
			return 0;
		else if (root->asNumber())
		{
			return root->asNumber()->value();
		}
		else if (root->asBinaryExpression())
		{
			auto _node = root->asBinaryExpression();
			double leftv = eval(_node->left(), env),
				rightv = eval(_node->right(), env);

#define EQ(NODE, OP) NODE->op() == OperatorType::OP
			if (EQ(_node, ADD))
			{
				return leftv + rightv;
			}
			else if (EQ(_node, SUB))
			{
				return leftv - rightv;
			}
			else if (EQ(_node, MUL))
			{
				return leftv * rightv;
			}
			else if (EQ(_node, DIV))
			{
				return leftv / rightv;
			}
			else if (EQ(_node, MOD))
			{
				return static_cast<int>(leftv) % static_cast<int>(rightv);
			}
			else if (EQ(_node, POW))
			{
				return pow(leftv, rightv);
			}
			else
			{
				return 0;
			}
#undef EQ
		}
		else if (root->asUnaryExpression())
		{
			auto _node = root->asUnaryExpression();
			if (_node->op() == OperatorType::SUB)
			{
				return eval(_node->child(), env) * -1;
			}
			else
				return eval(_node->child(), env);
		}
		else if (root->asBlockExpression())
		{
			auto _node = root->asBlockExpression();
			for (auto i = _node->children.begin(); i != _node->children.end(); ++i)
			{
				std::cout << ": " << eval(*i, env) << std::endl;
			}
			return 0;
		}
		else if (root->asAssignment())
		{
			auto _node = root->asAssignment();
			env.setValue(_node->Identifier(), _node->Expression());
			return 0;
		}
		else if (root->asIdentifier())
		{
			auto _node = root->asIdentifier();
			string _name = _node->Name();
			// if (env.getValue(_name) == nullptr)  Runtime Error
			return eval(env.getValue(_name), env);
		}
		else if (root->asIfStmt())
		{
			auto _node = root->asIfStmt();
			double condition = eval(_node->Condition());
			if (condition != 0)
				return eval(_node->Content());
			return 0.0;
		}
	}
}
