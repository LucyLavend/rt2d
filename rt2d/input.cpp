/**
 * This file is part of RT2D, a 2D OpenGL framework.
 *
 * - Copyright 2015 Rik Teerling <rik@onandoffables.com>
 *   - Initial commit
 *   - [meruiden] scaling of window
 */

#include <iostream>
#include <singleton.h>
#include <rt2d/input.h>

Input::Input()
{
	_window = nullptr;

	_windowWidth = 0;
	_windowHeight = 0;


	for(unsigned int i=0; i<GLFW_KEY_LAST; i++) {
		_keys[i] = false;
		_keysUp[i] = false;
		_keysDown[i] = false;
	}
	for (unsigned int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++) {
		_mouse[i] = false;
		_mouseUp[i] = false;
		_mouseDown[i] = false;
	}
}

Input::~Input()
{
	std::cout << "Input destructor" << std::endl;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Singleton<Input>::instance()->mouseScrollX = xoffset;
	Singleton<Input>::instance()->mouseScrollY = yoffset;
}

void joystickCallback(int jid, int event)
{
	if (event == GLFW_CONNECTED)
	{
		std::cout << "Joystick " << glfwGetJoystickName(jid) << " (" << jid << ") " << " has been connected." << std::endl;
	}
	else if (event == GLFW_DISCONNECTED)
	{
		std::cout << "Joystick " << jid << " has been disconnected." << std::endl;
	}
}

void Input::updateInput(GLFWwindow* w)
{
	Singleton<Input>::instance()->mouseScrollX = 0;
	Singleton<Input>::instance()->mouseScrollY = 0;

	_window = w;
	
	glfwSetScrollCallback(_window, scrollCallback);
	glfwSetJoystickCallback(joystickCallback);
	glfwPollEvents();

	// 32-97 = ' ' to '`'
	for(unsigned int i=32; i<97; i++) {
		_handleKey(i);
	}
	// Func + arrows + esc, etc
	for(unsigned int i=255; i<GLFW_KEY_LAST; i++) {
		_handleKey(i);
	}
	//  window size
	glfwGetWindowSize(_window, &_windowWidth, &_windowHeight);

	glfwGetCursorPos(_window, &_mouseX, &_mouseY);

	// Fix cursor position if window size is different from the set resolution
	_mouseX = ((float)SWIDTH / _windowWidth) * _mouseX;
	_mouseY = ((float)SHEIGHT / _windowHeight) * _mouseY;

	// mouse buttons
	for(unsigned int i=0; i<GLFW_MOUSE_BUTTON_LAST; i++) {
		_handleMouse(i);
	}

	// gamepad
	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	//std::cout << "Joystick 1 status: " << present << std::endl;

	if (present == 1) {
		int axesCount;
		const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		//std::cout << "Number of axes: " << axesCount << std::endl;

		int buttonCount;
		const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		if (GLFW_PRESS == buttons[1]) {
			std::cout << "X button pressed" << std::endl;
		}

		const char *name = glfwGetJoystickName(GLFW_JOYSTICK_1);
		//std::cout << name << std::endl;
	}
}

void Input::_handleMouse(unsigned int button)
{
	if (glfwGetMouseButton( _window, button ) == GLFW_PRESS) {
		if (_mouse[button] == false) { // if first time pressed down
			_mouse[button] = true;
			_mouseDown[button] = true;
			_mouseUp[button] = false;//added by mike
			//std::cout << "DOWN: " << button << std::endl;
		} else {
			// not the first time this is pressed
			// keys[button] is still true;
			_mouseDown[button] = false;
		}
	}
	if (glfwGetMouseButton( _window, button ) == GLFW_RELEASE) {
		if (_mouse[button] == true) { // still pressed
			_mouse[button] = false;
			_mouseUp[button] = true;
			_mouseDown[button] = false;//added by mike
			//std::cout << "UP: " << button << std::endl;
		} else {
			_mouseUp[button] = false;
		}
	}
}

void Input::_handleKey(unsigned int key)
{
	if (glfwGetKey( _window, key) == GLFW_PRESS) {
		if (_keys[key] == false) { // if first time pressed down
			_keys[key] = true;
			_keysDown[key] = true;
			//std::cout << "DOWN: " << key << std::endl;
		} else {
			// not the first time this is pressed
			// keys[key] is still true;
			_keysDown[key] = false;
		}
	}
	if (glfwGetKey( _window, key) == GLFW_RELEASE) {
		if (_keys[key] == true) { // still pressed
			_keys[key] = false;
			_keysUp[key] = true;
			//std::cout << "UP: " << key << std::endl;
		} else {
			_keysUp[key] = false;
		}
	}
}
