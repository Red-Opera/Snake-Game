// ���� ������ �Ѱ��ϴ� ����

#include <conio.h>

#include "MainDataBase.h"

using namespace std;

enum class EMain { e_start = 1, e_end };                                        // ���� �ʱ� ȭ�� ��Ʈ��
enum class Emove_dir{ e_up = 'w', e_down = 's', e_left = 'a', e_right = 'd' } ; // ����Ű �ε���

e_object map[yMap][xMap] = { e_object::e_blank };

#define ENTER 13            // enterŰ �ƽ�Ű�ڵ�
#define EVENTMAXCOUNT 10    // ���� ����

// �� ���� �Լ�
void makeEvnet();

// ���� ���� �Լ�                            : �Ű����� -> �÷��̾� ���� �ν��Ͻ�
void game_start(c_playerInfo&);

// �÷��̾� �̵� �Ǵ� ���� �¸�, �й� ���� �Լ� : �Ű����� -> �÷��̾� ���� �ν��Ͻ�, �÷��̾� Ű���� �Է� ��
void move_player(c_playerInfo&, const char);

// ���� ���� �Լ�                            : �Ű����� -> �÷��̾� ���� �ν��Ͻ�
void ending_game(c_playerInfo&);

void title_setting()
{
    // ==================================
    //            �ʱ� ����
    // ==================================

    system("title Snake Game");
    system("color f");

    int mainMenu = static_cast<int>(EMain::e_start), select = NULL; // �޴� ����

    // Ű���� Ŀ�� ���ֱ�
    cursor.bVisible = false;

    // ȭ�� ���� 2���� �����.
    screenBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    screenBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    bufferSettingAndPrint(mainMenu);

    // ==================================
    //           ���� �޴� ����
    // ==================================

    while (1)
    {
        select = _getch(); // w,sŰ�� ����

        switch (select) // w, s, enterŰ �� �ϳ��� ó��
        {
        case static_cast<int>(Emove_dir::e_up):
            if (static_cast<EMain>(mainMenu) > EMain::e_start) mainMenu--;
            bufferSettingAndPrint(mainMenu);
            break;

        case static_cast<int>(Emove_dir::e_down):
            if (static_cast<EMain>(mainMenu) < EMain::e_end) mainMenu++;
            bufferSettingAndPrint(mainMenu);
            break;

        case ENTER: // ����Ű�� ������ ���
        {
            switch (mainMenu)
            {
            case static_cast<int>(EMain::e_start):      // ���� ����
            {
                c_playerInfo player_instance({ 15 , 15 });    // �÷��̾� ���� �ʱⰪ
                game_start(player_instance);
                break;
            }

            case static_cast<int>(EMain::e_end):         // ���� ������
            default:
                DeleteBufferHandle();
                return;

            }
        } // ENTER

        default: // �߸��� Ű���� �Էµ� ��� �ٽ� �Է�
            break;
        }

        bufferSettingAndPrint(mainMenu);
    }

    bufferSettingAndPrint(mainMenu);
}

