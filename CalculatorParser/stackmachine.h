#pragma once
#include <vector>
#include <stack>
#include "object.h"

namespace runtime
{

	namespace stackmachine
	{
		class SMState;

		enum INST
		{
			IADD, ISUB, IMUL, IDIV, IPOW,
			IPUSH, IPOP,
			IJMP, // jump to the location depend on top data stack
			ILOAD, // load a value from variable table
			ICMP,
			ISTORE, // pop the top and store to the variable
			ISTOP,
#ifdef DEBUG
			IOUT, // output the top of the stack, only for debug
#endif // DEBUG
		};

		enum FLAG
		{
			FNA, FEQ, FNE
		};

		struct instruction
		{
			INST value;
			FLAG flag;
			Object p1;
		};

		void execute(SMState* state);

		class SMState final
		{
		public:
			SMState(const std::vector<instruction>& _ins): 
				instVec(_ins), inst_p(0), current_flag(FNA)
			{}
			~SMState()
			{
				std::stack<GCObject*> objStack;
				for (GCObject* _i = gc_root; _i != nullptr; _i = _i->next)
					objStack.push(_i);
				while (!objStack.empty())
				{
					delete objStack.top();
					objStack.pop();
				}
			}
			friend void execute(SMState*);
		private:
			FLAG current_flag;
			unsigned int inst_p;
			std::stack<Object> dataStack;
			std::vector<instruction> instVec;
			GCObject* gc_root;
		};


	}
}

