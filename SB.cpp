#include <iostream>
#include <string>
#include <locale>
#include <fstream>
#include <iomanip>
#include <random>
#include <ctime>
#include <stdio.h>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <Windows.h>

using namespace std;
#include "SB_TCP.h"
#include "SB.h"

void Init_Lib()
{
	vGame_Mode.clear();
	vGame_Mode.push_back("локальна");
	vGame_Mode.push_back("мережева");

	vGame_Place.clear();
	vGame_Place.push_back("комп'ютер");
	vGame_Place.push_back("сервер");
	vGame_Place.push_back("клієнт");


	Shot_Result_Short.clear();
	Shot_Result_Short.push_back("&");
	Shot_Result_Short.push_back("M");
	Shot_Result_Short.push_back("W");
	Shot_Result_Short.push_back("K");
	Shot_Result_Short.push_back("E");

	Shot_Result.clear();
	Shot_Result.push_back("нічого");
	Shot_Result.push_back("промазав");
	Shot_Result.push_back("поранив");
	Shot_Result.push_back("вбив");
	Shot_Result.push_back("вихід");
	Player_stan.clear();
	Player_stan.push_back("Гравець");
	Player_stan.push_back("Переможець");
	Player_stan.push_back("Програвший");
	Player_type.clear();
	Player_type.push_back("комп`ютер");
	Player_type.push_back("людина");
	Player_type.push_back("клієнт");
	Player_level.clear();
	Player_level.push_back("низький");
	Player_level.push_back("високий");
}
#pragma region Системний: координати + колір
//Зміна кольору тексту та фону, за замовченням білим на чорному
void SetColor(int text = White, int bg = Black)
{
	HANDLE Red = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(Red, (WORD)((bg << 4) | text));
}
int wherex()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.dwCursorPosition.X;
}

int wherey()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.dwCursorPosition.Y;
}

void gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}

int WhereX()
{
	POINT position; GetCursorPos(&position);
	return position.x;
}
int WhereY()
{
	POINT position; GetCursorPos(&position);
	return position.y;
}

#pragma endregion

//Game_Mode _Game_Mode;

#pragma region Class-Functions
//Desk::Desk(void)

Ship::Ship()
{
	desks = 0;
	Desks.clear();
};
Ship::Ship(int orient, int desks)
{
	desks = desks;
	orient = orient;
};
void Ship::Add(Desk *dsk) 
{
	desks++;
	Desks.push_back(*dsk);
};
void Ship::Kill(Desk* dsk)
{
};
ShipStan Ship::GetStan() {
	int alive_dsk = 0, wound_dsk = 0;
	for (auto &_Desks:Desks)
	{
		if (_Desks.alive == true) alive_dsk++; else wound_dsk++;
	}
	if (alive_dsk == desks) return ShipStan::alive;
	else if (wound_dsk > 0) return ShipStan::wound;
	else if (wound_dsk == desks) return ShipStan::killed;
	else return ShipStan::alive;
}

Ship::~Ship()
{
};
void Escadra::Add(Ship* ship)
{
	ships++;
	Ships.push_back(*ship);
};

bool Escadra::GetShip(int x, int y, Ship *ship1) {
	Desk *dsk = new Desk(x,y);
	bool retCode = false;
	for (auto& ship : Ships)
	{
		auto searchDesk = find(ship.Desks.begin(), ship.Desks.end(), dsk);
		if (searchDesk != ship.Desks.end())
		{
			ship1 = &ship;
			retCode = true;
		}
		else
		{
			ship1 = NULL;
			retCode = false;
		}
	}
	return retCode;
};

//функція початку нової гри для гравця
void Player::New_Shot()
{
	LastShot.x = -1;
	LastShot.y = -1;
	SecondCompShot = false;
	CompHit = false;
	NextCompHit = false;
};

//функція початку нової гри для гравця
void Player::New_Game()
{
	shot = 0;
	kill = 0;
	ps = Player_Stat::InGame;
	LastShot.x = -1;
	LastShot.y = -1;
	myShotRes = new ShotResult;
	SecondCompShot = false;
	CompHit = false;
	ShotResults.clear();
	//myEscadra = new Escadra();
};
//Створення нового гравця
Player::Player()
{
	New_Game();
	level = Player_Level::level0;
	pm = Player_Mode::comp;	//за замовченням тип гравця = коп'ютер
	myEscadra = new Escadra();
};
//Створення нового гравця
Player::Player(string name, Player_Mode pm1)
{
	New_Game();
	Name = name;
	level = Player_Level::level0;
	pm = pm1;
	myEscadra = new Escadra();
};

Player::~Player()
{
};

void Player::Add_Shot(ShotResult* myShotRes) {
	ShotResults.push_back(*myShotRes);
};
//Координати останнього влучного пострілу
Point::Point() {
	x = -1; y = -1;
}

#pragma endregion

#pragma region Function_work_with_file
//Отримати інформацію з файлу fname та вивести на екран
void Get_Info_From_File(string fname)
{
	string line;
	ifstream f(fname);
	if (f.is_open())
	{
		while (getline(f, line))
		{
			cout << line << endl;
		}
		f.close();
	}
	else
	{
		cout << "Відсутній файл: " << fname << endl;
	}
	system("pause");
}

#pragma endregion

