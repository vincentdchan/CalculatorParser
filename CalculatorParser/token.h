#pragma once
#include <iostream>
#include <cinttypes>
#include <utility>

typedef int32_t uc32;

#define OPERATOR_LIST(V) \
	V(ADD, "+", 10) \
	V(SUB, "-", 10) \
	V(MUL, "*", 11) \
	V(DIV, "/", 11) \
	V(MOD, "%", 12) \
	V(POW, "**", 13) \
	V(ILLEGAL_OP, "", 0)

#define TOKEN_LIST(V) \
	V(ILLEGAL) \
	V(OPEN_PAREN) \
	V(CLOSE_PAREN) \
	V(NUMBER) \
	V(ENDLINE) \
	V(ENDFILE) 


#define E(NAME, STR, PRE) NAME,
	enum OperatorType
	{
		OPERATOR_LIST(E)
		OP_NUM
	};
#undef E


#define E(NAME, STR, PRE) PRE,
	const int OpreatorPrecedence[OperatorType::OP_NUM] = 
	{
		OPERATOR_LIST(E)
	};
#undef E

int getPrecedence(OperatorType ot);

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
		Token(): _type(ILLEGAL)
		{}

		Token(const Token& _target)
		{
			_type = _target._type;
			location = _target.location;
		}

		Token(TYPE&& _t, Location&& _loc)
		{
			_type = move(_t);
			location = move(_loc);
		}


		Token(Token&& _target)
		{
			_type = move(_target._type);
			location = move(_target.location);
		}

		void set_type(TYPE t)
		{
			_type = t;
		}

		TYPE type() const
		{
			return  _type;
		}
		Location location;

		static bool isOperator(const Token& t)
		{
			return t.type() >= ADD && t.type() < ILLEGAL_OP;
		}

		static OperatorType toOperator(Token t)
		{
			if (t.type() >= Token::ADD && t.type() <= Token::ILLEGAL_OP)
				return static_cast<OperatorType>(t.type());
			else
				return OperatorType::ILLEGAL_OP;
		}

		Token& operator=(const Token& _t)
		{
			_type = _t._type;
			location = _t.location;
			return *this;
		}

		Token& operator=(Token&& _t)
		{
			_type = move(_t._type);
			location = move(_t.location);
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
