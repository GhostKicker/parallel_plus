#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <omp.h>
#include <time.h>

using namespace std;

int n;

template <typename T>
inline void deb(T& a)
{
    for (auto it : a)
        cout << it << " ";
    cout << endl;
}

template<typename T>
void q_sort_omp(vector<T>& arr, int left = 0, int right = n - 1)
{
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

    if (right - left > 2000)
    {
#pragma omp parallel sections
        {
#pragma omp section
            q_sort_omp(arr, left, r);
#pragma omp section
            q_sort_omp(arr, l, right);
        }
    }
    else
    {
        q_sort(arr, left, r);
        q_sort(arr, l, right);
    }
}

template<typename T>
void q_sort(vector<T>& arr, int left = 0, int right = n - 1)
{
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


template <typename T>
void mergesort_omp(vector<T>& arr, int l = 0, int r = n - 1)
{
    if (r - l < 1)
    {
        return;
    }

    int mid = (l + r) / 2;

    if (r - l > 10000)
    {
#pragma omp parallel sections
        {
#pragma omp section
            mergesort_omp(arr, l, mid);
#pragma omp section
            mergesort_omp(arr, mid + 1, r); 
        }
    }
    else
    {
        mergesort(arr, l, mid);
        mergesort(arr, mid + 1, r);
    }

    auto b1 = arr.begin() + l;
    auto e1 = arr.begin() + mid + 1;
    auto b2 = arr.begin() + mid + 1;
    auto e2 = arr.begin() + r + 1;

    vector<int> temp(r - l + 1);

    merge(b1, e1, b2, e2, temp.begin());

#pragma omp parallel for
    for (int i = 0; i < temp.size(); ++i)
        arr[l + i] = temp[i];


}

template <typename T>
void mergesort(vector<T>& arr, int l = 0, int r = n - 1)
{
    if (r - l < 1)
    {
        return;
    }

    int mid = (l + r) / 2;

    mergesort(arr, l, mid);
    mergesort(arr, mid + 1, r);

    auto b1 = arr.begin() + l;
    auto e1 = arr.begin() + mid + 1;
    auto b2 = arr.begin() + mid + 1;
    auto e2 = arr.begin() + r + 1;

    vector<int> temp(r - l + 1);

    merge(b1, e1, b2, e2, temp.begin());

    for (int i = 0; i < temp.size(); ++i)
        arr[l + i] = temp[i];
}



int main()
{
    clock_t cl1, cl2;


    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    cin >> n;
    vector<int> v(n);

    omp_set_num_threads(100);

    for (int i = 0; i < n; i++)
        v[i] = rand();

    //int cur = n;

    //for (auto& it : v)
    //    it = cur--;


    auto v1 = v;
    auto v2 = v;
    auto v3 = v;
    auto v4 = v;
    
    int time;

    cl1 = clock();
    q_sort_omp(v1);
    cl2 = clock();
    cout << "qsort: parallel: " << n
        << " elements in " << (cl2 - cl1) / double(CLOCKS_PER_SEC)
        << " seconds" << endl;

    cl1 = clock();
    q_sort(v2);
    cl2 = clock();
    cout << "qsort: ordinary: " << n
        << " elements in " << (cl2 - cl1) / double(CLOCKS_PER_SEC)
        << " seconds" << endl;

    cl1 = clock();
    mergesort_omp(v2);
    cl2 = clock();
    cout << "merge: parallel: " << n
        << " elements in " << (cl2 - cl1) / double(CLOCKS_PER_SEC)
        << " seconds" << endl;

    cl1 = clock();
    mergesort(v3);
    cl2 = clock();
    cout << "merge: ordinary: " << n
        << " elements in " << (cl2 - cl1) / double(CLOCKS_PER_SEC)
        << " seconds" << endl;




    return 0;
}

