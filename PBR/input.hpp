#ifndef input_hpp
#define input_hpp

enum KEY_INPUT {
	KEY_NONE,
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
	KEY_W, KEY_S, KEY_A, KEY_D
};

int getKeyboardPress(GLFWwindow* window);

#endif /* input_hpp */
