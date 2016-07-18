#pragma once
#include <map>

#define OBJ_LIST(V) \
	V(NUL) \
	V(GC) \
	V(STRING) \
	V(SMALL_INT) \
	V(NUMBER)


namespace runtime
{
	typedef double TNumber;
	typedef int TSmallInt;

	class GCObject
	{
	public:
		GCObject* next;
		bool marked;
	};

	union _Value
	{
		GCObject* gc;
		TSmallInt si;		// small int
		TNumber number;
	};

	class Object
	{
	public:
#define E(NAME) NAME##,
		enum class TYPE
		{
			OBJ_LIST(E)
		};
#undef E
		Object(TYPE t = TYPE::NUL):
			type(t)
		{}
		_Value value;
		TYPE type;

		inline bool isNul() { return type == TYPE::NUL; }
		inline bool isGCObject() { return type == TYPE::GC; }
		inline bool isString() { return type == TYPE::STRING; }
		inline bool isSmallInt() { return type == TYPE::SMALL_INT; }
		inline bool isNumber() { return type == TYPE::NUMBER; }

		inline void setNull() { type == TYPE::NUL; }
		inline void setGCObject(GCObject* _obj)
		{
			value.gc = _obj;
			type = TYPE::GC;
		}
		inline void setString(GCObject* _str)
		{
			value.gc = _str;
			type = TYPE::STRING;
		}
		inline void setNumber(TNumber num)
		{
			value.number = num;
			type = TYPE::NUMBER;
		}
		inline void setSmallInt(TSmallInt num)
		{
			value.si = num;
			type = TYPE::NUMBER;
		}
	};

	inline Object make_null()
	{
		return Object(Object::TYPE::NUL);
	}

	class StateObject : public GCObject
	{
	public:
		StateObject() : gc_root(nullptr)
		{}
		std::map<std::string, Object> variables;
		GCObject* gc_root;
	};

}
