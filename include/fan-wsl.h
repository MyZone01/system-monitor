#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

// Function to get CPU temperature on WSL using WMI
float GetCPUTemperatureWSL() {
    IEnumWbemClassObject* pEnum = NULL;
    HRESULT hres = ExecWMIQuery(L"SELECT * FROM MSAcpi_ThermalZoneTemperature", &pEnum);
    if (FAILED(hres)) {
        // Handle error
        return -1.0f;
    }

    IWbemClassObject* pObject = NULL;
    ULONG uReturned = 0;
    float cpuTemperature = -1.0f;

    // Get the first result from the enumeration
    hres = pEnum->Next(WBEM_INFINITE, 1, &pObject, &uReturned);
    if (SUCCEEDED(hres) && uReturned > 0) {
        VARIANT vtTemperature;
        VariantInit(&vtTemperature);

        // Get the "CurrentTemperature" property (CPU temperature in degrees Celsius)
        hres = pObject->Get(L"CurrentTemperature", 0, &vtTemperature, 0, 0);
        if (SUCCEEDED(hres) && (vtTemperature.vt == VT_I4)) {
            int temperatureValue = vtTemperature.intVal;
            // Convert temperature from tenths of degrees Celsius to degrees Celsius
            cpuTemperature = static_cast<float>(temperatureValue) / 10.0f;
        }

        VariantClear(&vtTemperature);
        pObject->Release();
    }

    pEnum->Release();
    return cpuTemperature;
}


// Function to connect to WMI and execute a query
HRESULT ExecWMIQuery(const wchar_t* query, IEnumWbemClassObject** pEnum) {
    HRESULT hres;

    // Initialize COM
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) return hres;

    // Initialize security (optional, but needed for WMI)
    hres = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    );
    if (FAILED(hres)) {
        CoUninitialize();
        return hres;
    }

    // Obtain the initial locator to WMI
    IWbemLocator* pLoc = NULL;
    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
    if (FAILED(hres)) {
        CoUninitialize();
        return hres;
    }

    // Connect to WMI through the IWbemLocator::ConnectServer method
    IWbemServices* pSvc = NULL;
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        NULL,
        NULL,
        0,
        NULL,
        0,
        0,
        &pSvc
    );
    if (FAILED(hres)) {
        pLoc->Release();
        CoUninitialize();
        return hres;
    }

    // Set the proxy so that impersonation of the client occurs
    hres = CoSetProxyBlanket(
        pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE
    );
    if (FAILED(hres)) {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return hres;
    }

    // Execute the query to get an enumeration of the results
    hres = pSvc->ExecQuery(
        _bstr_t(L"WQL"),
        _bstr_t(query),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        pEnum
    );

    pSvc->Release();
    pLoc->Release();
    CoUninitialize();
    return hres;
}

// Function to get fan speed on WSL using WMI
int GetFanSpeedWSL() {
    IEnumWbemClassObject* pEnum = NULL;
    HRESULT hres = ExecWMIQuery(L"SELECT * FROM Win32_Fan", &pEnum);
    if (FAILED(hres)) {
        // Handle error
        return -1;
    }

    IWbemClassObject* pObject = NULL;
    ULONG uReturned = 0;
    int fanSpeed = -1;

    // Get the first result from the enumeration
    hres = pEnum->Next(WBEM_INFINITE, 1, &pObject, &uReturned);
    if (SUCCEEDED(hres) && uReturned > 0) {
        VARIANT vtFanSpeed;
        VariantInit(&vtFanSpeed);

        // Get the "CurrentSpeed" property (fan speed in RPM)
        hres = pObject->Get(L"CurrentSpeed", 0, &vtFanSpeed, 0, 0);
        if (SUCCEEDED(hres) && (vtFanSpeed.vt == VT_I4)) {
            fanSpeed = vtFanSpeed.intVal;
        }

        VariantClear(&vtFanSpeed);
        pObject->Release();
    }

    pEnum->Release();
    return fanSpeed;
}

// Function to get fan level on WSL using WMI
int GetFanLevelWSL() {
    IEnumWbemClassObject* pEnum = NULL;
    HRESULT hres = ExecWMIQuery(L"SELECT * FROM Win32_Fan", &pEnum);
    if (FAILED(hres)) {
        // Handle error
        return -1;
    }

    IWbemClassObject* pObject = NULL;
    ULONG uReturned = 0;
    int fanLevel = -1;

    // Get the first result from the enumeration
    hres = pEnum->Next(WBEM_INFINITE, 1, &pObject, &uReturned);
    if (SUCCEEDED(hres) && uReturned > 0) {
        VARIANT vtFanLevel;
        VariantInit(&vtFanLevel);

        // Get the "DesiredSpeed" property (fan level)
        hres = pObject->Get(L"DesiredSpeed", 0, &vtFanLevel, 0, 0);
        if (SUCCEEDED(hres) && (vtFanLevel.vt == VT_I4)) {
            fanLevel = vtFanLevel.intVal;
        }

        VariantClear(&vtFanLevel);
        pObject->Release();
    }

    pEnum->Release();
    return fanLevel;
}

// Function to get fan status on WSL using WMI
std::string GetFanStatusWSL() {
    IEnumWbemClassObject* pEnum = NULL;
    HRESULT hres = ExecWMIQuery(L"SELECT * FROM Win32_Fan", &pEnum);
    if (FAILED(hres)) {
        // Handle error
        return "Unknown";
    }

    IWbemClassObject* pObject = NULL;
    ULONG uReturned = 0;
    std::string fanStatus = "Unknown";

    // Get the first result from the enumeration
    hres = pEnum->Next(WBEM_INFINITE, 1, &pObject, &uReturned);
    if (SUCCEEDED(hres) && uReturned > 0) {
        VARIANT vtFanStatus;
        VariantInit(&vtFanStatus);

        // Get the "Status" property (fan status)
        hres = pObject->Get(L"Status", 0, &vtFanStatus, 0, 0);
        if (SUCCEEDED(hres) && (vtFanStatus.vt == VT_BSTR)) {
            fanStatus = _com_util::ConvertBSTRToString(vtFanStatus.bstrVal);
        }

        VariantClear(&vtFanStatus);
        pObject->Release();
    }

    pEnum->Release();
    return fanStatus;
}