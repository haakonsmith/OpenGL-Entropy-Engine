
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::ofstream;

#pragma once

namespace Entropy {

    namespace Performance {

        struct Profile {
            std::chrono::duration<long, std::nano> duration;
            std::string title;
        };

        struct FrameProfile {
            std::__1::chrono::steady_clock::time_point start;
            std::chrono::duration<long, std::nano> duration;
            std::vector<Profile> profiles;

            // FrameProfile(std::chrono::duration<long, std::nano> d) : duration(d) {}
        };

        class Profiler {
          private:
            std::vector<FrameProfile> frames;
            std::__1::chrono::steady_clock::time_point t_start;

            ofstream profileFile;

          public:
            void startFrame() {
                frames.push_back(FrameProfile());
                frames.back().start = std::chrono::high_resolution_clock::now();
            }
            void endFrame() {
                frames.back().duration = std::chrono::high_resolution_clock::now() - frames.back().start;
            }

            void addProfileToFrame(std::chrono::duration<long, std::nano> duration, std::string title) {
                Profile profile;

                profile.duration = duration;
                profile.title = title;

                frames.back().profiles.push_back(profile);

                profileFile << "{\n";
                profileFile << '\"cat\": \"' << title << '\",';
                profileFile << '},\n';

                profileFile.flush();
            }

            Profiler() {
                t_start = std::chrono::high_resolution_clock::now();
                profileFile = ofstream("Tracing/main.json");

                profileFile << "{\n";
                profileFile << '\"traceEvents\": [\n';

                profileFile.flush();
            }
            ~Profiler() {
                frames.clear();
                profileFile << '}\n';
                profileFile << '}\n';

                profileFile.flush();
                profileFile.close();
            }
        };

    }  // namespace Performance

    namespace App {
        static Performance::Profiler profiler = Performance::Profiler();
    };

    namespace Performance {
        class Timer {
          private:
            std::__1::chrono::steady_clock::time_point t_start;
            std::string name;

          public:
            Timer(std::string title) : name(title) { t_start = std::chrono::high_resolution_clock::now(); }
            ~Timer() { App::profiler.addProfileToFrame(std::chrono::high_resolution_clock::now() - t_start, name); }
        };
    }  // namespace Performance

}  // namespace Entropy
