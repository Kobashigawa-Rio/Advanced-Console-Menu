#pragma once

using namespace std;

template <typename T1, typename T2>
class Player {
	private:
		T1 name;
		T2 score;
	public:
		Player(T1 name, T2 score) : name(name), score(score) {};
		T1 getName() { return name; };
		T2 getScore() { return score; };
};
