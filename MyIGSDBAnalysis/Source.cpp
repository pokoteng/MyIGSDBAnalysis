#include<iostream>
#include<fstream>
#include<cstdio>
#include<ctime>
#include<string>
#include<vector>
#include<unordered_map>


//0101, 0010, ,
//
//1000 00 01 02 03  null  00 01 02 03  null   00 01 02 03  null
//0100 10 11 12 13        10 11 12 13         10 11 12 13
//0010 20 21 22 23        20 21 22 23         20 21 22 23
//0001
//
//
//1100
//1010
//1001
//0110
//0101
//0011
//
//計算10種的數量
//
//計算前三種童和不同的數量

using namespace std;

typedef struct
{
	int tiles[4];
	void Print() {
		for (int i = 0; i < 4; i++) {
			cout << oct << tiles[i] << endl;
		}

		cout << dec;
	};
}Tiles_;

int Card2Int(int c)
{
	int t = ((c >> 4) & 0xf) * 9 + (c & 0xf);
	if (t == 31)
		t = 33;
	else if (t == 33)
		t = 31;
	return t;
}

void addTiles(Tiles_ &T, int t)
{
	T.tiles[t / 9] += 1 << ((t % 9) * 3);
}

void minusTiles(Tiles_ &T, int t)
{
	T.tiles[t / 9] -= 1 << ((t % 9) * 3);
}

Tiles_ Int2Tiles(int t)
{
	Tiles_ T = { 0 };
	addTiles(T, t);
	return T;
}

//int mj[5];
//
//void TilesToMJ(const Tiles_ & T) {
//	mj[0] = T.tiles[0];
//	mj[1] = T.tiles[1];
//	mj[2] = T.tiles[2];
//	mj[3] = T.tiles[3] >> 12;
//	mj[4] = (T.tiles[3] & 0b111111111111);
//
//}


vector<int> split(const string& s, const char* delim)
{
	vector<int> tokens;
	char* str = new char[s.size() + 1]();
	copy(s.begin(), s.end(), str);
	char* next = nullptr;
	char* token = strtok_s(str, delim, &next);
	while (token != nullptr)
	{
		tokens.push_back(stoi(token));
		token = strtok_s(nullptr, delim, &next);
	}
	delete[] str;
	return tokens;
}

int Count_(const Tiles_ &T)
{
	int n = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 9; j++)
			n += (T.tiles[i] >> (j * 3)) & 7;
	}
	return n;
}

unordered_map<int, int> SuitMap, HonorMap;

int CountSteps(const Tiles_& T)
{
	int total, steps, min = 0xff;
	total = Count_(T);
	int SV[3] = { SuitMap[T.tiles[0]],SuitMap[T.tiles[1]], SuitMap[T.tiles[2]] };
	int HV = HonorMap[T.tiles[3]];
	for (int i = 0; i < 81; i++)
	{
		int group = 0, pair = 0, pre_group = 0, value = 0;
		for (int j = 0; j < 3; j++)
		{
			//value = SuitMap[T.tiles[j]];
			value = SV[j];
			if (((int)(i / pow(3, j)) % 3) == 1)
				value /= 1000;
			else if (((int)(i / pow(3, j)) % 3) == 2)
				value /= 1000000;
			group += (value % 1000) / 100;
			pair += value % 10;
			pre_group += ((value % 100) / 10) - (value % 10);
		}
		//value = HonorMap[T.tiles[3]];
		value = HV;
		if (((i / 27) % 3) == 1)
			value /= 1000;
		else if (((i / 27) % 3) == 2)
			value /= 1000000;
		group += (value % 1000) / 100;
		pair += value % 10;
		pre_group += ((value % 100) / 10) - (value % 10);
		steps = total / 3 * 2 + 1;
		steps -= group * 2 + pre_group + pair;
		if (group + pre_group + pair > total / 3 + 1)
			steps += group + pre_group + pair - (total / 3 + 1);
		if (pre_group != 0 && pre_group >= (total / 3 + 1 - group) && !pair)
			steps += 1;
		if (steps < min)
			min = steps;
	}
	return min;
}

Tiles_ TingCards(Tiles_ & Tiles) {
	Tiles_ res = { 0 };
	if (CountSteps(Tiles) != 1)
		return res;
	else {
		for (int i = 0; i < 34; i++) {
			addTiles(Tiles, i);
			if (CountSteps(Tiles) == 0) {
				addTiles(res, i);
			}

			minusTiles(Tiles, i);
		}
		return res;
	}

}

