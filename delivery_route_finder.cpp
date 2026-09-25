#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <limits>

using namespace std;

struct Road {
    int destination;
    int travelTime;
};

const int INF = numeric_limits<int>::max();

// Add a road between two locations
void addRoad(vector<vector<Road>>& graph,
             int first, int second, int travelTime) {

    graph[first].push_back({second, travelTime});
    graph[second].push_back({first, travelTime});
}

// Update road travel time due to traffic
void updateRoad(vector<vector<Road>>& graph,
                int first, int second, int newTime) {

    bool found = false;

    for (Road& road : graph[first]) {
        if (road.destination == second) {
            road.travelTime = newTime;
            found = true;
        }
    }

    for (Road& road : graph[second]) {
        if (road.destination == first) {
            road.travelTime = newTime;
        }
    }

    if (found) {
        cout << "Traffic update successful.\n";
    } else {
        cout << "Road not found.\n";
    }
}

// Dijkstra's shortest path algorithm
void dijkstra(const vector<vector<Road>>& graph,
              int source,
              vector<int>& distance,
              vector<int>& previous) {

    distance.assign(graph.size(), INF);
    previous.assign(graph.size(), -1);

    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;

    distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {

        int currentTime = pq.top().first;
        int currentLocation = pq.top().second;

        pq.pop();

        if (currentTime != distance[currentLocation]) {
            continue;
        }

        for (const Road& road : graph[currentLocation]) {

            int newTime = currentTime + road.travelTime;

            if (newTime < distance[road.destination]) {

                distance[road.destination] = newTime;
                previous[road.destination] = currentLocation;

                pq.push({
                    newTime,
                    road.destination
                });
            }
        }
    }
}

// Display delivery route
void displayRoute(int source,
                  int destination,
                  const vector<int>& distance,
                  const vector<int>& previous) {

    if (distance[destination] == INF) {

        cout << "Delivery location "
             << destination
             << " is unreachable.\n";

        return;
    }

    vector<int> path;
    int current = destination;

    while (current != -1) {

        path.push_back(current);
        current = previous[current];
    }

    cout << "Delivery Location " << destination
         << " | Delivery time: "
         << distance[destination]
         << " minutes | Route: ";

    for (int i = path.size() - 1; i >= 0; i--) {

        cout << path[i];

        if (i > 0) {
            cout << " -> ";
        }
    }

    cout << endl;
}

// Display routes to all delivery locations
void displayAllRoutes(const vector<vector<Road>>& graph,
                      int source,
                      const vector<int>& destinations) {

    vector<int> distance;
    vector<int> previous;

    dijkstra(
        graph,
        source,
        distance,
        previous
    );

    cout << "\n----------------------------------------\n";
    cout << "Shortest Delivery Routes from Location "
         << source << endl;
    cout << "----------------------------------------\n";

    int bestDestination = -1;
    int bestTime = INF;

    for (int destination : destinations) {

        displayRoute(
            source,
            destination,
            distance,
            previous
        );

        if (distance[destination] < bestTime) {

            bestTime = distance[destination];
            bestDestination = destination;
        }
    }

    if (bestDestination != -1) {

        cout << "\nNearest Delivery Location: "
             << bestDestination << endl;

        cout << "Minimum Delivery Time: "
             << bestTime
             << " minutes\n";

        cout << "Shortest Delivery Route: ";

        vector<int> bestPath;
        int current = bestDestination;

        while (current != -1) {

            bestPath.push_back(current);
            current = previous[current];
        }

        for (int i = bestPath.size() - 1; i >= 0; i--) {

            cout << bestPath[i];

            if (i > 0) {
                cout << " -> ";
            }
        }

        cout << endl;
    }
}

int main() {

    int numberOfLocations;
    int numberOfRoads;

    cout << "Enter number of locations: ";
    cin >> numberOfLocations;

    cout << "Enter number of roads: ";
    cin >> numberOfRoads;

    if (numberOfLocations <= 0 ||
        numberOfRoads < 0) {

        cout << "Invalid input.\n";
        return 1;
    }

    vector<vector<Road>> graph(numberOfLocations);

    cout << "\nEnter roads as:\n";
    cout << "start end travel_time\n";

    for (int i = 0; i < numberOfRoads; i++) {

        int first;
        int second;
        int travelTime;

        cin >> first >> second >> travelTime;

        if (first < 0 ||
            first >= numberOfLocations ||
            second < 0 ||
            second >= numberOfLocations ||
            travelTime <= 0) {

            cout << "Invalid road details.\n";
            return 1;
        }

        addRoad(
            graph,
            first,
            second,
            travelTime
        );
    }

    int source;
    int numberOfDestinations;

    cout << "\nEnter delivery source location: ";
    cin >> source;

    cout << "Enter number of delivery locations: ";
    cin >> numberOfDestinations;

    if (source < 0 ||
        source >= numberOfLocations ||
        numberOfDestinations <= 0) {

        cout << "Invalid source or destination count.\n";
        return 1;
    }

    vector<int> destinations(numberOfDestinations);

    cout << "Enter delivery locations: ";

    for (int& destination : destinations) {

        cin >> destination;

        if (destination < 0 ||
            destination >= numberOfLocations) {

            cout << "Invalid delivery location.\n";
            return 1;
        }
    }

    // Display initial routes
    displayAllRoutes(
        graph,
        source,
        destinations
    );

    // Traffic updates
    int numberOfUpdates;

    cout << "\nEnter number of traffic updates: ";
    cin >> numberOfUpdates;

    for (int i = 0; i < numberOfUpdates; i++) {

        int first;
        int second;
        int newTime;

        cout << "\nEnter traffic update:\n";
        cout << "start end new_travel_time\n";

        cin >> first >> second >> newTime;

        if (first < 0 ||
            first >= numberOfLocations ||
            second < 0 ||
            second >= numberOfLocations ||
            newTime <= 0) {

            cout << "Invalid traffic update.\n";
            return 1;
        }

        updateRoad(
            graph,
            first,
            second,
            newTime
        );

        cout << "\nTraffic updated."
             << " Recalculating delivery routes...\n";

        displayAllRoutes(
            graph,
            source,
            destinations
        );
    }

    return 0;
}
