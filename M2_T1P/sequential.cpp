#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <vector>

using namespace std;

// Function to generate random matrix
void generateRandomMatrix(vector<vector<int>>& matrix, int N) {
    for (int i = 0; i < N; ++i) {
        vector<int> row;
        for (int j = 0; j < N; ++j) {
            int value = rand() % 100; // Generate random value between 0 and 99
            row.push_back(value);
            cout << value << " ";
        }
        cout << endl;
        matrix.push_back(row);
    }
}

// Function to perform matrix multiplication
void multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int N = A.size();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function to write matrix to file
void printMatrixToFile(const vector<vector<int>>& matrix, int N, const string& filename) {
    ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        cout << "Error opening file for writing!" << endl;
        return;
    }

    // Write matrix to the file
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            outputFile << matrix[i][j] << " ";
        }
        outputFile << endl;
    }

    outputFile.close();
}

int main() {
    srand(time(0)); // Seed for random number generation

    int num_matrices;
    cout << "Enter the number of matrices: ";
    cin >> num_matrices;

    int N;
    cout << "Enter the size of the matrices (N x N): ";
    cin >> N;

    for (int m = 1; m <= num_matrices; ++m) {
        cout << "Matrix " << m << " (randomly generated):" << endl;

        // Initialize vectors for matrices A, B, and C
        vector<vector<int>> A, B, C;

        // Generate random matrices A and B
        cout << "Matrix A:" << endl;
        generateRandomMatrix(A, N);
        cout << "Matrix B:" << endl;
        generateRandomMatrix(B, N);

        // Allocate memory for matrix C
        C.resize(N, vector<int>(N, 0));

        // Measure the start time
        clock_t startTime = clock();

        // Multiply matrices A and B
        multiplyMatrices(A, B, C);

        // Measure the end time
        clock_t endTime = clock();

        // Calculate the execution time in milliseconds
        double executionTime = double(endTime - startTime) / CLOCKS_PER_SEC * 1000.0;

        // Write the result matrix C to a file
        string filename = "matrix_output_" + to_string(m) + ".txt";
        printMatrixToFile(C, N, filename);

        // Output execution time
        cout << "Execution time for matrix " << m << ": " << executionTime << " milliseconds." << endl;
        cout << "Result for matrix " << m << " saved in '" << filename << "'." << endl;
    }

    return 0;
}