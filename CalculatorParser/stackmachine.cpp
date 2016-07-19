#include "stdafx.h"
#include "stackmachine.h"

namespace runtime
{
	namespace stackmachine
	{

#define OPMACRO(OP) \
		o1 = state->dataStack.top(); state->dataStack.pop(); \
		o2 = state->dataStack.top(); state->dataStack.pop(); \
		state->dataStack.push(o1 OP o2);

		void execute(SMState* state)
		{
			Object o1, o2;
			for (;;)
			{
				instruction& current = state->instVec[state->inst_p++];
				if (current.flag != FNA && current.flag != state->current_flag)
					continue;
				switch (current.value)
				{
				case IADD:
					OPMACRO(+)
					break;
				case ISUB:
					OPMACRO(-)
					break;
				case IMUL:
					OPMACRO(*)
					break;
				case IDIV:
					OPMACRO(/)
					break;
				case IPOW:
					o1 = state->dataStack.top(); state->dataStack.pop();
					o2 = state->dataStack.top(); state->dataStack.pop();
					state->dataStack.push(o1.powWith(o2));
					break;
				case IPUSH:
					state->dataStack.push(current.p1);
					break;
				case IPOP:
					state->dataStack.pop();
					break;
				case IJMP:
					o1 = state->dataStack.top();
					if (o1.isSmallInt())
						state->inst_p += o1.value.si;
					break;
				case ICMP:
					o1 = state->dataStack.top(); state->dataStack.pop();
					o2 = state->dataStack.top(); state->dataStack.pop();
					if (o1.equalTo(o2))
						state->current_flag = FEQ;
					else
						state->current_flag = FNE;
					break;
				}
			}
		}

	};
};