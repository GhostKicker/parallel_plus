#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <set>
using namespace std;
using namespace chrono;

set<int> y;

struct foo
{
    static int r;
    int rrr;
};


#ifdef _WIN32
#include <windows.h>
#elif MACOS
#include <sys/param.h>
#include <sys/sysctl.h>
#else
#include <unistd.h>
#endif

int getNumberOfCores() {
#ifdef WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#elif MACOS
    int nm[2];
    size_t len = 4;
    uint32_t count;

    nm[0] = CTL_HW; nm[1] = HW_AVAILCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);

    if (count < 1) {
        nm[1] = HW_NCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);
        if (count < 1) { count = 1; }
    }
    return count;
#else
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}


int main()
{

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    omp_set_num_threads(getNumberOfCores());


    cout << getNumberOfCores() << endl;

    int n;
    cin >> n;

    vector<double> a(n);
    vector<double> b(n);

    for (auto& it : a) it = rand();
    for (auto& it : b) it = rand();

    steady_clock::time_point st;
    steady_clock::time_point fn;
    double res;


    st = steady_clock::now();
    res = 0;
    for (int i = 0; i < n; ++i) res += a[i] * b[i];
    fn = steady_clock::now();
    cout << res << " --- " << steady_clock::duration(fn - st).count() / 1000000 / 1e3 << " ord" << endl;

    st = steady_clock::now();
    res = 0;
    int num = 10000;
    vector<double> resulting(n / num + bool(n % num));

#pragma omp parallel for schedule(static, 100)
    for (int i = 0; i < n; i+=num)
    {
        int dex = i / num;
        int lim = min(num, n - i);
        for (int j = 0; j < lim; ++j)
            resulting[dex] += a[i + j] * b[i + j];
    }
    for (auto& it : resulting) res += it;
    fn = steady_clock::now();
    cout << res << " --- " << steady_clock::duration(fn - st).count() / 1000000 / 1e3 << " par" << endl;

    foo::r = 1;



    return 0;
}