#pragma region SeaBattle_Menu
//Головне меню програми
int M_MainMenu()
{
	int retCode = -1;
	cout << "\n\tГоловне МЕНЮ\n\n";
	cout << "1. Інформація\n" ;
	cout << "2. Налаштування\n";
	cout << "3. Гра\n" ;
	cout << "\n0. Вихід\n";
	cout << "Ваш вибір: ";
	cin >> retCode;
	return retCode;
}
//Інформаційне меню
void M_Info()
{
	int M_chs = -1;
	while (M_chs != 0)
	{
		system("cls");
		Show_MainHead();
		cout << "\n\tІнформація\n\n";
		cout << "1. Правила гри\n";
		cout << "2. Умови курсвої\n";
		cout << "3. Про розробника\n";
		cout << "\n0. Вихід\n";
		cout << "Ваш вибір: ";
		cin >> M_chs;
		system("cls");
		switch (M_chs)
		{
		case 1: {Get_Info_From_File("sb_info_Rule.txt");	}	break;
		case 2: {Get_Info_From_File("sb_info_Task.txt");	}	break;
		case 3: {Get_Info_From_File("sb_info_Autor.txt");	}	break;
		default:												break;
		}
	}
}
//Меню налаштування
void M_Setting()
{
	int M_chs = -1;
	while (M_chs != 0)
	{
		system("cls");
		Show_MainHead();
		cout << "\n\tНалаштування програми\n\n";
		cout << "1. Розмір ігрового поля\n";
		cout << "2. Кількість пострілів\n";
		cout << "3. Ім'я, Тип гравця\n";
		cout << "4. Режим гри\n";
		cout << "\n0. Вихід\n";
		cout << "Ваш вибір: ";
		cin >> M_chs;
		switch (M_chs)
		{
		case 1: {	CHM_SettingSize();		}break;
		case 2: {	CHM_SettingShot();		}break;
		case 3: {	M_SettingPlayer();		}break;
		case 4: {	CHM_SettingGameMode();	}break;
		default:			break;
		}
	}
}
//Меню Налаштування гравців
void M_SettingPlayer()
{
	bool cont_player = true;
	while (cont_player)
	{
		system("cls");
		Show_MainHead();
		int nameForPlay = 0;
		cout << "\n\tНалаштування гравців\n\n";
		cout << "Ім'я гравця 1: " << pl1->Name << " (" << Player_type[(static_cast<int>(pl1->pm))] << ")" <<
			" [" << Player_level[(static_cast<int>(pl1->level))] << "]\n";
		cout << "Ім'я гравця 2: " << pl2->Name << " (" << Player_type[(static_cast<int>(pl2->pm))] << ")" <<
			" [" << Player_level[(static_cast<int>(pl2->level))] << "]\n";
		cout << "\n0.Вихід\n";
		cout << "Якого граця змінити ?: ";
		cin >> nameForPlay;
		switch (nameForPlay)
		{
		case 1: { CHM_SettingPlayer(1, pl1);	} break;
		case 2: { CHM_SettingPlayer(2, pl2);	} break;
		case 0: { cont_player = false; } break;
		default:	break;
		}
	}
}

#pragma endregion


#pragma region Chat_Server_Client

void sm2c(SC_Com_type type, string msg)
{
	if (_Game_Mode == enGame_Mode::net && _Game_Place == enGame_Place::server) {
	SendMessageToClient(to_string((int)type) + "&" + msg);
	}
}
bool ApplyCommandFromServer(string command)
{
	bool cont = true;
	SC_Comands sccs; 
	sccs.myGetMessage(command);
	for (auto& cmd :sccs.SCCs) { //cout << "Тип повідомлення: " << cmd.type << " команда: " << cmd.command << endl;
	switch ((SC_Com_type)cmd.type) {
	case SC_Com_type::setCntShot: cnt_shot = stoi(cmd.command); break;
	case SC_Com_type::setPl1Name: pl1->Name = cmd.command; break;
	case SC_Com_type::setPl2Name: pl2->Name = cmd.command; break;
	case SC_Com_type::setPl1type: pl1->pm = (Player_Mode)stoi(cmd.command); break;
	case SC_Com_type::setPl2type: pl1->pm = (Player_Mode)stoi(cmd.command); break;
	case SC_Com_type::runGame: M_Game(); break;
	case SC_Com_type::runExitLoop: cont = false; break;
	default: break;
	}
	}
	return cont;
}
// Циклічний чат між сервером та клієнтом
void ChatSC() {	bool cont = true;

while (cont)
	{
	system("cls");
	Show_MainHead();
		if (_Game_Mode == enGame_Mode::net)
		{
			switch (_Game_Place)
			{
			case enGame_Place::server:
			{
				int typ;
				string msg;
				cout << "Тип повідомлення (0-вихід): "; cin >> typ;
				switch (typ)
				{
				case 0: cont = false;
				default:cout << " команда: "; cin >> msg; SendMessageToClient(typ + "&" + msg);
					break;

				}
				break;
			}
			case enGame_Place::client:
			{
				cont = ApplyCommandFromServer(ReceiveMessageFromServer());
				break;
			}
			default: cont = false;  break;
			}
		}
		else { cont = false; }
	}
}

#pragma endregion


