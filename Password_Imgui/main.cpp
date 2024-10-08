#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include<filesystem>
#include<string>

#pragma comment(lib,"../API/GLFW/lib-vc2022/glfw3.lib")
#pragma comment(lib,"../API/GLEW/glew32s.lib")
#pragma comment(lib,"Opengl32.lib")

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GUI.h"
#include "InputHandle.h"
#include "Utils.h"

GLFWwindow* Windows;

static std::string executablePath;
static std::string directoryPath;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Windows = glfwCreateWindow(width, height, "Password-Manager",NULL,NULL);

	glfwMakeContextCurrent(Windows);
	glfwSwapInterval(0);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext(NULL);
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	executablePath = GetExecutablePath();
	directoryPath = std::filesystem::path(executablePath).parent_path().string();
	// 修改文件路径，使其基于可执行文件所在目录
	fileInfo.filepath = directoryPath + "\\accounts.json";
	std::string ttfPath = directoryPath + "\\SimHei.ttf";

	//io.Fonts->AddFontFromFileTTF("kaiu.ttf", 30.0f,NULL,io.Fonts->GetGlyphRangesChineseFull());
	io.Fonts->AddFontFromFileTTF(ttfPath.c_str(), 30.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiViewportFlags_NoDecoration;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiCol_DockingEmptyBg;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	io.AddKeyEvent(ImGuiKey_Backspace, true); 
	io.AddKeyEvent(ImGuiKey_Backspace, false);

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();

	ImVec4* colors = style.Colors;
	{
		colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.17f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.37f, 0.36f, 0.36f, 102.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 171.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 255.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.22f, 0.22f, 0.22f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.45f, 0.45f, 0.45f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.46f, 0.46f, 0.46f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.17f, 0.17f, 0.17f, 0.95f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.78f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.45f, 0.45f, 0.45f, 0.80f);
		colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.51f, 0.51f, 0.51f, 0.70f);
		colors[ImGuiCol_Tab] = ImVec4(0.21f, 0.21f, 0.21f, 0.86f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.15f, 0.97f);
		colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.40f, 0.13f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.45f, 1.00f, 0.85f, 0.35f);
	}
	

	style.WindowRounding = 4;
	style.FrameRounding = 4;
	style.GrabRounding = 3;
	style.ScrollbarRounding = 0;
	style.ScrollbarSize = 7;

	// 设置键盘事件回调函数
	glfwSetKeyCallback(Windows, key_callback);
	ImGui_ImplGlfw_InitForOpenGL(Windows, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	

	std::string Text = "Hellow World 123";
	char textBox[50] = "Test Text Box";
	while (!glfwWindowShouldClose(Windows))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport();

		//DrawGUI();

		// 从文件加载账户信息
		std::vector<AccountInfo>& accounts = LoadAccountsFromFile(fileInfo.filepath);
		//std::string txtPath = directoryPath + "\\password.txt";
		//LoadAccountsFromTextFile(txtPath, accounts);
		DrawModifyAccountWindow();
		DrawAccountInfos();

		// 保存账户信息到文件
		if (fileInfo.isModified)
		{
			SaveAccountsToFile(fileInfo.filepath);
			std::cout << "账户信息已保存至: " << std::filesystem::current_path()<<"\\"<< fileInfo.filepath << std::endl;
			fileInfo.isModified = false;
		}
		

		/*ImGui::Begin("MyImguiWindow");
		ImGui::Text(Text.c_str());
		if (ImGui::Button("Save"))
		{
			Text="You Clicked The Button ";
			fileInfo.isModified = true;
			std::cout << std::filesystem::current_path() << '\n';
		}
		ImGui::InputText("Test Text Box",textBox,IM_ARRAYSIZE(textBox));
		ImGui::End();*/

		//ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		
		glfwSwapBuffers(Windows);
		glfwPollEvents();
	}
}