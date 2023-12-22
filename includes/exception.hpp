#ifndef EXCEPT_HPP
# define EXCEPT_HPP

#include <memory>
#include <string>

namespace ft
{
	class exception
	{
		char* _msg;
		size_t _msgSize;
		std::allocator<char> _al;

		char* _cpyMsg(const char* toCpy, size_t toCpySize);

	protected:

		void setMsg(const std::string& str);

	public:
		exception() throw();
		exception(const exception& o) throw();
		exception& operator=(const exception& o) throw();
		virtual ~exception() throw();
		virtual const char* what() const throw();
	};

	class logic_error : public exception
	{
	public:
		explicit logic_error(const std::string& str);
	};

	class bad_alloc : public exception
	{
	public:
		explicit bad_alloc(const std::string& str);
	};

	class out_of_range : public logic_error
	{
	public:
		explicit out_of_range(const std::string& str);
	};

	class length_error : public logic_error
	{
	public:
		explicit length_error(const std::string& str);
	};

	class invalid_argument : public logic_error
	{
	public:
		explicit invalid_argument(const std::string& str);
	};
}

#endif // !EXCEPT_HPP
