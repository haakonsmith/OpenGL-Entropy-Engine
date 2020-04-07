
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

using std::ofstream;

#pragma once

namespace Entropy {

    namespace Performance {

        struct Profile {
            long long start;
            long long end;
            std::string title;
        };

        class Profiler {
          private:
            const long long minProfile = 50000;

            size_t profileCount = 0;
            std::__1::chrono::steady_clock::time_point t_start;

            ofstream profileFile;

            bool writingToFile = false;

          public:
            inline void addProfileToFrame(Profile const& profile) {
                if ((profile.end - profile.start) > minProfile) { 
                    while (writingToFile) {
                        std::this_thread::sleep_for(std::chrono::microseconds(5));
                    }
                    writeProfile(profile); 
                }
            }

            inline void writeProfile(Profile const& profile) {
                writingToFile = true;

                if (profileCount++ > 0) profileFile << ",";

                profileFile << "{";
                profileFile << "\"cat\": \"function\",";
                profileFile << "\"dur\": \"" << (profile.end - profile.start) << "\",";
                profileFile << "\"name\": \"" << profile.title << "\",";
                profileFile << "\"ph\": \"X\",";
                profileFile << "\"pid\": \"0\",";
                profileFile << "\"tid\": \"0\",";
                profileFile << "\"ts\": \"" << profile.start << "\"";
                profileFile << "}";

                profileFile.flush();

                writingToFile = false;
            }

            Profiler() {
                t_start = std::chrono::high_resolution_clock::now();
                profileFile = ofstream("Tracing/main.json");

                profileFile << "{";
                profileFile << "\"traceEvents\": [";

                profileFile.flush();
            }
            ~Profiler() {
                std::cout << "Done profiling" << std::endl;
                profileFile << "\t],";
                profileFile << "\"displayTimeUnit\": \"ns\"";
                profileFile << "}";

                profileFile.flush();
                profileFile.close();
            }
        };

    }  // namespace Performance

    namespace App {
        extern Performance::Profiler profiler;
    };

    namespace Performance {
        class Timer {
          private:
            std::__1::chrono::steady_clock::time_point t_start;
            std::string name;

          public:
            Timer(std::string title) : name(title) { t_start = std::chrono::high_resolution_clock::now(); }
            ~Timer() {
                auto t_end = std::chrono::high_resolution_clock::now();

                Profile profile;
                profile.start =
                    std::chrono::time_point_cast<std::chrono::nanoseconds>(t_start).time_since_epoch().count();
                profile.end = std::chrono::time_point_cast<std::chrono::nanoseconds>(t_end).time_since_epoch().count();
                profile.title = name;

                App::profiler.addProfileToFrame(profile);
            }
        };
    }  // namespace Performance

}  // namespace Entropy