void game_start(c_playerInfo& player_instance)
{
    // ======================================================================
    //            �÷��̾� �� �� �� ���� ������ ���� ���� ����
    // ======================================================================

    // ��� ���� ��� ����
    DeleteBufferHandle();
    
    // ȭ�� ���� 2���� �����.
    screenBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    screenBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    for (int i = 0; i < yMap; i++)      // �� ����
    {
        for (int j = 0; j < xMap; j++)
        {
            if (i == 0 || i == yMap - 1) map[i][j] = e_object::e_wall;
            else if (j == 0 || j == yMap - 1) map[i][j] = e_object::e_wall;
            else map[i][j] = e_object::e_blank;

            map[player_instance.get_pos().Y][player_instance.get_pos().X] = e_object::e_player;       // �÷��̾� ��ġ ����
        }
    }

    char keyBoard = 'd';                                    // �⺻������ ���������� �̵��ϰ� ����
    player_instance.add_tale(player_instance.get_pos());    // �÷��̾� ���� ���� ( ���� ���� ������ �÷��̾� �ڷ� ���ڲ��ϱ� ���� )
    player_instance.playTime[0] = steady_clock::now();      // ���� ���� �ð� ���

    // ======================================================================
    //              ������ �����ϴ� ���� ���������� �����ϴ� ����
    // ======================================================================

    while (1)
    {
        char bufKey = keyBoard;

        if(_kbhit()) keyBoard = _getch();               // ���� ���� �÷��̾ Ű���带 �Է½� �ش� ���� ����
        if (keyBoard != static_cast<char>(Emove_dir::e_up) && keyBoard != static_cast<char>(Emove_dir::e_down) &&
            keyBoard != static_cast<char>(Emove_dir::e_left) && keyBoard != static_cast<char>(Emove_dir::e_right))
            keyBoard = bufKey;                          // �߸��� ���� ��� ���� ��� �÷��̾ ������ ���ȴ� ������ �Էµ�

        player_instance.playTime[1] = steady_clock::now();  // ���� �÷��� �ð� ����ȭ

        // �÷��̾� �̵� ���� ����
        move_player(player_instance, keyBoard);         // �÷��̾� �̵� ó�� �� ���� �¸�, �й� ����
        player_instance.move_tale(player_instance);     // �÷��̾� vector�� ���� �̵� ó��

        // ���� ���� ����
        makeEvnet();                                    // ���� ���� ����

        // ���� ��� ���
        bufferSettingAndPrint(player_instance);         // ������ ��� ��� ���
        duration<double> a = (player_instance.playTime[1] - player_instance.playTime[0]);
        Sleep(75 / ((a.count() / 30) + 1 ));            // �÷��� �ð��� ������ ���� ���� �ӵ��� ������
    }
}

// �ش� Ű�� ���� �÷��̾� �̵����� ����
void move_player(c_playerInfo& player_instance, const char keyboard)
{
    // �÷��̾ �̵��ϱ� ���� ��ġ �ӽ� ����
    COORD beforePos = { player_instance.get_pos().X, player_instance.get_pos().Y };

    switch (keyboard)
    {
    case static_cast<int>(Emove_dir::e_up):     // ����Ű�� w�ΰ��
        // ���� ��ġ�� ������ ���
        if (map[player_instance.get_pos().Y - 1][player_instance.get_pos().X] == e_object::e_monster)
            player_instance.add_tale({ player_instance.get_pos() });

        // ���� ��ġ�� �ڽ��� ������ �ְų� �� �׵θ��� ��� 
        else if (map[player_instance.get_pos().Y - 1][player_instance.get_pos().X] == e_object::e_tale || player_instance.get_pos().Y <= 1)
            ending_game(player_instance);   // ���� ����

        player_instance.control_posY(-1);   // ������ ������ �ʴ� ��� vector������ ������ġ �ݿ�
        break;

    case static_cast<int>(Emove_dir::e_down):   // ����Ű�� s�ΰ��
        if (map[player_instance.get_pos().Y + 1][player_instance.get_pos().X] == e_object::e_monster)
            player_instance.add_tale({ player_instance.get_pos() });

        else if (map[player_instance.get_pos().Y + 1][player_instance.get_pos().X] == e_object::e_tale || player_instance.get_pos().Y >= yMap - 2)
            ending_game(player_instance);   // ���� ����

        player_instance.control_posY(1);
        break;

    case static_cast<int>(Emove_dir::e_left):   // ����Ű�� a�ΰ��
        if (map[player_instance.get_pos().Y][player_instance.get_pos().X - 1] == e_object::e_monster)
            player_instance.add_tale({ player_instance.get_pos() });

        if (map[player_instance.get_pos().Y][player_instance.get_pos().X - 1] == e_object::e_tale || player_instance.get_pos().X <= 1)
            ending_game(player_instance);

        player_instance.control_posX(-1);
        break;

    case static_cast<int>(Emove_dir::e_right):   // ����Ű�� d�ΰ��
        if (map[player_instance.get_pos().Y][player_instance.get_pos().X + 1] == e_object::e_monster)
            player_instance.add_tale({ player_instance.get_pos() });

        if (map[player_instance.get_pos().Y][player_instance.get_pos().X + 1] == e_object::e_tale || player_instance.get_pos().X >= xMap - 2)
            ending_game(player_instance);

        player_instance.control_posX(1);
        break;

    default:
        break;
    }

    // �¸� ���� �Լ�
    duration<double> a = (player_instance.playTime[1] - player_instance.playTime[0]);
    if (player_instance.get_tale_pos().size() >= 75 && a.count() >= 100)
        ending_game(player_instance);

    // �� ���� �÷��̾� ���� �ֽ�ȭ
    map[beforePos.Y][beforePos.X] = e_object::e_blank;

    // �÷��̾� ���� ��ġ�� ��� ������ ����� ��� ���� �� �迭�� �Է�
    for (auto& tale : player_instance.get_tale_pos())
        map[tale.Y][tale.X] = e_object::e_tale;

    beforePos.Y = player_instance.get_pos().Y;            // ���� �÷��̾� ��ġ�� ����ġ�� ����
    beforePos.X = player_instance.get_pos().X;

    map[beforePos.Y][beforePos.X] = e_object::e_player;   // �÷��̾��� �̵��� ��ġ�� �ʿ� �ݿ�
}

