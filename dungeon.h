//=============================================================================
//
// ダンジョン処理 [dungeon.h]
// Author : GP11A243 06 楳田健斗
//
//=============================================================================
#ifndef _DUNGEON_H_
#define _DUNGEON_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MIN_ROOM_WIDTH				(12)	// 部屋の最小横サイズ
#define MIN_ROOM_HEIGHT				(9)		// 部屋の最小縦サイズ
#define MAX_ROOM_WIDTH				(20)	// 部屋の最大横サイズ
#define MAX_ROOM_HEIGHT				(15)	// 部屋の最大縦サイズ
#define MAP_WIDTH					(48)	// マップの横サイズ
#define MAP_HEIGHT					(40)	// マップの縦サイズ
#define MAX_ROOM_COUNT				(12)	// 部屋の最大数

#define TEXTURE_DUNGEON_SIZE_X		(128)	// テクスチャサイズ
#define TEXTURE_DUNGEON_SIZE_Y		(128)	// 同上

#define SPAWN_DISTANCE				(250)	// エネミーのスポーン距離


//*****************************************************************************
// 構造体宣言
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
// プロトタイプ宣言
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
