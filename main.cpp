#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <map>
#include <cfloat>
using namespace std;

const int N = 11;

// Structure for path information
struct Path {
    int distance;
    int traffic;
    float time;
        bool connected;

};

// Structure for activities
struct Activity {
    string name;
    int start;
    int end;
    float cost;
    string description;
};

// Structure for destination options
struct Destination {
    string name;
    float travelCost;
    float foodCost;
    float ticketCost;
    string bestTime;
    string description;
};

// Structure for hotel information
struct Hotel {
    string name;
    float price;
    float rating;
    string amenities;
};

class TripPlanner {
private:
    // Graph data
    Path adj[N][N];
    string districtNames[N];
    int speed;
    float minTourTime;
    Activity activities[15];  // Increased from 10 to 15
    int availableActivities[N][5];
    vector<vector<int>> allPaths;
    map<string, vector<Hotel>> hotelsByDistrict;
    vector<string> emergencyContacts;

    // Initialize adjacency matrix
       void initializeAdjMatrix() {
        int initialDistances[N][N] = {
            {0, 14, 9, 18, 10, 20, 6, 10000, 18, 10000, 19},
            {14, 0, 10000, 10000, 10000, 10000, 9, 10000, 13, 10000, 10000},
            {9, 10000, 0, 10000, 10000, 10000, 10000, 16, 10, 22, 20},
            {18, 10000, 10000, 0, 21, 22, 10000, 10000, 10000, 10000, 10000},
            {10, 10000, 10000, 21, 0, 10000, 7, 10000, 10000, 10000, 10000},
            {20, 10000, 10000, 22, 10000, 0, 10000, 10000, 10000, 10000, 9},
            {6, 9, 10000, 10000, 7, 10000, 0, 10000, 10000, 10000, 10000},
            {10000, 10000, 16, 10000, 10000, 10000, 10000, 0, 16, 10000, 10000},
            {18, 13, 10, 10000, 10000, 10000, 10000, 16, 0, 10000, 10000},
            {10000, 10000, 22, 10000, 10000, 10000, 10000, 10000, 10000, 0, 14},
            {19, 10000, 20, 10000, 10000, 9, 10000, 10000, 10000, 14, 0}
        };

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                adj[i][j].distance = initialDistances[i][j];
                adj[i][j].connected = (initialDistances[i][j] != 10000);
                adj[i][j].traffic = 0;
                adj[i][j].time = 0.0f;
            }
        }
    }

    // Initialize district names
    void initializeDistrictNames() {
        string names[N] = {
            "Central Delhi", "East Delhi", "New Delhi", "North Delhi",
            "North East Delhi", "North West Delhi", "Shahdara",
            "South Delhi", "South East Delhi", "South West Delhi", "West Delhi"
        };
        copy(names, names + N, districtNames);
    }

    // Initialize activities with more details
    void initializeActivities() {
        activities[0] = {"AB Exhibition", 10, 12, 250, "Art and culture exhibition showcasing Delhi's heritage"};
        activities[1] = {"Heritage Walk", 6, 9, 150, "Guided morning walk through Old Delhi's historic lanes"};
        activities[2] = {"Fireworks show", 21, 22, 300, "Evening fireworks display at India Gate"};
        activities[3] = {"Light show", 17, 18, 200, "Sound and light show at Red Fort"};
        activities[4] = {"CD Exhibition", 14, 16, 180, "Contemporary design exhibition at National Gallery"};
        activities[5] = {"Monuments Tour", 11, 14, 400, "Half-day tour covering Qutub Minar and Humayun's Tomb"};
        activities[6] = {"PQ Show", 13, 15, 350, "Performing arts show at Kamani Auditorium"};
        activities[7] = {"Water show", 10, 11, 100, "Musical fountain show at Nehru Park"};
        activities[8] = {"Birdwatching", 5, 7, 120, "Early morning birdwatching at Yamuna Biodiversity Park"};
        activities[9] = {"Art Gallery", 16, 17, 150, "Visit to National Gallery of Modern Art"};
        activities[10] = {"Food Tour", 19, 22, 500, "Evening street food tour in Chandni Chowk"};
        activities[11] = {"Museum Visit", 10, 13, 200, "Tour of National Museum with expert guide"};
        activities[12] = {"Yoga Session", 7, 8, 80, "Morning yoga at Lodhi Gardens"};
        activities[13] = {"Shopping Tour", 14, 18, 0, "Guided shopping tour in Connaught Place"};
        activities[14] = {"Cultural Dance", 20, 21, 250, "Traditional dance performance at Kingdom of Dreams"};
    }

    // Initialize available activities
    void initializeAvailableActivities() {
        int acts[N][5] = {
            {0,1,2,3,4}, {0,5,7,8,9}, {1,3,6,7,9}, {2,4,5,6,8}, {0,1,4,8,9},
            {0,4,5,6,7}, {2,3,4,7,8}, {1,2,4,6,9}, {0,1,3,5,7}, {5,6,7,8,9}, {3,4,6,8,9}
        };
        for (int i = 0; i < N; i++) {
            copy(acts[i], acts[i] + 5, availableActivities[i]);
        }
    }

    // Initialize hotels data
    void initializeHotels() {
        // Central Delhi hotels
        hotelsByDistrict["Central Delhi"] = {
            {"The Imperial", 8500, 4.8, "Pool, Spa, Free WiFi, Restaurant"},
            {"Hotel Palace Heights", 4500, 4.2, "Free WiFi, Restaurant, AC"},
            {"Bloomrooms @ Janpath", 3200, 3.9, "Free WiFi, Breakfast"}
        };

        // New Delhi hotels
        hotelsByDistrict["New Delhi"] = {
            {"The Lalit", 7500, 4.7, "Pool, Spa, 3 Restaurants, Bar"},
            {"Hotel Palace Heights", 4500, 4.2, "Free WiFi, Restaurant"},
            {"Hotel Grand Park Inn", 3800, 4.0, "Free WiFi, Breakfast"}
        };

        // South Delhi hotels
        hotelsByDistrict["South Delhi"] = {
            {"The Oberoi", 12000, 4.9, "Luxury Spa, Pool, Fine Dining"},
            {"The Lodhi", 15000, 4.9, "Luxury, Pool, Spa, Private Terraces"},
            {"Hotel Suryaa", 5500, 4.3, "Pool, Restaurant, Free WiFi"}
        };

        // Add more hotels for other districts...
    }

    // Initialize emergency contacts
    void initializeEmergencyContacts() {
        emergencyContacts = {
            "Police: 100",
            "Ambulance: 102",
            "Fire Brigade: 101",
            "Delhi Tourism Helpline: 1800-111-363",
            "Women's Helpline: 1091",
            "Tourist Police: +91-11-23239730"
        };
    }

