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
    destBuffer[bufferSize - 1] = '\0'; // ȷ���ַ����� null ��β
}

struct AccountInfo
{
    std::string username;
    std::string password;
	std::string serviceProvider; // ���˺ŷ����ṩ��
	std::string hint;   // ��ʾ��Ϣ
};

bool editInputInitialized = false; // �Ƿ��Ѿ���ʼ���޸�����������

// ת�� AccountInfo �� JSON ����
void to_json(nlohmann::json& j, const AccountInfo& account)
{
    j = nlohmann::json{ {"username", account.username}, {"password", account.password},{"provider", account.serviceProvider} ,{"hint", account.hint} };
}

// �� JSON ����ת���� AccountInfo
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
    // �ļ�·��
    std::filesystem::path filepath = "accounts.json";
};

FileInfo fileInfo;

// ʾ���˻�����
/*

*/
std::vector<AccountInfo> accounts = {
    {"user1", "password1"},
    // ��Ӹ����˻���Ϣ...
};

std::vector<AccountInfo>& LoadAccountsFromFile(const std::filesystem::path& filepath)
{
    std::ifstream inFile(filepath);

    if (!inFile.is_open())
    {
        std::cerr << "�޷����ļ�: " << filepath << std::endl;
        return accounts;
    }

    nlohmann::json j;
    inFile >> j;  // ���ļ��е�JSON���ݼ��ص�nlohmann::json������

    try
    {
        accounts = j.get<std::vector<AccountInfo>>();  // �����л�Ϊ std::vector<AccountInfo>
    }
    catch (const nlohmann::json::exception& e)
    {
        std::cerr << "JSON ��������: " << e.what() << std::endl;
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
        std::cerr << "�޷����ļ�: " << filepath << std::endl;
        return;
    }

    outFile << j.dump(4); // ����Ϊ4���ո񣬷����Ķ�
    outFile.close();
}

void PushColoredButton(ImVec4 color= ImVec4(0.2f, 0.7f, 0.3f, 1.0f))
{
    // ���İ�ť��ɫ (ImGuiCol_Button ��ʾ��ť�ı�����ɫ)
    ImGui::PushStyleColor(ImGuiCol_Button, color);         // ��ť�ı�����ɫ
	color.x += 0.1f; color.y += 0.1f; color.z += 0.1f;
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);  // �����ͣʱ����ɫ
    color.x -= 0.2f; color.y -= 0.2f; color.z -= 0.2f;
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);   // �����ʱ����ɫ
}

void PopColoredButton()
{
    // �ָ�ԭ������ɫ
    ImGui::PopStyleColor(3);  // 3��ʾ����֮ǰ������3����ɫ����
}

void DrawGUI()
{
	ImGui::Begin(u8"���봰��");

	// draw rect area
	//ImDrawList* DrawList = ImGui::GetForegroundDrawList();
	//DrawList->AddRectFilled(ImVec2(0, 0), ImGui::GetMousePos(), ImColor(60, 255, 51));

	ImGui::End();
}

// ��ǰѡ�е��˻�����
int selectedAccountIndex = 0;

// ���ڿ����޸Ĵ��ڵ���ʾ
bool showModifyWindow = false;

void DrawModifyAccountWindow()
{
    if (selectedAccountIndex >= 0 && selectedAccountIndex < accounts.size())
    {
        ImGui::Begin(u8"�޸��˻���Ϣ");

        // ���浱ǰ�û��������룬������ʱ�洢�û�����
        static char providerBuffer[64];
        static char hintBuffer[128];
        static char usernameBuffer[32];
        static char passwordBuffer[32];

        // ����ǰ�˻����û��������뿽����������
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
        // �����û��������������
        ImGui::InputText(u8"�û���*", usernameBuffer, IM_ARRAYSIZE(usernameBuffer));
        ImGui::InputText(u8"����*", passwordBuffer, IM_ARRAYSIZE(passwordBuffer));
		ImGui::InputText(u8"�����ṩ��", providerBuffer, IM_ARRAYSIZE(providerBuffer));
		ImGui::InputText(u8"��ʾ��Ϣ", hintBuffer, IM_ARRAYSIZE(hintBuffer));

        PushColoredButton();

        if (ImGui::Button(u8"����"))
        {
            // ���޸ĺ���û��������뱣��� accounts ����
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
        if (ImGui::Button(u8"�����˻�##"))
        {
			accounts.push_back({ "username", "password","new provider","no hint"});
			selectedAccountIndex = accounts.size() - 1;
            editInputInitialized = false;

            // ���������û��������뱣��� accounts ����
            SaveAccountsToFile(fileInfo.filepath);
        }
        PopColoredButton();

        ImGui::End();
    }
}

// �������ı�
static char searchQuery[128] = "";

void DrawAccountInfos()
{
    ImGui::Begin(u8"�˻��б�");

    // ���������
    ImGui::InputText(u8" ", searchQuery, sizeof(searchQuery));
	ImGui::SameLine();

    PushColoredButton(ImVec4(0.5f, 0.5f, 0.7f, 1.0f));
    if (ImGui::Button(u8"����##"))
    {
        
    }
    PopColoredButton();

    // ����һ����ֱ��������
    ImGui::BeginChild("ScrollArea", ImVec2(0, 600), true, ImGuiWindowFlags_HorizontalScrollbar);

    

    // �����˻���Ϣ������ÿһ���˺ŵ��û���������
    for (auto i=0;i<accounts.size();i++)
    {
        ImGui::TextColored(ImVec4(0.2f, 0.2f, 0.8f, 1.0f), u8"%s                   ", accounts[i].serviceProvider.c_str());
        ImGui::SameLine();
        if (ImGui::Button((u8"�������޸Ĵ����˻�##" + std::to_string(i)).c_str()))
        {
            selectedAccountIndex = i;
            editInputInitialized = false;
        }
        ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), u8"�û���:  ");
        ImGui::SameLine();
        ImGui::Text(u8" %s  ", accounts[i].username.c_str());
        //ImGui::InputText(u8" ", accounts[i].username.data(), IM_ARRAYSIZE(accounts[i].username.data()), ImGuiInputTextFlags_None, 0, accounts[i].username.data());

        if (ImGui::Button((u8"�����û���##" + std::to_string(i)).c_str()))
        {
            ImGui::SetClipboardText(accounts[i].username.c_str());  // �����û�����������
        }
        
        ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), u8"����:  ");
        ImGui::SameLine();
        ImGui::Text(u8" %s  ", accounts[i].password.c_str());

        if (ImGui::Button((u8"��������##" + std::to_string(i)).c_str()))
        {
            ImGui::SetClipboardText(accounts[i].password.c_str());  // �������뵽������
        }

        ImGui::Separator();
    }

    ImGui::EndChild();

    ImGui::End();
}