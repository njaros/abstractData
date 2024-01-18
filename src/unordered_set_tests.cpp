#include <sys/types.h>
#include <sys/stat.h>
#include "../includes/test_elements.hpp"

#if FT == 1
	#include "../includes/unordered_set.hpp"
	#define FT 1
	using namespace ft;
#else
	#include <unordered_set>
	#define FT 0
	using namespace std;
#endif

void unordered_set_tests(const std::string& currentPath, std::ostream& except)
{
	(void) currentPath;
	(void) except;
}
