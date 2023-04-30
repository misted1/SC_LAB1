#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <time.h> 
#include <omp.h>

using namespace std;
double execution_time = 0;

vector<double> Gaus(vector<vector<double>> A, vector<double> b) {
    int n = A.size();
    
    for (int i = 0; i < n; i++) {
        A[i].push_back(b[i]);
    }

   
    for (int i = 0; i < n; i++) {
        
        int max_row = i;
        for (int j = i + 1; j < n; j++) {
            if (abs(A[j][i]) > abs(A[max_row][i])) {
                max_row = j;
            }
        }

        
        swap(A[i], A[max_row]);

        
        double start = omp_get_wtime();
        omp_set_num_threads(8);
#pragma omp parallel for 
        for (int j = i + 1; j < n; j++) {
            double factor = A[j][i] / A[i][i];
            for (int k = i; k <= n; k++) {
                A[j][k] -= factor * A[i][k];
            }
        }
        double end = omp_get_wtime();
        double Temp = end - start;
        execution_time += Temp;

    }

    
    vector<double> x(n);
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0;
        //
        for (int j = i + 1; j < n; j++) {
            sum += A[i][j] * x[j];
        }
        //
        x[i] = (A[i][n] - sum) / A[i][i];
    }

    return x;
}

int main() {
    srand(time(0));
    const int rows = 4000;
    const int cols = 4000;
    vector<vector<double>> A(rows, vector<double>(cols));
    vector<double> b(rows);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            A[i][j] = rand() % 100; 
        }
    }

    for (int j = 0; j < cols; j++) {
        b[j] = rand() % 100; 
    }

    clock_t start = clock();
    vector<double> x = Gaus(A, b);
    clock_t end = clock();


    std::cout << "Time for OMP loop = " << execution_time << std::endl;

    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    /*for (int i = 0; i < x.size(); i++) {
        cout << x[i] << " ";
    }*/
    cout << endl;
    cout << "time = " << seconds;
    return 0;
}