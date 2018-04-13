#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <omp.h>
#include <Windows.h>

using namespace std;

const int limit = 32;

template <typename T>
inline void deb(T& a)
{
    for (auto it : a)
        cout << it << " ";
    cout << endl;
}

template<typename T>
void q_sort_omp(vector<T>& arr, int left = 0, int right = -123)
{
    if (right == -123) right = arr.size() - 1;
    if (left >= right) return;
    int l = left;
    int r = right;
    T mid = arr[(l + r) / 2];
    while (l <= r)
    {
        while (arr[l] < mid) l++;
        while (arr[r] > mid) r--;
        if (l <= r)
            swap(arr[l++], arr[r--]);
    }

#pragma omp parallel sections
    {
#pragma omp section
        q_sort_omp(arr, left, r);
#pragma omp section
        q_sort_omp(arr, l, right);
    }
}

template<typename T>
void q_sort(vector<T>& arr, int left = 0, int right = -123)
{
    if (right == -123) right = arr.size() - 1;
    if (left >= right) return;
    int l = left;
    int r = right;
    T mid = arr[(l + r) / 2];
    while (l <= r)
    {
        while (arr[l] < mid) l++;
        while (arr[r] > mid) r--;
        if (l <= r)
            swap(arr[l++], arr[r--]);
    }

    q_sort(arr, left, r);
    q_sort(arr, l, right);
}

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int n;
    cin >> n;
    vector<double> v(n);

    for (int i = 0; i < n; i++)
        v[i] = rand();

    auto v1 = v;
    auto v2 = v;
    
    int time;

    time = GetTickCount();
    q_sort_omp(v1);
    time -= GetTickCount();
    cout << "parallel sorted array of " << n
        << " elements in " << double(-time) / 1000.0
        << " seconds" << endl;
#pragma omp taskwait
    time = GetTickCount();
    q_sort(v2);
    time -= GetTickCount();
    cout << "ordinary sorted array of " << n
        << " elements in " << double(-time) / 1000.0
        << " seconds" << endl;


    return 0;
}

