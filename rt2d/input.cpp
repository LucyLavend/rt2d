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

	for (unsigned int i = 0; i < GLFW_JOYSTICK_LAST; i++) {
		setupJoystick(i);
	}

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

	// joystick buttons
	for (unsigned int i = 0; i < 40; i++) {
		_joyButtons[i] = false;
		_joyButtonsUp[i] = false;
		_joyButtonsDown[i] = false;
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

void joystickCallback(int joyID, int event)
{
	if (event == GLFW_CONNECTED)
	{
		std::cout << "Joystick/gamepad connected" << std::endl;
		Singleton<Input>::instance()->setupJoystick(joyID);
	}
	else if (event == GLFW_DISCONNECTED)
	{
		std::cout << "Joystick " << joyID << " has been disconnected." << std::endl;
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

	// gamepad buttons
	for (unsigned int i = 0; i < 40; i++) {
		_handleJoyButton(i);
	}
}

void Input::setupJoystick(int joyNumber) {

	if (glfwJoystickPresent(joyNumber)) {

		std::cout << "Name: " << glfwGetJoystickName(joyNumber) << std::endl;

		int buttonCount;
		joyButtons = glfwGetJoystickButtons(joyNumber, &buttonCount);
		std::cout << "Amount of buttons: " << buttonCount << std::endl;

		int axesCount;
		joyAxes = glfwGetJoystickAxes(joyNumber, &axesCount);
		std::cout << "Amount of axes: " << axesCount << std::endl;
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

void Input::_handleJoyButton(unsigned int button)
{
	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	if (present == 1) {
		if (joyButtons[button] == GLFW_PRESS) {
			if (_joyButtons[button] == false) { // if first time pressed down
				_joyButtons[button] = true;
				_joyButtons[button] = true;
				std::cout << "DOWN: " << button << std::endl;
			}
			else {
				// not the first time this is pressed
				// button[button] is still true;
				_joyButtonsDown[button] = false;
			}
		}
		if (joyButtons[button] == GLFW_RELEASE) {
			if (_joyButtons[button] == true) { // still pressed
				_joyButtons[button] = false;
				_joyButtonsUp[button] = true;
				std::cout << "UP: " << button << std::endl;
			}
			else {
				_joyButtonsUp[button] = false;
			}
		}
	}
}