public:
    TripPlanner() {
        initializeAdjMatrix();
        initializeDistrictNames();
        initializeActivities();
        initializeAvailableActivities();
        initializeHotels();
        initializeEmergencyContacts();
        speed = 32;
        minTourTime = INT_MAX;
        updateTraffic();
    }

    // Update traffic conditions randomly
    void updateTraffic() {
        srand(time(0));
        const int BASE_SPEED = 25; // Realistic Delhi speed (km/h)
        const float MIN_TIME = 0.08f; // 5 minutes minimum
        const float MAX_TIME = 2.0f; // 2 hours maximum

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (!adj[i][j].connected) continue;

                adj[i][j].traffic = rand() % 3;
                float speedMultipliers[3] = {1.0f, 0.7f, 0.4f}; // LOW, NORMAL, HIGH
                float effectiveSpeed = BASE_SPEED * speedMultipliers[adj[i][j].traffic];

                // Ensure minimum speed of 5 km/h
                effectiveSpeed = max(effectiveSpeed, 5.0f);

                adj[i][j].time = min(max(
                    adj[i][j].distance / effectiveSpeed,
                    MIN_TIME
                ), MAX_TIME);
            }
        }
    }
    // Get traffic condition as string
    string getTrafficCondition(int i, int j) {
        switch (adj[i][j].traffic) {
            case 2: return "HIGH";
            case 1: return "NORMAL";
            default: return "LOW";
        }
    }
