// ȭ�� ��¹��� ���� ���� �� Ŀ�� ���� ����

#include "MainDataBase.h"

using namespace std;

static int screenIndex;     // ���� �ε���
HANDLE screenBuffer[2];     // ���۸� ���� ����

extern char* startMenu(const int);
extern char* print_map(c_playerInfo&);

// Ŀ�� ���ֱ�
CONSOLE_CURSOR_INFO cursor;

// =====================================
//          RemoveBufferDelay
// =====================================

void bufferSettingAndPrint(int select)
{
    DWORD bufferSize;                                                                                                   // ���� ����� ���� ����
    FillConsoleOutputCharacter(screenBuffer[screenIndex], ' ', strlen(startMenu(select)), { 0, 0 }, &bufferSize);       // �ش� ���ۿ� ' '�� (absolMousePos = 0, y = 0)���� ������ �Է��ϰ� bufferSize�� ��ȯ  

    SetConsoleCursorPosition(screenBuffer[screenIndex], { 0, 0 });                                                      // �ش� ���ۿ� Ŀ���� 0,0���� �̵�
    WriteFile(screenBuffer[screenIndex], startMenu(select), strlen(startMenu(select)), &bufferSize, NULL);              // �ش� ���ۿ� startMenu�� ��ȯ�ϴ� ���� �Է�

    SetConsoleActiveScreenBuffer(screenBuffer[screenIndex]);                                                            // �ش� ���� ���
    screenIndex = !screenIndex;                                                                                         // ���� �ε����� 0 -> 1 �Ǵ� 1 -> 0���� ����
}

void bufferSettingAndPrint(c_playerInfo& player)
{
    DWORD bufferSize;

    FillConsoleOutputCharacter(screenBuffer[screenIndex], ' ', strlen(print_map(player)), { 0, 0 }, &bufferSize);        // �ش� ���ۿ� ' '�� (absolMousePos = 0, y = 0)���� ������ �Է��ϰ� bufferSize�� ��ȯ  

    SetConsoleCursorPosition(screenBuffer[screenIndex], { 0, 0 });                                                      // �ش� ���ۿ� Ŀ���� 0,0���� �̵�
    WriteFile(screenBuffer[screenIndex], print_map(player), strlen(print_map(player)), &bufferSize, NULL);                            // �ش� ���ۿ� startMenu�� ��ȯ�ϴ� ���� �Է�

    SetConsoleActiveScreenBuffer(screenBuffer[screenIndex]);                                                            // �ش� ���� ���
    screenIndex = !screenIndex;                                                                                         // ���� �ε����� 0 -> 1 �Ǵ� 1 -> 0���� ����
}

void DeleteBufferHandle()
{
    CloseHandle(screenBuffer[0]); // ���۸� ��� �ڵ� ����
    CloseHandle(screenBuffer[1]);
}