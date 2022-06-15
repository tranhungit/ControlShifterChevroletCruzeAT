const int controlPin = 8; // Digital pin 8 will be called 'pin8'
const int InputPin = A2;  // Analog input pin that senses Vout
int Value = 0;            // InputPin default value
const double Vin = 5.0;   // Input voltage
double Vout = 0.0;        // Vout default value
double R = 0.0;
bool activeManual = false; // Đoạn này để kích hoạt sang chế độ manual shift luôn, nhưng chưa nghĩ ra
int nCount = 0;
/*
 Lưu ý đoạn GET const này: Nạp lần 1 Monitor để lấy 3 giá trị gán vào VPlus, VMinus, sau khi gán xong thì nạp lại lần 2
 Sau khi test ổn thì comment mấy cái Serial.print để cho nhạy hơn khi bấm
*/
const double resistorPlus = 654227.37;       // Value of resistor when press shifters +
const double resistorMinus = 2170666.75;      // Value of resistor when press shifters -
//
const double errRate = 100.02;         // Sai số do nhiễu điện
unsigned long started = 0;
unsigned long timeout = 3000;         // Manual shifters will be off after 3 seconds

void setup ()
{
  Serial.begin(9600);      // Initialize serial communications at 9600 bps
  pinMode(InputPin, INPUT);
  pinMode(controlPin, OUTPUT);
  digitalWrite(controlPin, HIGH);
}
void loop ()
{
  R = getresistorInput();
  //Serial.print("R in loop>> ");
  //Serial.println(R);
  //Serial.print("Control pin state is: ");
  //Serial.println(digitalRead(controlPin));
  //check if pess shifters +
  if (R!=0 && R >= (resistorPlus - errRate) && R <= (resistorPlus + errRate)) {
    //Serial.println("Start control shifters +");
    started = millis();
    digitalWrite(controlPin, LOW);
  }
  //check if pess shifters -
  if (R != 0 && R >= (resistorMinus - errRate) && R <= (resistorMinus + errRate)) {
    //Serial.println("Start control shifters -");
    started = millis();
    digitalWrite(controlPin, LOW);
  }
  //check timeout
  if (started > 0) {
    long now = millis();
    long elapsed = now - started;
    if (elapsed > timeout) {
      //Serial.println("Stop control shifters + -");
      digitalWrite(controlPin, HIGH);
      started = 0;
    }
  }
  delay(100); // Delay in milliseconds between reeds
}
double getresistorInput() {
  Value = analogRead(InputPin);  // Read Vout on analog input pin A0 (Arduino can sense from 0-1023, 1023 is 5V)
  Vout = (Vin * Value) / 1023;    // Convert Vout to Voltage
  //Serial.println("analogRead>> ");
  //Serial.println(Value);
  if (Vout != Vin) {
    float r = 330000 * (1 / ((Vin / Vout) - 1)); //330000 điện trỡ 330k Ohm
    return r;
  }
  return 0.0;
}
