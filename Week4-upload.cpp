#include <stdlib.h>
#include <bangtal.h>
#include <stdio.h>
#include <time.h>

#define _CRT_SECURE_NO_WARNINGS

SceneID scene1;
ObjectID piece1[9];  //3X3 크기의 조각
ObjectID original_piece1[9];
ObjectID startButton, endButton, restartButton;
int blank = 8;  //비어있는 조각
bool gameon = false;    //사용자가 조각을 건드릴 수 있는가

time_t start, end;

/*TimerID timer1;
float timerValue = 0.1f;*/


//조각의 x좌표 함수
int pieceX(int i) {
    return 278 + 240 * (i % 3);
}

//조각의 y좌표 함수
int pieceY(int i) {
    return 480 - 240 * (i / 3);
}

ObjectID DIY(const char* name, const char* image, ObjectID object, SceneID scene, int x, int y, bool shown = true) {
    object = createObject(image);
    locateObject(object, scene, x, y);
    if (shown == true) {
        showObject(object);
    }
    return object;
}

// 조각 알아내기
int getpiece_Index(ObjectID object) {

    for (int i = 0; i < 9; i++) {
        if (piece1[i] == object) return i;
    }

    return -1;
}

// 조각 움직이기
void move_piece(int i) {

    ObjectID object = piece1[i];
    piece1[i] = piece1[blank];
    locateObject(piece1[i], scene1, pieceX(i), pieceY(i));

    piece1[blank] = object;
    locateObject(piece1[blank], scene1, pieceX(blank), pieceY(blank));

    blank = i;
}

// 조각의 이동가능성 체크
bool possible_move(int i) {
    if (i % 3 > 0 && blank == i - 1) return true;   //blank가 오른쪽으로 갈 수 있는 조건
    else if (i % 3 < 2 && blank == i + 1) return true;   //blank가 왼쪽으로 갈 수 있는 조건
    else if (i / 3 > 0 && blank == i - 3) return true;   //blank가 아래쪽으로 갈 수 있는 조건
    else if (i / 3 < 2 && blank == i + 3) return true;   //blank가 위쪽으로 갈 수 있는 조건

    return false;
}

// 조각 섞어주는 함수 정의
int random_move() {
    int i = -1;


    while (i == -1) {
        switch (rand() % 4) {
        case 0: //blank가 맨 왼쪽에 있지 않은 경우
            if (blank % 3 > 0) {
                i = blank - 1;
            }
            break;
        case 1: //blank가 맨 오른쪽에 있지 않은 경우
            if (blank % 3 < 2) {
                i = blank + 1;
            }
            break;
        case 2: //blank가 맨 위쪽에 있지 않은 경우
            if (blank / 3 > 0) {
                i = blank - 3;
            }
            break;
        case 3: //blank가 맨 아래쪽에 있지 않은 경우
            if (blank / 3 < 2) {
                i = blank + 3;
            }
            break;
        }
    }
    return i;
}

//스타트 버튼을 누르면 조각섞어주는 함수
void game_start() {
    int mixCount = 120;

    for (int i = 0; i < mixCount; i++) {
        move_piece(random_move());
    }
}

//퍼즐 맞추기 성공 여부
bool game_completed() {
    for (int i = 0; i < 9; i++) {
        if (piece1[i] != original_piece1[i]) return false;
    }
    return true;
}

//콘솔에 조각 배열번호 표시
void game_print() {
    for (int i = 0; i < 9; i++) {
        printf("%d", piece1[i]);
    }
    printf("\n");
}

void mousecallback(ObjectID object, int x, int y, MouseAction action) {

    if (gameon == false) {
        if (object == startButton) {
            game_start();
            hideObject(startButton);
            hideObject(piece1[blank]);
            locateObject(endButton, scene1, 85, 320);
            showObject(restartButton);

            gameon = true;

            start = time(NULL); //시간측정 시작
        }
        else if (object == endButton) {
            endGame();
        }
    }
    else if (gameon == true) {
        int i = getpiece_Index(object);
        if (possible_move(i) == true) {
            move_piece(i);

            if (game_completed()) {
                gameon = false;
                showObject(startButton);
                showObject(piece1[blank]);
                locateObject(endButton, scene1, 600, 70);

                showMessage("성공!\n");
                hideObject(restartButton);

                end = time(NULL);   //시간측정 종료
                double time_taken = difftime(end, start);  //걸린시간 계산
                int min = time_taken / 60;
                int temp = time_taken;
                int sec = temp % 60;

                printf("time taken: %d minute(s) %d second(s)\n", min, sec);  //콘솔창에 걸린 시간 표시

                char time[50];
                sprintf_s(time, "%d분 %d초 걸렸습니다.", min, sec);
                showMessage(time);  //게임 내에 걸린 시간 표시
            }
        }

        else if (object == endButton) {
            endGame();
        }

        else if (object == restartButton) {
            gameon = false;
            showObject(startButton);
            showObject(piece1[blank]);
            locateObject(endButton, scene1, 600, 70);
            hideObject(restartButton);
            for (int i = 0; i < 9; i++) {
                locateObject(piece1[i], scene1, pieceX(i), pieceY(i));
            }
        }

        game_print();
    }
}

//디버깅하기 전에는 오류가 발생하지않으나 디버깅 이후에 타이머 함수관련해서 계속 '예외발생' 오류가 발생하여
//타이머는 모두 제외시켰습니다.

/*void timercallback(TimerID timer) {

    move_piece(random_move());

    mixCount--;

    if (mixCount > 0) {
        setTimer(timer1, timerValue);
        startTimer(timer1);
    }
    else {
        gameon = true;
    }
}*/

int main() {

    srand(time(NULL));

    setMouseCallback(mousecallback);

    /*setTimerCallback(timercallback);
    timer1 = createTimer(timerValue);
    showTimer(timer1);*/

    setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, true);

    scene1 = createScene("puzzle", "Images/Wallpaper.png");


    char image[50];
    for (int i = 0; i < 9; i++) {
        sprintf_s(image, "Images/%d.png", i + 1);
        piece1[i] = DIY("퍼즐조각", image, piece1[i], scene1, pieceX(i), pieceY(i));
        original_piece1[i] = piece1[i];
    }


    startButton = DIY("시작버튼", "Images/start.png", startButton, scene1, 600, 130);
    endButton = DIY("종료버튼", "Images/end.png", endButton, scene1, 600, 70);
    restartButton = DIY("재시작버튼", "Images/restart.png", restartButton, scene1, 85, 380, false);

    scaleObject(startButton, 1.15f);
    scaleObject(endButton, 1.15f);
    scaleObject(restartButton, 1.15f);

    startGame(scene1);
}