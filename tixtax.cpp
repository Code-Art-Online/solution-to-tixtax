#include <iostream>
#include <bitset>
#include <vector>
using namespace std;

#define NULL 0
#define RED 1
#define BLUE 2
#define FULL 3
#define RED_WIN 4
#define BLUE_WIN 5
#define TIE 6

struct Board {
	bitset<81> red;
	bitset<81> blue;
	bitset<81> occupied;
	Board(bitset<81> reds, bitset<81> blues, int pos) {
		// pos: 0~8
		red = (reds >> pos * 9) & bitset<81>(511);
		blue = (blues >> pos * 9) & bitset<81>(511);
		occupied = red | blue;
	}
};

const bitset<81> masks[] = { 292, 146, 73, 448, 56, 7, 273, 84 };
int get_status(Board board) {
	for (auto i : masks) {
		if ((board.red & i) == i)
			return RED;
		if ((board.blue & i) == i)
			return BLUE;
	}
	if (board.occupied == 511)
		return FULL;
	return NULL;
}
int big_check(bitset<81> reds, bitset<81> blues) {
	int res[9];
	for (int i = 0; i < 9; ++i)
		res[i] = get_status(Board(reds, blues, i));
	if (res[0] == RED && res[1] == RED && res[2] == RED ||
		res[3] == RED && res[4] == RED && res[5] == RED ||
		res[6] == RED && res[7] == RED && res[8] == RED ||
		res[0] == RED && res[3] == RED && res[6] == RED ||
		res[1] == RED && res[4] == RED && res[7] == RED ||
		res[2] == RED && res[5] == RED && res[8] == RED ||
		res[0] == RED && res[4] == RED && res[8] == RED ||
		res[2] == RED && res[4] == RED && res[6] == RED)
		return RED_WIN;
	if (res[0] == BLUE && res[1] == BLUE && res[2] == BLUE ||
		res[3] == BLUE && res[4] == BLUE && res[5] == BLUE ||
		res[6] == BLUE && res[7] == BLUE && res[8] == BLUE ||
		res[0] == BLUE && res[3] == BLUE && res[6] == BLUE ||
		res[1] == BLUE && res[4] == BLUE && res[7] == BLUE ||
		res[2] == BLUE && res[5] == BLUE && res[8] == BLUE ||
		res[0] == BLUE && res[4] == BLUE && res[8] == BLUE ||
		res[2] == BLUE && res[4] == BLUE && res[6] == BLUE)
		return BLUE_WIN;
	for (int i = 0; i < 9; ++i)
		if (res[i] == NULL)
			return NULL;
	return TIE;
}

bitset<81> reds, blues;
int dfs(int pos, int color) {
	cout << reds << endl << blues << endl << endl;
	int res = big_check(reds, blues);
	if (res != NULL)
		return res;
	vector<int> results;
	if (pos == -1) {
		while (++pos < 9) {
			if (get_status(Board(reds, blues, pos)) == NULL) {
				for (int i = 0; i < 9; ++i) {
					if (reds[pos * 9 + i] || blues[pos * 9 + i])
						continue;
					if (color % 2 == 1)
						reds.set(pos * 9 + i);
					else
						blues.set(pos * 9 + i);
					if(get_status(Board(reds, blues, i)) != NULL)
						results.push_back(dfs(-1, color + 1));
					else
						results.push_back(dfs(i, color + 1));
					if (color % 2 == 1)
						reds.reset(pos * 9 + i);
					else
						blues.reset(pos * 9 + i);
				}
			}
		}
	}
	else {
		for (int i = 0; i < 9; ++i) {
			if (reds[pos * 9 + i] || blues[pos * 9 + i])
				continue;
			if (color % 2 == 1)
				reds.set(pos * 9 + i);
			else
				blues.set(pos * 9 + i);
			if (get_status(Board(reds, blues, i)) != NULL)
				results.push_back(dfs(-1, color + 1));
			else
				results.push_back(dfs(i, color + 1));
			if (color % 2 == 1)
				reds.reset(pos * 9 + i);
			else
				blues.reset(pos * 9 + i);
		}
	}
	bool flag = true;
	int sign = TIE;
	for (auto i : results)
		if (i != sign)
			flag = false;
	if (flag)
		return TIE; // 怎么走都会陷入重复
	sign = RED_WIN;
	for (auto i : results)
		if (i == sign)
			flag = true;
	if (color % 2 == 1 && flag)
		return RED_WIN; //先手有必胜策略
	flag = true;
	for (auto i : results)
		if (i != sign)
			flag = false;
	if (color % 2 == 0 && flag)
		return RED_WIN; //后手怎么走都会让先手赢
	//以下同理，是后手必胜情况
	sign = BLUE_WIN;
	for (auto i : results)
		if (i == sign)
			flag = true;
	if (color % 2 == 0 && flag)
		return BLUE_WIN;
	flag = true;
	for (auto i : results)
		if (i != sign)
			flag = false;
	if (color % 2 == 1 && flag)
		return BLUE_WIN;
	return TIE;
}

int main() {
	cout << dfs(-1, 1);
	return 0;
}