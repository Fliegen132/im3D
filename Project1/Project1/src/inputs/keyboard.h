#ifndef KEYBOARD_H
#define KEYBOARD_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>

class Keyboard
{
public:
	//key callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static bool key(int key);
	static bool keyChanged(int key);
	static bool keyCodeUp(int key);
	static bool keyCodeDown(int key);
private: 
	static bool keys[];
	static bool keysChanged[];
};

#endif // !KEYBOARD_H
