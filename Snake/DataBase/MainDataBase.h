#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <Windows.h>

#define interface struct

#define xMap 25								// x�� �� ũ��
#define yMap 25								// y�� �� ũ��

using namespace std;
using namespace std::chrono;

extern HANDLE screenBuffer[2];				// ���۸� ��� ����
extern CONSOLE_CURSOR_INFO cursor;			// Ŀ���� ������ ��� ����

enum class e_object { e_obj_start = 0, e_blank = e_obj_start, e_wall, e_player, e_tale, e_monster, e_obj_end = e_monster }; // �� ���� �ε���

interface IBaseInfo						// �÷��̾ �⺻������ ��ƾ� �� ����
{
	virtual ~IBaseInfo() = default;				// �Ҹ��� ����

	virtual COORD get_pos() = 0;				// �÷��̾� ��ǥ ����Լ�
	virtual void control_posX(const char) = 0;	// �÷��̾� X��ǥ �̵� �����Լ�
	virtual void control_posY(const char) = 0;	// �÷��̾� Y��ǥ �̵� �����Լ�

	virtual void add_tale(COORD) = 0;			// ���� �߰� �Լ�

	COORD pos;									// �÷��̾� ��ǥ
	vector<COORD> talePos;						// ���� ��ǥ
};

class c_playerInfo : public IBaseInfo	// �÷��̾��� ������ ��� Ŭ����
{
public:
	c_playerInfo(COORD info) : pos(info) {}						// �ʱ�ȭ�� �÷��̾��� ��ǥ �ʱ�ȭ

	COORD get_pos() { return pos; }								// �÷��̾��� ��ġ ������ ������
	vector<COORD> get_tale_pos() { return talePos; }			// �÷��̾� ���� ���� ������

	void add_tale(COORD addPos) { talePos.push_back(addPos); }	// ���� �߰�
	void erase_front_tale() { talePos.erase(talePos.begin()); }	// ���� ���� �� �տ� �ִ� ���� ���� ( �÷��̾� ���� �� �� ���� �ִ� ���� ���� )
	
	void control_posX(const char size) { pos.X += size; }		// X��ǥ �̵� �Լ� ( ��� : ���������� �̵�, ���� : �������� �̵� )
	void control_posY(const char size) { pos.Y += size; }		// Y��ǥ �̵� �Լ� ( ��� : �Ʒ������� �̵�, ���� : �������� �̵� )
	void move_tale(c_playerInfo&);								// ���� �̵� �Լ� ( ���� : ProceedGame.cpp, �Ű����� -> �÷��̾� ���� �ν��Ͻ� )

	~c_playerInfo() = default;									// Ŭ���� �Ҹ���

public:
	steady_clock::time_point playTime[2];						// playTime[0] : ���� ���� �ð�, playTime[1] : ���� �ð�
	
private:
	vector<COORD> talePos;
	COORD pos;
};

// ===================================
//	    ���� �� Ŀ�� ���� ���� �Լ�
// ===================================

void bufferSettingAndPrint(int);					// ���� ��� �Լ�
void bufferSettingAndPrint(c_playerInfo&);			// ���� ��� �Լ�
void DeleteBufferHandle();							// ���� ���� �Լ�

void title_setting();