#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include "../includes/test_elements.hpp"

#if FT == 1
	#include "../includes/stack.hpp"
	#include "../includes/vector.hpp"
	#define FT 1
	using namespace ft;
#else
	#include <stack>
	#include <vector>
	#define FT 0
	using namespace std;
#endif

void stack_tests(const std::string& currentPath)
{
	std::string fileName;
	std::ofstream outfile;
	std::boolalpha(outfile);

	//CONSTRUCTORS

	{
		fileName = currentPath + "constructor.log";
		outfile.open(fileName.c_str());
		vector<int> v(12, 4);
		deque<int> d;
		for (int i = 0; i < 30; ++i)
			d.push_back(i);
		stack<int> s1;
		stack<int> s2(d);
		stack<int, vector<int> > s_v1;
		stack<int, vector<int> > s_v2(v);

		displayForAdaptator(s1, "empty stack with default underlying container", outfile);
		displayForAdaptator(s2, "filled stack with default underlying container", outfile);
		displayForAdaptator(s_v1, "empty stack with vector underlying container", outfile);
		displayForAdaptator(s_v2, "filled stack with vector underlying container", outfile);

		outfile.close();
	}

	//TOP, PUSH, POP

	{
		fileName = currentPath + "top_push_pop.log";
		outfile.open(fileName.c_str());
		vector<int> v(12, 4);
		deque<int> d;
		for (int i = 0; i < 30; ++i)
			d.push_back(i);
		stack<int> s1;
		stack<int> s2(d);
		stack<int, vector<int> > s_v1;
		stack<int, vector<int> > s_v2(v);
		const stack<int> c1(d);

		outfile << "top = " << s2.top() << '\n';
		s2.push(44);
		outfile << "top = " << s2.top() << '\n';

		outfile << "top = " << s_v2.top() << '\n';
		s_v2.push(44);
		outfile << "top = " << s_v2.top() << '\n';
		s_v2.top() = 512;
		outfile << "top = " << s_v2.top() << '\n';

		for (int i = 0; i <= 30; ++i)
		{
			outfile << "top = " << s2.top() << " | ";
			s2.pop();
		}

		outfile << '\n';
		displayForAdaptator(s2, "full poped stack", outfile);

		for (int i = 0; i < 23; ++i)
		{
			s2.push(i * i + i);
			outfile << "top = " << s2.top() << " | ";
		}

		outfile << '\n';
		displayForAdaptator(s2, "re pushed stack", outfile);

		outfile << "const top : " << c1.top();

		outfile.close();
	}

	//RELATIONAL OPERATORS

	{
		typedef deque<std::string> V;
		typedef stack<std::string> S;
		fileName = currentPath + "relational.log";
		outfile.open(fileName.c_str());
		const V dc1(4, "fleur");
		V dv4(dc1);
		V dv1(5, "fleur");
		V dv2(5, "fleurs");
		V dv3(dv1);
		dv3.resize(7, "fleur");

		S c1(dc1);
		S v1(dv1);
		S v2(dv2);
		S v3(dv3);
		S v4(dv4);

		outfile << (c1 == v4) << '|' << (c1 == v1) << '|' << (v1 == v2) << '|' << (v3 == v1) << '|' << (v3 == v2) << '\n';
		outfile << (c1 != v4) << '|' << (c1 != v1) << '|' << (v1 != v2) << '|' << (v3 != v1) << '|' << (v3 != v2) << '\n';
		outfile << (c1 < v4) << '|' << (c1 < v1) << '|' << (v1 < v2) << '|' << (v3 < v1) << '|' << (v3 < v2) << '\n';
		outfile << (c1 <= v4) << '|' << (c1 <= v1) << '|' << (v1 <= v2) << '|' << (v3 <= v1) << '|' << (v3 <= v2) << '\n';
		outfile << (c1 > v4) << '|' << (c1 > v1) << '|' << (v1 > v2) << '|' << (v3 > v1) << '|' << (v3 > v2) << '\n';
		outfile << (c1 >= v4) << '|' << (c1 >= v1) << '|' << (v1 >= v2) << '|' << (v3 >= v1) << '|' << (v3 >= v2) << '\n';

		outfile.close();
	}
}