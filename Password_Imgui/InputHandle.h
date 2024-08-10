#include <GLFW/glfw3.h>
#include <iostream>



// 键盘事件回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);  // 设置窗口应关闭标志
    }
    else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
    {
        //io.AddKeyEvent(ImGuiKey_Backspace, true);
    }
}