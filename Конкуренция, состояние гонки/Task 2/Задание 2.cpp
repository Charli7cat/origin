#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <iomanip>
#include <sstream>
#include <atomic>
#include <condition_variable>

class MultiThreadProgress {
private:
    struct ThreadData {
        int threadNumber;
        std::thread::id threadId;
        int progress;
        int totalSteps;
        bool completed;
        double duration;
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> endTime;

        ThreadData(int num, int steps)
            : threadNumber(num), progress(0), totalSteps(steps),
            completed(false), duration(0.0) {
        }
    };

    int numThreads;
    int totalSteps;
    int barWidth;
    std::vector<ThreadData> threads;
    std::mutex consoleMutex;
    std::atomic<bool> running;
    std::atomic<int> completedThreads;

    std::mutex startMutex;
    std::condition_variable startCV;
    bool startSignal;

public:
    MultiThreadProgress(int threadsCount, int steps, int width = 50)
        : numThreads(threadsCount), totalSteps(steps), barWidth(width),
        running(true), completedThreads(0), startSignal(false) {

        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(i + 1, steps);
        }
    }

    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void moveCursorHome() {
        std::cout << "\033[H";
    }

    std::string threadIdToString(std::thread::id id) {
        std::stringstream ss;
        ss << id;
        return ss.str();
    }

    std::string getProgressBar(int progress, int total, int width) {
        std::string bar;
        float progressRatio = static_cast<float>(progress) / total;
        int progressWidth = static_cast<int>(progressRatio * width);

        bar += "[";
        for (int i = 0; i < width; ++i) {
            if (i < progressWidth) {
                if (i < width * 0.3) {
                    bar += "?";
                }
                else if (i < width * 0.6) {
                    bar += "?";
                }
                else if (i < width * 0.9) {
                    bar += "?";
                }
                else {
                    bar += "?";
                }
            }
            else {
                bar += " ";
            }
        }
        bar += "]";

        bar += " " + std::to_string(static_cast<int>(progressRatio * 100)) + "%";

        return bar;
    }

    void updateDisplay() {
        std::lock_guard<std::mutex> lock(consoleMutex);

        moveCursorHome();

        std::cout << "=== ÌÍÎÃÎÏÎÒÎ×ÍÛÉ ÐÀÑ×¨Ò Ñ ÏÐÎÃÐÅÑÑ-ÁÀÐÀÌÈ ===" << std::endl;
        std::cout << "Ïîòîêîâ: " << numThreads << " | Øàãîâ: " << totalSteps << std::endl;
        std::cout << std::string(80, '=') << std::endl;

        for (auto& thread : threads) {
            std::cout << "Ïîòîê #" << std::setw(2) << thread.threadNumber;
            std::cout << " [ID: " << threadIdToString(thread.threadId) << "] ";

            if (thread.completed) {
                std::cout << getProgressBar(thread.totalSteps, thread.totalSteps, barWidth);
                std::cout << " Ãîòîâî! Âðåìÿ: " << std::fixed << std::setprecision(3)
                    << thread.duration << "ñ";
            }
            else {
                std::cout << getProgressBar(thread.progress, thread.totalSteps, barWidth);

                auto now = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration<double>(now - thread.startTime).count();
                std::cout << " " << std::fixed << std::setprecision(1)
                    << elapsed << "ñ";
            }
            std::cout << std::endl;
        }

        std::cout << std::string(80, '=') << std::endl;
        std::cout << "Çàâåðøåíî ïîòîêîâ: " << completedThreads.load() << "/" << numThreads << std::endl;
        std::cout.flush();
    }

    void calculate(int threadIndex) {
        auto& data = threads[threadIndex];
        data.threadId = std::this_thread::get_id();
        data.startTime = std::chrono::high_resolution_clock::now();

        {
            std::unique_lock<std::mutex> lock(startMutex);
            startCV.wait(lock, [this]() { return startSignal; });
        }

        for (int step = 0; step < totalSteps && running; ++step) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50 + rand() % 30));

            data.progress = step + 1;

            updateDisplay();
        }

        data.endTime = std::chrono::high_resolution_clock::now();
        data.duration = std::chrono::duration<double>(data.endTime - data.startTime).count();
        data.completed = true;
        completedThreads++;

        updateDisplay();
    }

    void run() {
        clearScreen();

        std::cout << "=== ÌÍÎÃÎÏÎÒÎ×ÍÛÉ ÐÀÑ×¨Ò Ñ ÏÐÎÃÐÅÑÑ-ÁÀÐÀÌÈ ===" << std::endl;
        std::cout << "Ïîòîêîâ: " << numThreads << " | Øàãîâ: " << totalSteps << std::endl;
        std::cout << std::string(80, '=') << std::endl;

        std::vector<std::thread> threadsPool;
        for (int i = 0; i < numThreads; ++i) {
            threadsPool.emplace_back(&MultiThreadProgress::calculate, this, i);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        {
            std::lock_guard<std::mutex> lock(startMutex);
            startSignal = true;
            startCV.notify_all();
        }

        for (auto& thread : threadsPool) {
            thread.join();
        }

        std::cout << "\n" << std::string(80, '=') << std::endl;
        std::cout << "ÂÑÅ ÏÎÒÎÊÈ ÇÀÂÅÐØÈËÈ ÐÀÁÎÒÓ!" << std::endl;

        std::cout << "\n=== ÈÒÎÃÎÂÀß ÑÒÀÒÈÑÒÈÊÀ ===" << std::endl;
        for (auto& data : threads) {
            std::cout << "Ïîòîê #" << std::setw(2) << data.threadNumber
                << " [ID: " << threadIdToString(data.threadId) << "] "
                << "Âðåìÿ: " << std::fixed << std::setprecision(3)
                << data.duration << "ñ" << std::endl;
        }

        double totalTime = 0.0;
        for (auto& data : threads) {
            totalTime += data.duration;
        }
        double avgTime = totalTime / numThreads;
        std::cout << "\nÑðåäíåå âðåìÿ âûïîëíåíèÿ: " << std::fixed << std::setprecision(3)
            << avgTime << "ñ" << std::endl;
    }

    void setCalculationSpeed(int minDelayMs, int maxDelayMs) {
    }
};

