#define EA 3 // 모터드라이버 EA 핀, 아두이노 디지털 3번 핀에 연결
#define EB 11 // 모터드라이버 EB 핀, 아두이노 디지털 11번 핀에 연결
#define M_IN1 4 // 모터드라이버 IN1 핀, 아두이노 디지털 4번 핀에 연결
#define M_IN2 5 // 모터드라이버 IN2 핀, 아두이노 디지털 5번 핀에 연결
#define M_IN3 13 // 모터드라이버 IN3 핀, 아두이노 디지털 13번 핀에 연결
#define M_IN4 12 // 모터드라이버 IN4 핀, 아두이노 디지털 12번 핀에 연결
#define echo 6 // 초음파센서 에코(Echo) 핀, 아두이노 우노 보드의 디지털 6번 핀 연결
#define trig 7 // 초음파센서 트리거(Trigger) 핀, 아두이노 우노 보드의 디지털 7번 핀 연결
int motorA_vector = 0; 
// 모터의 회전방향이 반대일 시 0을 1로, 1을 0으로 바꿔주면 모터의 회전방향이 바뀜.
int motorB_vector = 1; 
// 모터의 회전방향이 반대일 시 0을 1로, 1을 0으로 바꿔주면 모터의 회전방향이 바뀜.
int motor_speed = 1023; // 모터 스피드
void setup() // 초기화
{
  pinMode(EA, OUTPUT); // EA 핀 출력 설정
  pinMode(EB, OUTPUT); // EB 핀 출력 설정
  pinMode(M_IN1, OUTPUT); // IN1 핀 출력 설정
  pinMode(M_IN2, OUTPUT); // IN2 핀 출력 설정
  pinMode(M_IN3, OUTPUT); // IN3 핀 출력 설정
  pinMode(M_IN4, OUTPUT); // IN4 핀 출력 설정
  pinMode(trig, OUTPUT); // trig 출력 설정
  pinMode(echo, INPUT); // echo 입력설정
  delay(3000); // 갑작스러운 움직임을 막기위한 3초간 지연
}

void loop() // 무한루프
{
  if (read_ultrasonic() > 16) // 만약 측정한 거리 값이 16보다 크면
  {
    motor_con(motor_speed, motor_speed); // 모터 A, 모터 B 모두 정회전 하여, 직진한다.
  }
  else if (read_ultrasonic() < 14) //만약 측정한 거리 값이 14보다 작으면
  {
    motor_con(-(motor_speed), -(motor_speed)); // 모터 A, 모터 B 모두 역회전하여, 후진한다.
  }
  else // 나머지 거리 값에서는
  {
    motor_con(0, 0);
  }
}

float read_ultrasonic(void) // 초음파센서 값 읽어오는 함수
{
  float return_time, howlong; // 시간 값과 거리 값을 저장하는 변수를 만든다.
  //초음파센서는 초음파를 발사 후 돌아오는 시간을 역산하여 거리 값으로 계산한다.
  digitalWrite(trig, HIGH); // 초음파 발사
  delay(5); // 0.05초간 지연
  digitalWrite(trig, LOW); // 초음파 발사 정지
  return_time = pulseIn(echo, HIGH); // 돌아오는 시간
  howlong = ((float)(340 *return_time) / 10000) / 2; // 시간을 거리로 계산
  return howlong; // 거리 값 리턴
}

void motor_con(int M1, int M2) // 모터 컨트롤 함수
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

  analogWrite(EA, abs(M1)); // M1의 절대값으로 A모터 속도 제어
  analogWrite(EB, abs(M2)); // M2의 절대값으로 B모터 속도 제어
}
