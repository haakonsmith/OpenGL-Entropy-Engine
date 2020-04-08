
#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

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
            std::mutex writeMutex;

            const long long minProfile = 5;

            size_t profileCount = 0;
            int frameCount = 1;

            std::chrono::steady_clock::time_point t_start;
            std::chrono::steady_clock::time_point t_frame_start;

            ofstream profileFile;

            bool writingToFile = false;

          public:
            double fps = 0;

            inline long long getTime() {
                return std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now())
                    .time_since_epoch()
                    .count();
            }

            inline void newFrame() {
                fps = frameCount / 
                      (double) std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t_start).count();

                fps = fps * 1000;
                frameCount++;

                t_frame_start = std::chrono::high_resolution_clock::now();
            }

            inline void endFrame() {
                auto frame_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t_frame_start);
                std::this_thread::sleep_for((std::chrono::milliseconds(16) - frame_duration));
            }

            inline void addProfileToFrame(Profile const& profile) {
                if ((profile.end - profile.start) > minProfile) { writeProfile(profile); }
            }

            inline void writeProfile(Profile const& profile) {
                std::lock_guard<std::mutex> guard(writeMutex);

                if (profileCount++ > 0) profileFile << ",";

                profileFile << "{";
                profileFile << "\"cat\": \"function\",";
                profileFile << "\"dur\": \"" << (profile.end - profile.start) << "\",";
                profileFile << "\"name\": \"" << profile.title << "\",";
                profileFile << "\"ph\": \"X\",";
                profileFile << "\"pid\": \"0\",";
                profileFile << "\"tid\": \"" << std::this_thread::get_id() << "\",";
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

                system("python3 Tracing/pack_zip.py");
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
                    std::chrono::time_point_cast<std::chrono::microseconds>(t_start).time_since_epoch().count();
                profile.end = std::chrono::time_point_cast<std::chrono::microseconds>(t_end).time_since_epoch().count();
                profile.title = name;

                App::profiler.addProfileToFrame(profile);
            }
        };
    }  // namespace Performance

}  // namespace Entropy
