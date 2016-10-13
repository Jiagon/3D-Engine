#pragma once
#include <GLFW/glfw3.h>
#include <map>

class InputManager
{
private:
public:
	InputManager();
	~InputManager();

	static void mouseClick(GLFWwindow * windowPtr, int button, int action, int mods);
	static void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);

	static std::map<int, bool> keyIsDown;
	static std::map<int, bool> keyWasDown;
};

