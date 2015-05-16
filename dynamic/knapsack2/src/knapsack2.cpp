
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


unsigned long int knapsack_top_down(std::vector<std::unordered_map<int, unsigned long int>>& a, std::vector<std::pair<int, int>> const& t, int i, int w) {

    //for(unsigned int i = t.size() - 1; i >= 0; i--) {

    //}
    
    int vi = t[i].first;
    int wi = t[i].second;

    unsigned long int a1;
    unsigned long int a2;

    if (i == 0)
        a1 = 0;
    else
        a1 = (a[i - 1].count(w) > 0) ? a[i - 1][w] : knapsack_top_down(a, t, i - 1, w);

    if (wi > w) 
        a2 = 0;
    else
        if (i == 0)
            a2 = vi;
        else
            a2 = (a[i - 1].count(w - wi) > 0) ? a[i - 1][w - wi] + vi: knapsack_top_down(a, t, i - 1, w - wi) + vi;

    a[i][w] = (a1 > a2) ? a1 : a2;

    return a[i][w];

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
    std::getline(fs, s);
    std::istringstream iss {s, std::ios_base::in};
    int W;
    iss >> W; 
    std::vector<std::pair<int, int>> t; 

    while (std::getline(fs, s)) {
        int value;
        int weight;
        std::istringstream iss {s, std::ios_base::in};
        iss >> value;
        iss >> weight;
        t.push_back(std::pair<int, int> {value, weight});
    }


    //position - elem number
    //value - hash with keys = needed weights, and values = computed max value
    std::vector<std::unordered_map<int, unsigned long int>> a;
    a.resize(t.size());

   // for (unsigned int row = 0; row < a.size(); row++)
   //     a[row].resize(W + 1, 0);

   // for (unsigned int i = 1; i <= t.size(); i++) {
   //     int vi = t[i - 1].first;
   //     int wi = t[i - 1].second;

   //     for (int x = 0; x <= W; x++) {
   //         int a1 = a[i - 1][x];
   //         int a2 = (wi > x) ? 0 : a[i - 1][x - wi] + vi;
   //         a[i][x] = (a1 > a2) ? a1 : a2;
   //     }
   // }

    unsigned long int max_value = knapsack_top_down(a, t, t.size() - 1, W);

    std::cout << "Input size: " << t.size() << std::endl;
    std::cout << "Optimal solutions' value: " << max_value << std::endl;

    //for (auto i : a) {
    //    for (auto j : i)
    //        std::cout << j << " ";
    //    std::cout << std::endl;
    //}

    fs.close();
}
