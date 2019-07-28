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


long long times[84] = { 0 };
double prob[84][4] = { 0 };

void HuCards(Tiles_ * Tiles, Tiles_ deads, int turn) 
{
	times[turn - 1]++;

	bool ting = false;
	
	for (int i = 0; i < 4; i++) {
		if (CountSteps(Tiles[i]) == 1) {
			ting = true;
		}
	}
	
	if (ting == false) {
		return;
	}


	long long cards[5] = { 0 };
	long long hucards[5] = { 0 };

	for (int t = 0; t < 34; t++) {

		
		bool hu = false;

		for (int i = 0; i < 4; i++) {
			Tiles[i].tiles[t / 9] += 1 << ((t % 9) * 3);
			if (CountSteps(Tiles[i]) == 0) {
				hu = true;
			}
			Tiles[i].tiles[t / 9] -= 1 << ((t % 9) * 3);
		}
			
		int deadcount = (deads.tiles[t / 9] >> ((t % 9) * 3)) & 7; //有幾張死牌已經出現了 ex有人碰過，此時deadcount為3
		if (deadcount < 0 || deadcount > 4) {
			cout << "deadcount error " << deadcount << endl;
		}
		cards[deadcount]++;
		if (hu) {
			hucards[deadcount]++;
		}
		
	}

	if (cards[0] + cards[1] + cards[2] + cards[3] + cards[4] != 34) {
		cout << "cards count error " << cards[0] + cards[1] + cards[2] + cards[3] << endl;
	}

	for (int i = 0; i < 4; i++) {
		if (hucards[i] > 0) {
			prob[turn - 1][i] += (double)hucards[i] / cards[i];
		}
	}
	
	
}

int main()
{
	


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

	//int stepToHu;
	ofstream outfile;
	outfile.open("result.csv");

	clock_t start, end;
	start = clock();
	fstream initial, process;
	string s;
	vector<int> initialv, processv;
	Tiles_ pTiles[4], deads;
	int processNo, turn;
	int t;

	initial.open("100ExpertsInitial.csv");
	process.open("100ExpertsProcess.csv");

	//skip掉標頭列
	getline(initial, s);  
	getline(process, s);


	bool lastRead = false; //看他是否要讀上一次讀的最後一行，換局時使用，因為上一輪已經讀了那行了

	for (int i = 0; i < 160000; i++)
	{
		//手牌初始化，發牌
		for (int i = 0; i < 4; i++) {
			pTiles[i].tiles[0] = pTiles[i].tiles[1] = pTiles[i].tiles[2] = pTiles[i].tiles[3] = 0;
			getline(initial, s);
			initialv = split(s, ",");
			int temp = Card2Int(initialv[2]);
			pTiles[i] = Int2Tiles(temp);
			for (int j = 3; j <= 17; j++) {
				addTiles(pTiles[i], Card2Int(initialv[j]));
			}
			processNo = initialv[0];
		}
		
		deads.tiles[0] = 0;
		deads.tiles[1] = 0;
		deads.tiles[2] = 0;
		deads.tiles[3] = 0;
		turn = 1;

		while (1) {
			

			if (lastRead == false) {
				getline(process, s);
				processv = split(s, ",");
				if (processv[0] != processNo) {
					lastRead = true;
					break;
				}
			}
			else {
				lastRead = false;
				
			}


			int player = processv[2];

			int action = (processv[3] >> 8) & 15;
			int card = processv[3] & 255;

			if (action == 1) //摸牌
			{
				t = Card2Int(card);
				if (t > 33)
					continue;
				pTiles[player].tiles[t / 9] += 1 << ((t % 9) * 3);
			}
			else if (action == 2) //丟牌
			{
				t = Card2Int(card);
				pTiles[player].tiles[t / 9] -= 1 << ((t % 9) * 3);

				//紀錄

				HuCards(pTiles, deads, turn);

				//紀錄end

				deads.tiles[t / 9] += 1 << ((t % 9) * 3);
				turn++;
				if (turn >= 85) {
					cout << "too big turn " << turn << endl;
				}
			}
			else if (action == 3) //碰
			{
				t = Card2Int(card);
				pTiles[player].tiles[t / 9] -= 2 << ((t % 9) * 3);

				deads.tiles[t / 9] += 2 << ((t % 9) * 3);
			}
			else if (action == 4) //吃
			{
				t = Card2Int(card);
				if (((processv[3] >> 12) & 0xf) == 1)
				{
					int z = t - 1;
					pTiles[player].tiles[z / 9] -= 1 << ((z % 9) * 3);
					deads.tiles[t / 9] += 1 << ((z % 9) * 3);
					z = t - 2;
					pTiles[player].tiles[z / 9] -= 1 << ((z % 9) * 3);
					deads.tiles[t / 9] += 1 << ((z % 9) * 3);

				}
				else if (((processv[3] >> 12) & 0xf) == 2)
				{
					int z = t + 1;
					pTiles[player].tiles[z / 9] -= 1 << ((z % 9) * 3);
					deads.tiles[t / 9] += 1 << ((z % 9) * 3);
					z = t - 1;
					pTiles[player].tiles[z / 9] -= 1 << ((z % 9) * 3);
					deads.tiles[t / 9] += 1 << ((z % 9) * 3);
				}
				else if (((processv[3] >> 12) & 0xf) == 4)
				{
					int z = t + 1;
					pTiles[player].tiles[z / 9] -= 1 << ((z % 9) * 3);
					deads.tiles[t / 9] += 1 << ((z % 9) * 3);
					z = t + 2;
					pTiles[player].tiles[z / 9] -= 1 << ((z % 9) * 3);
					deads.tiles[t / 9] += 1 << ((z % 9) * 3);
				}
			}
			else if (action == 5 || action == 6 || action == 7) //胡牌
			{

			}
			else if (action == 8) //明槓
			{
				t = Card2Int(card);
				pTiles[player].tiles[t / 9] -= 3 << ((t % 9) * 3);
				deads.tiles[t / 9] += 3 << ((t % 9) * 3);
			}
			else if (action == 9) //暗槓
			{
				t = Card2Int(card);
				pTiles[player].tiles[t / 9] -= 4 << ((t % 9) * 3);
			}
			else if (action == 10) //加槓
			{
				t = Card2Int(card);
				pTiles[player].tiles[t / 9] -= 1 << ((t % 9) * 3);
				deads.tiles[t / 9] += 1 << ((t % 9) * 3);
			}
		}



	}

	

	for (int i = 0; i < 84; i++) {
		outfile << times[i] << ",";
	}
	outfile << endl;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 84; j++) {
			if (times[j] == 0) {
				outfile << 0 << ",";
			}
			else {
				outfile << prob[j][i] / times[j] << ",";
			}
			
		}
		outfile << endl;
	}
	

	end = clock();
	double TotalTime = (double)(end - start) / CLOCKS_PER_SEC;
	cout << TotalTime << " second\n";
	system("PAUSE");
	return 0;
}