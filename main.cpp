#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using namespace std::chrono;

int MAX_RAND = INT_MAX;
const int EXPERIMENTS = 10;
const int MAX_LENGTH_TEST = 1000 * 1000 * 10;
const int MIN_LENGTH_TEST = 10;

int random_element(int min, int max) {
    static bool flag;
    if (!flag) {
        srand(time(NULL));
        flag = true;
    }
    return min + rand() % (max - min);
}

vector<int> generate_vector(int length) {
    vector<int> v;
    for (int i = 1; i <= length; ++i) {
        v.push_back(random_element(0, MAX_RAND));
    }
    return v;
}

void swap(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
}

void print_vector(vector<int>& v) {
    for (auto e : v) cout << e << " ";
    cout << endl;
}

// sort: [l; r)
void insertion_sort(vector<int>& v, int l, int r) {

    if (l >= r) return;

    for (int j = l + 1; j < r; ++j) {

        int key = v[j];

        int i = j - 1;
        while (i >= 0 and v[i] > key) {
            v[i + 1] = v[i];
            i--;
        }

        v[i + 1] = key;

    }

}

int partition(vector<int>& v, int low, int high) {
    int pivot = v[low];
    int i = low, j = high;

    while (true) {
        // Find leftmost element greater than
        // or equal to pivot
        while (v[i] < pivot) i++;

        // Find rightmost element smaller than
        // or equal to pivot
        while(v[j] > pivot) j--;

        // If two pointers met
        if (i >= j) {
            return j;
        }

        swap(v[i++], v[j--]);
    }
}

int partition_r(vector<int>& v, int low, int high) {

    vector<int> median = {
            random_element(low, high),
            random_element(low, high),
            random_element(low, high)
    };
    insertion_sort(median, 0, median.size());

    int random = median[1];

    swap(v[random], v[low]);

    return partition(v, low, high);
}

void qsort(vector<int>& v, int low, int high) {

    if (low >= high) return;

    int pi = partition_r(v, low, high);

    // Separately sort elements before
    // partition and after partition
    qsort(v, low, pi);
    qsort(v, pi + 1, high);
}

void calculate_time(vector<int>& data, int i, void (*method)(vector<int>&, int, int), string& name) {

    auto start = high_resolution_clock::now();
    method(data, 0, data.size());
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << name << " sort: i = " << i
         << ", time for n = " << "1e" << to_string(data.size()).size() - 1
         << " : " << duration.count() << endl;
}

void do_experiment(int i, int n, void (*method)(vector<int>&, int, int), string method_name) {

    vector<int> data = generate_vector(n);
    if (method_name == "insert") {
        if (n <= 100000) {
            calculate_time(data, i, method, method_name);
        }
    } else {
        calculate_time(data, i, method, method_name);
    }

}

int main() {

    for (int n = MIN_LENGTH_TEST; n <= MAX_LENGTH_TEST; n = n * 10) {

        for (int i = 1; i <= EXPERIMENTS; ++i) {
            do_experiment(i, n, insertion_sort, "insert");
        }

        cout << endl;

        for (int i = 1; i <= EXPERIMENTS; ++i) {
            do_experiment(i, n, qsort, "qsort");
        }

        cout << endl;

    }

    return 0;

}