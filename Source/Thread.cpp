#include <iostream>
#include <thread>
#include <vector>

using namespace std;

const int N = 100000000;
const int THREADS = 100;

void Sum(vector<int8_t>::iterator start, vector<int8_t>::iterator end, int& result)
{
    result = 0;
    for (auto iter = start; iter != end; ++iter)
        result += *iter;
    printf("[THREAD %d] Sum: %d\n", this_thread::get_id(), result);
}

int main()
{
    vector<int8_t> nums(N, 1);    

    int numsPerThread = N / THREADS;
    vector<int> results(THREADS);
    vector<thread> threads;
    for (int i = 0; i < THREADS; ++i)
    {
        vector<int8_t>::iterator start = nums.begin() + numsPerThread * i;
        vector<int8_t>::iterator end = nums.begin() + numsPerThread * (i + 1);
        threads.emplace_back(Sum, start, end, ref(results[i]));
    }

    for (thread& th : threads) th.join();

    int sum = 0;
    for (int result : results) sum += result;
    printf("Final Sum: %d\n", sum);
    return 0;
}