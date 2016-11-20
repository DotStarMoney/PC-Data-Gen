#pragma once

#define CALL_ON_TERMINATION(_X_) GlobalScopeExecutor EXEC##__COUNTER__(nullptr, _X_);
#define CALL_ON_START(_X_) GlobalScopeExecutor EXEC##__COUNTER__(_X_, nullptr);
#define CALL_ON_BOUNDS(_X_, _Y_) GlobalScopeExecutor EXEC##__COUNTER__(_X_, _Y_);

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