#pragma region SeaBattle_ChangeSetting
//Налаштування програми: режим гри / ігрового поля
void CHM_SettingGameMode() 
{
	//Режим гри _Game_Mode
	int _gm = -1;
	int _gp = -1;
	int _plClient = -1;
	while (_gm != 0)
	{
		system("cls");
		Show_MainHead();
		cout << "\n\tНалаштування програми РЕЖИМ гри. Поточне значення: " << vGame_Mode[(int)_Game_Mode] << endl << endl;
		cout << "1. Локальна гра" << endl;
		cout << "2. Мережева гра" << endl;
		cout << "3. Відключити мережу" << endl;
		cout << "4. Обмін даними" << endl;
		cout << "\n0. Вихід" << endl;
		cout << "Ваш вибір: ";
		cin >> _gm;
		switch (_gm)
		{
		case 1: { _Game_Mode = enGame_Mode::loc; _Game_Place = enGame_Place::comp; break; }
		case 2: 
		{ 
			while(_gp!=0)
			{
				_Game_Mode = enGame_Mode::net;
				cout << "\tНалаштування програми РЕЖИМ РОБОЧОГО МІСЦЯ. Поточне значення: " << vGame_Place[(int)_Game_Place] << endl << endl;
				cout << "1. Сервер" << endl;
				cout << "2. Клієнт" << endl << endl;
				cout << "0. Вихід" << endl;
				cout << "\nВаш вибір: ";
				cin >> _gp;
				switch (_gp)
				{
				case 1: {	_Game_Place = enGame_Place::server;	
					while (_plClient != 0)
					{
						cout << "\tВиберіть гравця, який буде віддаленим клієнтом. Поточне значення: " << _plClient << endl << endl;
						cout << "1. Гравець: " << pl1->Name << " (" << Player_type[(static_cast<int>(pl1->pm))] << ")" <<
							" [" << Player_level[(static_cast<int>(pl1->level))] << "]" << endl;
						cout << "2. Гравець: " << pl2->Name << " (" << Player_type[(static_cast<int>(pl2->pm))] << ")" <<
							" [" << Player_level[(static_cast<int>(pl2->level))] << "]" << endl << endl;
						cout << "0. Вихід" << endl;
						cout << "Ваш вибір: ";				cin >> _plClient;
						switch (_plClient)
						{
						case 1: {pl1->pm = Player_Mode::client;					break; }
						case 2: {pl2->pm = Player_Mode::client;					break; }
						case 0: break;
						default: break;
						}
					}
					system("cls");
					Show_MainHead();
					RunServer(); ChatSC();
					break; }
				case 2: {	_Game_Place = enGame_Place::client;	
					system("cls");
					Show_MainHead(); 
					RunClient(); ChatSC(); break; }
				case 0: {break;}
				}
				break; 
			}
			
		}
		case 3: { 
			if (_Game_Mode == enGame_Mode::net)
			{
				if (_Game_Place == enGame_Place::server) StopServer();
				else if (_Game_Place == enGame_Place::client) StopClient();
			}
			break;
		}
		case 4: { 
			if (_Game_Mode == enGame_Mode::net)
			{
				if (_Game_Place == enGame_Place::server) {
					system("cls");
					Show_MainHead(); ChatSC();
				}
				else										{	
					system("cls");
					Show_MainHead(); ChatSC();
				}		
			}
			break; 
		}
		case 0: { break; }
		default: { break; }
		}
	}
}
//Зміна розміру екрану в залежності від розміру ігрового поля
void SetScrSize() {
	scrsize = max(90,(sbf * 3 + 4) * 2 + 4);
	string s1 = "mode con cols=", s2 = " lines=50";
	s1 = s1 + to_string(scrsize) + s2;
	system(s1.c_str());
}
//Налаштування програми: розмір ігрового поля
void CHM_SettingSize()
{
	bool cont = true;
	int x;
	while (cont)
	{
		cout << "Вкажіть розмір ігрового поля: ([10;20]) (поточний= " << sbf << " ): ";
		cin >> x;
		if (x > 20 || x < 10) { cout << "Помилка!\n"; }
		else {	cont = false;	sbf = x; SetScrSize();}
	}
}
//Налаштування програми: кількість дозволений пострілів в ЗАЛПІ
void CHM_SettingShot()
{
	bool cont = true;
	int y;
	while (cont)
	{
		cout << "Вкажіть кількість пострілів: ([1;5]) (поточний= " << cnt_shot << " ): ";
		cin >> y;
		if (y > 5 || y < 1)	{ cout << "Помилка!\n"; }
		else {	cont = false;	cnt_shot = y;		}
	}
}
//Введення імені, типу гравця
void CHM_SettingPlayer(int i, Player* pl)
{
	string newName = ""; int pmode = 0; int plevel = 0;
	newName = pl->Name;
	cout << "\nНове ім'я гравця: " << i << ": ";	cin >> newName;
	cout << "\nТип гравця " << newName << " (0-комп'ютер, 1-людина, 2-мережевий клієнт): "; cin >> pmode;
	if (pmode == 0) {
		cout << "\nРівень складності " << newName << " (0-новачок, 1-профі): "; cin >> plevel;
	}
	pl->Name = newName;
	switch (pmode)
	{
	case 0: { pl->pm = Player_Mode::comp;
		switch (plevel)
		{
		case 0: pl->level = Player_Level::level0; break;
		case 1: pl->level = Player_Level::level1; break;
		}
		break; }
	case 1: { pl->pm = Player_Mode::human; break; }
	case 2: { pl->pm = Player_Mode::client; break; }
	default: { pl->pm = Player_Mode::comp; break; }
	}
}

#pragma endregion

