//
// Created by PCS on 3/6/2026.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <conio.h>
#include <cctype>
#include <map>
#include <chrono>

#include "network/TCPServer.h"

int main()
{
    const int PORT = 3000;

    TCPServer server(PORT);

    if (!server.start())
    {
        std::cout << "Failed to start server\n";
        return 1;
    }

    std::ofstream dataset("C:/Users/PCS/Desktop/dataset.csv", std::ios::app);

    if (!dataset.is_open())
    {
        std::cout << "Failed to open dataset\n";
        return 1;
    }

    std::cout << "\nControls:\n";
    std::cout << "  A-Z : start recording that label\n";
    std::cout << "  SPACE : stop recording\n";
    std::cout << "  [ : quit\n\n";

    std::queue<std::vector<double>> featureQueue;
    std::mutex queueMutex;

    std::atomic<bool> running(true);

    char currentLabel = '\0';
    int frameCounter = 0;
    std::map<char,int> sampleCount;
    int totalSamples = 0;

    std::thread receiver([&]()
    {
        while (running)
        {
            auto features = server.receiveVector();

            if (features.empty())
                continue;

            std::lock_guard<std::mutex> lock(queueMutex);
            featureQueue.push(features);
        }
    });

    while (running)
    {
        if (_kbhit())
        {
            char key = _getch();

            if (key == '[')
            {
                std::cout << "Quitting...\n";
                running = false;
                server.stop();
                break;
            }

            if (key == ' ')
            {
                currentLabel = '\0';
                std::cout << "Recording stopped\n";
            }
            else
            {
                key = std::toupper(key);

                if ((key >= 'A' && key <= 'Z') || (key >= '0' && key <= '9'))
                {
                    currentLabel = key;
                    frameCounter = 0;

                    {
                        std::lock_guard<std::mutex> lock(queueMutex);
                        std::queue<std::vector<double>> empty;
                        std::swap(featureQueue, empty);
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(200));

                    std::cout << "Recording label: " << currentLabel << "\n";
                }
            }
        }

        std::vector<double> features;

        {
            std::lock_guard<std::mutex> lock(queueMutex);

            if (!featureQueue.empty())
            {
                features = featureQueue.front();
                featureQueue.pop();
            }
        }

        if (features.empty())
            continue;

        if (currentLabel != '\0')
        {
            frameCounter++;

            if (frameCounter % 10 == 0)
            {
                dataset << currentLabel;

                for (double f : features)
                    dataset << "," << f;

                dataset << "\n";

                sampleCount[currentLabel]++;
                totalSamples++;

                std::cout << "Saved sample for "
                          << currentLabel
                          << " | label count: "
                          << sampleCount[currentLabel]
                          << " | total: "
                          << totalSamples
                          << "\n";
            }
        }
    }

    receiver.detach();

    dataset.close();

    std::cout << "\n===== Dataset Summary =====\n";

    for (const auto& pair : sampleCount)
        std::cout << pair.first << " : " << pair.second << " samples\n";

    std::cout << "Total samples: " << totalSamples << "\n";
    std::cout << "===========================\n";

    return 0;
}