class AdvancedMultiThreadProgress {
private:
    struct ThreadData {
        int threadNumber;
        std::thread::id threadId;
        int progress;
        int totalSteps;
        bool completed;
        double duration;
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
        int delayMs;

        ThreadData(int num, int steps, int delay)
            : threadNumber(num), progress(0), totalSteps(steps),
            completed(false), duration(0.0), delayMs(delay) {
        }
    };

    int numThreads;
    int totalSteps;
    int barWidth;
    std::vector<ThreadData> threads;
    std::mutex consoleMutex;
    std::atomic<bool> running;
    std::atomic<int> completedThreads;
    std::mutex startMutex;
    std::condition_variable startCV;
    bool startSignal;

    std::string getThreadIdString(std::thread::id id) {
        std::stringstream ss;
        ss << id;
        return ss.str();
    }

    std::string getProgressBar(int progress, int total, int width) {
        std::string bar;
        float ratio = static_cast<float>(progress) / total;
        int filled = static_cast<int>(ratio * width);

        bar += "[";
        for (int i = 0; i < width; ++i) {
            if (i < filled) {
                if (ratio < 0.3) bar += "?";
                else if (ratio < 0.6) bar += "?";
                else if (ratio < 0.85) bar += "?";
                else bar += "?";
            }
            else {
                bar += " ";
            }
        }
        bar += "]";

        bar += " " + std::to_string(static_cast<int>(ratio * 100)) + "%";

        return bar;
    }