#pragma region SeaBattle_Show_Functions
//Головна строка
void Show_MainHead()
{
	int sz = scrsize / 2;
	gotoxy(15, 0);	SetColor();	cout << string(5, '*'); SetColor(LightBlue); cout << "Sea Battle"; SetColor();
	cout << " [ " << vGame_Mode[(int)_Game_Mode] << " / " << vGame_Place[(int)_Game_Place] << " ] ";
	if (_Game_Place == enGame_Place::server) {	cout << GetClientName();	}
	else if (_Game_Place == enGame_Place::client) {	cout << GetServerName();	}
	cout << string(5, '*');
	gotoxy(0, 1); 	SetColor(ColorPlayerStat[(static_cast<int>(pl1->ps))]);
	cout << Player_stan[(static_cast<int>(pl1->ps))] << ": ";	SetColor();
	SetColor(LightGreen); 	cout << pl1->Name; 	SetColor();
	cout << "(" << Player_type[(static_cast<int>(pl1->pm))]
		<< ":" << Player_level[(static_cast<int>(pl1->level))] << ")";
	gotoxy(sz, 1);	SetColor(ColorPlayerStat[(static_cast<int>(pl2->ps))]);
	cout << Player_stan[(static_cast<int>(pl2->ps))] << ": ";	SetColor();
	SetColor(Yellow); 	cout << pl2->Name; 	SetColor();
	cout << "(" << Player_type[(static_cast<int>(pl2->pm))]
		<< ":" << Player_level[(static_cast<int>(pl2->level))] << ")";
	SetColor();
	cout << endl;
}
//Відображення 1-го ігрового поля - стара функція
void Show(int** mtx, int k, Player* pl)
{
	cout << "Гравець " << k << ": " << pl->Name << endl << "Постріл: " << pl->shot << " Влучень: " << pl->kill << endl;
	cout << "   ";
	for (int i = 0; i < sbf; i++) { cout << setw(2) << (char)(i + 65); }
	cout << endl;
	for (int i = 0; i < sbf; i++)
	{
		cout << setw(2) << i + 1 << " ";
		for (int j = 0; j < sbf; j++)
		{
			switch (mtx[j][i])
			{
			case 0: { cout << setw(2) << "-";			break; }
			case 8: { cout << setw(2) << (char)(183);	break; }
			case 1:
			case 2:
			case 3:
			case 4: {cout << setw(2) << mtx[j][i]; break; }
			case 6: {cout << setw(2) << "/"; break; }
			case 7: {cout << setw(2) << "+"; break; }
			case 9: {SetColor(LightRed); cout << setw(2) << "X"; break; }
			default:				break;
			};
		}
		cout << endl;
	}
}
//Відображення точки на ігровому полі
char ShowPoint(int val, bool hide = false)
{
	char RetVal = '\0';
	switch (val)
	{
	case 0: RetVal = (hide)?' ':'-'; break;
	case 8: RetVal = (hide) ? ' ' : (char)(183); break;
	case 1:
	case 2:
	case 3:
	case 4: RetVal = (hide) ? ' ' : val + 48; break;
	case 5: SetColor(LightRed, Blue); RetVal = 'x'; break;
	case 6: SetColor(Black, LightGray); RetVal = '/'; break;
	case 7: SetColor(Black, LightGray); RetVal = (hide) ? ' ' : '+'; break;
	case 9: SetColor(LightRed, Blue); RetVal = 'X';   break;
	default:
		break;
	}; return RetVal;
}
//Відображення двох ігрових полей
void Show2(int** mtx1, int** mtx2, Player* pl1, Player* pl2)
{
	bool hideMtx1 = true;
	bool hideMtx2 = true;
	if (_Game_Mode == enGame_Mode::loc)
	{
		if (pl1->pm == Player_Mode::human && pl2->pm == Player_Mode::comp) { hideMtx1 = false; hideMtx2 = true;}
		else if (pl1->pm == Player_Mode::comp && pl2->pm == Player_Mode::human) { hideMtx1 = true; hideMtx2 = false; }
	}
	else if (_Game_Mode == enGame_Mode::net)
	{
		if ((pl1->pm == Player_Mode::human || pl1->pm == Player_Mode::comp) && pl2->pm == Player_Mode::client) { hideMtx1 = false; hideMtx2 = true; }
		else if ((pl2->pm == Player_Mode::human || pl2->pm == Player_Mode::comp) && pl1->pm == Player_Mode::client) { hideMtx1 = true; hideMtx2 = false; }
	}

	int srcs = scrsize / 2; 
	SetColor(Blue);	Show_MainHead();	SetColor();
	gotoxy(0, 2);	cout << "Постріл: " << setw(2) << pl1->shot << " Влучень: " << setw(2) << pl1->kill;
	gotoxy(srcs, 2);	cout << "Постріл: " << setw(2) << pl2->shot << " Влучень: " << setw(2) << pl2->kill << endl;
	gotoxy(3, 4);	SetColor(Green);	for (int i = 0; i < sbf; i++) { cout << setw(2) << (char)(i + 65) << " "; }
	gotoxy(srcs + 3, 4);	SetColor(Green);	for (int i = 0; i < sbf; i++) { cout << setw(2) << (char)(i + 65) << " "; }
	cout << endl;
	SetColor();
	for (int i = 0; i < sbf; i++)
	{
		gotoxy(0, 5+i);	SetColor(Green); cout << setw(2) << i + 1 << " "; SetColor();
		for (int j = 0; j < sbf; j++) { SetColor(White); cout << setw(2) << ShowPoint(mtx1[i][j], hideMtx1) << " "; }
		gotoxy(srcs, 5+i);SetColor(Green); cout << setw(2) << i + 1 << " "; SetColor();
		for (int j = 0; j < sbf; j++) { SetColor(White); cout << setw(2) << ShowPoint(mtx2[i][j], hideMtx2) << " "; }
		cout << endl;
	}
	SetColor();
	cout << endl << endl;
	gotoxy(0, 10 + sbf);	cout << "Ходи гравця " << pl1->Name << ": ";
	gotoxy(srcs, 10 + sbf);	cout << "Ходи гравця " << pl2->Name << ": ";	cout << endl;	//for (auto& shot : pl1->ShotResults)
	int l = 0;
	int c1 = 0, c2 = 0;
	int sizev1 = pl1->ShotResults.size();
	int sizev2 = pl2->ShotResults.size();

	for (int f = 0; f < max(sizev1, sizev2); f++)
	{
		if (f < sizev1 && c1 < srcs-5) {
			gotoxy(c1, 11 + sbf + l);
			SetColor(ColorResult[(int)pl1->ShotResults[f].shres]);
			cout << setw(2) << (char)(pl1->ShotResults[f].pnt.x + 65) << pl1->ShotResults[f].pnt.y + 1 << "=" << Shot_Result_Short[(int)pl1->ShotResults[f].shres] << "; ";
			c1=c1+6;
		}
		SetColor();
		if (f < sizev2 && c2 < srcs-5) {
			gotoxy(srcs + c2, 11 + sbf + l);
			SetColor(ColorResult[(int)pl2->ShotResults[f].shres]);
			cout << setw(2) << (char)(pl2->ShotResults[f].pnt.x + 65) << pl2->ShotResults[f].pnt.y + 1 << "=" << Shot_Result_Short[(int)pl2->ShotResults[f].shres] << "; ";
			c2=c2+6;
		}
		SetColor();
		if (c2 >= srcs-4 || c1 >= srcs-4) { cout << endl; l++; c1 = 0; c2 = 0; }
	}
}

#pragma endregion

