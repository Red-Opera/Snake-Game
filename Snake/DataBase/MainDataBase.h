#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <Windows.h>

#define interface struct

#define xMap 25								// x축 맵 크기
#define yMap 25								// y축 맵 크기

using namespace std;
using namespace std::chrono;

extern HANDLE screenBuffer[2];				// 버퍼를 담는 변수
extern CONSOLE_CURSOR_INFO cursor;			// 커서의 정보를 담는 변수

enum class e_object { e_obj_start = 0, e_blank = e_obj_start, e_wall, e_player, e_tale, e_monster, e_obj_end = e_monster }; // 맵 관련 인덱스

interface IBaseInfo						// 플레이어가 기본적으로 담아야 할 정보
{
	virtual ~IBaseInfo() = default;				// 소멸자 정의

	virtual COORD get_pos() = 0;				// 플레이어 좌표 출력함수
	virtual void control_posX(const char) = 0;	// 플레이어 X좌표 이동 제어함수
	virtual void control_posY(const char) = 0;	// 플레이어 Y좌표 이동 제어함수

	virtual void add_tale(COORD) = 0;			// 꼬리 추가 함수

	COORD pos;									// 플레이어 좌표
	vector<COORD> talePos;						// 꼬리 좌표
};

class c_playerInfo : public IBaseInfo	// 플레이어의 정보를 담는 클래스
{
public:
	c_playerInfo(COORD info) : pos(info) {}						// 초기화시 플레이어의 좌표 초기화

	COORD get_pos() { return pos; }								// 플레이어의 위치 정보를 가져옴
	vector<COORD> get_tale_pos() { return talePos; }			// 플레이어 꼬리 정보 가져옴

	void add_tale(COORD addPos) { talePos.push_back(addPos); }	// 꼬리 추가
	void erase_front_tale() { talePos.erase(talePos.begin()); }	// 꼬리 원소 중 앞에 있는 원소 제거 ( 플레이어 꼬리 중 맨 끝에 있는 꼬리 제거 )
	
	void control_posX(const char size) { pos.X += size; }		// X좌표 이동 함수 ( 양수 : 오른쪽으로 이동, 음수 : 왼쪽으로 이동 )
	void control_posY(const char size) { pos.Y += size; }		// Y좌표 이동 함수 ( 양수 : 아래쪽으로 이동, 음수 : 위쪽으로 이동 )
	void move_tale(c_playerInfo&);								// 꼬리 이동 함수 ( 정의 : ProceedGame.cpp, 매개변수 -> 플레이어 정보 인스턴스 )

	~c_playerInfo() = default;									// 클래스 소멸자

public:
	steady_clock::time_point playTime[2];						// playTime[0] : 게임 시작 시간, playTime[1] : 현재 시간
	
private:
	vector<COORD> talePos;
	COORD pos;
};

// ===================================
//	    버퍼 및 커서 제어 관련 함수
// ===================================

void bufferSettingAndPrint(int);					// 버퍼 출력 함수
void bufferSettingAndPrint(c_playerInfo&);			// 버퍼 출력 함수
void DeleteBufferHandle();							// 버퍼 제거 함수

void title_setting();