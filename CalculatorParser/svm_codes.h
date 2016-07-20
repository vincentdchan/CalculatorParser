#pragma once

namespace runtime
{
	namespace StackVM
	{
		enum class VM_CODE
		{
			LoadV,					// A load variable
			LoadC,					// A load constant
			StoreV,					// A store the top of stack to A
			PushInt,				// A load A
			PushBool,				// A load A
			Pop,					// Pop
			JMP,
			Add,					// add the top two val
			Sub,
			Mul,
			Div,
			Mod,
			Pow,
			Out,
			STOP
		};
	}
}
