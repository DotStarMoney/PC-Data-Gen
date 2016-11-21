#pragma once

#ifndef P_ASSIGN
#define P_ASSIGN(_X_) assign(this, _X_)
#endif 

namespace pc
{
	template <typename T, typename C>
	class Parameter
	{
		friend class Context;
	public:
		Parameter() { has_assignment = false; }
		Parameter(const T& _val) { value = _val; has_assignment = false; }
		~Parameter() {}
		Parameter(const Parameter&) = delete;
		Parameter& operator=(Parameter<T, C>& _val)
		{
			if (has_assignment) (class_->*func)(_val);
			value = _val.value;
			return *this;
		}
		Parameter& operator=(T& _val)
		{
			if (has_assignment) (class_->*func)(_val);
			value = _val;
			return *this;
		}
		Parameter& operator=(const T& _val)
		{
			if (has_assignment) (class_->*func)(_val);
			value = _val;
			return *this;
		}
		operator T() const { return value; }
	private:
		void assign(C* _class, void(C::* _func)(T))
		{
			class_ = _class;
			func = _func;
			has_assignment = true;
		}
		bool has_assignment;
		T value;
		void(C::* func)(T);
		C* class_;
	};
}