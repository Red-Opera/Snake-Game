// 화면 출력버퍼 지연 제거 및 커서 제어 파일

#include "MainDataBase.h"

using namespace std;

static int screenIndex;     // 버퍼 인덱스
HANDLE screenBuffer[2];     // 버퍼를 담을 변수

extern char* startMenu(const int);
extern char* print_map(c_playerInfo&);

// 커서 없애기
CONSOLE_CURSOR_INFO cursor;

// =====================================
//          RemoveBufferDelay
// =====================================

void bufferSettingAndPrint(int select)
{
    DWORD bufferSize;                                                                                                   // 버퍼 사이즈를 구할 변수
    FillConsoleOutputCharacter(screenBuffer[screenIndex], ' ', strlen(startMenu(select)), { 0, 0 }, &bufferSize);       // 해당 버퍼에 ' '를 (absolMousePos = 0, y = 0)부터 끝까지 입력하고 bufferSize를 반환  

    SetConsoleCursorPosition(screenBuffer[screenIndex], { 0, 0 });                                                      // 해당 버퍼에 커서를 0,0으로 이동
    WriteFile(screenBuffer[screenIndex], startMenu(select), strlen(startMenu(select)), &bufferSize, NULL);              // 해당 버퍼에 startMenu가 반환하는 값을 입력

    SetConsoleActiveScreenBuffer(screenBuffer[screenIndex]);                                                            // 해당 버퍼 출력
    screenIndex = !screenIndex;                                                                                         // 버퍼 인덱스를 0 -> 1 또는 1 -> 0으로 변경
}

void bufferSettingAndPrint(c_playerInfo& player)
{
    DWORD bufferSize;

    FillConsoleOutputCharacter(screenBuffer[screenIndex], ' ', strlen(print_map(player)), { 0, 0 }, &bufferSize);        // 해당 버퍼에 ' '를 (absolMousePos = 0, y = 0)부터 끝까지 입력하고 bufferSize를 반환  

    SetConsoleCursorPosition(screenBuffer[screenIndex], { 0, 0 });                                                      // 해당 버퍼에 커서를 0,0으로 이동
    WriteFile(screenBuffer[screenIndex], print_map(player), strlen(print_map(player)), &bufferSize, NULL);                            // 해당 버퍼에 startMenu가 반환하는 값을 입력

    SetConsoleActiveScreenBuffer(screenBuffer[screenIndex]);                                                            // 해당 버퍼 출력
    screenIndex = !screenIndex;                                                                                         // 버퍼 인덱스를 0 -> 1 또는 1 -> 0으로 변경
}

void DeleteBufferHandle()
{
    CloseHandle(screenBuffer[0]); // 버퍼를 담는 핸들 삭제
    CloseHandle(screenBuffer[1]);
}