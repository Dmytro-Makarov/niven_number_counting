#include <iostream>
#include <numeric>
#include <fstream>
#include <vector>
#include "mpi.h"

using namespace std;

void saveToFile(vector<uint32_t> checked, vector<uint32_t> numbers, uint32_t array_size) {
    ofstream outFile("./niven_mpi.txt");

    for (uint32_t i = 0; i < array_size; ++i) {
        outFile << numbers.at(i) << " " << checked.at(i) << '\n';
    }
    outFile.close();
}

int checkNieven(uint32_t n) {
//    if (n == 0) return 0;

    // Adds the numbers to the sum
    uint32_t sum = 0;
    for (uint32_t temp = n; temp > 0; temp /= 10)
        sum += temp % 10;

    // Checks for division remainder
    return (n % sum == 0);
}

int main(int argc, char* argv[]) {

    int rank, size;

    MPI_Init(&argc, &argv);

    // find out process ID, and number of process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Prints rank and total size of threads
    // cout << "Rank: " << rank << " Size: " << size << endl;

    const uint32_t arraySize = 1000000;

    vector<uint32_t> checked(arraySize);
    vector<uint32_t> numbers(arraySize);

    std::iota(numbers.begin(), numbers.end(), 1);

    uint32_t segmentLen = numbers.size() / size;
//    uint32_t start = numbers.front() + rank * segmentLen;
//    uint32_t end;
//
//    if (rank == size - 1) {
//        end = numbers.size();
//    } else {
//        end = start + segmentLen - 1;
//    }

    vector<uint32_t> segment(segmentLen);

    auto startTime = MPI_Wtime();

    MPI_Scatter(numbers.data(), segmentLen, MPI_UINT32_T, segment.data(), segmentLen, MPI_UINT32_T, 0, MPI_COMM_WORLD);

    cout << segment.front() << endl;

    for (uint32_t i = 0; i < segment.size(); i++) {
        checked[i] = checkNieven(segment[i]);
    }

    std::vector<uint32_t> allChecked;
    if (rank == 0) {
        // Gather results from all processes
        allChecked.resize(numbers.size(), 0);
    }

    MPI_Gather(checked.data(), segmentLen, MPI_UINT32_T, allChecked.data(), segmentLen, MPI_UINT32_T, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "N: " << arraySize << endl;

        auto endTime = MPI_Wtime();
        cout << "Execution Time: " << (endTime - startTime) * 1000 << " ms" << endl;

        saveToFile(allChecked, numbers, numbers.size());
    }

    MPI_Finalize();

    return 0;
}