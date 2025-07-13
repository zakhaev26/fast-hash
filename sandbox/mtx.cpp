#include <iostream>
#include <thread>
#include <mutex>

int counter = 0;
std::mutex mtx;

void incrementSafe()
{
    for (int i = 0; i < 1'00'000; ++i)
    {
        std::unique_lock<std::mutex> lock(mtx);
        ++counter;
        lock.unlock();
    }
}

int main()
{
    for (int i = 0; i < 1000; ++i)
    {
        counter = 0;
        std::thread t1(incrementSafe);
        std::thread t2(incrementSafe);
        t1.join();
        t2.join();

        if (counter != 20000)
            std::cout << "Race detected: " << counter << std::endl;
    }
}
