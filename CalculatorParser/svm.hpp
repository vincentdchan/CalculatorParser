#pragma once
#ifdef DEBUG
#include <iostream>
#endif // DEBUG

#include <vector>
#include <cinttypes>
#include "svm_codes.h"
#include "object.h"

#define PRE(POINTER) ((POINTER) - 1)

namespace runtime
{
	namespace StackVM
	{


		class State final
		{
		public:
			State(std::size_t _var_size, const std::vector<Object>& _const) :
				var_size(_var_size), constant(_const)
			{
				variables = new Object[_var_size];
			}
			State(const State&) = delete;
			State& operator=(const State&) = delete;
			~State()
			{
				delete variables;
			}
			template<typename InstIter>
			friend class StackVM;
		private:
			std::size_t var_size;
			Object* variables;
			std::vector<Object> constant;
		};

		struct Instruction
		{
			uint16_t _content_;
			Instruction(VM_CODE _code, int param) : _content_(static_cast<int16_t>(_code))
			{
				_content_ = ((_content_ << 8) | (param & 0xFF));
			}
			inline VM_CODE getCode() const
			{
				return static_cast<VM_CODE>((_content_ >> 8) & 0xFF);
			}
			inline int getParam() const
			{
				return static_cast<int>(_content_ & 0xFF);
			}
		};

		template<typename InstIter>
		class StackVM final
		{
		public:
			const static unsigned int VAR_STACK_SIZE = 255;
			StackVM(InstIter _inst) : inst(_inst)
			{
				objStack = objStackBase_ = new Object[VAR_STACK_SIZE];
			}
			StackVM(StackVM&& _svm)
			{
				this->objStackBase_ = _svm.objStackBase_;
				this->objStack = _svm.objStack;
				inst = _svm.inst;

				this->objStack = nullptr;
				this->objStackBase_ = nullptr;
			}
			StackVM(const StackVM&) = delete;
			StackVM& operator=(const StackVM&) = delete;
			void setState(State* st) { state = st; }
			void execute();
			~StackVM()
			{
				delete objStackBase_;
			}
		private:
			InstIter inst;
			Object* objStackBase_;
			Object* objStack;
			State* state;
		};

		template<typename InstIter>
		void StackVM<InstIter>::execute()
		{
			for (;;)
			{
				InstIter current = inst++;
#ifdef DEBUG
				std::cout << "> " << static_cast<int>(current->getCode()) 
					<< " " << current->getParam() << std::endl;

#endif // DEBUG

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
					*PRE(objStack) = PRE(objStack)->applyOperator(OperatorType::ADD, *objStack);
					break;
				case VM_CODE::Sub:
					--objStack;
					*PRE(objStack) = PRE(objStack)->applyOperator(OperatorType::SUB, *objStack);
					break;
				case VM_CODE::Mul:
					--objStack;
					*PRE(objStack) = PRE(objStack)->applyOperator(OperatorType::MUL, *objStack);
					break;
				case VM_CODE::Div:
					--objStack;
					*PRE(objStack) = PRE(objStack)->applyOperator(OperatorType::DIV, *objStack);
					break;
				case VM_CODE::Mod:
					--objStack;
					*PRE(objStack) = PRE(objStack)->applyOperator(OperatorType::MOD, *objStack);
				case VM_CODE::Out:
					std::cout << *(--objStack) << std::endl;
				}
			}
		}

	}

}
