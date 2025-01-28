// 게임 진행을 총괄하는 파일

#include <conio.h>

#include "MainDataBase.h"

using namespace std;

enum class EMain { e_start = 1, e_end };                                        // 게임 초기 화면 컨트롤
enum class Emove_dir{ e_up = 'w', e_down = 's', e_left = 'a', e_right = 'd' } ; // 방향키 인덱스

e_object map[yMap][xMap] = { e_object::e_blank };

#define ENTER 13            // enter키 아스키코드
#define EVENTMAXCOUNT 10    // 몬스터 개수

// 적 생성 함수
void makeEvnet();

// 게임 시작 함수                            : 매개변수 -> 플레이어 정보 인스턴스
void game_start(c_playerInfo&);

// 플래이어 이동 또는 게임 승리, 패배 판정 함수 : 매개변수 -> 플레이어 정보 인스턴스, 플레이어 키보드 입력 값
void move_player(c_playerInfo&, const char);

// 게임 종료 함수                            : 매개변수 -> 플레이어 정보 인스턴스
void ending_game(c_playerInfo&);

void title_setting()
{
    // ==================================
    //            초기 세팅
    // ==================================

    system("title Snake Game");
    system("color f");

    int mainMenu = static_cast<int>(EMain::e_start), select = NULL; // 메뉴 선택

    // 키보드 커서 없애기
    cursor.bVisible = false;

    // 화면 버퍼 2개를 만든다.
    screenBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    screenBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    bufferSettingAndPrint(mainMenu);

    // ==================================
    //           메인 메뉴 실행
    // ==================================

    while (1)
    {
        select = _getch(); // w,s키로 조절

        switch (select) // w, s, enter키 중 하나를 처리
        {
        case static_cast<int>(Emove_dir::e_up):
            if (static_cast<EMain>(mainMenu) > EMain::e_start) mainMenu--;
            bufferSettingAndPrint(mainMenu);
            break;

        case static_cast<int>(Emove_dir::e_down):
            if (static_cast<EMain>(mainMenu) < EMain::e_end) mainMenu++;
            bufferSettingAndPrint(mainMenu);
            break;

        case ENTER: // 엔터키를 눌렸을 경우
        {
            switch (mainMenu)
            {
            case static_cast<int>(EMain::e_start):      // 게임 시작
            {
                c_playerInfo player_instance({ 15 , 15 });    // 플레이어 정보 초기값
                game_start(player_instance);
                break;
            }

            case static_cast<int>(EMain::e_end):         // 게임 끝내기
            default:
                DeleteBufferHandle();
                return;

            }
        } // ENTER

        default: // 잘못된 키값이 입력된 경우 다시 입력
            break;
        }

        bufferSettingAndPrint(mainMenu);
    }

    bufferSettingAndPrint(mainMenu);
}

void game_start(c_playerInfo& player_instance)
{
    // ======================================================================
    //            플레이어 및 적 등 게임 진행을 위한 세팅 영역
    // ======================================================================

    // 출력 버퍼 모두 제거
    DeleteBufferHandle();
    
    // 화면 버퍼 2개를 만든다.
    screenBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    screenBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    for (int i = 0; i < yMap; i++)      // 맵 세팅
    {
        for (int j = 0; j < xMap; j++)
        {
            if (i == 0 || i == yMap - 1) map[i][j] = e_object::e_wall;
            else if (j == 0 || j == yMap - 1) map[i][j] = e_object::e_wall;
            else map[i][j] = e_object::e_blank;

            map[player_instance.get_pos().Y][player_instance.get_pos().X] = e_object::e_player;       // 플레이어 위치 세팅
        }
    }

    char keyBoard = 'd';                                    // 기본적으로 오른쪽으로 이동하게 설정
    player_instance.add_tale(player_instance.get_pos());    // 플레이어 꼬리 생성 ( 다음 꼬리 생성시 플레이어 뒤로 오겠끔하기 위해 )
    player_instance.playTime[0] = steady_clock::now();      // 게임 시작 시각 기록

    // ======================================================================
    //              게임을 진행하는 동안 지속적으로 진행하는 영역
    // ======================================================================

    while (1)
    {
        char bufKey = keyBoard;

        if(_kbhit()) keyBoard = _getch();               // 게임 도중 플레이어가 키보드를 입력시 해당 값을 받음
        if (keyBoard != static_cast<char>(Emove_dir::e_up) && keyBoard != static_cast<char>(Emove_dir::e_down) &&
            keyBoard != static_cast<char>(Emove_dir::e_left) && keyBoard != static_cast<char>(Emove_dir::e_right))
            keyBoard = bufKey;                          // 잘못된 값이 들어 왔을 경우 플레이어가 이전에 눌렸던 값으로 입력됨

        player_instance.playTime[1] = steady_clock::now();  // 게임 플레이 시간 동기화

        // 플레이어 이동 제어 영역
        move_player(player_instance, keyBoard);         // 플레이어 이동 처리 및 게임 승리, 패배 판정
        player_instance.move_tale(player_instance);     // 플레이어 vector의 꼬리 이동 처리

        // 몬스터 생성 영역
        makeEvnet();                                    // 몬스터 스폰 관리

        // 최종 결과 출력
        bufferSettingAndPrint(player_instance);         // 지금한 결과 모두 출력
        duration<double> a = (player_instance.playTime[1] - player_instance.playTime[0]);
        Sleep(75 / ((a.count() / 30) + 1 ));            // 플레이 시간이 지나면 지날 수록 속도가 빨라짐
    }
}

