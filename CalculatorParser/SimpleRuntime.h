#pragma once
#include <iostream>
#include <cmath>
#include "ast.h"

namespace runtime
{
	using namespace parser;

	double eval(Node* root)
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
			double leftv = eval(_node->left()),
				rightv = eval(_node->right());

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
				return eval(_node->child()) * -1;
			}
			else
				return eval(_node->child());
		}
		else if (root->asBlockExpression())
		{
			auto _node = root->asBlockExpression();
			for (auto i = _node->children.begin(); i != _node->children.end(); ++i)
			{
				std::cout << ": " << eval(*i) << std::endl;
			}
			return 0;
		}
	}
}
