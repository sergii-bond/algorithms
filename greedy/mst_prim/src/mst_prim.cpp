
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

        //bool operator<(const Edge& e1, const Edge& e2) {
        //bool operator<(const Edge& e2) {
        //    return c > e2.cost();
        //}

        int cost() const {
            return c;
        }

        int vertex1() const {
            return v1;
        }

        int vertex2() const {
            return v2;
        }
};

bool operator<(const Edge& e1, const Edge& e2) {
    return e1.cost() > e2.cost();
}

bool operator==(const Edge& e1, const Edge& e2) {
    return ((e1.vertex1() == e2.vertex1() and e1.vertex2() == e2.vertex2()) or (e1.vertex2() == e2.vertex1() and e1.vertex1() == e2.vertex2()) );
}

struct Vcost {
    unsigned long to_v;
    int c;
    Vcost(unsigned long to_vertex, int cost) : to_v{to_vertex}, c{cost} {};
};

class Vertex {
    private:
        unsigned long id;
        int v_cost;
        std::list<Vcost> adj;
    public:
        Vertex(unsigned long index) : id{index} {
            v_cost = std::numeric_limits<int>::max();
        };

        void add_edge(unsigned long to_vertex, int cost) {
            adj.push_front(Vcost(to_vertex, cost));
        }

        unsigned long index() const {
            return id;
        }

        int cost() const {
            return v_cost;
        }

        void cost(int vector_cost) {
            v_cost = vector_cost;
        }

        const std::list<Vcost>& edges() const {
            return adj;
        }
};

bool operator<(const Vertex& v1, const Vertex& v2) {
    return v1.cost() > v2.cost();
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
    //std::unordered_map<unsigned long, Vertex> v;
    std::map<unsigned long, Vertex*> v;

    while (std::getline(fs, s)) {
        unsigned long v1_id;
        unsigned long v2_id;
        int c;
        std::istringstream iss {s, std::ios_base::in};
        iss >> v1_id;
        iss >> v2_id;
        iss >> c;

        if (v.count(v1_id) == 0) {
            Vertex* v1 = new Vertex {v1_id} ;
            v[v1_id] = v1;
        }

        if (v.count(v2_id) == 0) {
            Vertex* v2 = new Vertex {v2_id} ;
            v[v2_id] = v2;
        }

        v[v1_id]->add_edge(v2_id, c);
        v[v2_id]->add_edge(v1_id, c);

        //if (v1_id >= v.size() or v2_id >= v.size()) {
        //    if (v1_id > v2_id)
        //        v.resize(v1_id + 1, Vertex {0});
        //    else
        //        v.resize(v2_id + 1, Vertex {0});
        //}

        //if (v[v1_id] == 0) {
        //    v[v1_id] = v1_id;
        //}

        //if (v[v2_id] == 0) {
        //    Vertex v2 {v2_id};
        //    v[v2_id] = v2_id;
        //}


    }

    //for (auto x : v) {
    //    for (auto e : x.second->edges()) {
    //        std::cout << x.second->index() << " " << e.to_v << "(" << e.c << ") ";
    //    }
    //    std::cout << std::endl;
    //}

    std::vector<Vertex*> h;

    for (auto x : v) {
        h.push_back(x.second);
    }

    h[0]->cost(0);
    int sum = 0;
    std::make_heap(h.begin(), h.end(), [](Vertex* v1, Vertex* v2) { return v1->cost() > v2->cost(); } );
    //std::make_heap(h.begin(), h.end());
    unsigned long k = 0; 

    while(k < h.size()) {
        Vertex& v_mst = *h[0];

        //std::cout << "costs: ";
        //for(auto x : h){
        //    std::cout << x->cost() << " "; 
        //}
        
        //std::cout << std::endl;

        std::pop_heap(h.begin(), h.end() - k, [](Vertex* v1, Vertex* v2) { return v1->cost() > v2->cost(); } );
        sum += v_mst.cost();

        for(auto n : v_mst.edges()) {
            v[n.to_v]->cost(std::min(v[n.to_v]->cost(), n.c));
        }

        k++;
        std::make_heap(h.begin(), h.end() - k, [](Vertex* v1, Vertex* v2) { return v1->cost() > v2->cost(); } );
        //std::cout << "MST cost:" << sum << std::endl;
    }

    std::cout << "MST cost:" << sum << std::endl;

    //
    //
    //std::vector<unsigned long int> v_mst;
    //v_mst.push_back(1);
    //std::priority_queue<Edge> q {};
    //long int mst_cost {0} ;

    //for (auto e: v[1]) {
    //    q.push(e);
    //}

    //while (v_mst.size() != v.size() - 1) {
    //    Edge e_min = q.top();
    //    q.pop();
    //    mst_cost += e_min.cost() ;
    //    unsigned long int v_next = e_min.vertex2() ;
    //    std::cout << "e_min: " << e_min.cost() << " next v: " << v_next;
    //    v_mst.push_back(v_next);

    //    for (auto e_i : v[v_next]) {
    //        //std::cout << "e_i cost " << e_i.cost() << " "; 
    //        bool a = false;
    //        for (auto v_mst_i : v_mst) {
    //            if (v_mst_i == e_i.vertex2()) {
    //                a = true;
    //            } 
    //            //if (v_mst_i == e_i.vertex1()) {
    //            //}
    //        }

    //        if (a == false) {
    //            std::cout << " added: " << e_i.vertex1() << "-" << e_i.vertex2() ;  
    //            q.push(e_i);
    //        }
    //    }
    //    std::cout << " v_mst + " << v_next;
    //    std::cout << std::endl;
    //}


    //std::sort(t.begin(), t.end(), [](const Task& t1, const Task& t2) {if (t1 < t2) return false; else return true;}) ;

    //std::sort(t1.begin(), t1.end(), [](const Task1& t1, const Task1& t2) {if (t1 < t2) return false; else return true;}) ;

    //for (unsigned int i = 0; i < t.size(); i++) {
    //    std::cout << t[i].get_weight() << " " << t[i].get_length() << " " << t[i].get_cost() << std::endl;
    //}
    //std::cout << "Minimum weighted sum of completion times: " << wsum(t1) << std::endl;
    //std::cout << "Optimal Minimum weighted sum of completion times: " << wsum(t) << std::endl;
    //std::cout << "Size of int" << " " << sizeof(int) << std::endl ;
    //std::cout << "Size of long int" << " " << sizeof(long int) << std::endl ;
    //[&t]() { if (t[2] < t[1]) return true; else return false;} ;
    //if (t[1] < t[2]) 
    //    std::cout << "true" ;


    fs.close();
}