const float MIN_SEGMENT_TIME = 0.08f; // 5 minutes minimum
const float MAX_SEGMENT_TIME = 2.0f;  // 2 hours maximum
const int NO_CONNECTION = -1;
    // Display current traffic between two districts
    void showCurrentTraffic() {
        cout << "\nDistricts:\n";
        for (int i = 0; i < N; i++) {
            cout << " " << i << ". " << districtNames[i] << endl;
        }

        int src, dest;
        while (true) {
            cout << "\nEnter source and destination district numbers (0-10): ";
            cin >> src >> dest;

            if (src >= 0 && src < N && dest >= 0 && dest < N) {
                break;
            }
            cout << "Invalid district numbers! Please try again.\n";
        }

        if (!adj[src][dest].connected) {
            cout << "\nNo direct road exists between " << districtNames[src]
                 << " and " << districtNames[dest] << "!\n";
        } else {
            string trafficLevel[3] = {"LOW", "NORMAL", "HIGH"};
            cout << "\nFrom " << districtNames[src] << " to " << districtNames[dest] << ":\n";
            cout << "Distance: " << adj[src][dest].distance << " km\n";
            cout << "Traffic: " << trafficLevel[adj[src][dest].traffic] << endl;
            cout << "Estimated time: " << fixed << setprecision(2)
                 << adj[src][dest].time << " hours ("
                 << adj[src][dest].time * 60 << " minutes)\n";
        }
        cin.get();
    }

    // Update vehicle speed
    void updateSpeed() {
        int newSpeed;
        while (true) {
            cout << "\nEnter new speed in km/h (16-50): ";
            cin >> newSpeed;

            if (newSpeed > 15 && newSpeed <= 50) {
                speed = newSpeed;
                cout << "\nSpeed updated to " << speed << " km/h\n";
                updateTraffic();
                break;
            }
            cout << "Invalid speed! Please enter between 16-50 km/h\n";
        }
        cin.get();
    }

    // Find minimum distance node for Dijkstra's algorithm
    int findMinDistanceNode(const int distances[], const bool visited[]) {
        int minDistance = INT_MAX;
        int minIndex = -1;

        for (int i = 0; i < N; i++) {
            if (!visited[i] && distances[i] <= minDistance) {
                minDistance = distances[i];
                minIndex = i;
            }
        }
        return minIndex;
    }

    // Dijkstra's algorithm for shortest path with path tracking
    void findShortestPath() {
        cout << "\nDistricts:\n";
        for (int i = 0; i < N; i++) {
            cout << " " << i << ". " << districtNames[i] << endl;
        }

        int src, dest;
        while (true) {
            cout << "\nEnter source and destination district numbers (0-10): ";
            cin >> src >> dest;

            if (src >= 0 && src < N && dest >= 0 && dest < N) {
                break;
            }
            cout << "Invalid district numbers! Please try again.\n";
        }

        int distances[N];
        bool visited[N];
        int previous[N];  // To track the path

        for (int i = 0; i < N; i++) {
            distances[i] = INT_MAX;
            visited[i] = false;
            previous[i] = -1;
        }
        distances[src] = 0;

        for (int count = 0; count < N - 1; count++) {
            int u = findMinDistanceNode(distances, visited);
            if (u == -1) break;

            visited[u] = true;

            for (int v = 0; v < N; v++) {
                if (!visited[v] && adj[u][v].distance != 10000 &&
                    distances[u] != INT_MAX &&
                    distances[u] + adj[u][v].distance < distances[v]) {
                    distances[v] = distances[u] + adj[u][v].distance;
                    previous[v] = u;
                }
            }
        }

        cout << "\nShortest path from " << districtNames[src] << " to " << districtNames[dest] << ":\n";

        if (distances[dest] != INT_MAX) {
            // Reconstruct path
            vector<int> path;
            for (int at = dest; at != -1; at = previous[at]) {
                path.push_back(at);
            }
            reverse(path.begin(), path.end());

            cout << "Total distance: " << distances[dest] << " km\n";
            cout << "Route: ";
            for (size_t i = 0; i < path.size(); i++) {
                cout << districtNames[path[i]];
                if (i != path.size() - 1) {
                    cout << " -> ";
                }
            }
            cout << endl;

            // Calculate estimated time
            float totalTime = 0;
            for (size_t i = 0; i < path.size() - 1; i++) {
                int u = path[i], v = path[i+1];
                totalTime += adj[u][v].time;
            }
            cout << "Estimated travel time: " << fixed << setprecision(2) << totalTime << " hours\n";
        } else {
            cout << "No path exists between these districts!\n";
        }
        cin.get();
    }

    // Traveling Salesman Problem implementation
    // Traveling Salesman Problem implementation with corrected time calculation
#include <cfloat>  // Add this at the top with other includes

#include <cfloat>

