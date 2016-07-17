#pragma once

namespace runtime
{
	class GCObject
	{
	public:
		GCObject* next;
		bool marked;
	};

	class DoubleObject : public GCObject
	{
	public:
		double value;
	}

	typedef union
	{
		GCObject* gc;
		int value;
	} Value;

	class Object
	{
	public:
		enum class TYPE
		{
			STRING,
			SINTEGER,
		};
		Value value;
		TYPE type;
	};

}
