#ifndef _SB_H_
#define _SB_H_

#include <string>
#include <vector>

using namespace std;

//Глобальні змінні умови для гри
#pragma region SeaBattle_values
//Розмір поля бою
int sbf = 10;
//Розмір екрану терміналу
int scrsize = 100;
//Кількість пострілів
int cnt_shot = 1;

//Кількість палуб, при враженні яких настає перемога
#define WIN_CNT 20
//Матриця ескадр кораблів, 10 - кораблів : кількість палуб, номер корабля
int Escadr[10][2] =
{
	{1,1},{1,2},{1,3},{1,4},
	{2,1},{2,2},{2,3},
	{3,1},{3,2},
	{4,1}
};

#pragma endregion
#pragma region SeaBattle_Declare
//Об'ява двомірних матриць полів бою
int** mtx1, ** mtx2;
//Вектори Тип гри = 0- локальна 1- мережева
std::vector<string> vGame_Mode;
//Вектори Тип ігрового місця = 0- комп 1- сервер 2- кліент
std::vector<string> vGame_Place;
//Вектори СТАН грака
std::vector<string> Player_stan;
//Вектори ТИП грака
std::vector<string> Player_type;
//Вектори РІВЕНЬ грака
std::vector<string> Player_level;
//Вектори РЕЗУЛЬТАТ пострілу
std::vector<string> Shot_Result;
//Вектори РЕЗУЛЬТАТ пострілу СКОРОЧЕНО
std::vector<string> Shot_Result_Short;
#pragma endregion
#pragma region AllEnum
//Перелік ігрового місця
enum class enGame_Place {
	comp,
	server,
	client
};
//Перелік рівнів гри
enum class enGame_Mode{
	loc,
	net
};
//Режим гри _Game_Mode
enGame_Mode _Game_Mode = enGame_Mode::loc;
//Режим гри _Game_Place
enGame_Place _Game_Place = enGame_Place::comp;

//Перелік рівнів гри
enum class Player_Level {
	level0,
	level1
};
//Режими гри: комп-комп=0, комп-людина=1, людина-людина=2
enum Game_Mode
{
	comp_comp = 0,
	comp_human = 1,
	human_human = 2,
	human_comp = 3
};
//Напрямок наступного пострілу: вгору-0,вниз-1,вліво-2,вправо-3
enum class Direction
{
	toup,
	todown,
	toleft,
	toright
};
//Кольори шрифту та фону
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
//Колір для результатів пострілу
int ColorResult[5] = { White,LightRed,LightBlue,LightGreen,Yellow };
int ColorPlayerStat[5] = { White,LightGreen,LightRed};
//Тип гравця комп'ютер-0, людина-1, клієнт -2, сервер-3
enum class Player_Mode
{
	comp = 0,
	human = 1,
	client = 2
};
//Статус гравця: гравець-0, переможець-1, переможений-2
enum class Player_Stat
{
	InGame = 0,
	Winner = 1,
	Loser = 2
};
//Результат пострілу: вихід=-2,промах=-1, нічого=0, поранив=1, вбив=2
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
//Структура Point - Точка
struct Point {
public: int x, y;
	  Point();
};
//Структура Палуба корабля
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
//Структура Палуба корабля
struct  Ship{
public: 
	int desks=0; // кількість палуб у корабля
	int alive = true;
	int orient=-1; // орієнтація корабля 0 - горизонтально, 1- вертикально
	int deskskill = 0; // вбито палуб у корабля
	ShipStan stan = ShipStan::alive;
	vector<Desk> Desks; // палуби корабля
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

//Структура Результат пострілу
struct ShotResult
{
public:
	Point pnt; // остання точка постірул
	ShotRes shres = ShotRes::None; // результат пострілу
};
//Клас Гравець-базовий;
class Gamer {
public:
	string Name;
};
//Класс ГРАВЕЦЬ
class Player : public Gamer
{
public:
	Player_Stat ps;		//статус гравця
	Player_Mode pm;		//тип гравця
	Player_Level level;
	Escadra *myEscadra;

	vector<ShotResult> ShotResults;
	vector<ShotResult> NewShots;
	//Назва гравця
	ShotResult* myShotRes;
	int ShotDirection = -1;	//напрямок наступного пострілу 
	bool SecondCompShot = false;
	bool CompHit = false;
	bool NextCompHit = false;
	bool CompMiss[4] = { false, false, false, false };
	Point LastShot;		//останній влучний постріл
	int shot;			//лічильник пострілів
	int kill;			//лічильник вбитих

	void New_Game();	//функція початку нової гри
	void New_Shot();	//функція початку нової гри
	//Добати постріл іграка в вектор
	void Add_Shot(ShotResult* myShotRes);
	Player();
	Player(string name, Player_Mode pm1 = Player_Mode::comp);
	~Player();
private:
};
//Вказівники на двох гравців, типу ГРАВЕЦЬ
Player* pl1, * pl2;

#pragma endregion
#pragma region SeaBattle_functions

//Об'ява Функції "Є корабль?";
bool Is_Ship(int x, int y, int** mtx);
#pragma endregion

#pragma region SeaBattle_Show_Functions
void Show_MainHead();
//Відображення 1-го ігрового поля - стара функція
void Show(int** mtx, int k, Player* pl);
//Відображення точки на ігровому полі
char ShowPoint(int val, bool hide);
//Відображення двох ігрових полей
void Show2(int** mtx1, int** mtx2, Player* pl1, Player* pl2);

#pragma endregion
#pragma region BattleField_Function
//Заповнення ігрового поля морем
void ResetBattleField(int** mtx);
//Заповнення ігрового поля кораблями
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