#include <GLFW/glfw3.h>
#include <iostream>



// �����¼��ص�����
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);  // ���ô���Ӧ�رձ�־
    }
    else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
    {
        //io.AddKeyEvent(ImGuiKey_Backspace, true);
    }
}