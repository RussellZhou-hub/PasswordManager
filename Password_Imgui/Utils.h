#pragma once
#include<vector>
#include <iostream>
#include<filesystem>
#include<string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

std::string GetExecutablePath() {
    char buffer[MAX_PATH] ;
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::string(buffer);
}

#endif

void LoadAccountsFromTextFile(const std::string& filepath, std::vector<AccountInfo>& accounts) {
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "�޷����ļ�: " << filepath << std::endl;
        return;
    }

    std::string line;
    AccountInfo account;

    while (std::getline(file, line)) {
        if (!line.empty()) {
            if (account.serviceProvider.empty()) {
                // �����Ƿ����ṩ��
                account.serviceProvider = line;
            }
            else if (account.username.empty()) {
                // �������û���
                account.username = line;
            }
            else if (account.password.empty()) {
                // ����������
                account.password = line;
                // ��ǰ�˺���Ϣ����������ӵ��б�
                accounts.push_back(account);
                account = AccountInfo(); // ���� account �Դ�����һ���˺�
            }
        }
    }

    file.close();
}
