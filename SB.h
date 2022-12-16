#ifndef _SB_H_
#define _SB_H_

#include <string>
#include <vector>

using namespace std;

//�������� ���� ����� ��� ���
#pragma region SeaBattle_values
//����� ���� ���
int sbf = 10;
//����� ������ ��������
int scrsize = 100;
//ʳ������ �������
int cnt_shot = 1;

//ʳ������ �����, ��� ������� ���� ����� ��������
#define WIN_CNT 20
//������� ������ �������, 10 - ������� : ������� �����, ����� �������
int Escadr[10][2] =
{
	{1,1},{1,2},{1,3},{1,4},
	{2,1},{2,2},{2,3},
	{3,1},{3,2},
	{4,1}
};

#pragma endregion
#pragma region SeaBattle_Declare
//��'��� �������� ������� ���� ���
int** mtx1, ** mtx2;
//������� ��� ��� = 0- �������� 1- ��������
std::vector<string> vGame_Mode;
//������� ��� �������� ���� = 0- ���� 1- ������ 2- �����
std::vector<string> vGame_Place;
//������� ���� �����
std::vector<string> Player_stan;
//������� ��� �����
std::vector<string> Player_type;
//������� в���� �����
std::vector<string> Player_level;
//������� ��������� �������
std::vector<string> Shot_Result;
//������� ��������� ������� ���������
std::vector<string> Shot_Result_Short;
#pragma endregion
#pragma region AllEnum
//������ �������� ����
enum class enGame_Place {
	comp,
	server,
	client
};
//������ ���� ���
enum class enGame_Mode{
	loc,
	net
};
//����� ��� _Game_Mode
enGame_Mode _Game_Mode = enGame_Mode::loc;
//����� ��� _Game_Place
enGame_Place _Game_Place = enGame_Place::comp;

//������ ���� ���
enum class Player_Level {
	level0,
	level1
};
//������ ���: ����-����=0, ����-������=1, ������-������=2
enum Game_Mode
{
	comp_comp = 0,
	comp_human = 1,
	human_human = 2,
	human_comp = 3
};
//�������� ���������� �������: �����-0,����-1,����-2,������-3
enum class Direction
{
	toup,
	todown,
	toleft,
	toright
};
//������� ������ �� ����
enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};
//���� ��� ���������� �������
int ColorResult[5] = { White,LightRed,LightBlue,LightGreen,Yellow };
int ColorPlayerStat[5] = { White,LightGreen,LightRed};
//��� ������ ����'����-0, ������-1, �볺�� -2, ������-3
enum class Player_Mode
{
	comp = 0,
	human = 1,
	client = 2
};
//������ ������: �������-0, ����������-1, �����������-2
enum class Player_Stat
{
	InGame = 0,
	Winner = 1,
	Loser = 2
};
//��������� �������: �����=-2,������=-1, �����=0, �������=1, ����=2
enum class ShotRes
{
	None = 0,
	Miss = 1,
	Wound = 2,
	Kill = 3,
	Exit = 4
};
enum class SC_Com_type
{
	none,
	runExitLoop,
	ans_ok,
	ans_error,
	setPl1Name,
	setPl2Name,
	setPl1type,
	setPl2type,
	setSBF,
	setCntShot,
	runGame
	
};
#pragma endregion

#pragma region Function_Cursor
void SetColor(int text, int bg);
int wherex();
int wherey();
void gotoxy(int column, int line);
void SetScrSize();
#pragma endregion



