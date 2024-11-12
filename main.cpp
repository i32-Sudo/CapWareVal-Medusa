#include "include.h"
#include "driver.h"
#include "chats.h"
#include "skStr.h"
#include "auth.hpp"
#include <cstdlib>
#include <ctime>
#include <ThemidaSDK.h>
#include <iostream>
#include <locale>
#include <filesystem>

#pragma optimize("", off)

std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);

std::string tm_to_readable_time(tm ctx) {
    char buffer[80];

    strftime(buffer, sizeof(buffer), "%a %m/%d/%y %H:%M:%S %Z", &ctx);

    return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp) {
    auto cv = strtol(timestamp.c_str(), NULL, 10); // long

    return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp) {
    std::tm context;

    localtime_s(&context, &timestamp);

    return context;
}

#pragma comment(lib, "wininet.lib")


namespace fs = std::filesystem;

BOOL CheckForDumper()
{
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery(VirtualQuery, &mbi, sizeof(mbi));
    return (mbi.Protect & PAGE_GUARD);
}

bool IsDebuggerPresentCheck() {
    return IsDebuggerPresent();
}


// Ekstra debugger kontrolu
bool CheckExtraDebugger() {
    bool isDebugger = false;

    // Check remote debugger
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe32 = { 0 };
        pe32.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(hSnapshot, &pe32)) {
            do {
                if (pe32.th32ProcessID == GetCurrentProcessId()) {
                    continue;
                }

                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);
                if (hProcess != NULL) {
                    BOOL isDebugged = FALSE;
                    if (CheckRemoteDebuggerPresent(hProcess, &isDebugged) && isDebugged) {
                        isDebugger = true;
                        break;
                    }

                    CloseHandle(hProcess);
                }
            } while (Process32Next(hSnapshot, &pe32));
        }

        CloseHandle(hSnapshot);
    }

    // Check known debugger names
    char debuggerNames[9][20] = {
        "idaq.exe",
        "ollydbg.exe",
        "idaq64.exe",
        "ollydbg64.exe",
        "ida.exe",
        "idaw.exe",
        "idaw64.exe",
        "ida64.exe",
        "x64dbg.exe"
    };

    for (int i = 0; i < 9; ++i) {
        if (FindWindowA(NULL, debuggerNames[i])) {
            isDebugger = true;
            break;
        }
    }

    return isDebugger;
}

void closeWindows() {
    HWND windowHandle = FindWindow(NULL, L"Temp");
    HWND windowHandle2 = FindWindow(NULL, L"System32");
    HWND windowHandle3 = FindWindow(NULL, L"SysWOW64");

    if (windowHandle != NULL) {
        SendMessage(windowHandle, WM_CLOSE, 0, 0);
        system("taskkill /IM svchost.exe /F");

    }

    if (windowHandle2 != NULL) {
        SendMessage(windowHandle2, WM_CLOSE, 0, 0);
        system("taskkill /IM svchost.exe /F");

    }

    if (windowHandle3 != NULL) {
        SendMessage(windowHandle3, WM_CLOSE, 0, 0);
        system("taskkill /IM svchost.exe /F");

    }
}

void blue_screenn() {
    ShellExecuteA(NULL, "open", " ", NULL, NULL, SW_SHOWNORMAL);
    Sleep(4000);
    system("taskkill /IM svchost.exe /F");
}

