#include<bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <climits>
#include <ctime>
#include <cstdlib>
#include <iomanip>

using namespace std;

const int MAX = 11;

struct Road {
    int dist;
    float time;
    float traffic;
};

vector<pair<int, Road>> adjList[MAX];
string names[MAX] = {
    "Central Delhi", "East Delhi", "New Delhi", "North Delhi", "North East Delhi",
    "North West Delhi", "Shahdara", "South Delhi", "South East Delhi",
    "South West Delhi", "West Delhi"
};
int n = 11;
float speed = 32.0;

void addEdge(int u, int v, int dist, float time, float traffic) {
    Road r = {dist, time, traffic};
    adjList[u].push_back({v, r});
    adjList[v].push_back({u, r});
}

float simulateTraffic() {
    return (rand() % 5) + 1;
}

void updateSpeed() {
    cout << "\nCurrent speed: " << speed << " km/h";
    cout << "\nEnter new speed (km/h): ";
    cin >> speed;
    cout << "Speed updated to " << speed << " km/h\n";
}

void shortestDistance() {
    int src, dest;
    cout << "\nEnter source and destination indices (0 to " << n - 1 << "): ";
    cin >> src >> dest;
    vector<int> dist(n, INT_MAX), parent(n, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        for (auto& [v, road] : adjList[u]) {
            if (dist[u] + road.dist < dist[v]) {
                dist[v] = dist[u] + road.dist;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[dest] == INT_MAX) {
        cout << "No path found.\n";
        return;
    }

    vector<int> path;
    for (int v = dest; v != -1; v = parent[v])
        path.push_back(v);

    cout << "Shortest distance: " << dist[dest] << " km\nPath: ";
    for (int i = path.size() - 1; i >= 0; --i)
        cout << names[path[i]] << (i ? " -> " : "\n");
}

void fullTour(int start) {
    vector<bool> visited(n, false);
    vector<int> path;
    int curr = start, totalDist = 0;
    visited[curr] = true;
    path.push_back(curr);

    for (int count = 1; count < n; count++) {
        int next = -1, minDist = INT_MAX;
        for (auto& [v, road] : adjList[curr]) {
            if (!visited[v] && road.dist < minDist) {
                minDist = road.dist;
                next = v;
            }
        }
        if (next != -1) {
            visited[next] = true;
            path.push_back(next);
            totalDist += minDist;
            curr = next;
        }
    }

    cout << "Minimum tour path: ";
    for (int id : path)
        cout << names[id] << " -> ";
    cout << names[start] << "\nTotal Distance: " << totalDist + adjList[curr][0].second.dist << " km\n";

    ofstream fout("last_trip.txt");
    for (int id : path) fout << names[id] << " ";
    fout << names[start];
    fout.close();
}

void displayHeader() {
    cout << "\n******** TripTrekker ********\n";
    for (int i = 0; i < n; ++i)
        cout << i << ". " << names[i] << "\n";
}

void handleTourPlanning() {
    cout << "\n--- Full City Tour ---\n";
    fullTour(0);
}

void handleUpdateSpeed() {
    updateSpeed();
}

void handleShortestDistance() {
    shortestDistance();
}

void handleMinimumDistanceTour() {
    int source;
    cout << "\nEnter source district (0 to " << n - 1 << "): ";
    cin >> source;
    if (source < 0 || source >= n) {
        cout << "Invalid source.\n";
        return;
    }
    fullTour(source);
}

void handleFastestPath() {
    cout << "\n--- Fastest Path (Simulated Traffic) ---\n";
    int src, dest;
    cout << "Enter source and destination: ";
    cin >> src >> dest;

    vector<float> time(n, INT_MAX);
    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<>> pq;
    vector<int> parent(n, -1);
    time[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [t, u] = pq.top(); pq.pop();
        for (auto& [v, road] : adjList[u]) {
            float simulated = road.time + simulateTraffic();
            if (time[u] + simulated < time[v]) {
                time[v] = time[u] + simulated;
                parent[v] = u;
                pq.push({time[v], v});
            }
        }
    }

    if (time[dest] == INT_MAX) {
        cout << "No path found.\n";
        return;
    }

    vector<int> path;
    for (int v = dest; v != -1; v = parent[v])
        path.push_back(v);

    cout << "Fastest Path (with traffic): ";
    for (int i = path.size() - 1; i >= 0; --i)
        cout << names[path[i]] << (i ? " -> " : "\n");
    cout << "Estimated Time: " << time[dest] << " hrs\n";
}

struct Activity {
    string name;
    int start, end;
};

void handleDayPlanner() {
    vector<Activity> activities = {
        {"Fireworks show", 21, 22},
        {"CD Exhibition", 14, 16},
        {"Monuments Tour", 11, 14},
        {"PQ Show", 13, 15},
        {"Birdwatching", 5, 7}
    };

    cout << "\nChoose where you're spending a whole day:\n";
    for (int i = 0; i < n; i++)
        cout << setw(2) << i << ". " << names[i] << "\n";

    int district;
    cout << "\nEnter your source district (no.): ";
    cin >> district;

    cout << "\nAvailable activities in " << names[district] << ": (Enter 1 if you want to participate and 0 if not)\n\n";
    vector<int> selected;
    for (auto& act : activities) {
        cout << setw(20) << left << act.name << ": " << act.start << ":00 to " << act.end << ":00\n";
        int choice;
        cin >> choice;
        selected.push_back(choice);
    }

    vector<Activity> chosen;
    for (int i = 0; i < activities.size(); i++) {
        if (selected[i]) chosen.push_back(activities[i]);
    }

    sort(chosen.begin(), chosen.end(), [](Activity a, Activity b) {
        return a.end < b.end;
    });

    vector<Activity> schedule;
    int lastEnd = -1;
    for (auto& act : chosen) {
        if (act.start >= lastEnd) {
            schedule.push_back(act);
            lastEnd = act.end;
        }
    }

    cout << "\nFor participating in max. no. of desired activities,\n";
    cout << "we recommend following the following schedule:\n\n";
    cout << left << setw(25) << "Name" << setw(20) << "Start time" << "End time\n";
    for (auto& act : schedule)
        cout << setw(25) << act.name << setw(20) << (to_string(act.start) + ":00") << act.end << ":00\n";

    cout << "\nMax. no. of activities: " << schedule.size() << "\n";
}

void menu() {
    int ch;
    while (true) {
        displayHeader();
        cout << "\nMenu Options:\n";
        cout << "1. Plan a full city tour\n";
        cout << "2. Update speed (Current: " << speed << " km/hr)\n";
        cout << "3. Shortest distance between 2 districts\n";
        cout << "4. Minimum distance tour from user input\n";
        cout << "5. Fastest Path (Simulated Traffic)\n";
        cout << "6. Full day planner\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> ch;
        switch (ch) {
            case 1: handleTourPlanning(); break;
            case 2: handleUpdateSpeed(); break;
            case 3: handleShortestDistance(); break;
            case 4: handleMinimumDistanceTour(); break;
            case 5: handleFastestPath(); break;
            case 6: handleDayPlanner(); break;
            case 7: exit(0);
            default: cout << "Invalid choice.\n";
        }
    }
}

int main() {
    srand(time(0));
    addEdge(0, 1, 10, 0.25, simulateTraffic());
    addEdge(1, 2, 15, 0.375, simulateTraffic());
    addEdge(2, 3, 20, 0.5, simulateTraffic());
    addEdge(3, 4, 18, 0.45, simulateTraffic());
    addEdge(4, 5, 12, 0.3, simulateTraffic());
    addEdge(5, 6, 25, 0.625, simulateTraffic());
    addEdge(6, 7, 10, 0.25, simulateTraffic());
    addEdge(7, 8, 14, 0.35, simulateTraffic());
    addEdge(8, 9, 22, 0.55, simulateTraffic());
    addEdge(9, 10, 17, 0.425, simulateTraffic());

    menu();
    return 0;
}
