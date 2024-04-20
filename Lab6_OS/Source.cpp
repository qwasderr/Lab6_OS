#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;
using namespace std::chrono;
void generateMatrix(vector<vector<int>>& matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        vector<int> row;
        for (int j = 0; j < cols; ++j) {
            row.push_back(rand() % 10);
        }
        matrix.push_back(row);
    }
}
int multiplyVectors(const vector<int>& vec1, const vector<int>& vec2) {
    int result = 0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        result += vec1[i] * vec2[i];
    }
    return result;
}
void parallelMultiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& result, int start_index, int end_index, mutex& mtx) {
    int n = A.size();
    int k = B[0].size();

    for (int index = start_index; index < end_index; ++index) {
        int i = index / k;
        int j = index % k;

        int value = multiplyVectors(A[i], B[j]);

        //mtx.lock();
        //cout << "[" << i << "," << j << "]=" << value << endl;
        //mtx.unlock();

        result[i][j] = value;
    }
}
int main() {
    srand(time(nullptr));

    int n = 100;
    int m = 100;
    int k = 100;

    vector<vector<int>> A, B;
    generateMatrix(A, n, m);
    generateMatrix(B, m, k);

    vector<vector<int>> result(n, vector<int>(k, 0));

    int num_threads;
    cin >> num_threads;
    vector<thread> threads;
    std::mutex mtx;
    int items_per_thread = n * k / num_threads;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < num_threads; ++i) {
        int start_index = i * items_per_thread;
        int end_index = (i == num_threads - 1) ? n * k : (i + 1) * items_per_thread;
        threads.emplace_back(parallelMultiplyMatrices, ref(A), ref(B), ref(result), start_index, end_index, ref(mtx));
    }
    for (auto& t : threads) {
        t.join();
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<nanoseconds>(stop - start);

    cout << "Time taken: " << duration.count() << " nanoseconds" << endl;
}
