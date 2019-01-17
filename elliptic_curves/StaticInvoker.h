#pragma once

template<typename ClassName>
class StaticInvoker
{
public:
	StaticInvoker()
	{
		ClassName::Init();
	}
};