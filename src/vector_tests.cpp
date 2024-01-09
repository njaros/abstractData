#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#if FT == 1
    #include "../includes/vector.hpp"
    #include "../includes/set.hpp"
    #define FT 1
    using namespace ft;
#else
    #include <vector>
    #include <set>
    #define FT 0
    using namespace std;
#endif

template < typename T >
void    displayData(const T& container, const char* name, std::ostream& stream)
{
	typename T::const_iterator it;
	typename T::const_iterator end;

	it = container.begin();
	end = container.end();
	stream << name << " content :" << '\n';
	while (it != end)
	{
		stream << *it << '\n';
		it++;
	}
	stream << '\n';
}

template < typename T >
void	displayInfo(const T& container, const char* name, std::ostream& stream)
{
	stream << "size " << name << " : " << container.size() << '\n';
	stream << "capacity " << name << " : " << container.capacity() << '\n';
	stream << "max_size " << name << " : " << container.max_size() << "\n";
    displayData(container, name, stream);
}

void    vector_tests(const std::string& currentPath)
{
    std::string fileName;

    //CONSTRUCTOR ASSERTS

    set<std::string> toolForRangeFill;
    toolForRangeFill.insert("gsh");
    toolForRangeFill.insert("");
    toolForRangeFill.insert("joue");
    toolForRangeFill.insert("clap");
    toolForRangeFill.insert("wboui");
    toolForRangeFill.insert("zboui");

    const set<std::string> constRange(toolForRangeFill);

    fileName = currentPath + "constructors.log";
    
    std::ofstream outfile(fileName.c_str(), std::ios_base::out);

    vector<std::string> empty;
    displayInfo(empty, "default constructor", outfile);

    const vector<std::string> filled(12, "canard");
    displayInfo(filled, "filled constructor", outfile);

    const vector<std::string> ranged(toolForRangeFill.begin(), toolForRangeFill.end());
    displayInfo(ranged, "range constructor with another container type", outfile);

    vector<std::string> ranged2(constRange.begin(), constRange.end());
    displayInfo(ranged2, "range constructor with a const another container type", outfile);

    vector<std::string> ranged3(filled.begin(), filled.end());
    displayInfo(ranged3, "range constructor with a const vector", outfile);

    vector<std::string> copy(ranged);
    displayInfo(copy, "copy constructor with a const vector", outfile);

    copy = filled;
    displayInfo(copy, "operator =", outfile);

    //INSERT ASSERTS

    ranged2.insert(ranged2.begin(), empty.begin(), empty.end());
    ranged2.insert(ranged2.begin(), 0, "poule");
    displayInfo(ranged2, "result after insert nothing", outfile);
    
}