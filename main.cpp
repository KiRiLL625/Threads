#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mutex;
int progress_percent = 0;

void updateProgressPercents() {
    while (progress_percent < 100) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            progress_percent += 1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void imitate() {
    for (int i = 0; i < 10; ++i) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            progress_percent = i * 10;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (i == 9) {
            std::cout << "\nTask Finished" << std::endl;
        }
    }

    {
        std::lock_guard<std::mutex> lock(mutex);
        progress_percent = 100;
    }
}

int main() {
    std::thread progressThread(updateProgressPercents);
    std::thread taskThread(imitate);
    std::vector<char> progress(12);
    progress[0] = '[';
    progress[11] = ']';
    for(int i = 1; i < 11; ++i) {
        progress[i] = '.';
    }
    std::cout << "Progress: ";
    while(progress_percent < 100) {
        std::lock_guard<std::mutex> lock(mutex);
        std::cout << "\rProgress: " << progress_percent << "% " << std::string(progress.begin(), progress.end()) << std::flush;
        progress[1 + progress_percent / 10] = '#';
    }
    progressThread.join();
    taskThread.join();
    return 0;
}
