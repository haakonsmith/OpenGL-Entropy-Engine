// ***initializers***

int SCREEN_HEIGHT = 480;
int SCREEN_WIDTH = 640;

int init_main(){

    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }
    printf("main init");
    return 0;
}

GLFWwindow *init_window()
{

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return window;
    }

    glClearColor(0.0, 0.0, 0.0, 0.0);            //Set the cleared screen colour to black
    glViewport(0, 0, 1000, 1000); //This sets up the viewport so that the coordinates (0, 0) are at the top left of the window

    //Set up the orthographic projection so that coordinates (0, 0) are in the top left
    //and the minimum and maximum depth is -10 and 10. To enable depth just put in
    //glEnable(GL_DEPTH_TEST)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -10, 10);

    //Back to the modelview so we can draw stuff
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT); //Clear the screen and depth buffer

    printf("window init");
    return window;

    
}

