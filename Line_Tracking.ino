#define EA 3 // 모터드라이버 EA 핀, 아두이노 디지털 3번 핀에 연결
#define EB 11 // 모터드라이버 EB 핀, 아두이노 디지털 11번 핀에 연결
#define M_IN1 4 // 모터드라이버 IN1 핀, 아두이노 디지털 4번 핀에 연결
#define M_IN2 5 // 모터드라이버 IN2 핀, 아두이노 디지털 5번 핀에 연결
#define M_IN3 13 // 모터드라이버 IN3 핀, 아두이노 디지털 13번 핀에 연결
#define M_IN4 12 // 모터드라이버 IN4 핀, 아두이노 디지털 12번 핀에 연결
#define R_Sensor 10 // 오른쪽 트래킹(추적)센서 모듈 DO 핀, 아두이노 우노 보드의 10번 핀에 연결, //차대 프레임 아랫면에서 바라봤을때의 오른쪽
#define C_Sensor 9 // 가운데 트래킹(추적)센서 모듈 DO 핀, 아두이노 우노 보드의 9번 핀에 연결, //차대 프레임 아랫면에서 바라봤을때의 중앙
#define L_Sensor 8 // 왼쪽 트래킹(추적)센서 모듈 DO 핀, 아두이노 우노 보드의 8번 핀에 연결, 차대 //프레임 아랫면에서 바라봤을때의 왼쪽
int motorA_vector = 1; // 모터의 회전방향이 반대일 시 0을 1로, 1을 0으로 바꿔주면 모터의 회전방향이 바뀜.
int motorB_vector = 1; // 모터의 회전방향이 반대일 시 0을 1로, 1을 0으로 바꿔주면 모터의 회전방향이 바뀜.
int motor_speed = 1023; // 모터 스피드, 2WD 자동차 경우                      
void setup() //초기화
{
  pinMode(EA, OUTPUT); // EA 핀 출력 설정
  pinMode(EB, OUTPUT); // EB 핀 출력 설정
  pinMode(M_IN1, OUTPUT); // IN1 핀 출력 설정
  pinMode(M_IN2, OUTPUT); // IN2 핀 출력 설정
  pinMode(M_IN3, OUTPUT); // IN3 핀 출력 설정
  pinMode(M_IN4, OUTPUT); // IN4 핀 출력 설정
  pinMode(R_Sensor, INPUT); // 오른쪽 센서 D0 핀 입력 설정
  pinMode(C_Sensor, INPUT); // 가운데 센서 D0 핀 입력 설정
  pinMode(L_Sensor, INPUT); // 왼쪽 센서 D0 핀 입력 설정
  delay(3000); // 갑작스러운 움직임을 막기위한 3초간 지연
}

void loop() // 무한루프
{
  if (digitalRead(C_Sensor)) // 만약 가운데 센서가 감지되면
  {
    motor_con(motor_speed, motor_speed); // 직진하라
  }
  else if (digitalRead(L_Sensor)) // 만약 왼쪽 센서가 감지되면
  {
    motor_con(-(motor_speed), motor_speed); // 모터 B를 반대로 움직여라
  }
  else if (digitalRead(R_Sensor)) //만약 오른쪽 센서가 감지되면
  {
    motor_con(motor_speed, -(motor_speed)); // 모터 A를 반대로 움직여라
  }
}

void motor_con(int M1,
  int M2) // 모터 컨트롤 함수,  M1 차대 후면에서 바라봤을대 왼쪽 모터, M2 오른쪽 모터
{
  if (M1 > 0) // 모터A 정회전
  {
    digitalWrite(M_IN1, motorA_vector); // IN1번에 HIGH(motorA_vector가 0이면 LOW)
    digitalWrite(M_IN2, !motorA_vector); // IN2번에 LOW(motorA_vector가 0이면 HIGH)
  }
  else if (M1 < 0) // 모터A 역회전
  {
    digitalWrite(M_IN1, !motorA_vector); // IN1번에 LOW(motorA_vector가 0이면 HIGH) 
    digitalWrite(M_IN2, motorA_vector); // IN2번에 HIGH(motorA_vector가 0이면 LOW)
  }
  else // 모터 A 정지
  {
    digitalWrite(M_IN1, LOW); // IN1번에 LOW
    digitalWrite(M_IN2, LOW); // IN2번에 LOW
  }

  if (M2 > 0) // 모터B 정회전
  {
    digitalWrite(M_IN3, motorB_vector); // IN3번에 HIGH(motorB_vector가 0이면 LOW)
    digitalWrite(M_IN4, !motorB_vector); // IN4번에 LOW(motorB_vector가 0이면 HIGH) 
  }
  else if (M2 < 0) // 모터B 역회전
  {
    digitalWrite(M_IN3, !motorB_vector); // IN3번에 LOW(motorB_vector가 0이면 HIGH)
    digitalWrite(M_IN4, motorB_vector); // IN4번에 HIGH(motorB_vector가 0이면 LOW)
  }
  else //모터 B 정지
  {
    digitalWrite(M_IN3, LOW); // IN3번에 LOW
    digitalWrite(M_IN4, LOW); // IN4번에 LOW
  }

  analogWrite(EA,
    abs(M1)); // M1의 절대값으로 A모터 속도 제어
  analogWrite(EB,
    abs(M2)); // M2의 절대값으로 B모터 속도 제어
}
