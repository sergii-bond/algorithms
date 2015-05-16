
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

class UnionFind {
    private:
        std::vector<unsigned long int> v;
        std::vector<std::list<unsigned long>*> clusters;
        //int max_spacing;
        unsigned long int num_of_clusters;

    public:
        UnionFind(unsigned long num_of_nodes) {
            v.resize(num_of_nodes);
            clusters.resize(num_of_nodes);

            for (unsigned long int i = 0; i < v.size(); i++) {
                v[i] = i; 
                //std::list<unsigned long>* fl = new std::list<unsigned long> {i};
                clusters[i] = new std::list<unsigned long> {i}; 
            }

            num_of_clusters = num_of_nodes;
        };

        unsigned long int find(unsigned long int node) const {
            return v[node];
        }

        void unite(unsigned long cluster1, unsigned long cluster2) {
            //std::unique_ptr<std::list<unsigned long>> old_cluster;
            std::list<unsigned long>* old_cluster;
            unsigned long parent_id;

            if (clusters[cluster1]->size() < clusters[cluster2]->size()) {
                old_cluster = clusters[cluster1];
                //std::list<unsigned long>& old_cluster {clusters[cluster1]};
                parent_id = cluster2;
            } else {
                old_cluster = clusters[cluster2];
                parent_id = cluster1;
            }


            for (auto node : *old_cluster) {
                v[node] = parent_id; 
            }

            clusters[parent_id]->merge(*old_cluster);

            num_of_clusters--; 
        }

        unsigned long clusters_number() {
            return num_of_clusters;
        }

        void print_clusters() const {
            for(auto x : clusters) {
                std::cout << "cluster :";
                auto p = x->begin();
                while(p != x->end()) {
                    std::cout << " " << *p << " (" << v[*p] << ") " ;
                    p++;
                }
                    //std::cout << " " << *x->begin();
                std::cout << std::endl;
            }
        }

};

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
    unsigned long int n;
    iss >> n;
    UnionFind uf {n};
    std::priority_queue<Edge> q;

    while (std::getline(fs, s)) {
        unsigned long v1_id;
        unsigned long v2_id;
        int c;
        std::istringstream iss {s, std::ios_base::in};
        iss >> v1_id ;
        v1_id--;
        iss >> v2_id ;
        v2_id--;
        iss >> c;

        q.push(Edge {v1_id, v2_id, c});
    }

    unsigned long int k = 4;
    int max_spacing = 0;
    int t = 0;

    //std::cout << uf.find(1) << std::endl;

    while (uf.clusters_number() >= k) {
        //fetch minimum cost edge 
        Edge min_edge = q.top();
        q.pop();
        unsigned long u = min_edge.vertex1();
        unsigned long v = min_edge.vertex2();
        max_spacing = min_edge.cost();

        if (uf.find(u) != uf.find(v)) {
            uf.print_clusters();
            if (uf.clusters_number() == k) {
                std::cout << "Max spacing: " << max_spacing << std::endl;
                break;
            }
            uf.unite(uf.find(u), uf.find(v));
            std::cout << "Merged: " << u << " and " << v << std::endl;
            t++;
            //std::cout << "Max spacing: " << max_spacing << std::endl;
        }
    } 

    std::cout << "Times Merged: " << t << std::endl;
    std::cout << "Number of clusters: " << uf.clusters_number() << std::endl;
    std::cout << "Max spacing: " << max_spacing << std::endl;


    fs.close();
}
