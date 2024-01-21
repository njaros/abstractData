#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <map>
#include <cstring>
#include <string>

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
	ret += '/';
	ret += ctx;
	if (!container.empty())
	{
		ret += '/';
		ret += container;
	}
	return ret;
}

std::string fileType(const struct dirent* entry)
{
	switch (entry->d_type)
	{
		case DT_BLK:
		return "block file";
		case DT_CHR:
		return "character device";
		case DT_DIR:
		return "directory";
		case DT_FIFO:
		return "FIFO";
		case DT_REG:
		return "regular file";
		case DT_SOCK:
		return "socket";
		case DT_UNKNOWN:
		return "undeterminded";
	}
	return "!!!??";
}

std::string fileInfo(const struct dirent* entry)
{
	std::string ret(entry->d_name);
	ret += ' ';
	ret += fileType(entry);

	return ret;
}

int diff_files(const std::string& file1, const std::string& file2, std::ostream& diffFile)
{
	int diffCount = 0;
	unsigned int lineCount = 0;
	std::ifstream in1;
	std::ifstream in2;
	std::string str1;
	std::string str2;
	bool isExceptFile = false;

	in1.open(file1.c_str());
	in2.open(file2.c_str());
	if (in1.fail() || in2.fail())
		return -1;

	if (file1.find("exceptions.log", 0) != std::string::npos)
	{
		isExceptFile = true;
	}

	while (!in1.eof() && !in2.eof())
	{
		++lineCount;
		getline(in1, str1);
		getline(in2, str2);
		if (str1.compare(str2) && str1.compare(0, 8, "max_size") && str2.compare(0, 8, "max_size"))
		{
			diffCount = 1;
			diffFile << file1 << " : line " << lineCount << " :\n" << str1 << '\n';
			diffFile << file2 << " : line " << lineCount << " :\n" << str2 << '\n';
		}
		if (isExceptFile && !in1.eof() && !in2.eof())
		{
			getline(in1, str1);
			getline(in2, str2);
			++lineCount;
		}
	}

	while (!in1.eof())
	{
		++lineCount;
		getline(in1, str1);
		diffCount = 1;
		diffFile << file1 << " : line " << lineCount << " :\n" << str1 << '\n';
	}

	while (!in2.eof())
	{
		++lineCount;
		getline(in2, str2);
		diffCount = 1;
		diffFile << file2 << " : line " << lineCount << " :\n" << str2 << '\n';
	}

	return diffCount;
}

int diff_logs(const std::string& mainPath1, const std::string& mainPath2, std::ostream& diffFile)
{
	typedef std::map<std::string, std::string> fmap;

	int count = 0;
	int diff_res;
	DIR* dp1;
	DIR* dp2;
	struct dirent *entry1;
	struct dirent *entry2;
	fmap dirList1;
	fmap dirList2;
	fmap fileList1;
	fmap fileList2;
	fmap::const_iterator cit1;
	fmap::const_iterator cit2;

	dp1 = opendir(mainPath1.c_str());
	dp2 = opendir(mainPath2.c_str());

	if (!dp1 || !dp2)
		return -1;

	entry1 = readdir(dp1);
	while (entry1)
	{
		fileInfo(entry1);
		if (entry1->d_type == DT_DIR && strcmp(entry1->d_name, ".") && strcmp(entry1->d_name, ".."))
			dirList1[entry1->d_name] = dirName(mainPath1, entry1->d_name);
		else if (entry1->d_type == DT_REG)
			fileList1[entry1->d_name] = dirName(mainPath1, entry1->d_name);
		entry1 = readdir(dp1);
	}

	entry2 = readdir(dp2);
	while (entry2)
	{
		fileInfo(entry2);
		if (entry2->d_type == DT_DIR && strcmp(entry2->d_name, ".") && strcmp(entry2->d_name, "..") && strncmp(entry2->d_name, "NC", 2))
			dirList2[entry2->d_name] = dirName(mainPath2, entry2->d_name);
		else if (entry2->d_type == DT_REG)
			fileList2[entry2->d_name] = dirName(mainPath2, entry2->d_name);
		entry2 = readdir(dp2);
	}

	cit1 = dirList1.begin();
	cit2 = dirList2.begin();

	while (cit1 != dirList1.end() && cit2 != dirList2.end())
	{
		if (cit1->first < cit2->first)
		{
			diffFile << "missing directory in " << mainPath2 << " : " << cit1->first << '\n';
			++count;
			++cit1;
		}
		else if (cit2->first < cit1->first)
		{
			diffFile << "missing directory in " << mainPath1 << " : " << cit2->first << '\n';
			++count;
			++cit2;
		}
		else
		{
			diff_res = diff_logs(cit1->second, cit2->second, diffFile);
			if (diff_res == -1)
				return diff_res;
			count += diff_res;
			++cit1;
			++cit2;
		}
	}

	while (cit1 != dirList1.end())
	{
		diffFile << "missing directory in " << mainPath2 << " : " << cit1->first << '\n';
		++count;
		++cit1;
	}

	while (cit2 != dirList2.end())
	{
		diffFile << "missing directory in " << mainPath1 << " : " << cit2->first << '\n';
		++count;
		++cit2;
	}

	cit1 = fileList1.begin();
	cit2 = fileList2.begin();

	while (cit1 != fileList1.end() && cit2 != fileList2.end())
	{
		if (cit1->first < cit2->first)
		{
			diffFile << "missing file in " << mainPath2 << " : " << cit1->first << '\n';
			++count;
			++cit1;
		}
		else if (cit2->first < cit1->first)
		{
			diffFile << "missing file in " << mainPath1 << " : " << cit2->first << '\n';
			++count;
			++cit2;
		}
		else
		{
			if (cit1->first.compare(0, 2, "NC"))
			{
				diff_res = diff_files(cit1->second, cit2->second, diffFile);
				if (diff_res == -1)
					return diff_res;
				count += diff_res;
			}
			++cit1;
			++cit2;
		}
	}

	while (cit1 != fileList1.end())
	{
		diffFile << "missing file in " << mainPath2 << " : " << cit1->first << '\n';
		++count;
		++cit1;
	}

	while (cit2 != fileList2.end())
	{
		diffFile << "missing file in " << mainPath1 << " : " << cit2->first << '\n';
		++count;
		++cit2;
	}

	return count;
}

int main(void)
{
	int diffCount;
	std::string pwd = get_current_dir_name();
	std::string std = "std_logs";
	std::string ft = "ft_logs";
	std::string std_bonus = "std_logs_bonus";
	std::string ft_bonus = "ft_logs_bonus";

	diffCount = diff_logs(dirName(pwd, std), dirName(pwd, ft), std::cout);

	if (diffCount == -1)
	{
		std::cout << "mandatory part : an error occured : " << errno << " : " << strerror(errno) << "\n\n";
	}

	else
	{
		std::cout << "mandatory part : theyre is " << diffCount << " difference";
		if (diffCount > 1)
		{
			std::cout << 's';
		}
		std::cout << " on mandatory tests.\n\n";
	}

	diffCount = diff_logs(dirName(pwd, std_bonus), dirName(pwd, ft_bonus), std::cout);

	if (diffCount != -1)
	{
		std::cout << "bonus part : theyre is " << diffCount << " difference";
		if (diffCount > 1)
		{
			std::cout << 's';
		}
		std::cout << " on bonus tests.\n\n";
	}

	return 0;
}