#include "InputManager.h"

std::map<int, bool> InputManager::keyIsDown;
std::map<int, bool> InputManager::keyWasDown;

//----------------------------------------------------------------------------------------------------------->>CONSTRUCTOR / DESTRUCTOR<<
InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

//----------------------------------------------------------------------------------------------------------->>HANDLES MOUSE / KEY INPUT<<

void InputManager::mouseClick(GLFWwindow * windowPtr, int button, int action, int mods)
{
	keyIsDown[button] = action;
}

void InputManager::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	keyIsDown[key] = action;
}
