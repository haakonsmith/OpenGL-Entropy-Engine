
#ifndef BASEAPPLICATION
  #define BASEAPPLICATION
  #include <GLFW/glfw3.h>
  #include <iostream>
#else
#error "BaseApplication included twice"
#endif



namespace Entropy
{
  class BaseApplication
  {
  private:

    void initWindow(unsigned int SCREEN_WIDTH, unsigned int SCREEN_HEIGHT);
    

  public:
    static void glfwError(int id, const char* description)
    {
      std::cout << description << std::endl;
    }

    GLFWwindow* window;
    virtual void init() {std::cout<<"base" <<std::endl;};
    void mainLoop();
    virtual void loop() {glfwPollEvents();};

    bool initialized = false;


    BaseApplication();
    ~BaseApplication();
  };

} // namespace Entropy

