#include "Application.hpp"

#include "pch.gch"

namespace Entropy {

    class PerformatApplication : public BaseApplication {
        public:

        void loop() override;

    };

    void PerformatApplication::loop() {

        glfwPollEvents();
    }

}  // namespace Entropy