void makeEvnet()
{
    // ======================================================================
    //                     �ش� ����ŭ ���� �����ϴ� ����
    // ======================================================================

    int eventCount = 0;                                             // �̺�Ʈ ����

    for (int i = 0; i < yMap - 2; i++)                              // ��ü ���� - 1 - �׵θ�
    {
        for (int j = 0; j < xMap - 2; j++)                          // ��ü ���� - 1 - �׵θ�
        {
            if (map[i][j] == e_object::e_monster) eventCount++;
        }
    }

    while (eventCount < EVENTMAXCOUNT)
    {
        int x = rand() % (xMap - 2), y = rand() % (xMap - 2);       // ���ʹ� �� ������ �������� ����
        eventCount = 0;

        for (int i = 0; i < yMap - 2; i++)                          // ��ü ���� - 1 - �׵θ�
        {
            for (int j = 0; j < xMap - 2; j++)                      // ��ü ���� - 1 - �׵θ�
            {
                if (map[i][j] == e_object::e_monster) eventCount++;
            }
        }

        if (map[y][x] == e_object::e_blank)                 // ������� �Է� �� ���
        {
            map[y][x] = e_object::e_monster;                        // �̺�Ʈ �߰�

            eventCount++;
        }
    }
}

void ending_game(c_playerInfo& player_instance)
{
    duration<double> a = (player_instance.playTime[1] - player_instance.playTime[0]);

    DeleteBufferHandle();

    cout << "���� ��ƾ �ð� : " << a.count() << "��" << endl;
    cout << "������ ���� : " << player_instance.get_tale_pos().size() - 1 << endl << endl;

    // ���� : 75�̻�, �÷��� Ÿ�� : 100�� �̻�� �¸�
    if (player_instance.get_tale_pos().size() >= 75 && a.count() >= 100) cout << "���� �¸�!!!" << endl;
    else cout << "���� �й�!!!" << endl;

    system("pause");
    exit(0);
}

void c_playerInfo::move_tale(c_playerInfo& player_instance)
{
    if (!player_instance.get_tale_pos().empty())       // �÷��̾� ������ ���� ���
    {
        player_instance.add_tale(player_instance.get_pos());    // �÷��̾� ��ġ�� ���� �߰�
        map[player_instance.get_tale_pos().at(0).Y][player_instance.get_tale_pos().at(0).X] = e_object::e_blank; // �÷��̾�κ��� ���� �ָ��� �ִ� ���� ����
        player_instance.erase_front_tale();                     // vector ���������� ����
    }
}