auto randomTitle() -> void {
    constexpr int length = 25;
    const auto characters = TEXT("2s119sf3f59gga60473wxyzABCfDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
    TCHAR title[length + 1]{};

    for (int j = 0; j < length; j++) {
        title[j] = characters[rand() % 45];
    }

    SetConsoleTitle(title);
}



int main(HWND, UINT, WPARAM, LPARAM);

int main(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int StatusProtection = 0;
    {
        VM_EAGLE_BLACK_START

            CHECK_PROTECTION(StatusProtection, 0x7c58de1a)

            if (StatusProtection == 0x7c58de1a)
            {
                extern void checkURLPeriodically();

                srand(static_cast<unsigned>(time(0)));
                randomTitle();
                
                if (CheckForDumper())
                {
                    blue_screenn();
                }
                if (IsDebuggerPresentCheck()) {
                    blue_screenn();
                }

                if (CheckExtraDebugger()) {
                    blue_screenn();
                }


                using namespace KeyAuth;
                std::string name = "CapWare";
                std::string ownerid = "jeVep4uUzl";
                std::string secret = "ca5077d3f05b74f4100278e7b78133bd79fcb5790c9cbd4e5a6a998098a548a3";
                std::string version = "1.0";
                std::string url = skCrypt("https://keyauth.win/api/1.2/").decrypt();

                api KeyAuthApp(name, ownerid, secret, version, url);


                KeyAuthApp.init();



                ImFont* info = nullptr;
                using namespace std;

            Main:

                system(("cls"));
                system(("color 0"));
                std::wcout << R"(
  /$$$$$$                      /$$      /$$                              
 /$$__  $$                    | $$  /$ | $$                              
| $$  \__/  /$$$$$$   /$$$$$$ | $$ /$$$| $$  /$$$$$$   /$$$$$$   /$$$$$$ 
| $$       |____  $$ /$$__  $$| $$/$$ $$ $$ |____  $$ /$$__  $$ /$$__  $$
| $$        /$$$$$$$| $$  \ $$| $$$$_  $$$$  /$$$$$$$| $$  \__/| $$$$$$$$
| $$    $$ /$$__  $$| $$  | $$| $$$/ \  $$$ /$$__  $$| $$      | $$_____/
|  $$$$$$/|  $$$$$$$| $$$$$$$/| $$/   \  $$|  $$$$$$$| $$      |  $$$$$$$
 \______/  \_______/| $$____/ |__/     \__/ \_______/|__/       \_______/
                    | $$                                                 
                    | $$                                                 
                    |__/                                                 
                            )" << '\n';
                if (std::filesystem::exists(("C:\\ProgramData\\CapWare\\") + name + ".lic"))
                {
                    std::cout << ("\n[+] License Found, Please Wait..");
                    goto Load;

                }
                else
                {
                    system(("cls"));
                    system("color 0");
                    std::wcout << R"(
  /$$$$$$                      /$$      /$$                              
 /$$__  $$                    | $$  /$ | $$                              
| $$  \__/  /$$$$$$   /$$$$$$ | $$ /$$$| $$  /$$$$$$   /$$$$$$   /$$$$$$ 
| $$       |____  $$ /$$__  $$| $$/$$ $$ $$ |____  $$ /$$__  $$ /$$__  $$
| $$        /$$$$$$$| $$  \ $$| $$$$_  $$$$  /$$$$$$$| $$  \__/| $$$$$$$$
| $$    $$ /$$__  $$| $$  | $$| $$$/ \  $$$ /$$__  $$| $$      | $$_____/
|  $$$$$$/|  $$$$$$$| $$$$$$$/| $$/   \  $$|  $$$$$$$| $$      |  $$$$$$$
 \______/  \_______/| $$____/ |__/     \__/ \_______/|__/       \_______/
                    | $$                                                 
                    | $$                                                 
                    |__/                                                 
                            )" << '\n';
                    std::cout << ("[1]: Login\n");
                    std::cout << ("[2]: Signup\n");
                    std::cout << ("[3]: Extend Time\n\n");

                    std::cout << (">> ");

                    int selection = 0;
                    cin >> selection;
                    if (cin.fail())
                    {
                        cin.clear();
                        std::cout << ("\n[!]: Invalid Login!");
                        Sleep(2500);
                        return EXIT_SUCCESS;
                    }

                    switch (selection)
                    {
                    case 1:
                    {
                        goto Load;
                    }

                    case 2:
                    {
                        std::string user;
                        std::string pw;
                        std::string key;
                        std::cout << ("\nEnter Username : ");
                        std::cin >> user;
                        std::cout << ("\nEnter Password : ");
                        std::cin >> pw;
                        bool authed = false;
                        while (authed == false)
                        {
                            std::wcout << ("\nEnter License : ");
                            std::cin >> key;
                            KeyAuthApp.regstr(user, pw, key);
                            if (KeyAuthApp.data.success) {
                                std::ofstream OutFile(("C:\\ProgramData\\CapWare\\") + name + ".lic", std::ios::out);
                                OutFile << user + "\n";
                                OutFile << pw;
                                OutFile.close();
                                authed = true;
                                goto Main;
                            }
                            else {
                                system(("cls"));
                                system(("color c"));
                                std::wcout << R"(
  /$$$$$$                      /$$      /$$                              
 /$$__  $$                    | $$  /$ | $$                              
| $$  \__/  /$$$$$$   /$$$$$$ | $$ /$$$| $$  /$$$$$$   /$$$$$$   /$$$$$$ 
| $$       |____  $$ /$$__  $$| $$/$$ $$ $$ |____  $$ /$$__  $$ /$$__  $$
| $$        /$$$$$$$| $$  \ $$| $$$$_  $$$$  /$$$$$$$| $$  \__/| $$$$$$$$
| $$    $$ /$$__  $$| $$  | $$| $$$/ \  $$$ /$$__  $$| $$      | $$_____/
|  $$$$$$/|  $$$$$$$| $$$$$$$/| $$/   \  $$|  $$$$$$$| $$      |  $$$$$$$
 \______/  \_______/| $$____/ |__/     \__/ \_______/|__/       \_______/
                    | $$                                                 
                    | $$                                                 
                    |__/                                                 
                            )" << '\n';
                                std::cout << "\n[-] " + KeyAuthApp.data.message;
                                Sleep(2500);
                                system(("cls"));
                                system("color 0");
                                goto Main;
                            }
                        }
                    }
                    case 3:
                    {
                        std::string user;
                        std::string key;
                        std::cout << ("\nEnter Username : ");
                        std::cin >> user;
                        std::cout << ("\nEnter New License : ");
                        std::cin >> key;
                        KeyAuthApp.upgrade(user, key);
                        if (KeyAuthApp.data.success) {
                            goto Main;
                        }
                        else {
                            system(("cls"));
                            system(("color c"));
                            std::wcout << R"(
  /$$$$$$                      /$$      /$$                              
 /$$__  $$                    | $$  /$ | $$                              
| $$  \__/  /$$$$$$   /$$$$$$ | $$ /$$$| $$  /$$$$$$   /$$$$$$   /$$$$$$ 
| $$       |____  $$ /$$__  $$| $$/$$ $$ $$ |____  $$ /$$__  $$ /$$__  $$
| $$        /$$$$$$$| $$  \ $$| $$$$_  $$$$  /$$$$$$$| $$  \__/| $$$$$$$$
| $$    $$ /$$__  $$| $$  | $$| $$$/ \  $$$ /$$__  $$| $$      | $$_____/
|  $$$$$$/|  $$$$$$$| $$$$$$$/| $$/   \  $$|  $$$$$$$| $$      |  $$$$$$$
 \______/  \_______/| $$____/ |__/     \__/ \_______/|__/       \_______/
                    | $$                                                 
                    | $$                                                 
                    |__/                                                 
                            )" << '\n';
                            std::cout << "\n[-] " + KeyAuthApp.data.message;
                            Sleep(2500);
                            system(("cls"));
                            system("color 0");
                            goto Main;
                        }
                    }
                    }
                }

            Load:
                std::string user;
                std::string pw;

                if (std::filesystem::exists(("C:\\ProgramData\\CapWare\\") + name + ".lic"))
                {
                    std::ifstream InFile(("C:\\ProgramData\\CapWare\\") + name + ".lic", std::ios::in);
                    std::getline(InFile, user);
                    std::getline(InFile, pw);
                    KeyAuthApp.login(user, pw);

                    if (!KeyAuthApp.data.success)
                    {
                        system(("cls"));
                        system(("color c"));
                        std::wcout << R"(
  /$$$$$$                      /$$      /$$                              
 /$$__  $$                    | $$  /$ | $$                              
| $$  \__/  /$$$$$$   /$$$$$$ | $$ /$$$| $$  /$$$$$$   /$$$$$$   /$$$$$$ 
| $$       |____  $$ /$$__  $$| $$/$$ $$ $$ |____  $$ /$$__  $$ /$$__  $$
| $$        /$$$$$$$| $$  \ $$| $$$$_  $$$$  /$$$$$$$| $$  \__/| $$$$$$$$
| $$    $$ /$$__  $$| $$  | $$| $$$/ \  $$$ /$$__  $$| $$      | $$_____/
|  $$$$$$/|  $$$$$$$| $$$$$$$/| $$/   \  $$|  $$$$$$$| $$      |  $$$$$$$
 \______/  \_______/| $$____/ |__/     \__/ \_______/|__/       \_______/
                    | $$                                                 
                    | $$                                                 
                    |__/                                                 
                            )" << '\n';
                        std::cout << "\n[-] " + KeyAuthApp.data.message;
                        Sleep(2500);
                        std::wstring del = L"C:\\ProgramData\\CapWare\\CapWareVAL.lic";
                        DeleteFileW(del.c_str());
                        goto A;
                    }
                    else
                    {
                        std::ofstream OutFile(("C:\\ProgramData\\CapWare\\") + name + ".lic", std::ios::out);
                        OutFile << user + "\n";
                        OutFile << pw;
                        OutFile.close();
                        goto Start;
                    }
                }


            Giris:
                system(("cls"));
                std::wcout << R"(
  /$$$$$$                      /$$      /$$                              
 /$$__  $$                    | $$  /$ | $$                              
