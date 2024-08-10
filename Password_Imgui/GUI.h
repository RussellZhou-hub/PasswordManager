#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include <string>
#include <cstring>

#include "nlohmann/json.hpp"

void CopyStringToBuffer(const std::string& source, char* destBuffer, size_t bufferSize)
{
    strncpy_s(destBuffer, bufferSize, source.c_str(), bufferSize - 1);
    destBuffer[bufferSize - 1] = '\0'; // 确保字符串以 null 结尾
}

struct AccountInfo
{
    std::string username;
    std::string password;
	std::string serviceProvider; // 该账号服务提供商
	std::string hint;   // 提示信息
};

bool editInputInitialized = false; // 是否已经初始化修改密码的输入框

// 转换 AccountInfo 到 JSON 对象
void to_json(nlohmann::json& j, const AccountInfo& account)
{
    j = nlohmann::json{ {"username", account.username}, {"password", account.password},{"provider", account.serviceProvider} ,{"hint", account.hint} };
}

// 从 JSON 对象转换到 AccountInfo
void from_json(const nlohmann::json& j, AccountInfo& account)
{
    j.at("username").get_to(account.username);
    j.at("password").get_to(account.password);
	j.at("provider").get_to(account.serviceProvider);
    j.at("hint").get_to(account.hint);
}

struct FileInfo
{
	bool isModified = false;
    // 文件路径
    std::filesystem::path filepath = "accounts.json";
};

FileInfo fileInfo;

// 示例账户数据
/*

*/
std::vector<AccountInfo> accounts = {
    {"user1", "password1"},
    // 添加更多账户信息...
};

std::vector<AccountInfo>& LoadAccountsFromFile(const std::filesystem::path& filepath)
{
    std::ifstream inFile(filepath);

    if (!inFile.is_open())
    {
        std::cerr << "无法打开文件: " << filepath << std::endl;
        return accounts;
    }

    nlohmann::json j;
    inFile >> j;  // 将文件中的JSON内容加载到nlohmann::json对象中

    try
    {
        accounts = j.get<std::vector<AccountInfo>>();  // 反序列化为 std::vector<AccountInfo>
    }
    catch (const nlohmann::json::exception& e)
    {
        std::cerr << "JSON 解析错误: " << e.what() << std::endl;
    }

    inFile.close();
    return accounts;
}

void SaveAccountsToFile(const std::filesystem::path& filepath)
{
    nlohmann::json j = accounts;

    std::ofstream outFile(filepath);
    if (!outFile.is_open())
    {
        std::cerr << "无法打开文件: " << filepath << std::endl;
        return;
    }

    outFile << j.dump(4); // 缩进为4个空格，方便阅读
    outFile.close();
}

void PushColoredButton(ImVec4 color= ImVec4(0.2f, 0.7f, 0.3f, 1.0f))
{
    // 更改按钮颜色 (ImGuiCol_Button 表示按钮的背景颜色)
    ImGui::PushStyleColor(ImGuiCol_Button, color);         // 按钮的背景颜色
	color.x += 0.1f; color.y += 0.1f; color.z += 0.1f;
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);  // 鼠标悬停时的颜色
    color.x -= 0.2f; color.y -= 0.2f; color.z -= 0.2f;
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);   // 鼠标点击时的颜色
}

void PopColoredButton()
{
    // 恢复原来的颜色
    ImGui::PopStyleColor(3);  // 3表示我们之前推入了3个颜色属性
}

void DrawGUI()
{
	ImGui::Begin(u8"密码窗口");

	// draw rect area
	//ImDrawList* DrawList = ImGui::GetForegroundDrawList();
	//DrawList->AddRectFilled(ImVec2(0, 0), ImGui::GetMousePos(), ImColor(60, 255, 51));

	ImGui::End();
}

// 当前选中的账户索引
int selectedAccountIndex = 0;

// 用于控制修改窗口的显示
bool showModifyWindow = false;

