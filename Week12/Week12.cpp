// 기초컴퓨터프로그래밍 12주차, 하노이의 탑
// 20200690 박진철

// =============================================================================================

// 전처리기
#include <bangtal.h>
#include <stdio.h>
#include <Windows.h>
#include <vector>
using namespace std;

// 애니메이션
TimerID timer;
const Second animationTime = 0.25f;
int animation = 0;

// =============================================================================================

// 하노이의 탑 장면
SceneID hanoiTower;

// 시작 버튼
ObjectID startButton;

// 원판 모음
ObjectID disk[7];

// X좌표 (왼쪽 -> 오른쪽)
int diskX[3] = { 143, 508, 873 };

// Y좌표 (아래쪽 -> 위쪽)
int diskY[7] = { 198, 248, 298, 348, 398, 448, 498 };

// 원판 수
int diskN[3] = { 7, 0, 0 };

// 원판 이동 저장
class DISK_MOVE {
public:
	int n, x, y;
public:
	DISK_MOVE(int n, int x, int y) : n(n), x(x), y(y) {};
};

vector<DISK_MOVE> v;
vector<DISK_MOVE>::iterator it;

// =============================================================================================

// 하노이의 탑 함수 (재귀함수)
void towerOfHanoi(int n, int from, int to, int aux) {
	animation += 1;

	if (n == 1) {
		diskN[to] += 1;
		diskN[from] -= 1;
		v.push_back(DISK_MOVE(n - 1, diskX[to], diskY[diskN[to] - 1]));
		return;
	}
	
	towerOfHanoi(n - 1, from, aux, to);

	diskN[to] += 1;
	diskN[from] -= 1;
	v.push_back(DISK_MOVE(n - 1, diskX[to], diskY[diskN[to] - 1]));
	towerOfHanoi(n - 1, aux, to, from);
}

// 원판 이동 함수
void moveDisk() {
	locateObject(disk[(*it).n], hanoiTower, (*it).x, (*it).y);
	it++;
}

// 마우스 입력 처리 함수
void mouseCallback(ObjectID obj, int x, int y, MouseAction action) {
	if (obj == startButton) {
		hideObject(startButton);
		towerOfHanoi(7, 0, 1, 2);

		it = v.begin();
		setTimer(timer, animationTime);
		startTimer(timer);
	}
}

// 애니메이션
void timerCallback(TimerID timer) {
	moveDisk();

	if (animation > 1) {
		animation -= 1;

		setTimer(timer, animationTime);
		startTimer(timer);
	}
}

int main(void) {
	// 마우스 입력 및 애니메이션 처리
	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	// 타이머
	timer = createTimer();

	// 하노이의 탑 배경
	hanoiTower = createScene("Tower of Hanoi", "./Images/Background.png");

	// 원판
	for (int i = 0; i < 7; i++) {
		char fileName[20];
		sprintf_s(fileName, sizeof(fileName), "./Images/%d.png", i + 1);
		
		disk[i] = createObject(fileName);
		locateObject(disk[i], hanoiTower, diskX[0], diskY[6 - i]);
		showObject(disk[i]);
	}

	// 시작 버튼
	startButton = createObject("./Images/startButton.png");
	locateObject(startButton, hanoiTower, diskX[1], 70);
	showObject(startButton);

	// 시작
	startGame(hanoiTower);
}