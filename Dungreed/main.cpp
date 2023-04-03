//main.cpp : Main controller for managing my Dungreed Game

#include "framework.h"
#include "Client.h"
#include "hjApplication.h"
#include <wchar.h>
#include "hjRscManager.h"
#include "hjSceneManager.h"


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE   hInst; // 현재 인스턴스 의미
WCHAR       szTitle[MAX_LOADSTRING]; // 제목 표시줄 텍스트 의미
WCHAR       szWindowClass[MAX_LOADSTRING]; // 기본 창 클래스

// Dungreed application

hj::Application application;

// Data for using Window functions
ATOM                MyRegisterClass(HINSTANCE hInstance, LPCWSTR name, WNDPROC proc);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    AtlasWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


// Main code
int APIENTRY wWinMain(
    _In_        HINSTANCE   hInstance,
    _In_opt_    HINSTANCE   hPrevInstance,
    _In_        LPWSTR      lpCmdLine,
    _In_        int         nCmdShow
    )
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // 1. Class for define Window's information to memory 
    // 2. Create Window using CreateWindow()
    // 3. Output Window to Screen using ShowWindow()
    // 4. Iterate registered messages (in window) and Process input messages


    // 전역 문자열 초기화
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
   
    // 윈도우 제목 임의로 초기화
    wcsncpy_s(szTitle, L"Dungreed", MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DUNGREED, szWindowClass, MAX_LOADSTRING);
    // main window
    MyRegisterClass(hInstance, szWindowClass, WndProc);

    // main window
    MyRegisterClass(hInstance, L"AtlasWindow", AtlasWndProc);
    

    // 애플리케이션 초기화
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DUNGREED));

    MSG msg;

	// Set GDI+ 
	ULONG_PTR           m_gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;

    // Start GDI+
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            // 게임 로직 수행
            application.Run();
        }
    }
    // Shutdown GDI+.
    GdiplusShutdown(m_gdiplusToken);

    hj::SceneManager::Release();
    hj::RscManager::Release();

    return (int)msg.wParam;
}

//
//  Func(): MyRegisterClass()
//  Purpose: Register Window Class
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPCWSTR name, WNDPROC proc)
{
    WNDCLASSEXW wcex;

    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = proc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DUNGREED));
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName  = MAKEINTRESOURCEW(IDC_DUNGREED);
    wcex.lpszClassName = name;
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   Func(): InitInstance(HINSTANCE, int)
//
//   Purpose: Save instance handle and make primary Window
// 
//   Method:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들 저장

    HWND hWnd = CreateWindowW(
        szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        0, 0, 1600, 900,
        nullptr, nullptr, hInstance, nullptr);

    HWND hWnd2 = CreateWindowW(
        L"AtlasWindow", szTitle, WS_OVERLAPPEDWINDOW,
        1600, 0, 500, 500,
        nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    // 윈도우를 띄운 후 업데이트 수행
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
    ShowWindow(hWnd2, nCmdShow);
    UpdateWindow(hWnd2);

    // Dungreed 게임 초기화
    application.Initialize(hWnd);
    application.SetToolHwnd(hWnd2);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmld = LOWORD(wParam);
        // 메뉴 선택에 대한 구문 분석
        switch (wmld)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}