// ȭ�鿡 ����� �޼������� ó���ϴ� ����

#include "MainDataBase.h"

extern e_object map[yMap][xMap];

// ���� ȭ�� ���� ���� �Լ�
char* startMenu(const int select)
{
    char buffer[3000]; // ȭ�鿡 ����� ������ ũ��

    sprintf_s(buffer, "    ��������������������������������������������������������������������������������������������������������������������������������������������������������������\n");
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��\n");  
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��\n");  
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��\n");  
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                              ������ũ ����                                  ��\n");  
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��\n");  
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��\n");  
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��\n");  
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��\n");  
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��\n");  
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��\n");  
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��\n");  
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��\n");  
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��\n");
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��\n");
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��\n");
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                               ���ο� ����\t");
    if (select == 1) sprintf_s(buffer, 3000, "%s%s", buffer, " ��                               ��\n");
    else sprintf_s(buffer, 3000, "%s%s", buffer, "                                  ��\n");
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��    \n");
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��    \n");
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��    \n");
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��    \n");
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��    \n");
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��    \n");
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��    \n");
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                ������\t");
    if (select == 2) sprintf_s(buffer, 3000, "%s%s", buffer, " ��");
    else sprintf_s(buffer, 3000, "%s%s", buffer, "    ");
    sprintf_s(buffer, 3000, "%s%s", buffer, "	   Ȯ�� : Enter           ��\n");
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��    \n");
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��                                                                             ��    \n");
    sprintf_s(buffer, 3000, "%s%s", buffer, "    ��������������������������������������������������������������������������������������������������������������������������������������������������������������   ");

    return buffer;
}

// �� ȭ�� ���� ���� �Լ�
char* print_map(c_playerInfo& info)
{
    char buffer[3500]; // ȭ�鿡 ����� ������ ũ��

    sprintf_s(buffer, 3500, "\033[31m%s\033[m", "��");

    for (int y = 0; y < yMap; y++)
    {
        for (int x = 0; x < xMap; x++)
        {
            if (x == 0 && y == 0) continue;

            switch (map[y][x])
            {
            case e_object::e_blank:
                sprintf_s(buffer, 3500,"%s%s", buffer, "  ");
                break;

            case e_object::e_wall:
                sprintf_s(buffer, 3500,"%s\033[31m%s\033[m", buffer, "��");
                break;

            case e_object::e_player:
                sprintf_s(buffer, 3500,"%s\033[96m%s\033[m", buffer, "��");
                break;

            case e_object::e_tale:
                sprintf_s(buffer, 3500, "%s\033[92m%s\033[m", buffer, "��");
                break;

            case e_object::e_monster:
                sprintf_s(buffer, 3500,"%s\033[33m%s\033[m", buffer, "��");
                break;

            default:
                break;
            }
        }

        duration<double> a = (info.playTime[1] - info.playTime[0]);

        if (y == 4)
            sprintf_s(buffer, 3500, "%s\033[5m%s%.1lf��\033[m", buffer, "     ���� �÷��� �ð� : ", a);
      
        if (y == 5)
            sprintf_s(buffer, 3500, "%s\033[5m%s%.1lf�и���\033[m", buffer, "     ���� ���ǵ� : ", 75 / ((a.count() / 30) + 1));

        if (y == 6)
            sprintf_s(buffer, 3500, "%s\033[94m%s%d\033[m", buffer, "     ���� ���� : ", info.get_tale_pos().size() - 1);

        if (y == 7)
            sprintf_s(buffer, 3500, "%s\033[94m%s%-25d%s%d\033[m", buffer, "     x : ", info.get_pos().X, "y : ", info.get_pos().Y);

        sprintf_s(buffer, 3500, "%s%s", buffer, "\n");
    }

    return buffer;
}