#pragma once
#define TP1_2(x, y) x ## y
#define TP2_2(x, y) TP1_2(x, y)

#define CALL_ON_TERMINATION(_W_, _X_) \
	GlobalScopeExecutor TP2_2(_W_, __COUNTER__)(nullptr, _X_)
#define CALL_ON_START(_W_, _X_) \
	GlobalScopeExecutor TP2_2(_W_, __COUNTER__)(_X_, nullptr)
#define CALL_ON_BOUNDS(_W_, _X_, _Y_) \
	GlobalScopeExecutor TP2_2(_W_, __COUNTER__)(_X_, _Y_)

class GlobalScopeExecutor
{
public:
	GlobalScopeExecutor(
		void(*_construct)() = nullptr,
		void(*_destruct)() = nullptr)
	{
		if (_construct) _construct();
		destruct = _destruct;
	}
	~GlobalScopeExecutor()
	{
		if(destruct) destruct();
	}
private:
	void(*destruct)();

};