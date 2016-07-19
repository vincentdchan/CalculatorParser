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

	class Object;

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

	/********************************/
	/*  do not use virtual method   */
	/*  i don't want to generate a  */
	/*  v-tables                    */
	/********************************/

	TNumber _GetNumVal(Object _obj);

	class Object
	{
	public:
#define E(NAME) NAME##,
		enum class TYPE
		{
			OBJ_LIST(E)
		};
#undef E
		_Value value;
		TYPE type;

		explicit Object() : type(TYPE::NUL) { value.gc = nullptr; }
		explicit Object(GCObject* _object) : type(TYPE::GC) { value.gc = _object; }
		explicit Object(TSmallInt _int) : type(TYPE::SMALL_INT) { value.si = _int; }
		explicit Object(TNumber _num) : type(TYPE::NUMBER) { value.number = _num; }

		inline bool isNul() const { return type == TYPE::NUL; }
		inline bool isGCObject() const { return type == TYPE::GC; }
		inline bool isString() const { return type == TYPE::STRING; }
		inline bool isSmallInt() const { return type == TYPE::SMALL_INT; }
		inline bool isNumber() const { return type == TYPE::NUMBER; }

		inline void setNull() 
		{ 
			value.gc = nullptr;
			type = TYPE::NUL; 
		}
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

#define OP_HANDLE(OP) \
if (isSmallInt() && that.isSmallInt()) \
	return Object(value.number OP that.value.number); \
else if ((isSmallInt() || isNumber()) && (that.isNumber() || that.isSmallInt())) \
	return Object(_GetNumVal(*this) OP _GetNumVal(that)); \
else \
	return Object();

		Object operator+(Object that) const
		{
			OP_HANDLE(+)
		}

		Object operator-(Object that) const
		{
			OP_HANDLE(-)
		}

		Object operator*(Object that) const
		{
			OP_HANDLE(*)
		}

		Object operator/(Object that) const
		{
			OP_HANDLE(/)
		}

		operator bool() const
		{
			if (isSmallInt())
				return value.si == 0;
			else if (isNumber())
				return abs(value.number) < 0.0000001;
			else if (isNul())
				return false;
			else
				return true;
		}

		Object operator%(Object that) const
		{
			if (isSmallInt() && that.isSmallInt())
				return Object(value.si % that.value.si);
			else
				return Object();
		}

		Object powWith(Object that) const
		{
			return Object(0.0);
		}

		Object compareTo(Object that) const
		{
			return Object(0);
		}

		Object equalTo(Object that) const
		{
			return Object(0);
		}
	};

	inline Object make_null()
	{
		return Object();
	}

	class StateObject : public GCObject
	{
	public:
		StateObject() : gc_root(nullptr)
		{}
		std::map<std::string, Object> variables;
		GCObject* gc_root;

		~StateObject()
		{
			iteratorRelease(gc_root);
		}
	private:
		void iteratorRelease(GCObject* tree)
		{
			if (tree == nullptr) return;
			iteratorRelease(tree->next);
			delete tree;
		}
	};

}