int main()
{
	
	int r[4][13] = { 0 };

	fstream SuitFile, HonorFile;
	SuitFile.open("Suit.txt", ios::in);
	HonorFile.open("Honor.txt", ios::in);
	if (!SuitFile || !HonorFile)
		exit(-1);

	int tiles, value;
	while (SuitFile >> tiles >> value)
		SuitMap[tiles] = value;
	while (HonorFile >> tiles >> value)
		HonorMap[tiles] = value;

	int stepToHu;

	clock_t start, end;
	start = clock();
	fstream tte, ti, tp;
	string s;
	vector<int> ttes, tis, tps;
	Tiles_ tTiles;

	tte.open("TingTurnExp500.csv", fstream::in);
	ti.open("TingInitial.csv");
	tp.open("TingProcess.csv");

	//skip掉標頭列
	getline(tte, s);  
	getline(ti, s);
	getline(tp, s);

	for (int i = 0; i < 100000; i++)
	{


		getline(tte, s);
		ttes = split(s, ",");

		int counter = 0;

		while (getline(ti, s)) {
			tis = split(s, ",");

			if (ttes[0] == tis[0] && ttes[2] == tis[1]) {

				int temp = Card2Int(tis[2]);
				tTiles = Int2Tiles(temp);

				for (int i = 3; i <= 17; i++) {
					addTiles(tTiles, Card2Int(tis[i]));
				}

				

				break;
			}

		}


		while (getline(tp, s)) {
			tps = split(s, ",");

			if (ttes[0] == tps[1] && ttes[2] == tps[3]) {
				int actionCard;
				int previousCard[3] = { -1, -1, -1 }; // 前三張丟的牌

				while (ttes[1] != tps[2] && counter < 5000) {
					actionCard = tps[4];
					int action = (actionCard >> 8) & 15;
					int card = actionCard & 255;
					int t;

					getline(tp, s);
					tps = split(s, ",");

					if (action == 1) //摸牌
					{
						t = Card2Int(card);
						if (t > 33)
							continue;
						tTiles.tiles[t / 9] += 1 << ((t % 9) * 3);
					}
					else if (action == 2) //丟牌
					{
						t = Card2Int(card);
						tTiles.tiles[t / 9] -= 1 << ((t % 9) * 3);

						previousCard[2] = previousCard[1];
						previousCard[1] = previousCard[0];
						previousCard[0] = t;

					}
					else if (action == 3) //碰
					{
						t = Card2Int(card);
						tTiles.tiles[t / 9] -= 2 << ((t % 9) * 3);
					}
					else if (action == 4) //吃
					{
						t = Card2Int(card);
						if (((actionCard >> 12) & 0xf) == 1)
						{
							int z = t - 1;
							tTiles.tiles[z / 9] -= 1 << ((z % 9) * 3);
							z = t - 2;
							tTiles.tiles[z / 9] -= 1 << ((z % 9) * 3);
						}
						else if (((actionCard >> 12) & 0xf) == 2)
						{
							int z = t + 1;
							tTiles.tiles[z / 9] -= 1 << ((z % 9) * 3);
							z = t - 1;
							tTiles.tiles[z / 9] -= 1 << ((z % 9) * 3);
						}
						else if (((actionCard >> 12) & 0xf) == 4)
						{
							int z = t + 1;
							tTiles.tiles[z / 9] -= 1 << ((z % 9) * 3);
							z = t + 2;
							tTiles.tiles[z / 9] -= 1 << ((z % 9) * 3);
						}
					}
					else if (action == 5 || action == 6 || action == 7) //胡牌
					{
						cout << "Action5 error" << endl;
					}
					else if (action == 8) //明槓
					{
						t = Card2Int(card);
						tTiles.tiles[t / 9] -= 3 << ((t % 9) * 3);
					}
					else if (action == 9) //暗槓
					{
						t = Card2Int(card);
						tTiles.tiles[t / 9] -= 4 << ((t % 9) * 3);
					}
					else if (action == 10) //加槓
					{
						t = Card2Int(card);
						tTiles.tiles[t / 9] -= 1 << ((t % 9) * 3);
					}

				}


				Tiles_ tingCards = TingCards(tTiles);

				if (tingCards.tiles[0] != 0 && tingCards.tiles[1] == 0 && tingCards.tiles[2] == 0 && tingCards.tiles[3] == 0) {
					r[0][0]++;

					for (int i = 0; i < 3; i++) {
						
						if (previousCard[i] == -1) {
							r[i + 1][12]++;
							cout << i << " " << ttes[0] << endl;
						}
						else if (previousCard[i] / 9 == 0) {
							r[i + 1][0]++;
						}
						else if (previousCard[i] / 9 == 1) {
							r[i + 1][1]++;
						}
						else if (previousCard[i] / 9 == 2) {
							r[i + 1][2]++;
						}
						else if (previousCard[i] / 9 == 3) {
							r[i + 1][3]++;
						}
						
					}

					
				}
				else if (tingCards.tiles[0] == 0 && tingCards.tiles[1] != 0 && tingCards.tiles[2] == 0 && tingCards.tiles[3] == 0) {
					r[0][1]++;
					for (int i = 0; i < 3; i++) {

						if (previousCard[i] == -1) {
							r[i + 1][12]++;
							cout << i << " " << ttes[0] << endl;
						}
						else if (previousCard[i] / 9 == 0) {
							r[i + 1][4]++;
						}
						else if (previousCard[i] / 9 == 1) {
							r[i + 1][5]++;
						}
						else if (previousCard[i] / 9 == 2) {
							r[i + 1][6]++;
						}
						else if (previousCard[i] / 9 == 3) {
							r[i + 1][7]++;
						}

					}
				}
				else if (tingCards.tiles[0] == 0 && tingCards.tiles[1] == 0 && tingCards.tiles[2] != 0 && tingCards.tiles[3] == 0) {
					r[0][2]++;
					for (int i = 0; i < 3; i++) {

						if (previousCard[i] == -1) {
							r[i + 1][12]++;
							cout << i << " " << ttes[0] << endl;
						}
						else if (previousCard[i] / 9 == 0) {
							r[i + 1][8]++;
						}
						else if (previousCard[i] / 9 == 1) {
							r[i + 1][9]++;
						}
						else if (previousCard[i] / 9 == 2) {
							r[i + 1][10]++;
						}
						else if (previousCard[i] / 9 == 3) {
							r[i + 1][11]++;
						}

					}
				}
				else if (tingCards.tiles[0] == 0 && tingCards.tiles[1] == 0 && tingCards.tiles[2] == 0 && tingCards.tiles[3] != 0) {
					r[0][3]++;
				}
				else if (tingCards.tiles[0] != 0 && tingCards.tiles[1] != 0 && tingCards.tiles[2] == 0 && tingCards.tiles[3] == 0) {
					r[0][4]++;
				}
				else if (tingCards.tiles[0] != 0 && tingCards.tiles[1] == 0 && tingCards.tiles[2] != 0 && tingCards.tiles[3] == 0) {
					r[0][5]++;
				}
				else if (tingCards.tiles[0] != 0 && tingCards.tiles[1] == 0 && tingCards.tiles[2] == 0 && tingCards.tiles[3] != 0) {
					r[0][6]++;
				}
				else if (tingCards.tiles[0] == 0 && tingCards.tiles[1] != 0 && tingCards.tiles[2] != 0 && tingCards.tiles[3] == 0) {
					r[0][7]++;
				}
				else if (tingCards.tiles[0] == 0 && tingCards.tiles[1] != 0 && tingCards.tiles[2] == 0 && tingCards.tiles[3] != 0) {
					r[0][8]++;
				}
				else if (tingCards.tiles[0] == 0 && tingCards.tiles[1] == 0 && tingCards.tiles[2] != 0 && tingCards.tiles[3] != 0) {
					r[0][9]++;
				}

				break;
			}

		}



	}

	for (int i = 0; i < 10; i++) {
		cout << "R 0 " << i << ": " << r[0][i] << endl;
	}

	cout << endl;


	//印出前三張牌的統計資料
	for (int i = 1; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			cout << "R " << i << " " << j << ": " << r[i][j] << endl;
		}
		cout << endl;
	}

	

	end = clock();
	double TotalTime = (double)(end - start) / CLOCKS_PER_SEC;
	cout << TotalTime << " second\n";
	system("PAUSE");
	return 0;
}