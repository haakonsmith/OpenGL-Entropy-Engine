#include "Application.hpp"

#include <chrono>

#include "Profiler.hpp"

namespace Entropy {

    class PerformatApplication : public BaseApplication {
        public:

        void loop() override;

    };

    void PerformatApplication::loop() {

        glfwPollEvents();
    }

}  // namespace Entropy