#pragma region SeaBattle_Setting_Point
//Допустимі координати
bool Valid_Coord(int x, int y)
{
	if (x < 0 || y < 0 || x > sbf - 1 || y > sbf - 1) return false;
	else return true;
}
//Резурвування полів навколо точки
void Rezerv(int x1, int y1, int x2, int y2, int** mtx, int marker)
{
	for (int x = x1; x <= x2; x++) // проходимо по всіх точках від х1 до х2
	{
		for (int y = y1; y <= y2; y++) // проходимо по всіх точках від у1 до у2
		{
			if (Valid_Coord(x - 1, y - 1) && mtx[y - 1][x - 1] == 0) mtx[y - 1][x - 1] = marker;
			if (Valid_Coord(x - 1, y) && mtx[y][x - 1] == 0)         mtx[y][x - 1] = marker;
			if (Valid_Coord(x - 1, y + 1) && mtx[y + 1][x - 1] == 0) mtx[y + 1][x - 1] = marker;
			if (Valid_Coord(x, y - 1) && mtx[y - 1][x] == 0)         mtx[y - 1][x] = marker;
			if (Valid_Coord(x, y + 1) && mtx[y + 1][x] == 0)         mtx[y + 1][x] = marker;
			if (Valid_Coord(x + 1, y - 1) && mtx[y - 1][x + 1] == 0) mtx[y - 1][x + 1] = marker;
			if (Valid_Coord(x + 1, y) && mtx[y][x + 1] == 0)         mtx[y][x + 1] = marker;
			if (Valid_Coord(x + 1, y + 1) && mtx[y + 1][x + 1] == 0) mtx[y + 1][x + 1] = marker;
		}
	}
}
//Резурвування полів навколо вдалого влучення
void Rezerv_Shot(int x, int y, int** mtx, int marker)
{
	if (Valid_Coord(x - 1, y - 1) && mtx[y - 1][x - 1] == 8) mtx[y - 1][x - 1] = marker;
	if (Valid_Coord(x - 1, y + 1) && mtx[y + 1][x - 1] == 8) mtx[y + 1][x - 1] = marker;
	if (Valid_Coord(x + 1, y - 1) && mtx[y - 1][x + 1] == 8) mtx[y - 1][x + 1] = marker;
	if (Valid_Coord(x + 1, y + 1) && mtx[y + 1][x + 1] == 8) mtx[y + 1][x + 1] = marker;
}

#pragma endregion

#pragma region SeaBattle_Fill_Battle_Fields
//Заповнення ігрового поля морем
void ResetBattleField(int** mtx)
{
	for (int i = 0; i < sbf; i++)
	{
		for (int j = 0; j < sbf; j++)
		{
			mtx[i][j] = 0;
		}
	}
}
//Заповнення ігрового поля кораблями
void SetShipToBattleField(Player *pl, int** mtx)
{
	bool Ok = false;
	ResetBattleField(mtx);
	pl->myEscadra->Ships.clear();
	for (int e = 9; e >= 0; e--)
	{
		int Complite = 0, z = 0;
		while (Ok == false)
		{
			int x, y;
			x = rand() % sbf;
			y = rand() % sbf;
			int v = rand() % 2, h = 1 - v;
			for (int ss = 0; ss < Escadr[e][0]; ss++)
			{
				if (x + ss * h < sbf && y + ss * v < sbf)
				{
					if (mtx[y + ss * v][x + ss * h] == 0)
					{
						Complite++;
					}
				}
			}
			int x2 = 0, y2 = 0;
			if (Complite == Escadr[e][0])
			{
				Ship* ship = new Ship();
				ship->orient = (h == 1) ? 0 : 1;
				for (int ss = 0; ss < Escadr[e][0]; ss++)
				{
					if (x + ss * h < sbf && y + ss * v < sbf)
					{
						if (mtx[y + ss * v][x + ss * h] == 0)
						{
							mtx[y + (ss * v)][x + (ss * h)] = Escadr[e][0];
							Ok = true;
							x2 = x + ss * h;
							y2 = y + ss * v;
							Desk* dsk = new Desk(x2, y2);
							dsk->value = Escadr[e][0];
							ship->Add(dsk);
						}
					}
				}
				pl->myEscadra->Add(ship);
			}
			Complite = 0;
			Rezerv(x, y, x2, y2, mtx,8);
		}Ok = false;
		z++;
	}
}

#pragma endregion

#pragma region SeaBattle_Chack_Point
//Точка є кораблем?
bool Is_Ship(int x, int y, int** mtx)
{
	if (mtx[y][x] == 1 || mtx[y][x] == 2 || mtx[y][x] == 3 || mtx[y][x] == 4) return true; else return false;
}
//Точка має сусідні точки-кораблі/палуби
bool Has_Neib(int x, int y, int** mtx)
{
	int cnt = 0;
	if (Valid_Coord(x - 1, y) && Is_Ship(x - 1, y, mtx)) cnt++;
	if (Valid_Coord(x, y - 1) && Is_Ship(x, y - 1, mtx)) cnt++;
	if (Valid_Coord(x, y + 1) && Is_Ship(x, y + 1, mtx)) cnt++;
	if (Valid_Coord(x + 1, y) && Is_Ship(x + 1, y, mtx)) cnt++;
	if (cnt > 0) return true; else return false;
}

#pragma endregion


