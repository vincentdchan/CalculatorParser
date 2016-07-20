#include "stdafx.h"
#include <cinttypes>
#include <iostream>
#include "svm_codes.h"
#include "object.h"
#include "svm.h"

namespace runtime
{
	namespace StackVM
	{

		void StackVM::execute()
		{
			for (;;)
			{
				Instruction* current = inst++;
				if (current->getCode() == VM_CODE::STOP)
					break;
				switch (current->getCode())
				{
				case VM_CODE::LoadV:
					*(objStack++) = state->variables[current->getParam()];
					break;
				case VM_CODE::LoadC:
					*(objStack++) = state->constant[current->getParam()];
					break;
				case VM_CODE::StoreV:
					state->variables[current->getParam()] = *(--objStack);
					break;
				case VM_CODE::PushInt:
					(objStack++)->setSmallInt(current->getParam());
					break;
				case VM_CODE::PushBool:
					(objStack++)->setBool(current->getParam() == 0 ? false : true);
					break;
				case VM_CODE::Pop:
					--objStack;
					break;
				case VM_CODE::JMP:
					inst += current->getParam() - 1;
					break;
				case VM_CODE::Add:
					--objStack;
					*(objStack - 1) = objStack->applyOperator(OperatorType::ADD, *objStack);
					break;
				case VM_CODE::Sub:
					--objStack;
					*(objStack - 1) = objStack->applyOperator(OperatorType::SUB, *objStack);
					break;
				case VM_CODE::Mul:
					--objStack;
					*(objStack - 1) = objStack->applyOperator(OperatorType::MUL, *objStack);
					break;
				case VM_CODE::Div:
					--objStack;
					*(objStack - 1) = objStack->applyOperator(OperatorType::DIV, *objStack);
					break;
				case VM_CODE::Mod:
					--objStack;
					*(objStack - 1) = objStack->applyOperator(OperatorType::MOD, *objStack);
				case VM_CODE::Out:
					std::cout << *(--objStack) << std::endl;
				}
			}
		}

	}
}