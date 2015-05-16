
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <array>
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

class Edge {
    private:
        unsigned long int v1;
        unsigned long int v2;
        int c;
    public:
        Edge(unsigned long int vertex1, unsigned long int vertex2, int cost) : v1{vertex1}, v2{vertex2}, c{cost} {};

        int cost() const {
            return c;
        }

        unsigned long int vertex1() const {
            return v1;
        }

        unsigned long int vertex2() const {
            return v2;
        }
};

bool operator<(const Edge& e1, const Edge& e2) {
    return e1.cost() > e2.cost();
}

bool operator==(const Edge& e1, const Edge& e2) {
    return ((e1.vertex1() == e2.vertex1() and e1.vertex2() == e2.vertex2()) or (e1.vertex2() == e2.vertex1() and e1.vertex1() == e2.vertex2()) );
}

//std::array<std::array<std::array<int, 1001>, 1001>, 1001> a;
//int a[1001][1001][1001];
//int a[1001][1001];

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

    std::string s1;
    std::getline(fs, s1);
    std::istringstream iss {s1, std::ios_base::in};
    unsigned int n;
    iss >> n;

    //from 1 to n
    std::vector<std::vector<int>> g;
    int inf = 100000;
    g.resize(n + 1);
    
    for(auto& x : g) {
        x.resize(n + 1, inf);
    }

    while (std::getline(fs, s1)) {
        unsigned long v1_id;
        unsigned long v2_id;
        int c;
        std::istringstream iss {s1, std::ios_base::in};
        iss >> v1_id ;
        //v1_id--;
        iss >> v2_id ;
        //v2_id--;
        iss >> c;
        g[v1_id][v2_id] = c;
    }

    //All pairs shortest path Floyd-Warshall
    //a[i,j,k]
    //i [1,n]
    //j [1,n]
    //k [0,n]
    //int s = n + 1;
    //a.resize(s*s*s);
    //std::vector<std::vector<std::vector<int>>> a;
    std::vector<std::vector<int>> a;
    std::vector<std::vector<int>> a_k;
    a.resize(n + 1);
    a_k.resize(n + 1);
    
    for(auto& j : a) 
        j.resize(n + 1);

    for(auto& j : a_k) 
        j.resize(n + 1);

    //    for(auto& k : j) {
    //        k.resize(n + 1, 0);
    //    }
    //}

    //initialize a[i,j,0]
    for(unsigned int i = 1; i < n + 1; i++) {
        for(unsigned int j = 1; j < n + 1; j++) {
            if (i == j) {
                //a[i][j][0] = 0;
                a[i][j] = 0;
                //a[s*s*i + s*j + 0] = 0;
            } else {
                //c_ij if edge ij belongs to G
                //inf otherwise
                //a[i][j][0] = g[i][j];
                a[i][j] = g[i][j];
                //a[s*s*i + s*j + 0] = g[i][j];
            }
        }
    }

    //main part
    for(unsigned int k = 1; k < n + 1; k++) {
        std::cout << "k = " << k << std::endl;
        for(unsigned int i = 1; i < n + 1; i++) 
            for(unsigned int j = 1; j < n + 1; j++) {
                //a[i][j][k] = std::min(a[i][j][k - 1], a[i][k][k - 1] + a[k][j][k - 1]);
                //a[i][j][k] = (a[i][j][k - 1] < a[i][k][k - 1] + a[k][j][k - 1]) ? a[i][j][k - 1] : a[i][k][k - 1] + a[k][j][k - 1];
                a_k[i][j] = (a[i][j] < a[i][k] + a[k][j]) ? a[i][j] : a[i][k] + a[k][j];
                //int a_ij = a[s*s*i + s*j + k-1];
                //int a_ik = a[s*s*i + s*k + k-1];
                //int a_kj = a[s*s*k + s*j + k-1];
                //a[s*s*i + s*j + k] = (a_ij < a_ik + a_kj) ? a_ij : a_ik + a_kj;
            }

        for(unsigned int i = 1; i < n + 1; i++) 
            for(unsigned int j = 1; j < n + 1; j++) 
                a[i][j] = a_k[i][j];

    }

    //report negative cycles if any
    bool neg_cycles = false;
    for(unsigned int i = 1; i < n + 1; i++) 
        //if (a[i][i][n] < 0)
        if (a[i][i] < 0)
        //if (a[s*s*i + s*i + n] < 0)
            neg_cycles = true;

    if (neg_cycles == true) {
        std::cout << "Has negative cycles" << std::endl;
    } else {
        std::cout << "Has NO negative cycles" << std::endl;

        //int shortest = a[1][2][n];
        int shortest = a[1][2];
        //int shortest = a[s*s*1 + s*2 + n];
        for(unsigned int i = 1; i < n + 1; i++) 
            for(unsigned int j = 1; j < n + 1; j++) 
                //if (a[i][j][n] < shortest && i != j)
                //    shortest = a[i][j][n];
                if (a[i][j] < shortest && i != j)
                    shortest = a[i][j];
                //if (a[s*s*i + s*j +n] < shortest && i != j)
                //    shortest = a[s*s*i + s*j +n];

        std::cout << "Shortest shortest path's value: " << shortest << std::endl;

    }

    //std::vector<std::vector<int>> a;

    //a.resize(n + 1);
    //
    //for(auto& x : a) {
    //    x.resize(n, inf);
    //}
    ////pick source and A[0,s] = 0; A[0, v] = +inf already.
    //a[0][0] = 0;
    ////std::cout << "Has NO negative cycles" << std::endl;

    //for(unsigned long i = 1; i < a.size(); i++) {
    //    int min_e = inf;

    //    for(unsigned long v_id = 0; v_id < a[i].size(); v_id++) {
    //        for(unsigned long t = 0; t < g[v_id].size(); t++) {
    //            if (g[v_id][t] != inf) {
    //               int e = a[i - 1][t] + g[v_id][t];
    //               if (e < min_e) 
    //                   min_e = e;
    //            }
    //        }
    //        a[i][v_id] = std::min(a[i - 1][v_id], min_e);
    //    }
    //}


    //for (unsigned long vi = 0; vi < a[a.size() - 1].size(); vi++) {
    //    if (a[a.size() - 1][vi] != a[a.size() - 2][vi]) {
    //        std::cout << "Has negative cycles " << a[a.size() - 1][vi] << " " << a[a.size() - 2][vi] << std::endl;
    //        break;
    //    }
    //}

    //std::cout << "Has NO negative cycles" << std::endl;


    //for (auto i : a) {
    //    for (auto j : i)
    //        std::cout << j << " ";
    //    std::cout << std::endl;
    //}

    fs.close();
}