    void updateDisplay() {
        std::lock_guard<std::mutex> lock(consoleMutex);

        std::cout << "\033[H";
        std::cout << "\033[J";

        std::cout << "?????????????????????????????????????????????????????????????????????????????" << std::endl;
        std::cout << "?              ÌÍÎÃÎÏÎÒÎ×ÍÛÉ ÐÀÑ×¨Ò Ñ ÏÐÎÃÐÅÑÑ-ÁÀÐÀÌÈ                      ?" << std::endl;
        std::cout << "?????????????????????????????????????????????????????????????????????????????" << std::endl;
        std::cout << "? Ïîòîêîâ: " << std::setw(3) << numThreads
            << " | Øàãîâ: " << std::setw(5) << totalSteps
            << " | ßäåð: " << std::thread::hardware_concurrency()
            << "                                          ?" << std::endl;
        std::cout << "?????????????????????????????????????????????????????????????????????????????" << std::endl;

        for (auto& data : threads) {
            std::cout << "? ";
            std::cout << "Ïîòîê #" << std::setw(2) << data.threadNumber;
            std::cout << " [ID: " << std::setw(8) << getThreadIdString(data.threadId) << "] ";

            if (data.completed) {
                std::cout << getProgressBar(data.totalSteps, data.totalSteps, barWidth);
                std::cout << " ? Ãîòîâ! ";
                std::cout << std::fixed << std::setprecision(3) << data.duration << "ñ";
            }
            else {
                std::cout << getProgressBar(data.progress, data.totalSteps, barWidth);
                auto now = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration<double>(now - data.startTime).count();
                std::cout << " ? " << std::fixed << std::setprecision(1) << elapsed << "ñ";
            }

            int remainingSpace = 80 - 50 - 15 - 10;
            for (int i = 0; i < remainingSpace; ++i) std::cout << " ";
            std::cout << " ?" << std::endl;
        }

        std::cout << "?????????????????????????????????????????????????????????????????????????????" << std::endl;
        std::cout << "? Çàâåðøåíî: " << std::setw(3) << completedThreads.load()
            << "/" << std::setw(3) << numThreads << " ïîòîêîâ";

        if (completedThreads.load() == numThreads) {
            std::cout << " ? ÂÑÅ ÏÎÒÎÊÈ ÇÀÂÅÐØÅÍÛ!";
        }

        int remainingSpace = 80 - 25 - 15;
        for (int i = 0; i < remainingSpace; ++i) std::cout << " ";
        std::cout << "?" << std::endl;

        std::cout << "?????????????????????????????????????????????????????????????????????????????" << std::endl;
        std::cout.flush();
    }

    void calculate(int threadIndex) {
        auto& data = threads[threadIndex];
        data.threadId = std::this_thread::get_id();
        data.startTime = std::chrono::high_resolution_clock::now();

        {
            std::unique_lock<std::mutex> lock(startMutex);
            startCV.wait(lock, [this]() { return startSignal; });
        }

        for (int step = 0; step < totalSteps && running; ++step) {
            int variation = (data.threadNumber * 7 + step * 3) % 30;
            int delay = data.delayMs + variation;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));

            data.progress = step + 1;
            updateDisplay();
        }

        data.endTime = std::chrono::high_resolution_clock::now();
        data.duration = std::chrono::duration<double>(data.endTime - data.startTime).count();
        data.completed = true;
        completedThreads++;
        updateDisplay();
    }

