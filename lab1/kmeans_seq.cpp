#include <bits/stdc++.h>
using namespace std;
#define int long long

int dist(pair<int,int>& a, pair<int,int>& b)
{
    int dx = a.first - b.first;
    int dy = a.second - b.second;
    return dx*dx + dy*dy;
}

int32_t main()
{
    bool MANUAL = true;   // 🔁 true → manual, false → random

    int n, k;
    vector<pair<int,int>> points;
    vector<pair<int,int>> centroids;

    if(MANUAL)
    {
        // -------- MANUAL DATA --------
        n = 8;
        points = {
            {2, 3},
            {3, 4},
            {5, 3},
            {8, 7},
            {7, 8},
            {9, 6},
            {1, 2},
            {4, 5}
        };

        k = 3;
        centroids = {
            {2, 3},
            {5, 3},
            {8, 7}
        };
    }
    else
    {
        // -------- RANDOM DATA --------
        srand(time(0));

        int L = 10000, R = 100000;
        n = L + rand() % (R - L + 1);

        points.resize(n);
        for(int i = 0; i < n; i++)
            points[i] = {rand() % 1000, rand() % 1000};

        k = 6;
        centroids.resize(k);
        for(int i = 0; i < k; i++)
            centroids[i] = points[rand() % n];
    }

    vector<vector<pair<int,int>>> clusters(k);

    // -------- K-MEANS --------
    while(true)
    {
        for(int i = 0; i < n; i++)
        {
            int mn_dist = LLONG_MAX, mn_index = -1;
            for(int j = 0; j < k; j++)
            {
                int d = dist(points[i], centroids[j]);
                if(d < mn_dist)
                {
                    mn_dist = d;
                    mn_index = j;
                }
            }
            clusters[mn_index].push_back(points[i]);
        }

        bool changed = false;
        for(int i = 0; i < k; i++)
        {
            if(clusters[i].empty()) continue;

            int sum_x = 0, sum_y = 0;
            for(auto &p : clusters[i])
            {
                sum_x += p.first;
                sum_y += p.second;
            }

            int new_x = sum_x / clusters[i].size();
            int new_y = sum_y / clusters[i].size();

            if(new_x != centroids[i].first || new_y != centroids[i].second)
            {
                centroids[i] = {new_x, new_y};
                changed = true;
            }
        }

        if(!changed) break;

        for(int i = 0; i < k; i++)
            clusters[i].clear();
    }

    // -------- OUTPUT --------
    for(int i = 0; i < k; i++)
    {
        cout << "Centroid " << i+1 << ": ("
             << centroids[i].first << ", "
             << centroids[i].second << ")\n";

        cout << "Cluster " << i+1 << ": ";
        for(auto &p : clusters[i])
            cout << "(" << p.first << ", " << p.second << ") ";
        cout << "\n\n";
    }

    return 0;
}
