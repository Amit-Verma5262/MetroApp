#include <bits/stdc++.h>
using namespace std;

class Graph_M {
public:
    class Vertex {
    public:
        unordered_map<string, int> nbrs;
    };

    unordered_map<string, Vertex> vtces;

    int numVertex() {
        return vtces.size();
    }

    bool containsVertex(const string &vname) {
        return vtces.find(vname) != vtces.end();
    }

    void addVertex(const string &vname) {
        Vertex vtx;
        vtces[vname] = vtx;
    }

    void removeVertex(const string &vname) {
        Vertex &vtx = vtces[vname];
        vector<string> keys;
        for (auto &nbr : vtx.nbrs) {
            keys.push_back(nbr.first);
        }

        for (auto &key : keys) {
            Vertex &nbrVtx = vtces[key];
            nbrVtx.nbrs.erase(vname);
        }

        vtces.erase(vname);
    }

    int numEdges() {
        int count = 0;
        for (auto &vtx : vtces) {
            count += vtx.second.nbrs.size();
        }
        return count / 2;
    }

    bool containsEdge(const string &vname1, const string &vname2) {
        if (!containsVertex(vname1) || !containsVertex(vname2)) {
            return false;
        }
        return vtces[vname1].nbrs.find(vname2) != vtces[vname1].nbrs.end();
    }

    void addEdge(const string &vname1, const string &vname2, int value) {
        if (!containsVertex(vname1) || !containsVertex(vname2)) {
            return;
        }
        vtces[vname1].nbrs[vname2] = value;
        vtces[vname2].nbrs[vname1] = value;
    }

    void removeEdge(const string &vname1, const string &vname2) {
        if (!containsEdge(vname1, vname2)) {
            return;
        }
        vtces[vname1].nbrs.erase(vname2);
        vtces[vname2].nbrs.erase(vname1);
    }

    void display_Map() {
        cout << "\t Delhi Metro Map" << endl;
        cout << "\t------------------" << endl;
        cout << "----------------------------------------------------\n" << endl;

        for (auto &vtx : vtces) {
            cout << vtx.first << " =>\n";
            for (auto &nbr : vtx.second.nbrs) {
                cout << "\t" << nbr.first << "\t" << nbr.second << "\n";
            }
        }

        cout << "\t------------------" << endl;
        cout << "---------------------------------------------------\n" << endl;
    }

    void display_Stations() {
        cout << "\n***********************************************************************\n" << endl;
        int i = 1;
        for (auto &vtx : vtces) {
            cout << i << ". " << vtx.first << endl;
            i++;
        }
        cout << "\n***********************************************************************\n" << endl;
    }

    bool hasPath(const string &vname1, const string &vname2, unordered_map<string, bool> &processed) {
        if (containsEdge(vname1, vname2)) {
            return true;
        }

        processed[vname1] = true;
        for (auto &nbr : vtces[vname1].nbrs) {
            if (processed.find(nbr.first) == processed.end()) {
                if (hasPath(nbr.first, vname2, processed)) {
                    return true;
                }
            }
        }

        return false;
    }

    class DijkstraPair {
    public:
        string vname;
        string psf;
        int cost;

        bool operator<(const DijkstraPair &other) const {
            return cost > other.cost;
        }
    };

    int dijkstra(const string &src, const string &des, bool nan) {
        int val = 0;
        vector<string> ans;
        unordered_map<string, DijkstraPair> map;

        priority_queue<DijkstraPair> heap;

        for (auto &key : vtces) {
            DijkstraPair np;
            np.vname = key.first;
            np.cost = INT_MAX;

            if (key.first == src) {
                np.cost = 0;
                np.psf = key.first;
            }

            heap.push(np);
            map[key.first] = np;
        }

        while (!heap.empty()) {
            DijkstraPair rp = heap.top();
            heap.pop();

            if (rp.vname == des) {
                val = rp.cost;
                break;
            }

            map.erase(rp.vname);
            ans.push_back(rp.vname);

            for (auto &nbr : vtces[rp.vname].nbrs) {
                if (map.find(nbr.first) != map.end()) {
                    int oc = map[nbr.first].cost;
                    int nc;
                    if (nan) {
                        nc = rp.cost + 120 + 40 * nbr.second;
                    } else {
                        nc = rp.cost + nbr.second;
                    }

                    if (nc < oc) {
                        DijkstraPair gp = map[nbr.first];
                        gp.psf = rp.psf + nbr.first;
                        gp.cost = nc;

                        map[nbr.first] = gp;
                        heap.push(gp);
                    }
                }
            }
        }

        return val;
    }

    class Pair {
    public:
        string vname;
        string psf;
        int min_dis;
        int min_time;
    };

    string Get_Minimum_Distance(const string &src, const string &dst) {
        int min = INT_MAX;
        string ans = "";
        unordered_map<string, bool> processed;
        deque<Pair> stack;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;

        stack.push_front(sp);

        while (!stack.empty()) {
            Pair rp = stack.front();
            stack.pop_front();

            if (processed.find(rp.vname) != processed.end()) {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst) {
                if (rp.min_dis < min) {
                    ans = rp.psf;
                    min = rp.min_dis;
                }
                continue;
            }

            for (auto &nbr : vtces[rp.vname].nbrs) {
                if (processed.find(nbr.first) == processed.end()) {
                    Pair np;
                    np.vname = nbr.first;
                    np.psf = rp.psf + nbr.first + "  ";
                    np.min_dis = rp.min_dis + nbr.second;
                    stack.push_front(np);
                }
            }
        }

        ans += to_string(min);
        return ans;
    }

