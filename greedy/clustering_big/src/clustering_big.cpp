
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
#include <bitset>
#include <chrono>
#include <cmath>


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


constexpr int nbits {24};

class NBitNodes {
    private:
        std::vector<std::bitset<nbits>> v;

    public:
        NBitNodes() {};

        void add_node(std::string s) {
            std::bitset<nbits> bs {s};
            v.push_back(bs);
        }

        std::string see_node(unsigned long node_id) {
            return v[node_id].to_string();
        }

        int bit_diff(unsigned long node1_id, unsigned long node2_id) {
            std::bitset<nbits> bs {v[node1_id]};
            return (bs^=v[node2_id]).count();
        }

        int bit_diff_interval(unsigned long node1_id, unsigned long node2_id, int lsb, int msb) {
            return (((v[node1_id] >> lsb) << lsb + nbits - 1 - msb ) ^= ((v[node2_id] >> lsb) << lsb + nbits - 1 - msb)).count();
        }

        int bit_count_interval(unsigned long node_id, int lsb, int msb) const {
            return (((v[node_id] >> lsb) << lsb + nbits - 1 - msb )).count();
        }

        unsigned long size() const {
            return v.size();
        }

        //int bit_count_interval(unsigned long node_id, int lsb, int msb) const {
        //    return c[node_id][0];
        //}

        //int bit_count(unsigned long node_id, int div2) {
        //}
};

struct NodeId_Count {
    unsigned long node_id;
    int count;
};

bool operator<(const NodeId_Count& n1, const NodeId_Count& n2) {
    return (n1.count > n2.count);
}

unsigned long cluster(NBitNodes& nodes, std::vector<unsigned long>& node_ids, int lsb, int msb) {

    UnionFind uf {nodes.size()};
    std::cout << "Initial number of clusters: " << uf.clusters_number() << std::endl;

    std::cout << "msb = " << msb << " lsb = " << lsb << std::endl;

    std::vector<NodeId_Count> counts;

    std::cout << "     counting started, " << "node_ids size: " << node_ids.size() << std::endl;

    for (unsigned long i = 0; i < node_ids.size(); i++) {
      int bit_count = nodes.bit_count_interval(node_ids[i], lsb, msb);
      counts.push_back(NodeId_Count {node_ids[i], bit_count});
    }

    std::cout << "     counting completed" << std::endl;

    std::cout << "     sorting started" << std::endl;
    std::sort(counts.begin(), counts.end());
    std::cout << "     sorting completed" << std::endl;

    //process the sorted array
    std::vector<std::vector<unsigned long>*> groups; 
    //std::vector<unsigned long> group_i ;
    std::priority_queue<Edge> q;
    int c_old = counts[0].count;
    //            std::cout << nodes.see_node(counts[0].node_id) << std::endl; 
    //            std::cout << nodes.see_node(counts[1].node_id) << std::endl; 

    for (unsigned long i = 0; i < counts.size(); i++) {
        int c = counts[i].count;
        int low_c = c - 2;
        unsigned long v1_id = counts[i].node_id;
        //if (i == 0)
        //    std::cout << "Node1 outside: " << nodes.see_node(v1_id) << std::endl; 

        if (c < c_old) {
            std::cout << "    c: " << c << std::endl;
            c_old = c;
        }

        for (unsigned long j = i + 1; j < counts.size(); j++) {
            //if (i == 0 and j == 1) 
            //    std::cout << "Node1 inside beg: " << nodes.see_node(v1_id) << std::endl; 
            if (counts[j].count < low_c)
                break;

            unsigned long v2_id = counts[j].node_id;
            int diff = nodes.bit_diff(v1_id, v2_id);
            //if (i == 0 and j == 1) 
            //    std::cout << "Node1 inside after diff: " << nodes.see_node(v1_id) << std::endl; 

            //if (i == 0 and j == 1) {
            //    std::cout << nodes.see_node(v1_id) << std::endl; 
            //    std::cout << nodes.see_node(v2_id) << std::endl; 
            //    std::cout << diff << std::endl; 
            //}

            if (diff <= 2) {
                q.push(Edge {v1_id, v2_id, diff});
            }

        }
    }

    int t = 0;

    //std::cout << uf.find(1) << std::endl;

    while (q.size() > 0) {
        //fetch minimum cost edge 
        Edge min_edge = q.top();
        q.pop();
        unsigned long u = min_edge.vertex1();
        unsigned long v = min_edge.vertex2();
        //max_spacing = min_edge.cost();

        if (uf.find(u) != uf.find(v)) {
            //if (uf.clusters_number() == k)
            //    break;
            uf.unite(uf.find(u), uf.find(v));
            t++;
            //std::cout << "Max spacing: " << max_spacing << std::endl;
        }
    } 

    std::cout << "Merges done: " << t << std::endl;
    std::cout << "Final number of clusters: " << uf.clusters_number() << std::endl;

    return uf.clusters_number();
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
    unsigned long int n;
    iss >> n;
    //UnionFind uf {n};
    //std::priority_queue<Edge> q;
    NBitNodes nodes;
    int msb = 23;
    int lsb = 0;

    while (std::getline(fs, s)) {
        std::string s1 {};

        for (unsigned int i = 0; i < s.size(); i++) {
            if (s[i] == '0' or s[i] == '1') 
                s1 += s[i];
        }
        nodes.add_node(s1);
    }

    std::vector<unsigned long> g;
    g.resize(n);

    for (unsigned long i = 0; i < g.size(); i ++) {
        g[i] = i;
    }

    //std::vector<std::vector<unsigned long>*> clusters;
    unsigned long clusters;

    auto t1 = std::chrono::system_clock::now();
    //std::cout << "Procedure Started at: " << t1 << std::endl ;
    clusters = cluster(nodes, g, lsb, msb) ;
    //clusters = cluster(nodes, g, 0, 23) ;
    auto t2 = std::chrono::system_clock::now();
    std::cout << "Number of clusters: " << clusters << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::seconds>(t2-t1).count() << " s" << std::endl;

    //std::sort(g.begin(), g.end(), [&nodes, lsb, msb](const unsigned long n1, const unsigned long n2) {if (nodes.bit_count_interval(n1, lsb, msb) < nodes.bit_count_interval(n2, lsb, msb)) return false; else return true;}) ;

    //for (unsigned long i = 0; i < 10; i++) {
    //    std::cout << nodes.bit_count_interval(g[i], lsb, msb)<< std::endl;
    //    //std::cout << nodes.see_node(i)<< std::endl;
    //    //std::cout << nodes.bit_count_interval(i, lsb, msb)<< std::endl;
    //}

    //std::cout << nodes.bit_diff(0, 1) << std::endl;
    //std::cout << nodes.bit_diff_interval(0, 1, 3, 5) << std::endl;

    //while (uf.clusters_number() >= k) {
    //    //fetch minimum cost edge 
    //    Edge min_edge = q.top();
    //    q.pop();
    //    unsigned long u = min_edge.vertex1();
    //    unsigned long v = min_edge.vertex2();
    //    max_spacing = min_edge.cost();

    //    if (uf.find(u) != uf.find(v)) {
    //        if (uf.clusters_number() == k)
    //            break;
    //        uf.unite(uf.find(u), uf.find(v));
    //        t++;
    //        //std::cout << "Max spacing: " << max_spacing << std::endl;
    //    }
    //} 

    //std::cout << "Times Merged: " << t << std::endl;
    //std::cout << "Number of clusters: " << uf.clusters_number() << std::endl;
    //std::cout << "Max spacing: " << max_spacing << std::endl;


    fs.close();
}
