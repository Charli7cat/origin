#include <iostream>
#include <windows.h>
#include <winhttp.h>
#include <sql.h>
#include <sqlext.h>

#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "ws2_32.lib")

int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    std::cout << "=== My Console App ===" << std::endl;
    std::cout << "Platform: Windows" << std::endl;
    std::cout << std::string(40, '=') << std::endl;
    
    std::cout << "\n--- Network Test ---" << std::endl;
    HINTERNET hSession = WinHttpOpen(L"Test/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, 
                                      WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hSession) {
        std::cout << "WinHTTP initialized successfully!" << std::endl;
        WinHttpCloseHandle(hSession);
    } else {
        std::cout << "WinHTTP initialization failed!" << std::endl;
    }
    
    std::cout << "\n--- Database Test ---" << std::endl;
    SQLHENV hEnv;
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) == SQL_SUCCESS) {
        SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
        std::cout << "ODBC initialized successfully!" << std::endl;
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    } else {
        std::cout << "ODBC initialization failed!" << std::endl;
    }
    
    std::cout << "\nPress Enter to exit..." << std::endl;
    std::cin.get();
    
    return 0;
}