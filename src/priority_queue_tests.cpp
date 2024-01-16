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
	#include "../includes/deque.hpp"
	#define FT 1
	using namespace ft;
#else
	#include <queue>
	#include <deque>
	#define FT 0
	using namespace std;
#endif

void priority_queue_tests(const std::string& currentPath)
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
		priority_queue<int> s1;
		priority_queue<int, deque<int>, less<int> > s2(less<int>(), d);
		priority_queue<int> s_v1;
		priority_queue<int, vector<int>, greater<int> > s_v2(greater<int>(), v);
		const priority_queue<int, deque<int>, less<int> > s3(less<int>(), d);
		const priority_queue<int, vector<int>, less<int> > s_v3(d.begin() + 4, d.end() - 5);

		displayForAdaptator(s1, "empty priority_queue with default underlying container", outfile);
		displayForAdaptator(s2, "filled priority_queue with default underlying container", outfile);
		displayForAdaptator(s_v1, "empty priority_queue with vector underlying container", outfile);
		displayForAdaptator(s_v2, "filled priority_queue with vector underlying container", outfile);
		displayForAdaptator(s_v3, "filled priority_queue with vector underlying container", outfile);
		displayForAdaptator(s3, "const filled priority_queue with default underlying container", outfile);

		outfile.close();
	}

	//TOP, PUSH, POP

	{
		fileName = currentPath + "top_push_pop.log";
		outfile.open(fileName.c_str());
		vector<int> v;
		deque<int> d;
		size_t seed = 0;
		for (int i = 0; i < 30; ++i)
		{
			v.push_back(getRandom(seed, 25));
			d.push_front(getRandom(seed, 15));
		}
		priority_queue<std::string> ve;
		priority_queue<int, vector<int>, greater<int> > vgf(greater<int>(), v);
		priority_queue<int, deque<int> > de;
		priority_queue<int> vf(less<int>(), v);
		const priority_queue<int, deque<int> > cdf(less<int>(), d);

		outfile << cdf.top() << '\n';
		outfile << "\n\n";

		while (vgf.size())
		{
			outfile << vgf.top() << " | ";
			vgf.pop();
		}
		outfile << "\n\n";

		while (vf.size())
		{
			outfile << vf.top() << " | ";
			vf.pop();
		}
		outfile << "\n\n";

		while (de.size() < 30)
		{
			de.push(getRandom(seed, 78));
			outfile << de.top() << '\n';
		}
		outfile << "\n\n";

		while (de.size())
		{
			outfile << de.top() << " | ";
			de.pop();
		}

		outfile << "\n\n";

		ve.push("hello");
		outfile << ve.top() << '\n';
		ve.push("hhello");
		outfile << ve.top() << '\n';
		ve.push("hehello");
		outfile << ve.top() << '\n';
		ve.push("ehllo");
		outfile << ve.top() << '\n';

		outfile << "\n\n";

		while (ve.size())
		{
			outfile << ve.top() << '\n';
			ve.pop();
		}

		outfile.close();
	}
}