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
        std::cerr << "无法打开文件: " << filepath << std::endl;
        return;
    }

    std::string line;
    AccountInfo account;

    while (std::getline(file, line)) {
        if (!line.empty()) {
            if (account.serviceProvider.empty()) {
                // 该行是服务提供商
                account.serviceProvider = line;
            }
            else if (account.username.empty()) {
                // 该行是用户名
                account.username = line;
            }
            else if (account.password.empty()) {
                // 该行是密码
                account.password = line;
                // 当前账号信息已完整，添加到列表
                accounts.push_back(account);
                account = AccountInfo(); // 重置 account 以处理下一个账号
            }
        }
    }

    file.close();
}
