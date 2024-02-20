#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

struct Point {
    double x = 0.0;
    double y = 0.0;
    string name;

    Point(double x, double y, string name) : x(x), y(y), name(name) {}

    bool operator==(const Point& rhs) {
        if (x != rhs.x)
            return false;
        if (y != rhs.y)
            return false;
        return true;
    }
    bool operator==(Point& rhs) {
        if (x != rhs.x)
            return false;
        if (y != rhs.y)
            return false;
        return true;
    }

    double calc_euclidean_distance(Point& other_point) {
        return sqrt(pow(other_point.x - x, 2) + pow(other_point.y - y, 2));
    }
};

void print_distances_from_centers(vector<Point>& points, vector<Point>& centers) {
    for (Point& c : centers) {
        for (Point& p : points) {
            if (!(c == p)) {
                cout << c.name << ": (" << c.x << ", " << c.y << ") ";
                cout << "-> ";
                cout << p.name << ": (" << p.x << ", " << p.y << ") ";
                cout << "= " << p.calc_euclidean_distance(c) << endl;
            }
        }
        cout << endl;
    }
}

vector<vector<Point>> cluster_points_near_centers(vector<Point>& points, vector<Point>& centers) {
    vector<vector<Point>> clusters = { {},{} };
    for (Point& p : points) {
        unsigned int low = 0;
        double low_dist = p.calc_euclidean_distance(centers[low]);
        for (auto i = 0; i < centers.size(); i++) {
            double dist = p.calc_euclidean_distance(centers[i]);
            if (dist < low_dist) {
                low = i;
                low_dist = dist;
            }
        }
        clusters[low].push_back(p);
        cout << p.name << " is closest to " << centers[low].name << endl;
    }
    return clusters;
}

vector<Point> get_new_centers(vector<vector<Point>>& clusters) {
    vector<Point> new_centers;
    for (auto i = 0; i < clusters.size(); i++) {
        string name = "x" + to_string(i + 1);
        double x_sum = 0.0;
        double y_sum = 0.0;
        for (Point& p : clusters[i]) {
            x_sum += p.x;
            y_sum += p.y;
        }
        double x_mean = x_sum / clusters[i].size();
        double y_mean = y_sum / clusters[i].size();
        new_centers.emplace_back(x_mean, y_mean, name);
    }
    return new_centers;
}

bool compare_centers(vector<Point>& new_centers, vector<Point>& centers) {
    if (new_centers.size() != centers.size())
        return false;
    for (auto i = 0; i < centers.size(); i++) {
        if (!(new_centers[i] == centers[i])) {
            return false;
        }
    }
    return true;
}

void print_clusters(vector<vector<Point>>& clusters, vector<Point> centers) {
    for (auto i = 0; i < clusters.size(); i++) {
        cout << "Cluster " << i + 1 << " centered at (" << centers[i].x << ", " << centers[i].y << ")" << endl;
        for (auto& p : clusters[i]) {
            cout << p.name << ": (" << p.x << ", " << p.y << ")" << endl;
        }
        cout << endl;
    }
}

int main() {

    Point a1(0.0, 0.0, "a1");
    Point a2(0.0, 4.0, "a2");
    Point a3(2.0, 1.0, "a3");
    Point a4(2.0, 3.0, "a4");
    Point a5(4.0, 0.0, "a5");
    Point a6(4.0, 4.0, "a6");

    vector<Point> points = { a1, a2, a3, a4, a5, a6 };
    vector<Point> centers = { a1, a2 };
    size_t count = 1;

    cout << setprecision(16);
    while (true) {

        cout << "________________________ Iteration # " << count << " ________________________" << endl;

        auto clusters = cluster_points_near_centers(points, centers);

        print_distances_from_centers(points, centers);

        auto new_centers = get_new_centers(clusters);

        if (compare_centers(new_centers, centers)) {
            cout << "\n\n_________ COMPLETE: NEW CENTERS MATCH OLD CENTERS. " << count << " TOTAL ITERATIONS._________" << endl;
            print_clusters(clusters, centers);
            break;
        }
        centers = new_centers;
        count++;
    }
}
