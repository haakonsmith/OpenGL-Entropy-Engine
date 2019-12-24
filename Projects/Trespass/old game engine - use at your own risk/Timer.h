
class Timer
{
private:
  double time;
public:
  int getTicks();
  Timer();
  ~Timer();
};

Timer::Timer()
{
  time = glfwGetTime();
}

Timer::~Timer()
{
}
