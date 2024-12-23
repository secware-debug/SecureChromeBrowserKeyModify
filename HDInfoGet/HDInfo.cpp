#include <iostream>
#include <windows.h>
#include <comdef.h>
#include <Wbemidl.h>
#include <fstream>
#include <string>
#pragma comment(lib, "wbemuuid.lib")

void InitializeCOM()
{
    HRESULT hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        std::cerr << "Failed to initialize COM library. Error code = 0x" << std::hex << hres << std::endl;
        exit(1);
    }
}

void InitializeSecurity()
{
    HRESULT hres = CoInitializeSecurity(
        NULL,
        -1,                          // COM negotiates authentication
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities
        NULL                         // Reserved
    );

    if (FAILED(hres)) {
        std::cerr << "Failed to initialize security. Error code = 0x" << std::hex << hres << std::endl;
        CoUninitialize();
        exit(1);
    }
}

IWbemServices* ConnectWMI()
{
    IWbemLocator* pLoc = NULL;

    HRESULT hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (LPVOID*)&pLoc
    );

    if (FAILED(hres)) {
        std::cerr << "Failed to create IWbemLocator object. Error code = 0x" << std::hex << hres << std::endl;
        CoUninitialize();
        exit(1);
    }

    IWbemServices* pSvc = NULL;

    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), // WMI namespace
        NULL,                    // User name
        NULL,                    // User password
        0,                       // Locale
        NULL,                    // Security flags
        0,                       // Authority
        0,                       // Context object
        &pSvc                    // IWbemServices proxy
    );

    if (FAILED(hres)) {
        std::cerr << "Could not connect to WMI namespace. Error code = 0x" << std::hex << hres << std::endl;
        pLoc->Release();
        CoUninitialize();
        exit(1);
    }

    std::cout << "Connected to WMI namespace ROOT\\CIMV2" << std::endl;

    hres = CoSetProxyBlanket(
        pSvc,                        // Indicates the proxy to set
        RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
        RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
        NULL,                        // Server principal name
        RPC_C_AUTHN_LEVEL_CALL,      // Authentication level
        RPC_C_IMP_LEVEL_IMPERSONATE, // Impersonation level
        NULL,                        // Client identity
        EOAC_NONE                    // Proxy capabilities
    );

    if (FAILED(hres)) {
        std::cerr << "Could not set proxy blanket. Error code = 0x" << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        exit(1);
    }

    pLoc->Release(); // IWbemLocator no longer needed
    return pSvc;
}

void QueryWMI(IWbemServices* pSvc, const std::wstring& query, const std::wstring& propertyName, const std::wstring& displayName)
{
    IEnumWbemClassObject* pEnumerator = NULL;
    // Open a wide-character file stream
    std::wofstream file(L"output.txt", std::ios::out | std::ios::app); // `app` to append if the file exists

    if (file.is_open()) {
       // std::cout << "File opened successfully" << std::endl;
    }
    else {
        std::wcerr << L"Failed to open the file." << std::endl;
    }
    HRESULT hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t(query.c_str()),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator
    );

    if (FAILED(hres)) {
        std::wcerr << L"Query failed: " << query << L". Error code = 0x" << std::hex << hres << std::endl;
        return;
    }

    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (0 == uReturn) break;

        VARIANT vtProp;
        hr = pclsObj->Get(propertyName.c_str(), 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr) && vtProp.vt == VT_BSTR) {
            std::wcout << displayName << L": " << vtProp.bstrVal << std::endl;
            file << displayName << L": " << vtProp.bstrVal << std::endl;
        }
        VariantClear(&vtProp);
        pclsObj->Release();
    }

    pEnumerator->Release();
    file.close();
}

int main()
{
    InitializeCOM();
    InitializeSecurity();

    IWbemServices* pSvc = ConnectWMI();

    QueryWMI(pSvc, L"SELECT SerialNumber FROM Win32_BaseBoard", L"SerialNumber", L"Motherboard");           // Motherboard serial
    QueryWMI(pSvc, L"SELECT SerialNumber FROM Win32_DiskDrive WHERE Index = 0", L"SerialNumber", L"HDD");          // HDD serial
    QueryWMI(pSvc, L"SELECT SerialNumber FROM SoftwareLicensingService", L"SerialNumber", L"Win_Product"); // Windows product ID
    QueryWMI(pSvc, L"SELECT UUID FROM Win32_ComputerSystemProduct", L"UUID", L"BIOS UUID");              // BIOS UUID
    QueryWMI(pSvc, L"SELECT ProcessorId FROM Win32_Processor", L"ProcessorId", L"CPU ID");            // CPU ID
    QueryWMI(pSvc, L"SELECT PrimaryOwnerName FROM Win32_Tpm", L"PrimaryOwnerName", L"Endorsement Key");        // TPM Endorsement key (if available)
    QueryWMI(pSvc, L"SELECT SerialNumber FROM Win32_OperatingSystem", L"SerialNumber", L"Win Product Id");        // Windows Product ID
     
    pSvc->Release();
    CoUninitialize();
    return 0;
}
