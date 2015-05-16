
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <map>
#include <limits>
#include <forward_list>


/**
 * Prints help for this executable.
 * @param prog The name of the current executable
 * @return the exit code for this program
 */
int print_usage(const std::string& prog)
{
    std::cerr << std::endl;
    std::cerr << "Usage: " << prog << " file.txt" << std::endl;
    return 1;
}



int main(int argc, char* argv[]) {
    if (argc < 2) {
        return print_usage(argv[0]);
    }

    std::fstream fs;
    fs.open(argv[1], std::ios_base::in);

    if (!fs) {
        std::cerr << "File " << argv[1] << " could not be opened for reading" << std::endl;
        return 1;
    }

    std::string s;
    //std::getline(fs, s);
    //std::istringstream iss {s, std::ios_base::in};
    std::vector<std::pair<int, double>> x; 

    while (std::getline(fs, s)) {
        int key;
        double weight;
        std::istringstream iss {s, std::ios_base::in};
        iss >> key;
        iss >> weight;
        x.push_back(std::pair<int, double> {key, weight});
    }

    //sort by key here
    //

    std::vector<std::vector<double>> a;
    a.resize(x.size() + 1);

    for (int row = 0; row < a.size(); row++)
        a[row].resize(x.size() + 1, 0);
    //std::cout << "a: " << a[0][0] << std::endl;

    for (int s = 0; s <= x.size() - 1; s++) {

        for (int i = 0; i < x.size(); i++) {

            if (i + s < x.size()) {
                double sum_p = 0;

                for (int k = i; k <= i + s; k++)
                    sum_p += x[k].second;

                a[i][i + s] = sum_p + a[i + 1][i + s];
                    //std::cout << "i: " << i << "j: " << j << std::endl;

                for (int r = i + 1; r <= i + s; r++) {
                    std::cout << "i: " << i << " s: " << s << " r: " << r << " a[i,i+s]: "  << a[i][i+s] << " sum_p: "  << sum_p << " a[i][r-1] " << a[i][r-1] << " a[r+1]"<< std::endl;
                    double tmp = sum_p + a[i][r - 1] + a[r + 1][i + s];
                    if (tmp < a[i][i + s]) 
                        a[i][i + s] = tmp;
                }
            }
        }
    }


    std::cout << "Min average search time: " << a[0][x.size() - 1] << std::endl;

    for (auto i : a) {
        for (auto j : i)
            std::cout << j << " ";
        std::cout << std::endl;
    }

    fs.close();
}
