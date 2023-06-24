#include <iostream>
#include <map>
#include <string>
#include <ctime>
#include <stdexcept>

class MetricRegister {
private:
    std::map<int, std::pair<int, int>> metrics;
    static MetricRegister* instance;
    MetricRegister() {}

public:
    static MetricRegister* getInstance() {
        return instance ? instance : instance = new MetricRegister();
    }

    bool add_metr(int resp_t, int wait_t) {
        if (resp_t >= 0 && wait_t >= 0) {
            int timestamp = metrics.size() + 1;
            metrics.emplace(timestamp, std::make_pair(resp_t, wait_t));
            return true;
        }
        else {
            return false;
        }
    }

    void show_metr() {
        if (metrics.empty()) {
            std::cout << "Metrics is empty" << std::endl;
            return;
        }
        std::cout << "Metrics:" << std::endl;
        for (const auto& metric : metrics) {
            int timestamp = metric.first;
            int responseTime = metric.second.first;
            int waitTime = metric.second.second;
            std::cout << "Timestamp #" << timestamp << ", response time: " << responseTime << ", wait time: " << waitTime << std::endl;
        }
    }

    int count_cometr(int key) {
        auto metricIter = metrics.find(key);
        if (metricIter != metrics.end()) {
            const auto& metric = metricIter->second;
            return metric.first + metric.second;
        }
        throw std::out_of_range("Invalid key");
    }

    void test() {
        for (int i = 0; i < 5; i++) {
            int x = rand();
            int y = rand();
            add_metr(x, y);
            if (metrics[metrics.size()] == std::pair<int, int>(x, y)) {
                std::cout << "Test #" << 2 * i + 1 << ": passed" << std::endl;
            }
            else {
                std::cout << "Test #" << 2 * i + 1 << ": failed" << std::endl;
            }
            if (count_cometr(metrics.size()) == x + y) {
                std::cout << "Test #" << 2 * i + 2 << ": passed" << std::endl;
            }
            else {
                std::cout << "Test #" << 2 * i + 2 << ": failed" << std::endl;
            }
        }
        metrics.clear();
    }
};

MetricRegister* MetricRegister::instance = nullptr;

int main() {
    srand(time(NULL));
    MetricRegister::getInstance()->test();

    std::string input;
    while (std::getline(std::cin, input)) {
        if (input.find("add_metr") != std::string::npos) {
            std::string add_metrCmd = "add_metr";
            size_t cmdLength = add_metrCmd.length();
            size_t cmdIndex = input.find(add_metrCmd);
            try {
                std::string arguments = input.substr(cmdIndex + cmdLength + 1);
                int spaceIndex = arguments.find(' ');
                if (spaceIndex == -1) {
                    std::cout << "Invalid command format, print two values of times" << std::endl;
                }
                else {
                    int resp_t = std::stoi(arguments.substr(0, spaceIndex));
                    int wait_t = std::stoi(arguments.substr(spaceIndex + 1));
                    if (MetricRegister::getInstance()->add_metr(resp_t, wait_t)) {
                        std::cout << "Metrics with response time: " << resp_t << ", wait time: "
                            << wait_t << " successfully added" << std::endl;
                    }
                    else {
                        std::cout << "Times must have non-negative values" << std::endl;
                    }
                }
            }
            catch (...) {
                std::cout << "Invalid command format" << std::endl;
            }
        }
        else if (input.find("show_metr") != std::string::npos) {
            MetricRegister::getInstance()->show_metr();
        }
        else if (input.find("count_cometr") != std::string::npos) {
            std::string count_cometrCmd = "count_cometr";
            size_t cmdLength = count_cometrCmd.length();
            size_t cmdIndex = input.find(count_cometrCmd);
            std::string arguments = input.substr(cmdIndex + cmdLength);
            try {
                int key = std::stoi(arguments);
                std::cout << "Total metric: " << MetricRegister::getInstance()->count_cometr(key) << std::endl;
            }
            catch (const std::invalid_argument& ex) {
                std::cout << "Invalid key format" << std::endl;
            }
            catch (const std::out_of_range& ex) {
                std::cout << "Key out of range" << std::endl;
            }
        }
        else {
            std::cout << "Undefined command" << std::endl;
        }
    }
    std::cin.ignore();
    return 0;
}