#pragma once
#include <iostream>
#include <utility>
#include <memory>
#include "CalculatorParser.h"


namespace lex
{

	using namespace std;

	struct Location
	{
		Location(int _line = -1, int _begin = -1, int _end = -1): 
			line(_line), begin(_begin), end(_end) {}
		int line;
		int begin, end;
	};

	class Token
	{
	public:
#define EXTEND_OP(NAME, STR, PRECEDENCE) NAME##,
#define EXTEND_TOKEN(NAME) NAME##,
		enum TYPE
		{
			OPERATOR_LIST(EXTEND_OP)
			TOKEN_LIST(EXTEND_TOKEN)
			TOKEN_NUMBER
		};
#undef EXTEND_OP
#undef EXTEND_TOKEN
		Location location;
		std::unique_ptr<std::string> pLiteral;

		Token(): _type(ILLEGAL)
		{}

		Token(const Token& _target) = delete;
		/*
		Token(const Token& _target)
		{
			_type = _target._type;
			location = _target.location;
			pLiteral = move(_target.pLiteral);
		}
		*/

		Token(TYPE _t, Location _loc)
		{
			_type = _t;
			location = _loc;
		}


		Token(Token&& _target)
		{
			_type = _target._type;
			location = _target.location;
			pLiteral = move(_target.pLiteral);
		}

		void set_type(TYPE t)
		{
			_type = t;
		}

		TYPE type() const
		{
			return  _type;
		}

		static bool isOperator(const Token& t)
		{
			return t.type() >= ADD && t.type() < ILLEGAL_OP;
		}

		static OperatorType toOperator(const Token& t)
		{
			if (t.type() >= Token::ADD && t.type() <= Token::ILLEGAL_OP)
				return static_cast<OperatorType>(t.type());
			else
				return OperatorType::ILLEGAL_OP;
		}

		Token& operator=(const Token& _t) = delete;
		/*
		Token& operator=(const Token& _t)
		{
			_type = _t._type;
			location = _t.location;
			return *this;
		}
		*/

		Token& operator=(Token&& _t)
		{
			_type = _t._type;
			location = _t.location;
			pLiteral = move(_t.pLiteral);
			return *this;
		}

		bool operator==(const Token& t) const
		{
			return this->_type == t._type;
		}

		bool operator!=(const Token& t) const
		{
			return this->_type != t._type;
		}

		bool operator==(TYPE _t) const
		{
			return this->_type == _t;
		}

		bool operator!=(TYPE _t) const
		{
			return this->_type != _t;
		}

		operator bool() const
		{
			return type() != TYPE::ILLEGAL;
		}
	private:
		TYPE _type;
	};
}
