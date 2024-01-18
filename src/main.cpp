#include "../includes/test_elements.hpp"
#include "../includes/exception.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <sys/time.h>

std::string filePath(const std::string& ctx, const std::string& container)
{
	std::string ret(ctx);
	ret += '/';
	ret += container;
	ret += '/';
	return ret;
}

std::string dirName(const std::string& current, const std::string& ctx, std::string container = "")
{
	std::string ret(current);
	ret += ctx;
	if (!container.empty())
	{
		ret += '/';
		ret += container;
	}
	return ret;
}

void displayTime(timeval t1, int idFT, const char* testName)
{
	timeval t2;
	gettimeofday(&t2, 0);
	if (idFT == 1)
		std::cout << "FT";
	else
		std::cout << "STD";
	std::cout<< "::" << testName << " time elapsed : " <<
		(t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec) << "usec\n";
}

int main()
{
	std::string mainPath;
	std::string ctxPath;
	std::ofstream exceptionFile;
	timeval t1;

	mainPath = get_current_dir_name();
	mainPath += '/';
	if (FT == 1)
		ctxPath = "ft_logs";
	else
		ctxPath = "std_logs";
	
	mkdir(dirName(mainPath, ctxPath).c_str(), 0777);
	exceptionFile.open((ctxPath + "/exceptions.log").c_str());

	gettimeofday(&t1, 0);
	mkdir(dirName(mainPath, ctxPath, "vector").c_str(), 0777);
	vector_tests(filePath(ctxPath, "vector"), exceptionFile);
	displayTime(t1, FT, "VECTOR");

	gettimeofday(&t1, 0);
	mkdir(dirName(mainPath, ctxPath, "deque").c_str(), 0777);
	deque_tests(filePath(ctxPath, "deque"), exceptionFile);
	displayTime(t1, FT, "DEQUE");

	gettimeofday(&t1, 0);
	mkdir(dirName(mainPath, ctxPath, "stack").c_str(), 0777);
	stack_tests(filePath(ctxPath, "stack"));
	displayTime(t1, FT, "STACK");

	gettimeofday(&t1, 0);
	mkdir(dirName(mainPath, ctxPath, "queue").c_str(), 0777);
	queue_tests(filePath(ctxPath, "queue"));
	displayTime(t1, FT, "QUEUE");

	gettimeofday(&t1, 0);
	mkdir(dirName(mainPath, ctxPath, "priority_queue").c_str(), 0777);
	priority_queue_tests(filePath(ctxPath, "priority_queue"));
	displayTime(t1, FT, "PRIORITY_QUEUE");

	gettimeofday(&t1, 0);
	mkdir(dirName(mainPath, ctxPath, "map").c_str(), 0777);
	map_tests(filePath(ctxPath, "map"), exceptionFile);
	displayTime(t1, FT, "MAP");

	gettimeofday(&t1, 0);
	mkdir(dirName(mainPath, ctxPath, "multimap").c_str(), 0777);
	multimap_tests(filePath(ctxPath, "multimap"));
	displayTime(t1, FT, "MULTIMAP");

	gettimeofday(&t1, 0);
	mkdir(dirName(mainPath, ctxPath, "set").c_str(), 0777);
	set_tests(filePath(ctxPath, "set"));
	displayTime(t1, FT, "SET");

	gettimeofday(&t1, 0);
	mkdir(dirName(mainPath, ctxPath, "multiset").c_str(), 0777);
	multiset_tests(filePath(ctxPath, "multiset"));
	displayTime(t1, FT, "MULTISET");

	gettimeofday(&t1, 0);
	mkdir(dirName(mainPath, ctxPath, "list").c_str(), 0777);
	list_tests(filePath(ctxPath, "list"));
	displayTime(t1, FT, "LIST");

	exceptionFile.close();
	
	return 0;
}