void findOptimalTour() {
    // Display district list
    cout << "\nDistricts:\n";
    for (int i = 0; i < N; i++) {
        cout << " " << i << ". " << districtNames[i] << endl;
    }

    // Get starting district from user
    int startDistrict;
    while (true) {
        cout << "\nEnter starting district number (0-10): ";
        cin >> startDistrict;

        if (startDistrict >= 0 && startDistrict < N) {
            break;
        }
        cout << "Invalid district number! Please try again.\n";
    }

    vector<int> path(N + 1);
    vector<bool> visited(N, false);
    int minDistance = INT_MAX;
    float minTime = FLT_MAX;

    // Start from user-selected district
    path[0] = startDistrict;
    visited[startDistrict] = true;
    tspHelper(startDistrict, 1, 0, 0.0f, minDistance, minTime, path, visited);

    cout << "\nOPTIMAL TOUR STARTING FROM " << districtNames[startDistrict] << "\n";
    cout << "============================================\n";
    cout << "Total distance: " << minDistance << " km\n";
    cout << "Total time: " << fixed << setprecision(2) << minTime << " hours\n\n";

    cout << "DETAILED ITINERARY:\n";
    cout << "------------------\n";

    float cumulativeTime = 0.0f;
    for (int i = 0; i < N; i++) {
        int u = path[i], v = path[i+1];
        cumulativeTime += adj[u][v].time;

        cout << i+1 << ". " << districtNames[u] << " to " << districtNames[v] << "\n";
        cout << "   Distance: " << adj[u][v].distance << " km\n";
        cout << "   Traffic: " << getTrafficCondition(u, v) << "\n";
        cout << "   Segment time: " << fixed << setprecision(2) << adj[u][v].time << " hours\n";
        cout << "   Cumulative time: " << fixed << setprecision(2) << cumulativeTime << " hours\n";
        cout << endl;
    }

    cin.get();
}