    string Get_Minimum_Time(const string &src, const string &dst) {
        int min = INT_MAX;
        string ans = "";
        unordered_map<string, bool> processed;
        deque<Pair> stack;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;

        stack.push_front(sp);

        while (!stack.empty()) {
            Pair rp = stack.front();
            stack.pop_front();

            if (processed.find(rp.vname) != processed.end()) {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst) {
                if (rp.min_time < min) {
                    ans = rp.psf;
                    min = rp.min_time;
                }
                continue;
            }

            for (auto &nbr : vtces[rp.vname].nbrs) {
                if (processed.find(nbr.first) == processed.end()) {
                    Pair np;
                    np.vname = nbr.first;
                    np.psf = rp.psf + nbr.first + "  ";
                    np.min_time = rp.min_time + 120 + 40 * nbr.second;
                    stack.push_front(np);
                }
            }
        }

        double minutes = ceil((double)min / 60);
        ans += to_string(minutes);
        return ans;
    }

    vector<string> get_Interchanges(const string &str) {
        vector<string> arr;
        stringstream ss(str);
        string res;
        vector<string> tokens;
        while (getline(ss, res, ' ')) {
            if (!res.empty()) {
                tokens.push_back(res);
            }
        }

        arr.push_back(tokens[0]);
        int count = 0;
        for (size_t i = 1; i < tokens.size() - 1; i++) {
            size_t index = tokens[i].find('~');
            string s = tokens[i].substr(index + 1);

            if (s.length() == 2) {
                string prev = tokens[i - 1].substr(tokens[i - 1].find('~') + 1);
                string next = tokens[i + 1].substr(tokens[i + 1].find('~') + 1);

                if (prev == next) {
                    arr.push_back(tokens[i]);
                } else {
                    arr.push_back(tokens[i] + " ==> " + tokens[i + 1]);
                    i++;
                    count++;
                }
            } else {
                arr.push_back(tokens[i]);
            }
        }

        arr.push_back(to_string(count));
        return arr;
    }
};

int main() {
    Graph_M g = Graph_M();

    // Adding stations (vertices)
    g.addVertex("Noida Sector 62");
    g.addVertex("Botanical Garden");
    g.addVertex("Yamuna Bank");
    g.addVertex("Rajiv Chowk");
    g.addVertex("Vaishali");
    g.addVertex("Moti Nagar");
    g.addVertex("Huda City Centre");
    g.addVertex("Saket");
    g.addVertex("AIIMS");
    g.addVertex("Mundka");
    g.addVertex("Bikaji Cama Place");
    g.addVertex("Janak Puri West");
    g.addVertex("Rajouri Garden");
    g.addVertex("Kashmere Gate");
    g.addVertex("Anand Vihar ISBT");
    g.addVertex("Tis Hazari");
    g.addVertex("Rithala");

    // Adding edges (distances)
    g.addEdge("Noida Sector 62", "Botanical Garden", 8);
    g.addEdge("Botanical Garden", "Yamuna Bank", 10);
    g.addEdge("Yamuna Bank", "Rajiv Chowk", 6);
    g.addEdge("Rajiv Chowk", "Vaishali", 7);
    g.addEdge("Rajiv Chowk", "Moti Nagar", 11);
    g.addEdge("Moti Nagar", "Huda City Centre", 12);
    g.addEdge("Huda City Centre", "Saket", 10);
    g.addEdge("Saket", "AIIMS", 8);
    g.addEdge("AIIMS", "Bikaji Cama Place", 6);
    g.addEdge("Bikaji Cama Place", "Mundka", 13);
    g.addEdge("Mundka", "Janak Puri West", 8);
    g.addEdge("Janak Puri West", "Rajouri Garden", 6);
    g.addEdge("Rajouri Garden", "Kashmere Gate", 9);
    g.addEdge("Kashmere Gate", "Anand Vihar ISBT", 7);
    g.addEdge("Anand Vihar ISBT", "Tis Hazari", 10);
    g.addEdge("Tis Hazari", "Rithala", 9);

    // User Interface
    while (true) {
        cout << "Menu:\n";
        cout << "1. List all stations\n";
        cout << "2. Show the metro map\n";
        cout << "3. Get shortest distance between two stations\n";
        cout << "4. Get shortest time between two stations\n";
        cout << "5. Get shortest path (distance-wise)\n";
        cout << "6. Get shortest path (time-wise)\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            g.display_Stations();
            break;
        case 2:
            g.display_Map();
            break;
        case 3: {
            string src, dst;
            cout << "Enter source station: ";
            cin >> src;
            cout << "Enter destination station: ";
            cin >> dst;
            int dist = g.dijkstra(src, dst, false);
            cout << "Shortest distance: " << dist << " km\n";
            break;
        }
        case 4: {
            string src, dst;
            cout << "Enter source station: ";
            cin >> src;
            cout << "Enter destination station: ";
            cin >> dst;
            int time = g.dijkstra(src, dst, true);
            cout << "Shortest time: " << ceil((double)time / 60) << " minutes\n";
            break;
        }
        case 5: {
            string src, dst;
            cout << "Enter source station: ";
            cin >> src;
            cout << "Enter destination station: ";
            cin >> dst;
            string result = g.Get_Minimum_Distance(src, dst);
            cout << "Shortest path (distance-wise): " << result << "\n";
            break;
        }
        case 6: {
            string src, dst;
            cout << "Enter source station: ";
            cin >> src;
            cout << "Enter destination station: ";
            cin >> dst;
            string result = g.Get_Minimum_Time(src, dst);
            cout << "Shortest path (time-wise): " << result << "\n";
            break;
        }
        case 7:
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
