#include "Application.hpp"

#include <chrono>

#include "Profiler.hpp"

namespace Entropy {

    class PerformatApplication : public BaseApplication {
        public:

        void loop() override;

    };

    void PerformatApplication::loop() {
        App::profiler.newFrame();
        glfwPollEvents();
    }

}  // namespace Entropy