#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace chrono;

inline double f(int n, double a)
{
    return 1 / pow(n, a);
}

double parallel_calc(int n, double a, int m)
{
    double res = 0;
    omp_set_num_threads(m);

#pragma omp parallel for
    for (int curn = 1; curn <= n; ++curn)
        res += f(curn, a);
    
    return res;
}

double ordinary_calc(int n, double a)
{
    double res = 0;

    for (int curn = 1; curn <= n; ++curn)
        res += f(n, a);

    return res;
}

int main()
{
#ifdef _DEBUG
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif

    int n;      // N - number of series members
    double a;   // power
    int m;      // number of threads
    cin >> n >> a >> m;

    steady_clock::time_point st, fn;
    st = steady_clock::now();
    cout << parallel_calc(n, a, m) << endl;
    fn = steady_clock::now();
    cout << duration_cast<microseconds>(fn - st).count() << " microseconds" << endl;

    //steady_clock::time_point st, fn;
    //cout << "---------------" << endl;
    //cout << "relation with N" << endl;
    //for (int i = 100; i <= 10000; i += 100)
    //{
    //    st = steady_clock::now();
    //    parallel_calc(i, 10, 4);
    //    fn = steady_clock::now();

    //    cout << i << ", ";
    //    cout << duration_cast<microseconds>(fn - st).count() << endl;
    //}
    //cout << "---------------" << endl;


    //cout << "relation with a" << endl;
    //for (int i = 100; i <= 10000; i += 100)
    //{
    //    st = steady_clock::now();
    //    parallel_calc(300, i, 8);
    //    fn = steady_clock::now();

    //    cout << i << ", ";
    //    cout << duration_cast<microseconds>(fn - st).count() << endl;
    //}
    //cout << "---------------" << endl;

    //cout << "relation with m" << endl;
    //for (int i = 1; i <= 100; i += 1)
    //{
    //    st = steady_clock::now();
    //    parallel_calc(300, 10, i);
    //    fn = steady_clock::now();

    //    cout << i << ", ";
    //    cout << duration_cast<microseconds>(fn - st).count() << endl;
    //}
    //cout << "---------------" << endl;

}