// 해당 키에 따른 플레이어 이동방향 선택
void move_player(c_playerInfo& player_instance, const char keyboard)
{
    // 플레이어가 이동하기 전의 위치 임시 저장
    COORD beforePos = { player_instance.get_pos().X, player_instance.get_pos().Y };

    switch (keyboard)
    {
    case static_cast<int>(Emove_dir::e_up):     // 방향키가 w인경우
        // 다음 위치가 몬스터인 경우
        if (map[player_instance.get_pos().Y - 1][player_instance.get_pos().X] == e_object::e_monster)
            player_instance.add_tale({ player_instance.get_pos() });

        // 다음 위치가 자신의 꼬리가 있거나 맵 테두리인 경우 
        else if (map[player_instance.get_pos().Y - 1][player_instance.get_pos().X] == e_object::e_tale || player_instance.get_pos().Y <= 1)
            ending_game(player_instance);   // 게임 종료

        player_instance.control_posY(-1);   // 게임이 끝나지 않는 경우 vector변수에 다음위치 반영
        break;

    case static_cast<int>(Emove_dir::e_down):   // 방향키가 s인경우
        if (map[player_instance.get_pos().Y + 1][player_instance.get_pos().X] == e_object::e_monster)
            player_instance.add_tale({ player_instance.get_pos() });

        else if (map[player_instance.get_pos().Y + 1][player_instance.get_pos().X] == e_object::e_tale || player_instance.get_pos().Y >= yMap - 2)
            ending_game(player_instance);   // 게임 종료

        player_instance.control_posY(1);
        break;

    case static_cast<int>(Emove_dir::e_left):   // 방향키가 a인경우
        if (map[player_instance.get_pos().Y][player_instance.get_pos().X - 1] == e_object::e_monster)
            player_instance.add_tale({ player_instance.get_pos() });

        if (map[player_instance.get_pos().Y][player_instance.get_pos().X - 1] == e_object::e_tale || player_instance.get_pos().X <= 1)
            ending_game(player_instance);

        player_instance.control_posX(-1);
        break;

    case static_cast<int>(Emove_dir::e_right):   // 방향키가 d인경우
        if (map[player_instance.get_pos().Y][player_instance.get_pos().X + 1] == e_object::e_monster)
            player_instance.add_tale({ player_instance.get_pos() });

        if (map[player_instance.get_pos().Y][player_instance.get_pos().X + 1] == e_object::e_tale || player_instance.get_pos().X >= xMap - 2)
            ending_game(player_instance);

        player_instance.control_posX(1);
        break;

    default:
        break;
    }

    // 승리 판정 함수
    duration<double> a = (player_instance.playTime[1] - player_instance.playTime[0]);
    if (player_instance.get_tale_pos().size() >= 75 && a.count() >= 100)
        ending_game(player_instance);

    // 맵 상의 플레이어 정보 최신화
    map[beforePos.Y][beforePos.X] = e_object::e_blank;

    // 플레이어 꼬리 위치를 담는 변수에 저장된 모든 정보 맵 배열에 입력
    for (auto& tale : player_instance.get_tale_pos())
        map[tale.Y][tale.X] = e_object::e_tale;

    beforePos.Y = player_instance.get_pos().Y;            // 기존 플레이어 위치를 현위치로 변경
    beforePos.X = player_instance.get_pos().X;

    map[beforePos.Y][beforePos.X] = e_object::e_player;   // 플레이어의 이동할 위치에 맵에 반영
}

void makeEvnet()
{
    // ======================================================================
    //                     해당 수만큼 적을 생성하는 영역
    // ======================================================================

    int eventCount = 0;                                             // 이벤트 개수

    for (int i = 0; i < yMap - 2; i++)                              // 전체 길이 - 1 - 테두리
    {
        for (int j = 0; j < xMap - 2; j++)                          // 전체 높이 - 1 - 테두리
        {
            if (map[i][j] == e_object::e_monster) eventCount++;
        }
    }

    while (eventCount < EVENTMAXCOUNT)
    {
        int x = rand() % (xMap - 2), y = rand() % (xMap - 2);       // 몬스터는 맵 내에서 랜덤으로 생성
        eventCount = 0;

        for (int i = 0; i < yMap - 2; i++)                          // 전체 길이 - 1 - 테두리
        {
            for (int j = 0; j < xMap - 2; j++)                      // 전체 높이 - 1 - 테두리
            {
                if (map[i][j] == e_object::e_monster) eventCount++;
            }
        }

        if (map[y][x] == e_object::e_blank)                 // 빈공간에 입력 할 경우
        {
            map[y][x] = e_object::e_monster;                        // 이벤트 추가

            eventCount++;
        }
    }
}

void ending_game(c_playerInfo& player_instance)
{
    duration<double> a = (player_instance.playTime[1] - player_instance.playTime[0]);

    DeleteBufferHandle();

    cout << "게임 버틴 시간 : " << a.count() << "초" << endl;
    cout << "꼬리의 길이 : " << player_instance.get_tale_pos().size() - 1 << endl << endl;

    // 꼬리 : 75이상, 플레이 타임 : 100초 이상시 승리
    if (player_instance.get_tale_pos().size() >= 75 && a.count() >= 100) cout << "게임 승리!!!" << endl;
    else cout << "게임 패배!!!" << endl;

    system("pause");
    exit(0);
}

void c_playerInfo::move_tale(c_playerInfo& player_instance)
{
    if (!player_instance.get_tale_pos().empty())       // 플레이어 꼬리가 있을 경우
    {
        player_instance.add_tale(player_instance.get_pos());    // 플레이어 위치에 꼬리 추가
        map[player_instance.get_tale_pos().at(0).Y][player_instance.get_tale_pos().at(0).X] = e_object::e_blank; // 플레이어로부터 가장 멀리에 있는 꼬리 제거
        player_instance.erase_front_tale();                     // vector 변수에서도 적용
    }
}