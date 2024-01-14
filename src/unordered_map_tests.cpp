#include <sys/types.h>
#include <sys/stat.h>
#include "../includes/test_elements.hpp"

#if FT == 1
	#include "../includes/unordered_map.hpp"
	#define FT 1
	using namespace ft;
#else
	#include <unordered_map>
	#define FT 0
	using namespace std;
#endif