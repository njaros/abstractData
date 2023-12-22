#include "../includes/exception.hpp"

namespace ft
{
	exception::exception() throw() : _msg(0), _msgSize(0)
	{}
	exception::exception(const exception& o) throw() : _msg(_cpyMsg(o._msg, o._msgSize)), _msgSize(o._msgSize)
	{}

	exception& exception::operator=(const exception& o) throw()
	{
		if (this == &o)
			return *this;
		_al.deallocate(_msg, _msgSize);
		_msgSize = o._msgSize;
		_cpyMsg(o._msg, _msgSize);

		return *this;
	}

	exception::~exception() throw()
	{
		_al.deallocate(_msg, _msgSize);
	}

	char* exception::_cpyMsg(const char* toCpy, size_t toCpySize)
	{
		char* cpy = 0;
		size_t idx = 0;

		if (toCpy)
		{
			cpy = _al.allocate(toCpySize);
			while (toCpy[idx])
			{
				cpy[idx] = toCpy[idx];
				++idx;
			}
			cpy[idx] = '\0';
		}
		return cpy;
	}

	void exception::setMsg(const std::string& str)
	{
		_al.deallocate(_msg, _msgSize);
		_msgSize = str.size() + 1;
		_msg = _cpyMsg(str.c_str(), _msgSize);
	}

	const char* exception::what() const throw()
	{
		return _msg ? _msg : "Unknown exception";
	}

	logic_error::logic_error(const std::string& str) : exception::exception()
	{
		setMsg(str);
	}

	bad_alloc::bad_alloc(const std::string& str) : exception::exception()
	{
		setMsg(str);
	}

	out_of_range::out_of_range(const std::string& str) : logic_error::logic_error(str)
	{}

	length_error::length_error(const std::string& str) : logic_error::logic_error(str)
	{}

	invalid_argument::invalid_argument(const std::string& str) : logic_error::logic_error(str)
	{}
}