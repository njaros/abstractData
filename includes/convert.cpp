#include <iostream>
#include <fstream>

int main(int ac, char **av)
{
    std::ifstream input(av[1]);

    std::string fileNameOutPut = av[1];
    fileNameOutPut += "_utf8";

    char c = input.get();
    if (c == EOF)
    {
        c = input.get();
        if (c == -2)
        {
            std::cout << "that's an utf16 file, convert proceeds\n";
            std::ofstream out(fileNameOutPut);
            c = input.get();
            while (c != EOF)
            {
                if (c)
                    out.write(&c, 1);
                    c = input.get();
            }
            out.close();
            std::cout << "convert finished\n";
        }
    }
    else
        std::cout << "that's not an utf16 file\n";
    input.close();
}