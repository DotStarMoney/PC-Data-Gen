#pragma once

#ifndef P_ASSIGN
#define P_ASSIGN(_X_) assign(this, _X_)
#endif 

namespace pc
{
	template <typename T, typename C>
	class Parameter_RO
	{
		friend class Context;
		friend class Image;
	public:
		~Parameter_RO() {}
		operator T() const 
		{ 
			if (has_assignment) (class_->*func)();
			return value; 
		}
	private:
		Parameter_RO() { has_assignment = false; }
		Parameter_RO(const T& _val) { value = _val; has_assignment = false; }
		Parameter_RO(const Parameter_RO&);
		void operator=(const Parameter_RO& _val) 
		{ 
			value = _val; 
			has_assignment = false;
		}
	private:
		void assign(C* _class, void(C::* _func)())
		{
			class_ = _class;
			func = _func;
			has_assignment = true;
		}
		bool has_assignment;
		T value;
		void(C::* func)();
		C* class_;
	};
}