#include <stdio.h>
#include <string.h> // For strcasecmp
#include <math.h>   // For sqrt and pow

#include "location_logic.h"
#include "structs.h"

// --- Private Functions & Data (Not declared in the .h file) ---

// Hardcoded graph representation
static int graph[MAX_LOCATIONS][MAX_HOTELS][MAX_HOTELS] = {
    // Location 0: Medavakkam
    {{3, 4, 6, 9, 11}, {2, 0, 1, 5, 6}, {3, 1, 0, 3, 4}, {4, 5, 3, 0, 2}, {5, 6, 4, 2, 0}},
    // Location 1: Besant Nagar
    {{0, 1, 2, 3, 4}, {1, 0, 3, 5, 7}, {2, 3, 0, 2, 6}, {3, 5, 2, 0, 4}, {4, 7, 6, 4, 0}},
    // Location 2: T Nagar
    {{0, 1, 2, 3, 4}, {1, 0, 3, 5, 7}, {2, 3, 0, 2, 6}, {3, 5, 2, 0, 4}, {4, 7, 6, 4, 0}},
    // Location 3: Anna Nagar
    {{0, 1, 2, 3, 4}, {1, 0, 3, 5, 7}, {2, 3, 0, 2, 6}, {3, 5, 2, 0, 4}, {4, 7, 6, 4, 0}},
    // Location 4: Thiruporur
    {{0, 1, 2, 3, 4}, {1, 0, 3, 5, 7}, {2, 3, 0, 2, 6}, {3, 5, 2, 0, 4}, {4, 7, 6, 4, 0}}
};


// Finds the vertex with the minimum distance value
static int minDistance(int dist[], int visited[], int n) {
    int min = INF, min_index = -1;
    for (int v = 0; v < n; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// Prints the sorted hotels table
static void printSortedHotels(int dist[], float ratings[], int n, struct Hotel hotels[]) {
    printf("\nHotels nearby (sorted by distance & rating):\n");
    printf("+------------------------------+---------------+---------+\n");
    printf("| %-28s | Distance (km) | Rating  |\n", "Hotel Name");
    printf("+------------------------------+---------------+---------+\n");
    for (int i = 0; i < n; i++) {
        printf("| %-28s | %-13d | %-7.1f |\n", hotels[i].name, dist[i], ratings[i]);
    }
    printf("+------------------------------+---------------+---------+\n");
}

// Calculates a combined score for sorting
static void calculateCombinedScore(int dist[], float ratings[], float combinedScores[], int n) {
    for (int i = 0; i < n; i++) {
        float normalizedDistance = (float)dist[i];
        float normalizedRating = ratings[i] / 10.0;
        combinedScores[i] = (0.7 * normalizedRating) + (0.3 * (1.0 / normalizedDistance));
    }
}

// --- Public Functions (Declared in the .h file) ---

int restaurant(int x, int y) {
    if ((x==2 && y==3)||(x==33 && y==28)||(x==27 && y==31)||(x==8 && y==8)||(x==11 && y==13)||
        (x==24 && y==24)||(x==14 && y==19)||(x==2 && y==16)||(x==33 && y==8)||(x==27 && y==23)||
        (x==8 && y==2)||(x==12 && y==26)||(x==19 && y==11)||(x==24 && y==14)||(x==3 && y==8)||
        (x==6 && y==34)||(x==12 && y==23)||(x==33 && y==21)||(x==27 && y==14)||(x==24 && y==26)||
        (x==16 && y==1)||(x==1 && y==13)||(x==34 && y==1)||(x==27 && y==7)||(x==23 && y==18)||
        (x==18 && y==33)||(x==13 && y==7)||(x==7 && y==21)||(x==2 && y==22)||(x==7 && y==13)||
        (x==13 && y==3)||(x==17 && y==9)||(x==22 && y==31)||(x==28 && y==18)||(x==32 && y==26)||
        (x==1 && y==27)||(x==9 && y==19)||(x==12 && y==31)||(x==17 && y==24)||(x==23 && y==6)||
        (x==28 && y==4)||(x==31 && y==11)||(x==2 && y==32)||(x==8 && y==29)||(x==14 && y==16)||
        (x==31 && y==19)||(x==29 && y==34)||(x==24 && y==4)||(x==18 && y==26)) {
        return 0;
    }
    return 1;
}

int road(int x, int y) {
    if (x % 5 == 0 || y % 5 == 0) return 0;
    return 1;
}

void dijkstra(int location, int src, struct Hotel hotels[], int n) {
    int dist[MAX_HOTELS];
    int visited[MAX_HOTELS];

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[src] = 0;

    for (int count = 0; count < n - 1; count++) {
        int u = minDistance(dist, visited, n);
        if (u == -1) continue; // No path found
        visited[u] = 1;

        for (int v = 0; v < n; v++) {
            if (!visited[v] && graph[location][u][v] && dist[u] != INF && (dist[u] + graph[location][u][v] < dist[v])) {
                dist[v] = dist[u] + graph[location][u][v];
            }
        }
    }

    float ratings[n];
    for (int i = 0; i < n; i++) {
        ratings[i] = hotels[i].rating;
    }

    float combinedScores[n];
    calculateCombinedScore(dist, ratings, combinedScores, n);

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (combinedScores[i] < combinedScores[j]) {
                // Swap all related arrays together
                int tempDist = dist[i]; dist[i] = dist[j]; dist[j] = tempDist;
                float tempRating = ratings[i]; ratings[i] = ratings[j]; ratings[j] = tempRating;
                float tempScore = combinedScores[i]; combinedScores[i] = combinedScores[j]; combinedScores[j] = tempScore;
                struct Hotel tempHotel = hotels[i]; hotels[i] = hotels[j]; hotels[j] = tempHotel;
            }
        }
    }
    printSortedHotels(dist, ratings, n, hotels);
}