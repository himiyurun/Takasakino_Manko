#include <Keyboard.h>
#include <time.h>

const int kahen = A0;       // 可変抵抗器が接続されているアナログピン
const int DIN_PIN = 13;     // スイッチが接続されているデジタルピン

int Nr = 0;  // 前回の値を保存する変数
bool Lock = false; // Lockの初期値をfalseに設定
unsigned long switchReleaseTime = 0; // スイッチが離された時間を保存

// 各地点の値を配列に格納
static const int points[] = {211, 286, 362, 442, 522, 601, 675, 743, 807, 870, 933, 994};
static const int numPoints = 12;

const int points1[] = {994};
const int numPoints1 = sizeof(points1) / sizeof(points1[0]);

//    EBを0としてマスコンの位置を保存する
int nowpoints = 0;

void setup() {
  pinMode(DIN_PIN, INPUT_PULLUP);
  
  Serial.begin(9600);
  Keyboard.begin();
}

void loop() {
  int N = analogRead(kahen);
  int switchState = digitalRead(DIN_PIN);  // スイッチの状態を読み取る

  Serial.println(N);

  // スイッチが押されたときにLockを有効にする
  if (switchState == LOW) {
    Lock = true;
    switchReleaseTime = 0;  // スイッチが押されたらリセット
  } else {
    if (switchReleaseTime == 0) {
      switchReleaseTime = millis();  // 離された時間を記録
     // N = 1023;  // スイッチが離された時にNを1023にする
    } else if (millis() - switchReleaseTime >= 5000) {
      Lock = false;  // 5秒後にLockを解除
    }
  }

  if (Lock) { 
    // pointsの処理
    for (int i = 0; i < numPoints; i++) {
      if (N >= points[i] && Nr < points[i]) {
        Keyboard.write('Q');  // A地点からB地点に通過した場合
	nowpoints--;
      } else if (N < points[i] && Nr >= points[i]) {
        Keyboard.write('Z');  // B地点からA地点に通過した場合
	nowpoints++;
      }
    }
    else {
        keyboard.write('1');
    }

    // points1の処理
    for (int i = 0; i < numPoints1; i++) {
      if (N >= points1[i] && Nr < points1[i]) {
        Keyboard.write('1');  // A地点からB地点に通過した場合
      }
    }

    Nr = N;   // 現在の値を保存
  }

  delay(25);  // デバウンスのための遅延
}