void DrawModifyAccountWindow()
{
    if (selectedAccountIndex >= 0 && selectedAccountIndex < accounts.size())
    {
        ImGui::Begin(u8"修改账户信息");

        // 缓存当前用户名和密码，用于临时存储用户输入
        static char providerBuffer[64];
        static char hintBuffer[128];
        static char usernameBuffer[32];
        static char passwordBuffer[32];

        // 将当前账户的用户名和密码拷贝到缓冲区
        if (!editInputInitialized)
        {
			/*strncpy_s(usernameBuffer, sizeof(usernameBuffer), accounts[selectedAccountIndex].username.c_str(), sizeof(accounts[selectedAccountIndex].username.c_str()));
			strncpy_s(passwordBuffer, sizeof(usernameBuffer), accounts[selectedAccountIndex].password.c_str(), sizeof(accounts[selectedAccountIndex].password.c_str())+1);*/

            CopyStringToBuffer(accounts[selectedAccountIndex].username, usernameBuffer, sizeof(usernameBuffer));
            CopyStringToBuffer(accounts[selectedAccountIndex].password, passwordBuffer, sizeof(passwordBuffer));
            CopyStringToBuffer(accounts[selectedAccountIndex].serviceProvider , providerBuffer, sizeof(providerBuffer));
			CopyStringToBuffer(accounts[selectedAccountIndex].hint, hintBuffer, sizeof(hintBuffer));

			editInputInitialized = true;
        }
        // 处理用户名和密码的输入
        ImGui::InputText(u8"用户名*", usernameBuffer, IM_ARRAYSIZE(usernameBuffer));
        ImGui::InputText(u8"密码*", passwordBuffer, IM_ARRAYSIZE(passwordBuffer));
		ImGui::InputText(u8"服务提供商", providerBuffer, IM_ARRAYSIZE(providerBuffer));
		ImGui::InputText(u8"提示信息", hintBuffer, IM_ARRAYSIZE(hintBuffer));

        PushColoredButton();

        if (ImGui::Button(u8"保存"))
        {
            // 将修改后的用户名和密码保存回 accounts 数组
            accounts[selectedAccountIndex].username = std::string(usernameBuffer);
            accounts[selectedAccountIndex].password = std::string(passwordBuffer);
			accounts[selectedAccountIndex].serviceProvider = std::string(providerBuffer);
			accounts[selectedAccountIndex].hint = std::string(hintBuffer);
            SaveAccountsToFile(fileInfo.filepath);
        }
        PopColoredButton();

		ImGui::SameLine();
		ImGui::Text(u8"  ");
        ImGui::SameLine();

		PushColoredButton(ImVec4(0.2f, 0.2f, 0.7f, 1.0f));
        if (ImGui::Button(u8"新增账户##"))
        {
			accounts.push_back({ "username", "password","new provider","no hint"});
			selectedAccountIndex = accounts.size() - 1;
            editInputInitialized = false;

            // 将新增的用户名和密码保存回 accounts 数组
            SaveAccountsToFile(fileInfo.filepath);
        }
        PopColoredButton();

        ImGui::End();
    }
}

// 搜索框文本
static char searchQuery[128] = "";

void DrawAccountInfos()
{
    ImGui::Begin(u8"账户列表");

    // 添加搜索框
    ImGui::InputText(u8" ", searchQuery, sizeof(searchQuery));
	ImGui::SameLine();

    PushColoredButton(ImVec4(0.5f, 0.5f, 0.7f, 1.0f));
    if (ImGui::Button(u8"搜索##"))
    {
        
    }
    PopColoredButton();

    // 创建一个垂直滚动区域
    ImGui::BeginChild("ScrollArea", ImVec2(0, 600), true, ImGuiWindowFlags_HorizontalScrollbar);

    

    // 遍历账户信息并绘制每一个账号的用户名和密码
    for (auto i=0;i<accounts.size();i++)
    {
        ImGui::TextColored(ImVec4(0.2f, 0.2f, 0.8f, 1.0f), u8"%s                   ", accounts[i].serviceProvider.c_str());
        ImGui::SameLine();
        if (ImGui::Button((u8"点这里修改此项账户##" + std::to_string(i)).c_str()))
        {
            selectedAccountIndex = i;
            editInputInitialized = false;
        }
        ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), u8"用户名:  ");
        ImGui::SameLine();
        ImGui::Text(u8" %s  ", accounts[i].username.c_str());
        //ImGui::InputText(u8" ", accounts[i].username.data(), IM_ARRAYSIZE(accounts[i].username.data()), ImGuiInputTextFlags_None, 0, accounts[i].username.data());

        if (ImGui::Button((u8"复制用户名##" + std::to_string(i)).c_str()))
        {
            ImGui::SetClipboardText(accounts[i].username.c_str());  // 复制用户名到剪贴板
        }
        
        ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), u8"密码:  ");
        ImGui::SameLine();
        ImGui::Text(u8" %s  ", accounts[i].password.c_str());

        if (ImGui::Button((u8"复制密码##" + std::to_string(i)).c_str()))
        {
            ImGui::SetClipboardText(accounts[i].password.c_str());  // 复制密码到剪贴板
        }

        ImGui::Separator();
    }

    ImGui::EndChild();

    ImGui::End();
}