#pragma region SeaBattle_Shot_Function
//Отримати координати пострілу від людини
void GetUserShot(Player* pl, int cnt_s)
{
	int x = -1, y = -1;
	int wy = 7 + sbf; // 1 точка на екрані - для стирання повідомлення
	int ws = wy; // 1 точка на екрані - для стирання повідомлення
	bool repeat = true;
	string _Shot = "", _RepShot = "";
	while (repeat)
	{
		for (int f = ws; f <= wy+1; f++) {	gotoxy(0, f); cout << string(100, ' ') << endl;	}
		gotoxy(0, ws-1);
		SetColor();	cout << "Ваш крок, ";  
		SetColor(LightCyan); cout << pl->Name;
		SetColor(); cout << " (кількість пострілів " << cnt_s<<")";
		SetColor(); cout << " {формат " << ((cnt_s>1)?"A1,B4,F5)":"A1)");
		cout << " (0 - вихід)" << ": ";
		SetColor(); cin >> _Shot;
		pl->NewShots.clear();
		if (_Shot == "0")
		{
			repeat = false;
			pl->myShotRes->shres = ShotRes::Exit;
		}
		else {
			transform(_Shot.begin(), _Shot.end(), _Shot.begin(), ::toupper);
			// визначаємо СЕПАРАТОР для кількох постірлів
			const char separator = ',';
			//Визначаємо динамічний масив для збереження кількох пострілів
			std::vector<string> outputArray;
			//Створюємо stream з string
			std::stringstream streamData(_Shot);
			//Строкова змінна для збереження після розподілу строки пострілів
			std::string val;
			//Цикл заповнення динамічного масиву з строки пострілів
			while (getline(streamData, val, separator)) {
				outputArray.push_back(val);
			}
			//Друк пострілів
			//Передача масиву нових пострілів в клас Ігрок
			for (auto& val : outputArray) {
				char x1 = val[0];
				x = x1 - 65;
				y = atoi(val.substr(1, val.length() - 1).c_str()) - 1;
				if (Valid_Coord(x, y) == true)
				{
					pl->myShotRes->pnt.x = x;
					pl->myShotRes->pnt.y = y;
					pl->myShotRes->shres = ShotRes::None;
					pl->NewShots.push_back(*pl->myShotRes);
					repeat = false;
				}
				else
				{
					//Сповіщення про помилкові координати - вже були
					SetColor(Red);	cout << "Помилка: координат " << "x = " << (char)(x + 65) << " y = " << y + 1 << " не існує, спробуйте знову! " << endl;
					SetColor();
					wy = wherey(); // збереження координати точки на екрані
				}
			}
		}
	}
}
//Отримати координати пострілу від клієнта
void GetClientShot(Player* pl, int cnt_s)
{
	int x = -1, y = -1;
	int wy = 7 + sbf; // 1 точка на екрані - для стирання повідомлення
	int ws = wy; // 1 точка на екрані - для стирання повідомлення
	bool repeat = true;
	string _Shot = "", _RepShot = "";
	while (repeat)
	{
		for (int f = ws; f <= wy + 1; f++) { gotoxy(0, f); cout << string(100, ' ') << endl; }
		gotoxy(0, ws - 1);
		SetColor();	cout << "Ваш крок, ";
		SetColor(LightCyan); cout << pl->Name;
		SetColor(); cout << " (кількість пострілів " << cnt_s << ")";
		SetColor(); cout << " {формат " << ((cnt_s > 1) ? "A1,B4,F5)" : "A1)");
		cout << " (0 - вихід)" << ": ";
		SetColor(); cin >> _Shot;
		pl->NewShots.clear();
		if (_Shot == "0")
		{
			repeat = false;
			pl->myShotRes->shres = ShotRes::Exit;
		}
		else {
			transform(_Shot.begin(), _Shot.end(), _Shot.begin(), ::toupper);
			// визначаємо СЕПАРАТОР для кількох постірлів
			const char separator = ',';
			//Визначаємо динамічний масив для збереження кількох пострілів
			std::vector<string> outputArray;
			//Створюємо stream з string
			std::stringstream streamData(_Shot);
			//Строкова змінна для збереження після розподілу строки пострілів
			std::string val;
			//Цикл заповнення динамічного масиву з строки пострілів
			while (getline(streamData, val, separator)) { outputArray.push_back(val); }
			//Друк пострілів
			//Передача масиву нових пострілів в клас Ігрок
			for (auto& val : outputArray) {
				char x1 = val[0];
				x = x1 - 65;
				y = atoi(val.substr(1, val.length() - 1).c_str()) - 1;
				if (Valid_Coord(x, y) == true)
				{
					pl->myShotRes->pnt.x = x;
					pl->myShotRes->pnt.y = y;
					pl->myShotRes->shres = ShotRes::None;
					pl->NewShots.push_back(*pl->myShotRes);
					repeat = false;
				}
				else
				{
					//Сповіщення про помилкові координати - вже були
					SetColor(Red);	cout << "Помилка: координат " << "x = " << (char)(x + 65) << " y = " << y + 1 << " не існує, спробуйте знову! " << endl;
					SetColor();
					wy = wherey(); // збереження координати точки на екрані
				}
			}
		}
	}
}
//Згенерувати постріл для комп'ютера
void GetCompShot(Player* pl)
{
	switch (pl->level)
	{
	case Player_Level::level0:
		pl->myShotRes->pnt.x = rand() % sbf;
		pl->myShotRes->pnt.y = rand() % sbf;
		break;
	case Player_Level::level1:
		if (pl->CompHit == false)
		{
			pl->myShotRes->pnt.x = rand() % sbf;
			pl->myShotRes->pnt.y = rand() % sbf;
		}
		else
		{
			if (pl->SecondCompShot == false)
			{
				while (true)
				{
					pl->ShotDirection = 1 + rand() % 4; // випадковий напрямок
					if (pl->ShotDirection == 1 &&
						pl->CompMiss[0] == false &&
						Valid_Coord(pl->myShotRes->pnt.x, pl->myShotRes->pnt.y - 1)) {
						pl->myShotRes->pnt.y--; break; //Стріляє вище
					}
					else if (pl->ShotDirection == 2 &&
						pl->CompMiss[1] == false &&
						Valid_Coord(pl->myShotRes->pnt.x - 1, pl->myShotRes->pnt.y)) {
						pl->myShotRes->pnt.x--; break; //Стріляє лівіше
					}
					else if (pl->ShotDirection == 3 &&
						pl->CompMiss[2] == false &&
						Valid_Coord(pl->myShotRes->pnt.x, pl->myShotRes->pnt.y + 1)) {
						pl->myShotRes->pnt.y++; break; //Стріляє нижче
					}
					else if (pl->ShotDirection == 4 &&
						pl->CompMiss[3] == false &&
						Valid_Coord(pl->myShotRes->pnt.x + 1, pl->myShotRes->pnt.y)) {
						pl->myShotRes->pnt.x++; break; //Стріляє правіше
					}
					else
					{
						pl->CompMiss[static_cast<int>(pl->ShotDirection) - 1] = true; //Якщо нікуди не вийшло вистрілити
					}
					if (pl->CompMiss[0] == true && pl->CompMiss[1] == true &&
						pl->CompMiss[2] == true && pl->CompMiss[3] == true)
					{
						break; // спроб більше немає
					}
				}
			}
			else
			{ //Приидругому потраплянні стріляє в напрямку попереднього пострілу
				if (pl->ShotDirection == 1 &&
					Valid_Coord(pl->myShotRes->pnt.x, pl->myShotRes->pnt.y - 1)) {
					pl->myShotRes->pnt.y--;
				}
				else if (pl->ShotDirection == 2 &&
					Valid_Coord(pl->myShotRes->pnt.x - 1, pl->myShotRes->pnt.y)) {
					pl->myShotRes->pnt.x--;
				}
				else if (pl->ShotDirection == 3 &&
					Valid_Coord(pl->myShotRes->pnt.x, pl->myShotRes->pnt.y + 1)) {
					pl->myShotRes->pnt.y++;
				}
				else if (pl->ShotDirection == 4 &&
					Valid_Coord(pl->myShotRes->pnt.x + 1, pl->myShotRes->pnt.y)) {
					pl->myShotRes->pnt.x++;
				}
				else { pl->SecondCompShot = false; }
			}
		}

		break;
	}
}
//Вибір координат для пострілу
void ChooseCoordForShot(Player* pl, int cnt_s)
{
	pl->NewShots.clear();
	if(_Game_Mode==enGame_Mode::loc || (_Game_Mode == enGame_Mode::net && _Game_Place == enGame_Place::server))
	{ 
		switch (pl->pm)
		{
		case Player_Mode::comp: 	// грає комп'ютер
			for(int f=1;f<=cnt_s;f++)
			{
				GetCompShot(pl);
				pl->NewShots.push_back(*pl->myShotRes);
			}
			break;
		case Player_Mode::human: 	// грає людина
			GetUserShot(pl, cnt_s);
			break;
		case Player_Mode::client: 	// грає клієнт //змінити на клієнта
			for (int f = 1; f <= cnt_s; f++)
			{
				//змінити на клієнта
				GetCompShot(pl);
				pl->NewShots.push_back(*pl->myShotRes);
			}
			break;
		default: break;
		}
	}
	else if (_Game_Mode == enGame_Mode::loc || (_Game_Mode == enGame_Mode::net && _Game_Place == enGame_Place::client))
	{
		switch (pl->pm)
		{
		case Player_Mode::comp: 	// грає комп'ютер
			for (int f = 1; f <= cnt_s; f++)
			{
				GetCompShot(pl);
				pl->NewShots.push_back(*pl->myShotRes);
			}
			break;
		case Player_Mode::human: 	// грає людина
			GetUserShot(pl, cnt_s);
			break;
		case Player_Mode::client: 	// грає клієнт
			for (int f = 1; f <= cnt_s; f++)
			{
				GetCompShot(pl);
				pl->NewShots.push_back(*pl->myShotRes);
			}
			break;
		default: break;
		}
	}
}
//Функція пострілів
void Shot(Player* pl, int** mtx, Player* pl_enemy)
{
	int count_shots = cnt_shot;
	Point* pt;
	Ship* ship = new Ship();
	int x = -1, y = -1;
	bool TryAgain = true;
	while (TryAgain && pl->ps == Player_Stat::InGame && pl->myShotRes->shres != ShotRes::Exit)
	{
		ChooseCoordForShot(pl,count_shots);
		for (auto& newS : pl->NewShots)
		{
			pt = &newS.pnt;
			if (newS.shres != ShotRes::Exit && Valid_Coord(pt->x, pt->y))
			{
				switch (mtx[pt->y][pt->x])
				{
				case 0:
				case 8: mtx[pt->y][pt->x] = 6;
					pl->shot++;
					newS.shres = ShotRes::Miss;
					pl->Add_Shot(&newS);
					TryAgain = false;
					if (pl->level == Player_Level::level1)
					{
						pl->NextCompHit = false;
						if (pl->ShotDirection != -1) {
							pl->CompMiss[pl->ShotDirection - 1] = true;
							if ((pl->CompMiss[0] == true && pl->CompMiss[1] == true && pl->CompMiss[2] == true && pl->CompMiss[3] == true)
								|| pl->SecondCompShot == true) {
								pl->CompHit = false;
								for (int x = 0; x != 4; x++) {
									pl->CompMiss[x] = false;
								}
							}
							if (pl->SecondCompShot == false && pl->CompHit == true) {//1y--/2x--/3y++/4x++
								if (pl->ShotDirection == 1) {
									pt->y++; //Направляє координату правіше
								}
								else if (pl->ShotDirection == 2) {
									pt->x++; //Направляє координату нижче
								}
								else if (pl->ShotDirection == 3) {
									pt->y--; //Направляє координату лівіше
								}
								else if (pl->ShotDirection == 4) {
									pt->x--; //Направляє координату вище
								}
							}
						}
						pl->SecondCompShot = false;
					}
					break;
				case 1:
				case 2:
				case 3:
				case 4:
					pl->shot++;
					pl->kill++;
					if (pl->pm == Player_Mode::comp && pl->level == Player_Level::level1)
					{
						if (pl->CompHit == true) {
							pl->NextCompHit = true;
						}
						if (pl->NextCompHit == true) {
							pl->SecondCompShot = true;
						}
						pl->CompHit = true;
					}

					if (pl_enemy->myEscadra->GetShip(pt->x, pt->y, ship) == true) { ship->stan = ShipStan::wound; }
					if (Has_Neib(pt->x, pt->y, mtx)) {
						mtx[pt->y][pt->x] = 5;
						newS.shres = ShotRes::Wound;
					}
					else {
						mtx[pt->y][pt->x] = 9;
						newS.shres = ShotRes::Kill;
					}
					Rezerv_Shot(pt->x, pt->y, mtx, 7);
					TryAgain = false;
					pl->Add_Shot(&newS);
					break;
				case 5:
				case 6:                                                   //Вже промазав
				case 7:                                                   //Резерв діагональ
				case 9: 
				if (pl->pm == Player_Mode::human) {
					SetColor(Red);
					cout << pl->Name << ", ви вже поцілили в цю точку, оберіть іншу!\n"; 
					SetColor();
					system("pause");
				}		
		  			  count_shots = 1;
					  TryAgain = true;
					  //старт
					  if (pl->pm == Player_Mode::comp && pl->level == Player_Level::level1)
					  {
						  pl->NextCompHit = true;
						  if (pl->ShotDirection != -1) {
							  pl->CompMiss[pl->ShotDirection - 1] = true;
							  if ((pl->CompMiss[0] == true && pl->CompMiss[1] == true && pl->CompMiss[2] == true && pl->CompMiss[3] == true)
								  || pl->SecondCompShot == true) {
								  pl->CompHit = false;
								  for (int x = 0; x != 4; x++) {
									  pl->CompMiss[x] = false;
								  }
							  }
							  if (pl->SecondCompShot == false && pl->CompHit == true) {//1y--/2x--/3y++/4x++
								  if (pl->ShotDirection == 1) {
									  pt->y++; //Напраляє координату правіше
								  }
								  else if (pl->ShotDirection == 2) {
									  pt->x++; //Напраляє координату нижче
								  }
								  else if (pl->ShotDirection == 3) {
									  pt->y--; //Напраляє координату лівіше
								  }
								  else if (pl->ShotDirection == 4) {
									  pt->x--; //Напраляє координату вище
								  }
							  }
						  }
						  pl->SecondCompShot = false;
					  }	
					  pl->myShotRes->shres = ShotRes::None;
					  break;                                                  //Вже поцілив
				default: TryAgain = false;
					break;
				};
			}
		}
	}
}

