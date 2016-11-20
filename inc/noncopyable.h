#pragma once

#define NONCOPYABLE(_X_) _X_(const _X_&) = delete;				\
						 _X_& operator=(const _X_&) = delete;