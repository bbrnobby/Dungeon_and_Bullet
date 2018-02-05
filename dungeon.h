//=============================================================================
//
// �_���W�������� [dungeon.h]
// Author : GP11A243 06 ���c���l
//
//=============================================================================
#ifndef _DUNGEON_H_
#define _DUNGEON_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MIN_ROOM_WIDTH				(12)	// �����̍ŏ����T�C�Y
#define MIN_ROOM_HEIGHT				(9)		// �����̍ŏ��c�T�C�Y
#define MAX_ROOM_WIDTH				(20)	// �����̍ő剡�T�C�Y
#define MAX_ROOM_HEIGHT				(15)	// �����̍ő�c�T�C�Y
#define MAP_WIDTH					(48)	// �}�b�v�̉��T�C�Y
#define MAP_HEIGHT					(40)	// �}�b�v�̏c�T�C�Y
#define MAX_ROOM_COUNT				(12)	// �����̍ő吔

#define TEXTURE_DUNGEON_SIZE_X		(128)	// �e�N�X�`���T�C�Y
#define TEXTURE_DUNGEON_SIZE_Y		(128)	// ����

#define SPAWN_DISTANCE				(250)	// �G�l�~�[�̃X�|�[������


//*****************************************************************************
// �\���̐錾
//*****************************************************************************

typedef struct {
	int baseX;
	int baseY;
	int width;
	int height;
	int cntEnemy;
	int spawnEnemy;
	bool clear;
}ROOM;

typedef struct {
	int baseX;
	int baseY;
	int width;
	int height;
	ROOM room;
}SPACE;

enum MAPID
{
	MAP_NOT_OBJECT,
	MAP_BOUNDARY,
	MAP_WALL_UP,
	MAP_WALL_DOWN,
	MAP_WALL_RIGHT,
	MAP_WALL_LEFT,
	MAP_WALL_UPRIGHT,
	MAP_WALL_UPLEFT,
	MAP_WALL_DOWNRIGHT,
	MAP_WALL_DOWNLEFT,
	MAP_ROOM,
	MAP_TRAIL,
	MAP_PATH,
	MAP_START,
	MAP_EXIT,
	MAP_MAX_ID
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitDungeon(int type);
void UninitDungeon(void);
void UpdateDungeon(void);
void DrawDungeon(void);

int GetMap(int x, int y);
int GetMapByPos(float x, float y);

ROOM *GetRoom(int id);
ROOM *GetRoom2(int x, int y);
int GetRoomID(int x, int y);
int GetRoomIDByPos(float x, float y);
int GetExitRoomID(void);

void EnterRoom(int ID);
void LeaveRoom(int ID);
void ClearRoom(int ID);

#endif