public:
    AdvancedMultiThreadProgress(int threads, int steps, int width = 50)
        : numThreads(threads), totalSteps(steps), barWidth(width),
        running(true), completedThreads(0), startSignal(false) {

        int baseDelay = 50 + (threads > 8 ? 20 : 0);

        for (int i = 0; i < numThreads; ++i) {
            int delay = baseDelay + (i % 3) * 10;
            threads.emplace_back(i + 1, steps, delay);
        }
    }

    void run() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        std::vector<std::thread> threadPool;
        for (int i = 0; i < numThreads; ++i) {
            threadPool.emplace_back(&AdvancedMultiThreadProgress::calculate, this, i);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        {
            std::lock_guard<std::mutex> lock(startMutex);
            startSignal = true;
            startCV.notify_all();
        }

        for (auto& thread : threadPool) {
            thread.join();
        }

        std::cout << "\n\n" << std::string(80, '=') << std::endl;
        std::cout << "ÈÒÎÃÎÂÀß ÑÒÀÒÈÑÒÈÊÀ:" << std::endl;
        std::cout << std::string(80, '-') << std::endl;

        for (auto& data : threads) {
            std::cout << "Ïîòîê #" << std::setw(2) << data.threadNumber
                << " [ID: " << getThreadIdString(data.threadId) << "] "
                << "Âðåìÿ: " << std::fixed << std::setprecision(3)
                << data.duration << "ñ" << std::endl;
        }

        double totalTime = 0.0;
        for (auto& data : threads) {
            totalTime += data.duration;
        }

        std::cout << std::string(80, '-') << std::endl;
        std::cout << "Îáùåå âðåìÿ: " << std::fixed << std::setprecision(3)
            << totalTime << "ñ" << std::endl;
        std::cout << "Ñðåäíåå âðåìÿ: " << std::fixed << std::setprecision(3)
            << (totalTime / numThreads) << "ñ" << std::endl;
        std::cout << "Àïïàðàòíûõ ÿäåð: " << std::thread::hardware_concurrency() << std::endl;
    }

    void setSpeedMultiplier(double multiplier) {
        for (auto& data : threads) {
            data.delayMs = static_cast<int>(data.delayMs * multiplier);
            if (data.delayMs < 10) data.delayMs = 10;
        }
    }
};

int main() {
    std::cout << "\033[2J\033[H"; // Î÷èñòêà ýêðàíà

    std::cout << "??????????????????????????????????????????????????????????????????" << std::endl;
    std::cout << "?        ÌÍÎÃÎÏÎÒÎ×ÍÛÉ ÐÀÑ×¨Ò Ñ ÏÐÎÃÐÅÑÑ-ÁÀÐÀÌÈ               ?" << std::endl;
    std::cout << "??????????????????????????????????????????????????????????????????" << std::endl;
    std::cout << std::endl;

    // Ïîëó÷åíèå ïàðàìåòðîâ îò ïîëüçîâàòåëÿ
    int numThreads, steps, barWidth;

    std::cout << "Ââåäèòå êîëè÷åñòâî ïîòîêîâ (ðåêîìåíäóåòñÿ 2-16): ";
    std::cin >> numThreads;
    if (numThreads < 1) numThreads = 1;
    if (numThreads > 32) numThreads = 32;

    std::cout << "Ââåäèòå êîëè÷åñòâî øàãîâ ðàñ÷¸òà (ðåêîìåíäóåòñÿ 20-100): ";
    std::cin >> steps;
    if (steps < 5) steps = 5;
    if (steps > 200) steps = 200;

    std::cout << "Ââåäèòå øèðèíó ïðîãðåññ-áàðà (ðåêîìåíäóåòñÿ 30-60): ";
    std::cin >> barWidth;
    if (barWidth < 10) barWidth = 10;
    if (barWidth > 100) barWidth = 100;

    std::cout << std::endl;
    std::cout << "Âûáåðèòå ðåæèì îòîáðàæåíèÿ:" << std::endl;
    std::cout << "1. Ñòàíäàðòíûé" << std::endl;
    std::cout << "2. Ðàñøèðåííûé (ñ ðàìêàìè)" << std::endl;
    std::cout << "Âàø âûáîð: ";
    int mode;
    std::cin >> mode;

    std::cout << std::endl;
    std::cout << "Íàæìèòå Enter äëÿ íà÷àëà...";
    std::cin.ignore();
    std::cin.get();

    if (mode == 2) {
        AdvancedMultiThreadProgress app(numThreads, steps, barWidth);
        app.run();
    }
    else {
        MultiThreadProgress app(numThreads, steps, barWidth);
        app.run();
    }

    std::cout << "\nÍàæìèòå Enter äëÿ âûõîäà...";
    std::cin.get();

    return 0;
}