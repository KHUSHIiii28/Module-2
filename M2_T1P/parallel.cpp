#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <pthread.h>

using namespace std;

const int NUM_THREADS = 4; // Number of threads for parallel computation

// Global variables for input and output matrices
int** inputMatrix1;
int** inputMatrix2;
int** resultMatrixParallel;
int matrixSize; // Size of the matrices

// Function to initialize a matrix with random values
void initializeMatrix(int** matrix, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            matrix[i][j] = rand() % 10; // Generate random values between 0 and 9
        }
    }
}

// Function to print a matrix to the console
void printMatrix(int** matrix, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

// Function to write a matrix to a file
void writeMatrixToFile(int** matrix, int size, const char* filename)
{
    ofstream outputFile(filename);
    if (!outputFile.is_open())
    {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            outputFile << matrix[i][j] << " ";
        }
        outputFile << endl;
    }

    outputFile.close();
    cout << "Matrix written to file: " << filename << endl;
}

// Function to perform matrix multiplication for a subset of rows assigned to a thread
void* pthreadMatrixMultiplication(void* threadid)
{
    long tid = (long)threadid;
    int range = matrixSize / NUM_THREADS;
    int start = tid * range;
    int end = start + range;

    for (int i = start; i < end; ++i)
    {
        for (int j = 0; j < matrixSize; ++j)
        {
            resultMatrixParallel[i][j] = 0;
            for (int k = 0; k < matrixSize; ++k)
            {
                resultMatrixParallel[i][j] += inputMatrix1[i][k] * inputMatrix2[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

int main()
{
    // Set the starting point for getting random numbers 
    // based on the current time, so we get different random numbers each time we run the program
    srand(time(0));

    cout << "Enter the size of the matrix: ";
    cin >> matrixSize;

    // Allocate memory for matrices dynamically
    inputMatrix1 = new int*[matrixSize];
    inputMatrix2 = new int*[matrixSize];
    resultMatrixParallel = new int*[matrixSize];
    for (int i = 0; i < matrixSize; ++i)
    {
        inputMatrix1[i] = new int[matrixSize];
        inputMatrix2[i] = new int[matrixSize];
        resultMatrixParallel[i] = new int[matrixSize];
    }

    // Initialize matrices A and B with random values
    initializeMatrix(inputMatrix1, matrixSize);
    initializeMatrix(inputMatrix2, matrixSize);

    // Perform parallel matrix multiplication
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    clock_t parallelStartTime = clock();
    for (t = 0; t < NUM_THREADS; ++t)
    {
        rc = pthread_create(&threads[t], NULL, pthreadMatrixMultiplication, (void*)t);
        if (rc)
        {
            cout << "Error: Unable to create thread, " << rc << endl;
            exit(-1);
        }
    }
    for (t = 0; t < NUM_THREADS; ++t)
    {
        pthread_join(threads[t], NULL);
    }
    clock_t parallelEndTime = clock();
    double parallelExecutionTime = double(parallelEndTime - parallelStartTime) / CLOCKS_PER_SEC * 1000;

    // Print and write result of parallel multiplication...
    cout << "Parallel Result Matrix:" << endl;
    printMatrix(resultMatrixParallel, matrixSize);
    cout << endl;
    writeMatrixToFile(resultMatrixParallel, matrixSize, "parallel_result_matrix.txt");
    cout << "Parallel Execution time: " << parallelExecutionTime << " milliseconds" << endl;

    // Deallocate memory for matrices
    for (int i = 0; i < matrixSize; ++i)
    {
        delete[] inputMatrix1[i];
        delete[] inputMatrix2[i];
        delete[] resultMatrixParallel[i];
    }
    delete[] inputMatrix1;
    delete[] inputMatrix2;
    delete[] resultMatrixParallel;

    return 0;
}