#pragma endregion

//Функція ігри
void M_Game()
{
	bool cont = true;
	system("cls");	gotoxy(0, 0); cout << "Гра";
	srand(static_cast<unsigned int>(time(NULL))); // генератор довільних чисел
	mtx1 = new int* [sbf];	for (int i = 0; i < sbf; i++)	mtx1[i] = new int[sbf]; //ігрове поле 1
	mtx2 = new int* [sbf];	for (int i = 0; i < sbf; i++)	mtx2[i] = new int[sbf]; //ігрове поле 2
	switch (_Game_Mode)
	{
	case enGame_Mode::loc:
		SetShipToBattleField(pl1, mtx1);
		SetShipToBattleField(pl2, mtx2);
		break;
	case enGame_Mode::net:
		switch (_Game_Place)
		{
		case enGame_Place::server:
		case enGame_Place::client:
			switch (pl1->pm)
			{
			case Player_Mode::client:
				ResetBattleField(mtx1);
				SetShipToBattleField(pl2, mtx2);
				break;
			case Player_Mode::human:
			case Player_Mode::comp:
				ResetBattleField(mtx2);
				SetShipToBattleField(pl1, mtx1);
				break;
			default: break;
			}
			switch (pl2->pm)
			{
			case Player_Mode::client:
				ResetBattleField(mtx2);
				SetShipToBattleField(pl1, mtx1);
				break;
			case Player_Mode::human:
			case Player_Mode::comp:
				ResetBattleField(mtx1);
				SetShipToBattleField(pl2, mtx2);
				break;
			}
			break;
		}
		break;
		default: break;
	}
	pl1->New_Game();
	pl2->New_Game();
	Show2(mtx1, mtx2, pl1, pl2);
	do
	{
		do
		{
			Shot(pl1, mtx2, pl2);
			gotoxy(0, 0); cout << "Гра";
			Show2(mtx1, mtx2, pl1, pl2);
			if (pl1->kill == WIN_CNT)
			{
				cont = false;
				pl1->ps = Player_Stat::Winner;
				pl2->ps = Player_Stat::Loser;
				break;
			}

		} while (pl1->myShotRes->shres == ShotRes::Wound || pl1->myShotRes->shres == ShotRes::Kill);
		do
		{
			Shot(pl2, mtx1, pl1);
			gotoxy(0, 0); cout << "Гра";
			Show2(mtx1, mtx2, pl1, pl2);
			if (pl2->kill == WIN_CNT)
			{
				cont = false;
				pl2->ps = Player_Stat::Winner;
				pl1->ps = Player_Stat::Loser;
				break;
			}
		} while (pl2->myShotRes->shres == ShotRes::Wound || pl2->myShotRes->shres == ShotRes::Kill);

	} while (cont == true && ( pl1->myShotRes->shres != ShotRes::Exit || pl2->myShotRes->shres != ShotRes::Exit));
	gotoxy(0, 0); cout << "Гра ЗАВЕРШЕНА";
	Show2(mtx1, mtx2, pl1, pl2);
	cout << endl;
	system("pause");
}
//головна програма
int main()
{
	setlocale(LC_ALL, "ru");
	SetScrSize();
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Init_Lib();

	int retCode = 1;
	pl1 = new Player("Кампуктер1",	Player_Mode::comp);
	pl2 = new Player("Руся",		Player_Mode::comp);
	pl1->level = Player_Level::level1;
	pl2->level = Player_Level::level1;
	while (retCode != 0)
	{
		system("cls");
		Show_MainHead();
		retCode = M_MainMenu();
		switch (retCode)
		{
		case 1: { M_Info();				} break;
		case 2: { M_Setting();			} break;
		case 3: { M_Game();				} break;
		case 0: { cout << "\t\t\t\tКінець гри!\n";	} break;
		default: break;
		}
	}
	system("pause");
	return 0;
}
