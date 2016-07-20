#pragma once
#include <vector>
#include "object.h"

namespace runtime
{
	namespace StackVM
	{


		class State final
		{
		public:
			State(Object* _var, Object* _const) :
				variables(_var), constant(_const)
			{}
			friend class StackVM;
		private:
			Object* variables;
			Object* constant;
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

		class StackVM final
		{
		public:
			const static unsigned int VAR_STACK_SIZE = 255;
			StackVM(Instruction* _inst) : inst(_inst)
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
			void execute();
			~StackVM()
			{
				delete objStackBase_;
			}
		private:
			Instruction* inst;
			Object* objStackBase_;
			Object* objStack;
			State* state;
		};

	}

}
