#include "../utils/thread_pool.hpp"
#include <chrono>
#include <iostream>

int main(void) {

    std::cout << "Max number of threads that can be executed physicly at the same time : " 
        << std::thread::hardware_concurrency() << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    {
        std::vector<std::future<void>> results;

        const std::size_t threadsNumber = 4;
        ThreadPool pool(threadsNumber);

        // put 10 000 tasks to pool's queue
        for (int i = 0; i < 10000; ++i) {
            results.emplace_back(pool.AddTask([] {
                float ff = 5.34f; //simulate some hard calculations
                for (int index = 0; index < 100000; ++index) {
                    ff += index * 0.01f - 12.f;
                } 
            }));
        }

        // wait until everything s done
        for (auto& r : results) r.get();
    }


    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Execution time : "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << std::endl;
    std::system("pause");
}