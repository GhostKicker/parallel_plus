#define _CRT_SECURE_NO_WARNINGS
#include <omp.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "windows.h"

#define wheels_change 0
#define tires_change 1

using namespace std;

template <typename T>
void crit_out(int cur_time, T s)
{
#pragma omp critical
    {
        cout << cur_time << ": " << s << endl;
        cerr << cur_time << ": " << s << endl;
    }
}


struct customer
{
    int no;
    bool was_served;
    int time_came;
    int time_gone;
    int type;

    customer() {};
    ~customer() {};

    customer(int t_st, int i)
    {
        no = i;
        time_came = t_st;
        time_gone = t_st + 100 + rand() % 300;
        was_served = false;
        type = rand() & 1;
    }

    void serve(int& cur_time)
    {
#pragma omp critical
        {
            was_served = true;
        }
        Sleep(450 - type * 300);
        cur_time += 450 - type * 300;
        crit_out(cur_time, "customer " + to_string(no) + " was served");
    }

    void process()
    {
        Sleep(time_came);

        if (type == wheels_change)
            crit_out(time_came, "customer " + to_string(no) + " came, needs to change wheels");
        else
            crit_out(time_came, "customer " + to_string(no) + " came, needs to change tires");

        Sleep(time_gone - time_came);

        if (!was_served)
            crit_out(time_gone, "customer " + to_string(no) + " is gone");
    }

};

struct box
{
    int num;
    vector<customer> customers;
};


void master(box& bx)
{
    int current_time = 0;
    bool tv = true;
    for (int i = 0; i < bx.num; ++i)
    {
        if (bx.customers[i].time_came > current_time)
        {
            if (!tv)
            {
                tv = true;
                crit_out(current_time, "work off, tv on");
            }

            Sleep(bx.customers[i].time_came - current_time);
            current_time = bx.customers[i].time_came;
        }
        if (current_time >= bx.customers[i].time_came && current_time <= bx.customers[i].time_gone)
        {
            if (tv)
            {
                tv = false;
                crit_out(current_time, "tv off, work on");
            }
            bx.customers[i].serve(current_time);
        }
        if (current_time > bx.customers[i].time_came)
            continue;
    }

    crit_out(current_time, "work for today is done");
}

int main() {

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    box bx;
    cin >> bx.num;

    bx.customers.resize(bx.num);

    int current_time = 0;

    for (int i = 0; i < bx.num; ++i)
    {
        bx.customers[i] = customer(current_time, i);
        current_time += 100 + rand() % 900;
    }

    omp_set_nested(true);
    omp_set_num_threads(bx.num + 10);

#pragma omp parallel sections
    {
#pragma omp section
        {
#pragma omp parallel for 
            for (int i = 0; i < bx.num; ++i)
            {
                bx.customers[i].process();
            }
        }
#pragma omp section
        {
            master(bx);
        }
    }
}