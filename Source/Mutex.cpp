#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

int result;
mutex mu;
atomic_flag spinlock = ATOMIC_FLAG_INIT;

void Sum1(int iter)
{
    while (iter--)
        result++;
}

void Sum2(int iter)
{
    while (iter--)
    {
        mu.lock();
        result++;
        mu.unlock();
    }
}

void Sum3(int iter)
{
    while (true)
    {
        if (mu.try_lock())
        {
            result++;
            mu.unlock();
            iter--;
            if (iter == 0) break;
        }
        else
            printf("[THREAD %d] Wait...\n", this_thread::get_id());
    }
}

void Sum4(int iter)
{
    while (true)
    {
        if (spinlock.test_and_set())
        {
            result++;
            spinlock.clear();
            iter--;
            if (iter == 0) break;
        }
        else
            printf("[THREAD %d] Wait...\n", this_thread::get_id());
    }
}

int main()
{
    int iter = 1000000, THREADS = 100;

    cout << "[싱글 스레드]" << "\n";
    result = 0;
    for (int i = 0; i < THREADS; ++i)
        Sum1(iter);
    cout << "결과: " << result << "\n\n";

    cout << "[동기화 미적용]" << "\n";
    result = 0;
    vector<thread> threads;
    for (int i = 0; i < THREADS; ++i)
        threads.emplace_back(Sum1, iter);
    for (thread& th : threads)
        th.join();
    cout << "결과: " << result << "\n\n";

    cout << "[Mutex.lock을 통한 동기화]" << "\n";
    result = 0;
    threads.clear();
    for (int i = 0; i < THREADS; ++i)
        threads.emplace_back(Sum2, iter);
    for (thread& th : threads)
        th.join();
    cout << "결과: " << result << "\n\n";

    cout << "[Mutex.try_lock을 통한 동기화]" << "\n";
    result = 0;
    THREADS = 3;
    threads.clear();
    for (int i = 0; i < THREADS; ++i)
        threads.emplace_back(Sum3, iter);
    for (thread& th : threads)
        th.join();
    cout << "결과: " << result << "\n\n";

    cout << "[atomic_flag로 구현된 spinlock을 통한 동기화]" << "\n";
    result = 0;
    threads.clear();
    for (int i = 0; i < THREADS; ++i)
        threads.emplace_back(Sum3, iter);
    for (thread& th : threads)
        th.join();
    cout << "결과: " << result << "\n\n";
    return 0;
}