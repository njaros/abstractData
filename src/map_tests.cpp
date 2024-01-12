#include <sys/types.h>
#include <sys/stat.h>
#include "../includes/test_elements.hpp"

#if FT == 1
	#include "../includes/map.hpp"
	#define FT 1
	using namespace ft;
#else
	#include <map>
	#define FT 0
	using namespace std;
#endif

void	map_tests(const std::string& currentPath, std::ostream& except)
{
	std::string fileName;
	std::ofstream outfile;

	std::boolalpha(outfile);

	//CONSTRUCTORS

	{
		fileName = currentPath + "constructor.log";
		outfile.open(fileName.c_str());

		map<int, std::string> me;
		me[3] = "pouet";
		me[2] = "lol";
		me[8] = "gigi";
		me[0] = "0";

		map<int, std::string> mc(me);

		map<int, std::string> mr(mc.begin() + 1, mc.end());

		

		outfile.close();
	}

	//ACCESS

	{
		fileName = currentPath + "access.log";
		outfile.open(fileName.c_str());
		map<int, int> m;

		m[2] = 3;

		try
		{
			m.at(3);
		}
		catch(const out_of_range& e)
		{
			except << "MAP : at out_of_range\n" << e.what() << '\n';
		}
	}

}