void tspHelper(int current, int count, int currentDistance, float currentTime,
               int& minDistance, float& minTime, vector<int>& path, vector<bool>& visited) {
    if (count == N) {
        // Return to starting point to complete the tour
        if (adj[current][path[0]].distance != 10000) {
            int totalDistance = currentDistance + adj[current][path[0]].distance;
            float totalTime = currentTime + adj[current][path[0]].time;

            if (totalDistance < minDistance ||
               (totalDistance == minDistance && totalTime < minTime)) {
                minDistance = totalDistance;
                minTime = totalTime;
                path[N] = path[0];  // Complete the cycle
            }
        }
        return;
    }

    for (int i = 0; i < N; i++) {
        if (!visited[i] && adj[current][i].distance != 10000) {
            visited[i] = true;
            path[count] = i;

            tspHelper(i, count + 1,
                     currentDistance + adj[current][i].distance,
                     currentTime + adj[current][i].time,
                     minDistance, minTime, path, visited);

            visited[i] = false;
        }
    }
}
    // Plan activities for a day in a district with cost consideration
 void planDailyActivities() {
    cout << "\nSelect a district to plan activities:\n";
    for (int i = 0; i < N; i++) {
        cout << " " << i << ". " << districtNames[i] << endl;
    }

    int district;
    while (true) {
        cout << "\nEnter district number (0-10): ";
        cin >> district;

        if (district >= 0 && district < N) {
            break;
        }
        cout << "Invalid district number! Please try again.\n";
    }

    float budget;
    cout << "\nEnter your daily activity budget (INR): ";
    cin >> budget;

    cout << "\nAvailable activities in " << districtNames[district] << " within your budget:\n";

    vector<Activity> affordableActivities;
    for (int i = 0; i < 5; i++) {
        Activity act = activities[availableActivities[district][i]];
        if (act.cost <= budget) {
            cout << "\n" << i + 1 << ". " << act.name << " (" << act.start << ":00-" << act.end << ":00)\n";
            cout << "   Cost: INR " << act.cost << "\n";
            cout << "   Description: " << act.description << "\n";

            char choice;
            cout << " Participate? (y/n): ";
            cin >> choice;

            if (tolower(choice) == 'y') {
                affordableActivities.push_back(act);
            }
        }
    }

    if (affordableActivities.empty()) {
        cout << "\nNo affordable activities selected or available. Enjoy your free day!\n";
        cin.get();
        return;
    }

    // Sort activities by end time
    sort(affordableActivities.begin(), affordableActivities.end(),
         [](const Activity& a, const Activity& b) { return a.end < b.end; });

    cout << "\nRecommended schedule for maximum activities within budget:\n";
    cout << "--------------------------------------------------------\n";

    int lastEnd = -1;
    int count = 0;
    float totalCost = 0;
    for (const auto& act : affordableActivities) {
        if (act.start >= lastEnd && (totalCost + act.cost) <= budget) {
            cout << " " << act.name << ": " << act.start << ":00-" << act.end << ":00\n";
            cout << "   Cost: INR " << act.cost << "\n";
            cout << "   Description: " << act.description << "\n\n";
            lastEnd = act.end;
            totalCost += act.cost;
            count++;
        }
    }

    cout << "\nActivities attended: " << count << endl;
    cout << "Total cost: INR " << totalCost << endl;
    if (totalCost < budget) {
        cout << "Remaining budget: INR " << (budget - totalCost) << endl;
    }
    cin.get();
}

    // Plan trip by budget with Delhi-specific destinations
    void planByBudget() {
        float budget;
        cout << "\nEnter your trip budget (INR): ";
        cin >> budget;

        vector<Destination> destinations = {
            {"Old Delhi Heritage Tour", 500, 300, 200, "Oct-Mar", "Explore Red Fort, Chandni Chowk and Jama Masjid"},
            {"New Delhi Modern Tour", 400, 500, 300, "All Year", "Visit India Gate, Rashtrapati Bhavan and Parliament"},
            {"South Delhi Cultural Tour", 600, 400, 350, "Nov-Feb", "Qutub Minar, Lotus Temple and Hauz Khas Village"},
            {"Delhi Food Crawl", 300, 800, 0, "All Year", "Street food tour across Delhi's best eateries"},
            {"Delhi Shopping Spree", 200, 200, 0, "Oct-Mar", "Shopping in Sarojini, Janpath and Connaught Place"}
        };

        bool found = false;
        cout << "\nAvailable Delhi experiences within budget:\n";
        cout << "----------------------------------------\n";

        for (const auto& dest : destinations) {
            float total = dest.travelCost + dest.foodCost + dest.ticketCost;
            if (total <= budget) {
                found = true;
                cout << "\n" << dest.name << "\n";
                cout << "  Travel: INR " << dest.travelCost << "\n";
                cout << "  Food: INR " << dest.foodCost << "\n";
                cout << "  Tickets: INR " << dest.ticketCost << "\n";
                cout << "  Total: INR " << total << "\n";
                cout << "  Best Time: " << dest.bestTime << "\n";
                cout << "  Description: " << dest.description << "\n";
            }
        }

        if (!found) {
            cout << "\nNo experiences fit your budget. Consider increasing your budget\n";
            cout << "or looking for free attractions like museums and parks.\n";
        }
        cin.get();
    }

    // Show hotel recommendations by district
    void showHotelRecommendations() {
        cout << "\nSelect a district for hotel recommendations:\n";
        for (int i = 0; i < N; i++) {
            cout << " " << i << ". " << districtNames[i] << endl;
        }

        int district;
        while (true) {
            cout << "\nEnter district number (0-10): ";
            cin >> district;

            if (district >= 0 && district < N) {
                break;
            }
            cout << "Invalid district number! Please try again.\n";
        }

        string districtName = districtNames[district];
        cout << "\nHotel Recommendations in " << districtName << ":\n";
        cout << "----------------------------------------\n";

        if (hotelsByDistrict.find(districtName) != hotelsByDistrict.end()) {
            int count = 1;
            for (const auto& hotel : hotelsByDistrict[districtName]) {
                cout << "\n" << count++ << ". " << hotel.name << "\n";
                cout << "   Price: INR " << hotel.price << " per night\n";
                cout << "   Rating: " << hotel.rating << "/5\n";
                cout << "   Amenities: " << hotel.amenities << "\n";
            }
        } else {
            cout << "\nNo hotel data available for this district.\n";
            cout << "Consider looking in nearby districts.\n";
        }
        cin.get();
    }

    // Show emergency contacts
    void showEmergencyContacts() {
        cout << "\nImportant Emergency Contacts in Delhi:\n";
        cout << "------------------------------------\n";
        for (const auto& contact : emergencyContacts) {
            cout << " * " << contact << "\n";
        }
        cout << "\nAdditional Tips:\n";
        cout << "- Save these numbers in your phone\n";
        cout << "- For medical emergencies, major hospitals are available in all districts\n";
        cout << "- Tourist police can assist with lost documents or theft cases\n";
        cin.get();
    }

    // Show weather information
    void showWeatherInfo() {
        cout << "\nDelhi Weather Information:\n";
        cout << "-------------------------\n";
        cout << "Summer (April-June):\n";
        cout << " - Hot and dry, temperatures 25°C to 45°C\n";
        cout << " - Carry sunscreen, hats and stay hydrated\n\n";

        cout << "Monsoon (July-September):\n";
        cout << " - Humid with occasional heavy rains\n";
        cout << " - Temperatures 27°C to 35°C\n";
        cout << " - Carry umbrella and waterproof footwear\n\n";

        cout << "Winter (October-March):\n";
        cout << " - Cool and foggy in December-January\n";
        cout << " - Temperatures 5°C to 22°C\n";
        cout << " - Carry warm clothing, especially for mornings and evenings\n";
        cin.get();
    }

    // Collect user feedback with more options
    void collectFeedback() {
        cout << "\nDelhi Tourism Feedback System\n";
        cout << "----------------------------\n";

        string name, email;
        cout << "\nEnter your name: ";
        cin.ignore();
        getline(cin, name);

        cout << "Enter your email: ";
        getline(cin, email);

        cout << "\nPlease rate the following aspects of your Delhi experience (1-5 stars):\n";

        vector<pair<string, int>> ratings = {
            {"Overall Satisfaction", 0},
            {"Transportation", 0},
            {"Food Options", 0},
            {"Cultural Attractions", 0},
            {"Safety", 0},
            {"Cleanliness", 0}
        };

        for (auto& item : ratings) {
            while (true) {
                cout << " " << item.first << ": ";
                cin >> item.second;

                if (item.second >= 1 && item.second <= 5) {
                    break;
                }
                cout << "Please enter a rating between 1 and 5.\n";
            }
        }

        string comments;
        cout << "\nAdditional comments/suggestions: ";
        cin.ignore();
        getline(cin, comments);

        cout << "\nThank you for your feedback, " << name << "!\n";
        cout << "We appreciate your time and will use your input to improve Delhi tourism.\n";

        cout << "\nSummary of your feedback:\n";
        for (const auto& item : ratings) {
            cout << " " << item.first << ": " << string(item.second, '*') << "\n";
        }
        cin.get();
    }

    // Generate detailed packing list
    void generatePackingList() {
        string weather, activityType;
        int days;

        cout << "\nDelhi Trip Packing List Generator\n";
        cout << "--------------------------------\n";

        cout << "\nEnter expected weather (hot/mild/cold): ";
        cin >> weather;

        cout << "Enter number of trip days: ";
        cin >> days;

        cout << "Enter main activity type (sightseeing/trekking/shopping): ";
        cin >> activityType;

        cout << "\nRecommended Packing List for Delhi:\n";
        cout << "----------------------------------\n";

        // Weather-specific items
        if (weather == "hot") {
            cout << "\nFor Hot Weather:\n";
            cout << "- Lightweight, breathable clothing\n";
            cout << "- Sunscreen (SPF 30+)\n";
            cout << "- Sunglasses with UV protection\n";
            cout << "- Wide-brimmed hat or cap\n";
            cout << "- Reusable water bottle\n";
            cout << "- Electrolyte packets\n";
        } else if (weather == "cold") {
            cout << "\nFor Cold Weather:\n";
            cout << "- Warm jacket or coat\n";
            cout << "- Gloves and scarf\n";
            cout << "- Thermal innerwear\n";
            cout << "- Layered clothing\n";
            cout << "- Moisturizer and lip balm\n";
        } else {
            cout << "\nFor Mild Weather:\n";
            cout << "- Layered clothing\n";
            cout << "- Light jacket or sweater\n";
            cout << "- Comfortable walking shoes\n";
            cout << "- Compact umbrella\n";
        }

        // Activity-specific items
        if (activityType == "trekking") {
            cout << "\nFor Trekking:\n";
            cout << "- Hiking shoes/boots\n";
            cout << "- Backpack (20-30L)\n";
            cout << "- Moisture-wicking socks\n";
            cout << "- Trekking poles (optional)\n";
            cout << "- First aid kit\n";
        } else if (activityType == "shopping") {
            cout << "\nFor Shopping:\n";
            cout << "- Foldable shopping bag\n";
            cout << "- Comfortable walking shoes\n";
            cout << "- Small backpack or tote\n";
            cout << "- Local currency in small denominations\n";
        } else {
            cout << "\nFor Sightseeing:\n";
            cout << "- Comfortable walking shoes\n";
            cout << "- Small backpack\n";
            cout << "- Camera or smartphone\n";
            cout << "- Portable charger\n";
            cout << "- Guidebook or maps\n";
        }

        // General items
        cout << "\nEssentials:\n";
        cout << "- Passport/ID copies\n";
        cout << "- Hotel reservation details\n";
        cout << "- Prescription medications\n";
        cout << "- Basic first aid kit\n";
        cout << "- Toiletries\n";
        cout << "- Clothes for " << days << " days\n";
        cout << "- Phone charger and adapter\n";
        cout << "- Local SIM card (available at airport)\n";

        cout << "\nDelhi-Specific Tips:\n";
        cout << "- Carry tissues/wet wipes (many public restrooms don't provide)\n";
        cout << "- Have small change for auto-rickshaws and street vendors\n";
        cout << "- Dress modestly when visiting religious sites\n";
        cout << "- Carry a photocopy of your passport/visa\n";

        cin.get();
    }

    // Show transport information with more details
