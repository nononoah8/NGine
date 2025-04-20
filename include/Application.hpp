#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>
#include <thread>
#include <cstdlib>
#include <iostream>

class Application {
public:
    static void Quit() {
        exit(0);
    }
    static void Sleep(int milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
    static int GetFrame() {
        // TODO: Implement frame counting logic
        return frameNumber;
    }
    static void OpenUrl(const std::string& url) {
        #ifdef _WIN32
            std::string command = "start " + url;
        #elif __APPLE__
            std::string command = "open " + url;
        #elif __linux__
            std::string command = "xdg-open " + url;
        #endif

        std::system(command.c_str());
    }

    static int frameNumber;
};

#endif