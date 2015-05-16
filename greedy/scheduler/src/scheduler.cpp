
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>


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

class Task {
    private:
        int w; // weight (priority) of a task
        int l; // length of a task (time)
        float c; // cost = w / l

    public:
        Task(int weight, int length) : w{weight}, l{length} {
            c = float(w) / float(l);
            };


        int get_weight() const {
            return w;
        }

        int get_length() const {
            return l;
        }

        float get_cost() const {
            return c;
        }
};

class Task1 {
    private:
        int w; // weight (priority) of a task
        int l; // length of a task (time)
        float c; // cost = w / l

    public:
        Task1(int weight, int length) : w{weight}, l{length} {
            c = float(w) - float(l);
            };


        int get_weight() const {
            return w;
        }

        int get_length() const {
            return l;
        }

        float get_cost() const {
            return c;
        }
};

bool operator<(const Task& task1, const Task& task2) {
    if (task1.get_cost() < task2.get_cost())
        return true;
    else
        return false;
}

bool operator<(const Task1& task1, const Task1& task2) {
    if (task1.get_cost() < task2.get_cost())
        return true;
    else if (task1.get_cost() == task2.get_cost()) {
        if (task1.get_weight() < task2.get_weight())
            return true;
        else
            return false;

    }
    else
        return false;
}

long int wsum(std::vector<Task>& t) {
    long int sum = 0;
    int ct = 0;

    for (auto x : t) {
        ct += x.get_length();
        sum += x.get_weight() * ct ;
    }

    return sum;
}

long int wsum(std::vector<Task1>& t) {
    long int sum = 0;
    int ct = 0;

    for (auto x : t) {
        ct += x.get_length();
        sum += x.get_weight() * ct ;
    }

    return sum;
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
    //std::cout << s << std::endl;
    std::vector<Task> t;
    std::vector<Task1> t1;

    while (std::getline(fs, s)) {
        int w, l;
        std::istringstream iss {s, std::ios_base::in};
        iss >> w;
        iss >> l;
        Task task_i {w, l}; 
        Task1 task_i1 {w, l}; 
        t.push_back(task_i);
        t1.push_back(task_i1);
    }

    std::sort(t.begin(), t.end(), [](const Task& t1, const Task& t2) {if (t1 < t2) return false; else return true;}) ;

    std::sort(t1.begin(), t1.end(), [](const Task1& t1, const Task1& t2) {if (t1 < t2) return false; else return true;}) ;

    //for (unsigned int i = 0; i < t.size(); i++) {
    //    std::cout << t[i].get_weight() << " " << t[i].get_length() << " " << t[i].get_cost() << std::endl;
    //}
    std::cout << "Minimum weighted sum of completion times: " << wsum(t1) << std::endl;
    std::cout << "Optimal Minimum weighted sum of completion times: " << wsum(t) << std::endl;
    //std::cout << "Size of int" << " " << sizeof(int) << std::endl ;
    //std::cout << "Size of long int" << " " << sizeof(long int) << std::endl ;
    //[&t]() { if (t[2] < t[1]) return true; else return false;} ;
    //if (t[1] < t[2]) 
    //    std::cout << "true" ;


    fs.close();
}
