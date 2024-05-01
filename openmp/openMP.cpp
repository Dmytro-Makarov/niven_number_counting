#include <iostream>
#include <numeric>
#include <omp.h>
#include <fstream>
#include <vector>

using namespace std;

int checkNieven(uint32_t n) {
//    if (n == 0) return 0;

    // Adds the numbers to the sum
    uint32_t sum = 0;
    for (uint32_t temp = n; temp > 0; temp /= 10)
        sum += temp % 10;

    // Checks for division remainder
    return (n % sum == 0);
}

void saveToFile(vector<uint32_t> checked, vector<uint32_t> numbers, uint32_t array_size) {
    ofstream outFile("./niven_openmp.txt");

    for (uint32_t i = 0; i < array_size; ++i) {
        outFile << numbers.at(i) << " " << checked.at(i) << '\n';
    }
    outFile.close();
}

int main() {
    int threadAmount;

    cout << "Amount of threads to use: ";
    cin >> threadAmount;
    cout << "Using threads: " << threadAmount << endl;

    const uint32_t arraySize = 1000000;

    cout << "N: " << arraySize << endl;

    vector<uint32_t> checked(arraySize);
    vector<uint32_t> numbers(arraySize);

    std::iota(numbers.begin(), numbers.end(), 1);

    auto startTime = omp_get_wtime();

#pragma omp parallel default(none) shared(numbers, checked) num_threads(threadAmount)
    {
        int threadNum = omp_get_num_threads();
        int threadId = omp_get_thread_num();

        uint32_t segmentLen = numbers.size() / threadNum;
        uint32_t start = threadId * segmentLen;
        uint32_t end;
        if (threadId == threadNum - 1) {
            end = numbers.size();
        } else {
            end = start + segmentLen;
        }

        for (uint32_t i = start; i < end; i++) {
            checked[i] = checkNieven(numbers[i]);
        }
    }

    auto endTime = omp_get_wtime();

	cout << "Execution Time: " << (endTime - startTime) * 1000 << " ms" << endl;

    saveToFile(checked, numbers, arraySize);

    return 0;
}