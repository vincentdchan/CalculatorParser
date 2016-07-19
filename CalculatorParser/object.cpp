#include "stdafx.h"
#include "object.h"

namespace runtime
{
	TNumber _GetNumVal(Object _obj)
	{
		if (_obj.isNumber())
			return _obj.value.number;
		else if (_obj.isSmallInt())
			return static_cast<TNumber>(_obj.value.si);
		else
			return 0.0;
	}
};