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
		ctxPath = "ft_logs_bonus";
	else
		ctxPath = "std_logs_bonus";
	
	mkdir(dirName(mainPath, ctxPath).c_str(), 0777);
	exceptionFile.open((ctxPath + "/exceptions.log").c_str());

	gettimeofday(&t1, 0);
	mkdir(dirName(mainPath, ctxPath, "unordered_map").c_str(), 0777);
	unordered_map_tests(filePath(ctxPath, "unordered_map"), exceptionFile);
	displayTime(t1, FT, "UNORDERED_MAP");

	gettimeofday(&t1, 0);
	mkdir(dirName(mainPath, ctxPath, "unordered_set").c_str(), 0777);
	unordered_set_tests(filePath(ctxPath, "unordered_set"), exceptionFile);
	displayTime(t1, FT, "UNORDERED_SET");

	gettimeofday(&t1, 0);
	mkdir(dirName(mainPath, ctxPath, "unordered_multimap").c_str(), 0777);
	unordered_multimap_tests(filePath(ctxPath, "unordered_multimap"), exceptionFile);
	displayTime(t1, FT, "UNORDERED_MULTIMAP");

	gettimeofday(&t1, 0);
	mkdir(dirName(mainPath, ctxPath, "unordered_multiset").c_str(), 0777);
	unordered_multiset_tests(filePath(ctxPath, "unordered_multiset"), exceptionFile);
	displayTime(t1, FT, "UNORDERED_MULTISET");

	exceptionFile.close();
}