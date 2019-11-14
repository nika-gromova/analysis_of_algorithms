#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <time.h>
#include <windows.h>
#include <stdlib.h>
#include <ctime>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#define items_number 5
using namespace std;

time_t currentTime()
{
    chrono::time_point<chrono::high_resolution_clock> now = chrono::high_resolution_clock::now();
    time_t now_t = chrono::system_clock::to_time_t(now);
    return now_t;
}

const string time_t_to_string(time_t t) {
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&t);
    strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
    return buf;
}

queue<pair<int, int>> que1, que2, que3, result;
mutex mtx1, mtx2, mtx3, mtx4, mtx_log;
vector<pair<time_t, string>> items;

void first_line()
{
    bool finish = false;
    while(!finish)
    {
        mtx1.lock();
        if (!que1.empty())
        {
            pair<int, int> cur_item = que1.front();
            que1.pop();
            int index = cur_item.first;
            int t0 = cur_item.second;
            if (t0 != -1)
            {
                mtx_log.lock();
                string tmp = "line 1: start " + to_string(index) + " item " + to_string(t0) + " milliseconds";
                items.push_back({currentTime(), tmp});
                mtx_log.unlock();

                Sleep(t0);

                mtx2.lock();
                que2.push({index, t0 * 3});
                //que2.push({index, t0 + 101});
                mtx2.unlock();

                mtx_log.lock();
                tmp = "line 1: end " + to_string(index) + " item";
                items.push_back({currentTime(), tmp});
                mtx_log.unlock();
            }
            else
            {
                mtx2.lock();
                que2.push({-1, -1});
                mtx2.unlock();
                finish = true;
            }
        }
        mtx1.unlock();
    }
}

void second_line()
{
    bool finish = false;
    while(!finish)
    {
        mtx2.lock();
        if (!que2.empty())
        {
            pair<int, int> cur_item = que2.front();
            int index = cur_item.first;
            int t0 = cur_item.second;
            que2.pop();
            if (t0 != -1)
            {
                mtx_log.lock();
                string tmp = "line 2: start " + to_string(index) + " item " + to_string(t0) + " milliseconds";
                items.push_back({currentTime(), tmp});
                mtx_log.unlock();

                Sleep(t0);

                mtx3.lock();
                que3.push({index, 1500});
                //que3.push({index, t0 + 50});
                mtx3.unlock();

                mtx_log.lock();
                tmp = "line 2: end " + to_string(index) + " item ";
                items.push_back({currentTime(), tmp});
                mtx_log.unlock();
            }
            else
            {
                mtx3.lock();
                que3.push({-1, -1});
                mtx3.unlock();
                finish = true;
            }
        }
        mtx2.unlock();
    }
}

void third_line()
{
    bool finish = false;
    while(!finish)
    {
        mtx3.lock();
        if (!que3.empty())
        {
            pair<int, int> cur_item = que3.front();
            int index = cur_item.first;
            int t0 = cur_item.second;
            que3.pop();
            if (t0 != -1)
            {
                mtx_log.lock();
                string tmp = "line 3: start " + to_string(index) + " item " + to_string(t0) + " milliseconds";
                items.push_back({currentTime(), tmp});
                mtx_log.unlock();

                Sleep(t0);

                mtx4.lock();
                result.push({index, t0});
                mtx4.unlock();

                mtx_log.lock();
                tmp = "line 3: end " + to_string(index) + " item ";
                items.push_back({currentTime(), tmp});
                mtx_log.unlock();
            }
            else
            {
                finish = true;
            }
        }
        mtx3.unlock();
    }
}

void write_log(const string& name)
{
    ofstream file;
    file.open(name, ios_base::app);
    if (!file)
        cout << "error" << endl;
    else
    {
        for (auto s : items)
        {
            file << time_t_to_string(s.first) << " " << s.second << '\n';
        }
        file.close();
    }
}

void sort_log()
{
    bool flag;
    for (auto i = 0; i < items.size(); i++)
    {
        flag = false;
        for (auto j = 1; j < items.size() - i; j ++)
        {
            if (items[j].first < items[j - 1].first)
            {
                pair<time_t, string> tmp = items[j];
                items[j] = items[j - 1];
                items[j - 1] = tmp;
                flag = true;
            }
        }
        if (!flag)
            return;
    }
}

int main()
{

    thread line_one(&first_line);
    thread line_two(&second_line);
    thread line_three(&third_line);

    srand(time(NULL));
    for(int i = 0; i < items_number; ++i)
    {
        //int tmp = (1 + rand() % 3) * 1000 + 1;
        int tmp = 1000;
        cout << tmp << endl;
        mtx1.lock();
        que1.push({i, tmp});
        mtx1.unlock();
        Sleep(50);
    }
    mtx1.lock();
    que1.push({-1, -1});
    mtx1.unlock();

    line_one.join();
    line_two.join();
    line_three.join();
    if (result.size() == items_number)
    {
        cout << "all items passed the conveyor" << endl;
        sort_log();
        write_log("log.txt");
        cout << "total time (in seconds): " << items[items.size() - 1].first - items[0].first << endl;
    }
    else
        cout << "some problems happened" << endl;
    return 0;
}
