#include <iostream>
#include <numeric>
#include <fstream>
#include <vector>
#include <chrono>

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
    ofstream outFile("./niven.txt");

    for (uint32_t i = 0; i < array_size; ++i) {
        outFile << numbers.at(i) << " " << checked.at(i) << '\n';
    }
    outFile.close();
}

int main() {

    const uint32_t arraySize = 1000000;

    cout << "N: " << arraySize << endl;

    vector<uint32_t> checked(arraySize);
    vector<uint32_t> numbers(arraySize);

    std::iota(numbers.begin(), numbers.end(), 1);

    auto startTime = chrono::high_resolution_clock::now();

    for (uint32_t i = 1; i < numbers.size(); i++) {
        checked[i] = checkNieven(numbers[i]);
    }

    auto endTime = chrono::high_resolution_clock::now();

    chrono::duration<double> executionTime = endTime - startTime;

    cout << "Execution Time: " << executionTime.count() * 1000 << " ms" << endl;

    saveToFile(checked, numbers, arraySize);

    return 0;
}