void showTransportInfo() {
    system("CLS");
    cout << "\nDelhi Transport Information\n";
    cout << "--------------------------\n";

    cout << "\nAvailable Transport Options:\n";
    cout << "1. Delhi Metro\n";
    cout << "2. Public Buses\n";
    cout << "3. Auto Rickshaws\n";
    cout << "4. Taxis/Cabs\n";
    cout << "5. Cycle Rickshaws\n";
    cout << "6. E-Rickshaws\n";
    cout << "7. Show All Options\n";

    int choice;
    cout << "\nEnter the transport option number you want to know about (1-7): ";
    cin >> choice;

    system("CLS");
    cout << "\nDelhi Transport Information\n";
    cout << "--------------------------\n";

    switch(choice) {
        case 1:
            cout << "\n1. Delhi Metro\n";
            cout << "   - Website: https://www.delhimetrorail.com\n";
            cout << "   - Fares: INR 10-60 depending on distance\n";
            cout << "   - Operating hours: 6:00 AM - 11:00 PM\n";
            cout << "   - Tourist Tip: Get a 'Tourist Smart Card' for unlimited travel\n";
            cout << "   - Major Stations: New Delhi (Yellow Line), Rajiv Chowk (Busiest)\n";
            cout << "   - Covers most tourist attractions\n";
            cout << "   - Women-only coach available at front of each train\n";
            break;

        case 2:
            cout << "\n2. Public Buses\n";
            cout << "   - AC buses: INR 25-50\n";
            cout << "   - Non-AC buses: INR 10-25\n";
            cout << "   - Routes cover entire city\n";
            cout << "   - Tourist Tip: Ask conductor for help with destinations\n";
            cout << "   - Cluster buses (orange) are newer and more comfortable\n";
            cout << "   - DTC buses (red-green) are older but cover more routes\n";
            break;

        case 3:
            cout << "\n3. Auto Rickshaws\n";
            cout << "   - Minimum fare: INR 25\n";
            cout << "   - INR 12 per km after first 1.5 km\n";
            cout << "   - Always insist on meter or agree on fare beforehand\n";
            cout << "   - Tourist Tip: Use 'Delhi Auto Rickshaw Rate Calculator' app\n";
            cout << "   - Green autos run on CNG and are more eco-friendly\n";
            cout << "   - Available throughout the city 24/7\n";
            break;

        case 4:
            cout << "\n4. Taxis/Cabs\n";
            cout << "   - Minimum fare: INR 50\n";
            cout << "   - INR 15-20 per km depending on service\n";
            cout << "   - Apps: Uber, Ola, Meru\n";
            cout << "   - Tourist Tip: Prepaid taxis available at airport\n";
            cout << "   - Radio cabs can be booked by phone (e.g., Mega Cabs)\n";
            cout << "   - Luxury cabs available for special occasions\n";
            break;

        case 5:
            cout << "\n5. Cycle Rickshaws\n";
            cout << "   - Best for short distances in Old Delhi\n";
            cout << "   - Negotiate fare before boarding\n";
            cout << "   - Typical fare: INR 20-50 for short rides\n";
            cout << "   - Eco-friendly way to explore narrow lanes\n";
            cout << "   - Mostly found in Chandni Chowk area\n";
            cout << "   - Not allowed on main roads\n";
            break;

        case 6:
            cout << "\n6. E-Rickshaws\n";
            cout << "   - Environment friendly option\n";
            cout << "   - Fixed routes, shared rides\n";
            cout << "   - Fare: INR 10-30 depending on distance\n";
            cout << "   - Common near metro stations for last-mile connectivity\n";
            cout << "   - No pollution as they run on battery\n";
            cout << "   - Can carry 4-6 passengers\n";
            break;

        case 7:
            // Show all options
            cout << "\n1. Delhi Metro\n";
            cout << "   - Website: https://www.delhimetrorail.com\n";
            cout << "   - Fares: INR 10-60 depending on distance\n";
            cout << "   - Operating hours: 6:00 AM - 11:00 PM\n";
            cout << "   - Tourist Tip: Get a 'Tourist Smart Card' for unlimited travel\n";
            cout << "   - Major Stations: New Delhi (Yellow Line), Rajiv Chowk (Busiest)\n\n";

            cout << "2. Public Buses\n";
            cout << "   - AC buses: INR 25-50\n";
            cout << "   - Non-AC buses: INR 10-25\n";
            cout << "   - Routes cover entire city\n";
            cout << "   - Tourist Tip: Ask conductor for help with destinations\n\n";

            cout << "3. Auto Rickshaws\n";
            cout << "   - Minimum fare: INR 25\n";
            cout << "   - INR 12 per km after first 1.5 km\n";
            cout << "   - Always insist on meter or agree on fare beforehand\n";
            cout << "   - Tourist Tip: Use 'Delhi Auto Rickshaw Rate Calculator' app\n\n";

            cout << "4. Taxis/Cabs\n";
            cout << "   - Minimum fare: INR 50\n";
            cout << "   - INR 15-20 per km depending on service\n";
            cout << "   - Apps: Uber, Ola, Meru\n";
            cout << "   - Tourist Tip: Prepaid taxis available at airport\n\n";

            cout << "5. Cycle Rickshaws\n";
            cout << "   - Best for short distances in Old Delhi\n";
            cout << "   - Negotiate fare before boarding\n";
            cout << "   - Typical fare: INR 20-50 for short rides\n\n";

            cout << "6. E-Rickshaws\n";
            cout << "   - Environment friendly option\n";
            cout << "   - Fixed routes, shared rides\n";
            cout << "   - Fare: INR 10-30 depending on distance\n";
            break;

        default:
            cout << "Invalid choice! Showing all options by default.\n";
            // Recursively call with show all option
            showTransportInfo();
            return;
    }

    cout << "\nPress any key to return to main menu..."<<endl<<endl;
      cin.get();
}

    // Main menu with additional options
    void showMenu() {
        while (true) {
            cout << "\nDELHI TRIP PLANNER SYSTEM\n";
            cout << "-----------------------\n";
            cout << "1. View current traffic conditions\n";
            cout << "2. Update vehicle speed (Current: " << speed << " km/h)\n";
            cout << "3. Find shortest path between districts\n";
            cout << "4. Find optimal city tour route\n";
            cout << "5. Plan daily activities\n";
            cout << "6. Plan Delhi experiences by budget\n";
            cout << "7. Hotel recommendations\n";
            cout << "8. Emergency contacts\n";
            cout << "9. Delhi weather information\n";
            cout << "10. Generate packing list\n";
            cout << "11. View transport information\n";
            cout << "12. Provide feedback\n";
            cout << "13. Exit\n";
            cout << "\nEnter your choice: ";

            int choice;
            cin >> choice;

            switch (choice) {
                case 1: showCurrentTraffic(); break;
                case 2: updateSpeed(); break;
                case 3: findShortestPath(); break;
                case 4: findOptimalTour(); break;
                case 5: planDailyActivities(); break;
                case 6: planByBudget(); break;
                case 7: showHotelRecommendations(); break;
                case 8: showEmergencyContacts(); break;
                case 9: showWeatherInfo(); break;
                case 10: generatePackingList(); break;
                case 11: showTransportInfo(); break;
                case 12: collectFeedback(); break;
                case 13: exit(0);
                default:
                    cout << "Invalid choice! Please try again.\n";
                    cin.get();
            }
        }
    }
};

int main() {
    TripPlanner planner;
    planner.showMenu();
    return 0;
}