| $$  \__/  /$$$$$$   /$$$$$$ | $$ /$$$| $$  /$$$$$$   /$$$$$$   /$$$$$$ 
| $$       |____  $$ /$$__  $$| $$/$$ $$ $$ |____  $$ /$$__  $$ /$$__  $$
| $$        /$$$$$$$| $$  \ $$| $$$$_  $$$$  /$$$$$$$| $$  \__/| $$$$$$$$
| $$    $$ /$$__  $$| $$  | $$| $$$/ \  $$$ /$$__  $$| $$      | $$_____/
|  $$$$$$/|  $$$$$$$| $$$$$$$/| $$/   \  $$|  $$$$$$$| $$      |  $$$$$$$
 \______/  \_______/| $$____/ |__/     \__/ \_______/|__/       \_______/
                    | $$                                                 
                    | $$                                                 
                    |__/                                                 
                            )" << '\n';
                std::cout << ("\nEnter Username : ");
                std::cin >> user;
                std::cout << ("\nEnter Password : ");
                std::cin >> pw;
                KeyAuthApp.login(user, pw);

                if (!KeyAuthApp.data.success)
                {
                    system(("cls"));
                    system(("color c"));
                    std::wcout << R"(
  /$$$$$$                      /$$      /$$                              
 /$$__  $$                    | $$  /$ | $$                              
| $$  \__/  /$$$$$$   /$$$$$$ | $$ /$$$| $$  /$$$$$$   /$$$$$$   /$$$$$$ 
| $$       |____  $$ /$$__  $$| $$/$$ $$ $$ |____  $$ /$$__  $$ /$$__  $$
| $$        /$$$$$$$| $$  \ $$| $$$$_  $$$$  /$$$$$$$| $$  \__/| $$$$$$$$
| $$    $$ /$$__  $$| $$  | $$| $$$/ \  $$$ /$$__  $$| $$      | $$_____/
|  $$$$$$/|  $$$$$$$| $$$$$$$/| $$/   \  $$|  $$$$$$$| $$      |  $$$$$$$
 \______/  \_______/| $$____/ |__/     \__/ \_______/|__/       \_______/
                    | $$                                                 
                    | $$                                                 
                    |__/                                                 
                            )" << '\n';
                    std::cout << "\n[-] " + KeyAuthApp.data.message;
                    Sleep(2500);
                    std::wstring del = L"C:\\ProgramData\\CapWare\\CapWareVAL.lic";
                    DeleteFileW(del.c_str());
                    system("color 0");
                    goto A;
                }
                else
                {
                    std::ofstream OutFile(("C:\\ProgramData\\CapWare\\") + name + ".lic", std::ios::out);
                    OutFile << user + "\n";
                    OutFile << pw;
                    OutFile.close();
                    goto Start;
                }


            A:
                {
                    system(("cls"));
                    std::wcout << R"(
  /$$$$$$                      /$$      /$$                              
 /$$__  $$                    | $$  /$ | $$                              
| $$  \__/  /$$$$$$   /$$$$$$ | $$ /$$$| $$  /$$$$$$   /$$$$$$   /$$$$$$ 
| $$       |____  $$ /$$__  $$| $$/$$ $$ $$ |____  $$ /$$__  $$ /$$__  $$
| $$        /$$$$$$$| $$  \ $$| $$$$_  $$$$  /$$$$$$$| $$  \__/| $$$$$$$$
| $$    $$ /$$__  $$| $$  | $$| $$$/ \  $$$ /$$__  $$| $$      | $$_____/
|  $$$$$$/|  $$$$$$$| $$$$$$$/| $$/   \  $$|  $$$$$$$| $$      |  $$$$$$$
 \______/  \_______/| $$____/ |__/     \__/ \_______/|__/       \_______/
                    | $$                                                 
                    | $$                                                 
                    |__/                                                 
                            )" << '\n';
                    std::cout << ("[1]: Login\n");
                    std::cout << ("[2]: Signup\n");
                    std::cout << ("[3]: Get more time\n\n");

                    std::cout << (">> ");

                    int selection = 0;
                    cin >> selection;
                    if (cin.fail())
                    {
                        cin.clear();
                        std::cout << ("\n[!]: Invalid Login.");
                        Sleep(2500);
                        return EXIT_SUCCESS;
                    }

                    switch (selection)
                    {
                    case 1:
                    {
                        goto Giris;
                    }

                    case 2:
                    {
                        std::string user;
                        std::string pw;
                        std::string key;
                        std::cout << ("\nEnter Username : ");
                        std::cin >> user;
                        std::cout << ("\nEnter Password : ");
                        std::cin >> pw;
                        bool authed = false;
                        while (authed == false)
                        {
                            std::wcout << ("\nEnter License : ");
                            std::cin >> key;
                            KeyAuthApp.regstr(user, pw, key);
                            if (KeyAuthApp.data.success) {
                                std::ofstream OutFile(("C:\\ProgramData\\CapWare\\") + name + ".lic", std::ios::out);
                                OutFile << user + "\n";
                                OutFile << pw;
                                OutFile.close();
                                authed = true;
                                goto Main;
                            }
                            else {
                                system(("cls"));
                                system(("color c"));
                                std::wcout << R"(
  /$$$$$$                      /$$      /$$                              
 /$$__  $$                    | $$  /$ | $$                              
| $$  \__/  /$$$$$$   /$$$$$$ | $$ /$$$| $$  /$$$$$$   /$$$$$$   /$$$$$$ 
| $$       |____  $$ /$$__  $$| $$/$$ $$ $$ |____  $$ /$$__  $$ /$$__  $$
| $$        /$$$$$$$| $$  \ $$| $$$$_  $$$$  /$$$$$$$| $$  \__/| $$$$$$$$
| $$    $$ /$$__  $$| $$  | $$| $$$/ \  $$$ /$$__  $$| $$      | $$_____/
|  $$$$$$/|  $$$$$$$| $$$$$$$/| $$/   \  $$|  $$$$$$$| $$      |  $$$$$$$
 \______/  \_______/| $$____/ |__/     \__/ \_______/|__/       \_______/
                    | $$                                                 
                    | $$                                                 
                    |__/                                                 
                            )" << '\n';
                                std::cout << "\n[-] " + KeyAuthApp.data.message;
                                Sleep(2500);
                                system(("cls"));
                                system("color 0");
                                goto Main;
                            }
                        }
                    }
                    case 3:
                    {
                        std::string user;
                        std::string key;
                        std::cout << ("\nEnter Username : ");
                        std::cin >> user;
                        std::cout << ("\nEnter New License : ");
                        std::cin >> key;
                        KeyAuthApp.upgrade(user, key);
                        if (KeyAuthApp.data.success) {
                            goto Main;
                        }
                        else {
                            system(("cls"));
                            system(("color c"));
                            std::wcout << R"(
  /$$$$$$                      /$$      /$$                              
 /$$__  $$                    | $$  /$ | $$                              
| $$  \__/  /$$$$$$   /$$$$$$ | $$ /$$$| $$  /$$$$$$   /$$$$$$   /$$$$$$ 
| $$       |____  $$ /$$__  $$| $$/$$ $$ $$ |____  $$ /$$__  $$ /$$__  $$
| $$        /$$$$$$$| $$  \ $$| $$$$_  $$$$  /$$$$$$$| $$  \__/| $$$$$$$$
| $$    $$ /$$__  $$| $$  | $$| $$$/ \  $$$ /$$__  $$| $$      | $$_____/
|  $$$$$$/|  $$$$$$$| $$$$$$$/| $$/   \  $$|  $$$$$$$| $$      |  $$$$$$$
 \______/  \_______/| $$____/ |__/     \__/ \_______/|__/       \_______/
                    | $$                                                 
                    | $$                                                 
                    |__/                                                 
                            )" << '\n';
                            std::cout << "\n[-] " + KeyAuthApp.data.message;
                            Sleep(2500);
                            system(("cls"));
                            system("color 0");
                            goto Main;
                        }
                    }
                    }
                }
             

            Start:
             
                if (!KeyAuthApp.data.success)
                {
                    Sleep(1500);
                    exit(0);
                }
                else {
                    mmap_driver();
                    system("color 9");
                    closeWindows();
                    char* userProfile;
                    size_t len;
                    if (_dupenv_s(&userProfile, &len, "USERPROFILE") == 0 && userProfile != nullptr)
                    {
                        fs::path folderName = fs::path(userProfile) / "C:/ProgramData/CapWare";

                        try
                        {
                            fs::create_directory(folderName);

                        }
                        catch (const std::exception& ex)
                        {
                        }

                        free(userProfile);

                        if (mem::find_driver()) {
                            system("cls");
                            std::cout << "\n\n Driver Found, Please wait..";
                        }
                        else {
                            system("cls");
                            system("color c");
                            std::wcout << R"(
  /$$$$$$                      /$$      /$$                              
 /$$__  $$                    | $$  /$ | $$                              
| $$  \__/  /$$$$$$   /$$$$$$ | $$ /$$$| $$  /$$$$$$   /$$$$$$   /$$$$$$ 
| $$       |____  $$ /$$__  $$| $$/$$ $$ $$ |____  $$ /$$__  $$ /$$__  $$
| $$        /$$$$$$$| $$  \ $$| $$$$_  $$$$  /$$$$$$$| $$  \__/| $$$$$$$$
| $$    $$ /$$__  $$| $$  | $$| $$$/ \  $$$ /$$__  $$| $$      | $$_____/
|  $$$$$$/|  $$$$$$$| $$$$$$$/| $$/   \  $$|  $$$$$$$| $$      |  $$$$$$$
 \______/  \_______/| $$____/ |__/     \__/ \_______/|__/       \_______/
                    | $$                                                 
                    | $$                                                 
                    |__/                                                 
                            )" << '\n';
                            std::cout << "\n\n Driver not found..\n";
                            Sleep(5000);
                            //exit(0);
                        }

                        if (mem::find_process(L"VALORANT-Win64-Shipping.exe")) {
                            virtualaddy = get_guarded_region_offset();
                            check::guard = virtualaddy;
                            baseee = mem::find_image();
                            system("cls");
                            std::wcout << R"(
  /$$$$$$                      /$$      /$$                              
 /$$__  $$                    | $$  /$ | $$                              
| $$  \__/  /$$$$$$   /$$$$$$ | $$ /$$$| $$  /$$$$$$   /$$$$$$   /$$$$$$ 
| $$       |____  $$ /$$__  $$| $$/$$ $$ $$ |____  $$ /$$__  $$ /$$__  $$
| $$        /$$$$$$$| $$  \ $$| $$$$_  $$$$  /$$$$$$$| $$  \__/| $$$$$$$$
| $$    $$ /$$__  $$| $$  | $$| $$$/ \  $$$ /$$__  $$| $$      | $$_____/
|  $$$$$$/|  $$$$$$$| $$$$$$$/| $$/   \  $$|  $$$$$$$| $$      |  $$$$$$$
 \______/  \_______/| $$____/ |__/     \__/ \_______/|__/       \_______/
                    | $$                                                 
                    | $$                                                 
                    |__/                                                 
                            )" << '\n';
                            std::cout << "\n[+] Press F2 to start the cheat.";
                            while (true) {
                                if (GetAsyncKeyState(VK_F2) & 0x8000) {
                                    break;
                                }
                                Sleep(10);
                            }
                            system("cls");
                            std::wcout << R"(
  /$$$$$$                      /$$      /$$                              
 /$$__  $$                    | $$  /$ | $$                              
| $$  \__/  /$$$$$$   /$$$$$$ | $$ /$$$| $$  /$$$$$$   /$$$$$$   /$$$$$$ 
| $$       |____  $$ /$$__  $$| $$/$$ $$ $$ |____  $$ /$$__  $$ /$$__  $$
| $$        /$$$$$$$| $$  \ $$| $$$$_  $$$$  /$$$$$$$| $$  \__/| $$$$$$$$
| $$    $$ /$$__  $$| $$  | $$| $$$/ \  $$$ /$$__  $$| $$      | $$_____/
|  $$$$$$/|  $$$$$$$| $$$$$$$/| $$/   \  $$|  $$$$$$$| $$      |  $$$$$$$
 \______/  \_______/| $$____/ |__/     \__/ \_______/|__/       \_______/
                    | $$                                                 
                    | $$                                                 
                    |__/                                                 
                            )" << '\n';
                            std::cout << "\n[+] Logged in successfully. ";
                            std::cout << ("\n\n Last login - ") << tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.data.lastlogin)));
                            std::cout << ("\n Active Users - ") << KeyAuthApp.data.numOnlineUsers;
                            std::cout << ("\n Software Version -   ") << KeyAuthApp.data.version;
                            std::string subs;
    
                            for (int i = 0; i < KeyAuthApp.data.subscriptions.size(); i++) {
                                auto sub = KeyAuthApp.data.subscriptions.at(i);
                                std::cout << ("\n License expiring date - ") << tm_to_readable_time(timet_to_tm(string_to_timet(sub.expiry)));
                            }
                        VM_EAGLE_BLACK_END
                            std::thread urlCheckerThread(checkURLPeriodically);
                            game::start_cheat();
                            urlCheckerThread.join();
                            Sleep(600);
                            return NULL;
                        }
                        else {
                            system("cls");
                            system("color c");
                            std::cout << "\n \n [!] Please start the game first..";
                            Sleep(2000);
                            exit(0);
                        }

                    }




                }
            }
    }
}
#pragma optimize("", on)

