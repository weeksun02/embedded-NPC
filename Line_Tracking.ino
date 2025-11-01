/*
 * 3센서 + 서보 라인 트레이서
 *
 * --- 하드웨어 연결 기준 ---
 * [모터 제어]
 * - ENA (모터A 속도) -> D5
 * - ENB (모터B 속도) -> D6
 * - MOTOR_L_1 (모터L) -> D8
 * - MOTOR_L_2 (모터L) -> D9
 * - MOTOR_R_1 (모터R) -> D10
 * - MOTOR_R_2 (모터R) -> D11
 *
 * [라인 트레이서 센서 (사용자 기준)]
 * - LIGHT_LEFT_PIN  -> A0 (왼쪽)
 * - LIGHT_CENTER_PIN -> A5 (중앙)  <-- 수정됨
 * - LIGHT_RIGHT_PIN  -> A3 (오른쪽)
 *
 * [앞바퀴 서보 모터]
 * - 서보 신호선 -> D2
 */

#include <Servo.h>

// --- 핀 정의 ---
Servo myservo; 

#define ENA_PIN 5 // ENA of DC motor driver module attach to pin5
#define ENB_PIN 6 // ENB of DC motor driver module attach to pin6
#define MOTOR_L_1 8 // left MOTOR_L_1 attach to pin8
#define MOTOR_L_2 9 // left MOTOR_L_2 attach to pin9
#define MOTOR_R_1 10 // right MOTOR_R_1 attach to pin10
#define MOTOR_R_2 11 // right MOTOR_R_2 attach to pin11

// --- 방향 정의 ---
#define FORWARD 0 // define forward=0, car move forward
#define BACK 1    // define back=1, car move back

// --- 서보 각도 ---
#define ANGLE_RIGHT_MAX 70    // (40 + 30)
#define ANGLE_RIGHT_HALF 60   // (40 + 20)
#define ANGLE_LEFT_MAX 10     // (40 - 30)
#define ANGLE_LEFT_HALF 20    // (40 - 20)
#define ANGLE_MIDDLE 40       // 기준 정면 각도

// --- 센서 핀 ---
#define LIGHT_LEFT_PIN A0   // 왼쪽 센서
#define LIGHT_CENTER_PIN A3 // 중앙 센서
#define LIGHT_RIGHT_PIN A5  // 오른쪽 센서

// --- 핀 정의 ---
#define SERVO_PIN 2     // 서보 핀 (D2)
#define THRESHOLD 400   // 센서 임계값 (조정 필요)

// --- 전역 변수 (속도) ---
#define SPEED 80       // 이미지 기준 속도 (너무 빠르면 80으로 줄이세요)

// --- 모터 함수 선언 ---
void stopMotors();
void moveMotors(int direction, int speed);
void setSteering(int angle);

void setup() {
  // 모터 핀 모드 설정
  pinMode(ENA_PIN, OUTPUT);
  pinMode(ENB_PIN, OUTPUT);
  pinMode(MOTOR_L_1, OUTPUT);
  pinMode(MOTOR_L_2, OUTPUT);
  pinMode(MOTOR_R_1, OUTPUT);
  pinMode(MOTOR_R_2, OUTPUT);
  
  // 서보 모터 초기화
  myservo.attach(SERVO_PIN);
  myservo.write(ANGLE_MIDDLE); // 시작 시 앞바퀴 정렬
  delay(500); // 서보 정렬 시간
  
  stopMotors(); 
}

void loop() {
  // 3개의 센서 값을 읽음 (값이 크면 검은색이라고 가정)
  bool sLeft = (analogRead(LIGHT_LEFT_PIN) > THRESHOLD);   // A0
  bool sCenter = (analogRead(LIGHT_CENTER_PIN) > THRESHOLD); // A5
  bool sRight = (analogRead(LIGHT_RIGHT_PIN) > THRESHOLD);  // A3

  /*
   * 3센서 라인 트레이싱 로직
   * [ A0(L) | A5(C) | A3(R) ]
   */

  // [ 0 | 1 | 0 ] : (흰|검|흰) -> 직진
  if (!sLeft && sCenter && !sRight) {
    setSteering(ANGLE_MIDDLE);
    moveMotors(FORWARD, SPEED);
  }
  // [ 0 | 1 | 1 ] : (흰|검|검) -> 약간 우회전
  else if (!sLeft && sCenter && sRight) {
    setSteering(ANGLE_RIGHT_HALF);
    moveMotors(FORWARD, SPEED);
  }
  // [ 1 | 1 | 0 ] : (검|검|흰) -> 약간 좌회전
  else if (sLeft && sCenter && !sRight) {
    setSteering(ANGLE_LEFT_HALF);
    moveMotors(FORWARD, SPEED);
  }
  // [ 0 | 0 | 1 ] : (흰|흰|검) -> 최대 우회전
  else if (!sLeft && !sCenter && sRight) {
    setSteering(ANGLE_RIGHT_MAX);
    moveMotors(FORWARD, SPEED);
  }
  // [ 1 | 0 | 0 ] : (검|흰|흰) -> 최대 좌회전
  else if (sLeft && !sCenter && !sRight) {
    setSteering(ANGLE_LEFT_MAX);
    moveMotors(FORWARD, SPEED);
  }
  // [ 0 | 0 | 0 ] : (흰|흰|흰) -> 라인 이탈 -> 정지
  else if (!sLeft && !sCenter && !sRight) {
    stopMotors();
  }
  // [ 1 | 1 | 1 ] : (검|검|검) -> 교차로 -> 일단 직진 (또는 정지)
  else if (sLeft && sCenter && sRight) {
    setSteering(ANGLE_MIDDLE);
    moveMotors(FORWARD, SPEED);
  }
}

// --- 모터 제어 함수 (ENA=5, ENB=6) ---

/**
 * 정지 (뒷바퀴 정지, 앞바퀴 정렬)
 */
void stopMotors() {
  digitalWrite(MOTOR_L_1, LOW);
  digitalWrite(MOTOR_L_2, LOW);
  digitalWrite(MOTOR_R_1, LOW);
  digitalWrite(MOTOR_R_2, LOW);
  analogWrite(ENA_PIN, 0); // D5
  analogWrite(ENB_PIN, 0); // D6
  myservo.write(ANGLE_MIDDLE);
}

/**
 * 전진 또는 후진 (두 뒷바퀴)
 */
void moveMotors(int direction, int speed) {
  if (direction == FORWARD) {
    digitalWrite(MOTOR_L_1, HIGH);
    digitalWrite(MOTOR_L_2, LOW);
    digitalWrite(MOTOR_R_1, HIGH);
    digitalWrite(MOTOR_R_2, LOW);
  } else if (direction == BACK) {
    digitalWrite(MOTOR_L_1, LOW);
    digitalWrite(MOTOR_L_2, HIGH);
    digitalWrite(MOTOR_R_1, LOW);
    digitalWrite(MOTOR_R_2, HIGH);
  }
  analogWrite(ENA_PIN, speed); // D5
  analogWrite(ENB_PIN, speed); // D6
}

/**
 * 조향 (앞바퀴 각도 조절)
 */
void setSteering(int angle) {
  myservo.write(angle);
}