#pragma region myClasses
struct SC_Comand
{
	SC_Com_type type;
	string command;
	string message;
};
struct SC_Comands
{
	const char separator = '|';
	const char separator2 = '&';
	vector<SC_Comand> SCCs;
	void myGetMessage(string msg) {
		SCCs.clear();
		std::stringstream streamData(msg);
		string val;
		while (std::getline(streamData, val, separator))
		{
			std::stringstream streamData2(val);
			string val2; int f = true;
			SC_Comand sc;
			sc.message = val;
			while (std::getline(streamData2, val2, separator2))
			{
				if (f == true) { sc.type = (SC_Com_type)stoi(val2); f = false; }
				else { sc.command = val2; f = true; }
			}
			SCCs.push_back(sc);
		}
	};
	void mySetMessage(SC_Com_type type, string msg) {
		SC_Comand sc;
		sc.type = type;
		sc.command = msg;
		sc.message = to_string((int)type) + separator2 + msg;
		SCCs.push_back(sc);
	};
};
//��������� Point - �����
struct Point {
public: int x, y;
	  Point();
};
//��������� ������ �������
struct Desk : public Point{
public: 
	  int value = 0;
	  bool alive = true;
	  Desk() { x = -1; y = -1; };
	  Desk(int x1, int y1) { x = x1; y = y1; };
	  Desk(Point pt) { x = pt.y; y = pt.y; };
	  bool operator==(const Desk& dsk)	{ return (x == dsk.x && y == dsk.y);  }
	  bool operator==(const Desk* dsk) { return (x == dsk->x && y == dsk->y); }
};
enum class ShipStan {alive,wound,killed};
//��������� ������ �������
struct  Ship{
public: 
	int desks=0; // ������� ����� � �������
	int alive = true;
	int orient=-1; // �������� ������� 0 - �������������, 1- �����������
	int deskskill = 0; // ����� ����� � �������
	ShipStan stan = ShipStan::alive;
	vector<Desk> Desks; // ������ �������
	Ship();
	Ship(int orient, int desks);
	void Add(Desk *dsk);
	void Kill(Desk* dsk);
	ShipStan GetStan(); 
	~Ship();
};
struct  Escadra{
public:
	int ships = 0;
	vector<Ship> Ships;
	Escadra() { Ships.clear(); };
	void Add(Ship *ship);
	//Ship *GetShip(Point pnt);
	bool GetShip(int x, int y, Ship * ship);
};

//��������� ��������� �������
struct ShotResult
{
public:
	Point pnt; // ������� ����� �������
	ShotRes shres = ShotRes::None; // ��������� �������
};
//���� �������-�������;
class Gamer {
public:
	string Name;
};
//����� �������
class Player : public Gamer
{
public:
	Player_Stat ps;		//������ ������
	Player_Mode pm;		//��� ������
	Player_Level level;
	Escadra *myEscadra;

	vector<ShotResult> ShotResults;
	vector<ShotResult> NewShots;
	//����� ������
	ShotResult* myShotRes;
	int ShotDirection = -1;	//�������� ���������� ������� 
	bool SecondCompShot = false;
	bool CompHit = false;
	bool NextCompHit = false;
	bool CompMiss[4] = { false, false, false, false };
	Point LastShot;		//������� ������� ������
	int shot;			//�������� �������
	int kill;			//�������� ������

	void New_Game();	//������� ������� ���� ���
	void New_Shot();	//������� ������� ���� ���
	//������ ������ ������ � ������
	void Add_Shot(ShotResult* myShotRes);
	Player();
	Player(string name, Player_Mode pm1 = Player_Mode::comp);
	~Player();
private:
};
//��������� �� ���� �������, ���� �������
Player* pl1, * pl2;

#pragma endregion
#pragma region SeaBattle_functions

//��'��� ������� "� �������?";
bool Is_Ship(int x, int y, int** mtx);
#pragma endregion

#pragma region SeaBattle_Show_Functions
void Show_MainHead();
//³���������� 1-�� �������� ���� - ����� �������
void Show(int** mtx, int k, Player* pl);
//³���������� ����� �� �������� ���
char ShowPoint(int val, bool hide);
//³���������� ���� ������� �����
void Show2(int** mtx1, int** mtx2, Player* pl1, Player* pl2);

#pragma endregion
#pragma region BattleField_Function
//���������� �������� ���� �����
void ResetBattleField(int** mtx);
//���������� �������� ���� ���������
void SetShipToBattleField(Player* pl, int** mtx);


#pragma endregion


#pragma region SeaBattle_MenuFunction
void CHM_SettingSize();
void CHM_SettingShot();
void CHM_SettingPlayer(int i, Player* pl);
void CHM_SettingGameMode();
int M_MainMenu();
void M_Info();
void M_Setting();
void M_SettingPlayer();
void M_Game();
#pragma endregion

#endif