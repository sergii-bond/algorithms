
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
#include <set>


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
    int n;
    iss >> n;

    //from 1 to n
    std::vector<std::vector<double>> g;
    int inf = 1000000;
    g.resize(n + 1);
    
    for(auto& x : g) {
        x.resize(n + 1, inf);
    }

    std::vector<std::pair<double, double>> v_xy;
    v_xy.resize(n + 1);
    int v_id = 1;

    while (std::getline(fs, s1)) {
        double x;
        double y;
        std::istringstream iss {s1, std::ios_base::in};
        iss >> x ;
        //v1_id--;
        iss >> y ;
        //v2_id--;
        v_xy[v_id] = std::pair<double,double> {x,y};
        v_id++;
    }

    for(int i = 1; i < n + 1; i++) {
        for(int j = 1; j < n + 1; j++) {
            g[i][j] = sqrt(pow(v_xy[i].first - v_xy[j].first, 2) + pow(v_xy[i].second - v_xy[j].second, 2));
            //std::cout << g[i][j] << std::endl;
        }
    }

    //auto a1 = new std::vector<char>;
    //std::cout << a1[1] ;
    //char* pvalue  = NULL;   // Pointer initialized with null
    //pvalue  = new char[0]; // Request memory for the variable
    //pvalue[1] = 'a';

    std::unordered_map<std::vector<bool>, std::vector<double>> a;
    std::unordered_map<std::vector<bool>, std::vector<double>> a_new;
    //auto a = new std::unordered_map<std::vector<bool>, std::vector<double>>;
    //auto a_new = new std::unordered_map<std::vector<bool>, std::vector<double>>;
    //auto a_tmp = new std::unordered_map<std::vector<bool>, std::vector<double>>;
    std::vector<bool> b;
    b.resize(n - 1, 0);

    //std::vector<int>* h = new std::vector<int>(n + 1, inf); 
    std::vector<double> h;
    h.resize(n+1, inf);

    //base case
    h[1] = 0;
    //std::cout << "Size of h: " << h->size() << std::endl;

    //a.insert({b, h});
    a.insert(std::pair<std::vector<bool>, std::vector<double>>(b, h));
    //for(int i = 1; i < n + 1; i++) {
    //}

    for(int m = 1; m < n; m++) {
        std::cout << "m = " << m << std::endl;
        b.clear();
        b.resize(n - 1, 0);
        // need to start from the ascending order for the permutations to work
        for(int t = n - 2; t > n - 2 - m; t--) {
            b[t] = 1;
            //std::cout << b[t];
        }
        //std::cout << std::endl;

        //permute
        do {
            
            //h = new std::vector<int>; 
            h.clear();
            h.resize(n+1, inf);
            a_new.insert(std::pair<std::vector<bool>, std::vector<double>>(b, h));
            //need to think how to avoid this
            //here we get a new S, a set with vertices ids as keys
            std::set<int> s;
            s.clear();
            //we insert vertice 1 here, because it doens't take part in combinations
            s.insert(1);

            for(int t = 0; t < b.size(); t++) {
                if (b[t] == 1) {
                    s.insert(t + 2);
                }
            }

            //for each j e S:
            for(auto j : s) {
                if (j != 1) {
                    std::vector<bool> b_new = b;
                    b_new[j - 2] = 0;

                    double min_cost = inf;

                    for(auto k : s) {
                        if (k != j) {
                            double new_cost;

                            if (a.count(b_new) == 0) {
                                new_cost = g[k][j];
                            } else {
                                new_cost = a[b_new][k] + g[k][j];
                            }

                            if (new_cost < min_cost) {
                                min_cost = new_cost;
                            }
                        }
                    }
                    a_new[b][j] = min_cost;
                }
            }

            //for(int t = 0; t < b.size(); t++) {
            //    //j belongs to S
            //    if (b[t] == 1) {
            //        //int x = 0;
            //        //std::cout << t + 2 << " ";
            //        int j = t + 2;
            //    }
            //}

            //std::cout << std::endl;
        } while (std::next_permutation(b.begin(), b.end()));

        //a_tmp = a;
        //a->clear();
        //a = a_new;
        //a_new = a_tmp;
        std::swap(a, a_new);
        a_new.clear();
    }

    //find the shortest tour
    b.clear();
    b.resize(n - 1, 1);
    double min_tour = inf;

    for(int j = 1; j < n + 1; j++) {
        double new_tour = a[b][j] + g[j][1];
        if (new_tour < min_tour) {
            min_tour = new_tour;
        }
    }

    std::cout << "TSP solution: " << min_tour << std::endl;

    //int s = n + 1;
    //a.resize(s*s*s);
    //std::vector<std::vector<std::vector<int>>> a;
    //std::vector<std::vector<int>> a;
    //std::vector<std::vector<int>> a_k;
    //a.resize(n + 1);
    //a_k.resize(n + 1);
    //
    //for(auto& j : a) 
    //    j.resize(n + 1);

    //for(auto& j : a_k) 
    //    j.resize(n + 1);

    ////    for(auto& k : j) {
    ////        k.resize(n + 1, 0);
    ////    }
    ////}

    ////initialize a[i,j,0]
    //for(unsigned int i = 1; i < n + 1; i++) {
    //    for(unsigned int j = 1; j < n + 1; j++) {
    //        if (i == j) {
    //            //a[i][j][0] = 0;
    //            a[i][j] = 0;
    //            //a[s*s*i + s*j + 0] = 0;
    //        } else {
    //            //c_ij if edge ij belongs to G
    //            //inf otherwise
    //            //a[i][j][0] = g[i][j];
    //            a[i][j] = g[i][j];
    //            //a[s*s*i + s*j + 0] = g[i][j];
    //        }
    //    }
    //}

    ////main part
    //for(unsigned int k = 1; k < n + 1; k++) {
    //    std::cout << "k = " << k << std::endl;
    //    for(unsigned int i = 1; i < n + 1; i++) 
    //        for(unsigned int j = 1; j < n + 1; j++) {
    //            //a[i][j][k] = std::min(a[i][j][k - 1], a[i][k][k - 1] + a[k][j][k - 1]);
    //            //a[i][j][k] = (a[i][j][k - 1] < a[i][k][k - 1] + a[k][j][k - 1]) ? a[i][j][k - 1] : a[i][k][k - 1] + a[k][j][k - 1];
    //            a_k[i][j] = (a[i][j] < a[i][k] + a[k][j]) ? a[i][j] : a[i][k] + a[k][j];
    //            //int a_ij = a[s*s*i + s*j + k-1];
    //            //int a_ik = a[s*s*i + s*k + k-1];
    //            //int a_kj = a[s*s*k + s*j + k-1];
    //            //a[s*s*i + s*j + k] = (a_ij < a_ik + a_kj) ? a_ij : a_ik + a_kj;
    //        }

    //    for(unsigned int i = 1; i < n + 1; i++) 
    //        for(unsigned int j = 1; j < n + 1; j++) 
    //            a[i][j] = a_k[i][j];

    //}

    ////report negative cycles if any
    //bool neg_cycles = false;
    //for(unsigned int i = 1; i < n + 1; i++) 
    //    //if (a[i][i][n] < 0)
    //    if (a[i][i] < 0)
    //    //if (a[s*s*i + s*i + n] < 0)
    //        neg_cycles = true;

    //if (neg_cycles == true) {
    //    std::cout << "Has negative cycles" << std::endl;
    //} else {
    //    std::cout << "Has NO negative cycles" << std::endl;

    //    //int shortest = a[1][2][n];
    //    int shortest = a[1][2];
    //    //int shortest = a[s*s*1 + s*2 + n];
    //    for(unsigned int i = 1; i < n + 1; i++) 
    //        for(unsigned int j = 1; j < n + 1; j++) 
    //            //if (a[i][j][n] < shortest && i != j)
    //            //    shortest = a[i][j][n];
    //            if (a[i][j] < shortest && i != j)
    //                shortest = a[i][j];
    //            //if (a[s*s*i + s*j +n] < shortest && i != j)
    //            //    shortest = a[s*s*i + s*j +n];

    //    std::cout << "Shortest shortest path's value: " << shortest << std::endl;

    //}

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
