game:
	clang++ -framework OpenGL -std=c++17 -stdlib=libc++ -w -Weverything main.cpp include/Entropy/Application.cpp include/Entropy/Graphics/2dRenderer.cpp include/main/shader.cpp -o play -I include -gfull -L lib -l glfw

test:
	clang++ -framework OpenGL -std=c++17 -stdlib=libc++ -w -Weverything test.cpp -o test -I include -L lib -l glfw

test2:
	clang++ -framework OpenGL -std=c++17 -stdlib=libc++ -w -Weverything main.cpp -o play -I include -L lib -l glfw -g -O0 -Weffc++