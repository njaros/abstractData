#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include "../includes/test_elements.hpp"

#if FT == 1
	#include "../includes/queue.hpp"
	#include "../includes/vector.hpp"
	#define FT 1
	using namespace ft;
#else
	#include <queue>
	#include <vector>
	#define FT 0
	using namespace std;
#endif

void	queue_tests(const std::string& currentPath)
{
	std::string fileName;
	std::ofstream outfile;
	std::boolalpha(outfile);

	{
		fileName = currentPath + "constructor.log";
		outfile.open(fileName.c_str());
		vector<int> v(12, 4);
		deque<int> d;
		for (int i = 0; i < 30; ++i)
			d.push_back(i);
		queue<int> s1;
		queue<int> s2(d);
		queue<int, vector<int> > s_v1;
		queue<int, vector<int> > s_v2(v);
		const queue<int> s3(d);

		displayForAdaptator(s1, "empty queue with default underlying container", outfile);
		displayForAdaptator(s2, "filled queue with default underlying container", outfile);
		displayForAdaptator(s_v1, "empty queue with vector underlying container", outfile);
		displayForAdaptator(s_v2, "filled queue with vector underlying container", outfile);
		displayForAdaptator(s3, "const filled queue with default underlying container", outfile);

		outfile.close();
	}

	//TOP, FRONT, PUSH, POP

	{
		fileName = currentPath + "top_front_push_pop.log";
		outfile.open(fileName.c_str());

		deque<int> d;
		for (int i = 0; i < 100; ++i)
			d.push_back(i);
		
		queue<int> q(d);

		for (int i = 100; i < 130; ++i)
		{
			outfile << q.back() << " | " << q.front() << '\n';
			q.pop();
			q.push(i);
		}

		const queue<int> q2(d);

		outfile << q2.back() << " | " << q2.front();

		outfile.close();
	}

	//RELATIONAL OPERATORS

	{
		typedef deque<std::string> V;
		typedef queue<std::string> S;
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