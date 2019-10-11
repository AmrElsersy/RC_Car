#include <SoftwareSerial.h>
#include <NewPing.h>

#define n1 7
#define n2 8 
#define n3 5
#define n4 4
#define en1 6
#define en2 3
#define echo 13
#define trig 12
#define IR_Left 11
#define IR_Middle 9
#define IR_Right 10
#define Delay 1

NewPing sonar(trig,echo,200);
int d, v, x , l1,l2,l3;
int f1,f2,f3 ;
char i;
int linefollower;
int rlinefollower;

int duration, distance;

void setup()
{  
  Serial.begin(9600);

  pinMode(IR_Left,INPUT);
  pinMode(IR_Right,INPUT);
  pinMode(IR_Middle,INPUT);
  pinMode(n1,OUTPUT);
  pinMode(n2,OUTPUT);
  pinMode(n3,OUTPUT);
  pinMode(n4,OUTPUT);
  pinMode(en1,OUTPUT);
  pinMode(en1,OUTPUT);
  
  pinMode(trig,OUTPUT);
  pinMode(echo, INPUT);
}

long UltraSonicRead()
{
  digitalWrite(trig, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trig, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration/58.2;

  return distance;
  
}

void SharpRight()
{
  digitalWrite(n1,1);
  digitalWrite(n2,0);
  digitalWrite(n3,1);
  digitalWrite(n4,0);
  analogWrite(en1,v);
  analogWrite(en2,v);
}

void SharpLeft()
{
  digitalWrite(n1,0);
  digitalWrite(n2,1);
  digitalWrite(n3,0);
  digitalWrite(n4,1);
  analogWrite(en1,v);
  analogWrite(en2,v);
}

void Forward()
{
  digitalWrite(n1,1);
  digitalWrite(n2,0);
  digitalWrite(n3,0);
  digitalWrite(n4,1);
}

void Back()
{
  digitalWrite(n1,0);
  digitalWrite(n2,1);
  digitalWrite(n3,1);
  digitalWrite(n4,0);
}

void ForwardLeft()
{
  Forward();
  analogWrite(en1,0);
  analogWrite(en2,v);
}

void ForwardRight()
{
  Forward();
  analogWrite(en1,v);
  analogWrite(en2,0);
}

void BackLeft()
{
  Back();
  analogWrite(en1,0);
  analogWrite(en2,v);
}

void BackRight()
{
  Back();
  analogWrite(en1,v);
  analogWrite(en2,0);
}
void stop()
{
        digitalWrite(n1,0);
        digitalWrite(n2,0);
        digitalWrite(n3,0);
        digitalWrite(n4,0); 
}

void loop()
{
  if (Serial.available())
  {
      i = Serial.read();
      if ( i != 'S')
           linefollower = 0;
      if (i=='V')
           linefollower = 1;
      if (i =='W')             
           linefollower = 2;
            
    Serial.print(i);
if (! linefollower)
{      


      if(i == '0')
        x = 0;
      else if(i == '1')
        x = 10;
      else if(i == '2')
        x = 20;
      else if(i == '3')
        x = 30;
      else if(i == '4')
        x = 40;
      else if(i == '5')
        x = 50;
      else if(i == '6')
        x = 60;
      else if(i == '7')
        x = 70;
      else if(i == '8')
        x = 80;
      else if(i == '9')
        x = 90;
      else if(i == 'q')
        x = 100;
    
      v = map(x,0,100,0,255);

    
//  digitalWrite(trig, LOW); 
//  delayMicroseconds(2);
//  digitalWrite(trig, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trig, LOW);
//  duration = pulseIn(echo, HIGH);
//  distance = duration/58.2;

    distance = sonar.ping_cm();
    Serial.print(distance);
    Serial.print("\n");
     if(distance > 15 || distance == 0)
     {
      if (i == 'S')
      {
        digitalWrite(n1,0);
        digitalWrite(n2,0);
        digitalWrite(n3,0);
        digitalWrite(n4,0); 
      }
      else if (i=='F')
      {
        Forward(); 
        analogWrite(en1,v);
        analogWrite(en2,v);
      }  
      else if (i=='B')
      {
        Back();
        analogWrite(en1,v);
        analogWrite(en2,v);
      }  
      else if (i=='R')
      {
        SharpRight();
      }  
      else if (i=='L')
      {
        SharpLeft();
      }  
      else if (i=='I')
      {
        ForwardRight();
      }  
      else if (i=='G')
      {
        ForwardLeft();
      }  
      else if (i=='H')
      {
        BackLeft();
      }  
      else if (i=='J')
      {
        BackRight();
      }   
    }

    else 
    {
      Back();
    }
}

// ============================ Line Follwer ================================ 
else if (linefollower==1)
{
bool flag = true;
Serial.print("LineFollower");

while (flag)
{
   LineFollower2();
   delay(5);

// ============== Get Out Of Line Follower ===============
if (Serial.available())
{  
   char r = Serial.read();

   if ( r == 'v')
   {
       flag = false;
       linefollower = false;         
   }
}
// =======================================================

} // while 

} // else

// =============================================================================

// ============================ Line Follwer ================================ 
else if (linefollower==2)
{
bool flag = true;
Serial.print("rpj");

while (flag)
{
   LineFollower3();
   delay(5);

// ============== Get Out Of Line Follower ===============
if (Serial.available())
{  
   char r = Serial.read();

   if ( r == 'w')
   {
       flag = false;
       linefollower = false;         
   }
}
// =======================================================

} // while 

} // else

// =============================================================================

  }
}


void LineFollower2()
{
l1=digitalRead(IR_Left);
l2=digitalRead(IR_Middle);
l3=digitalRead(IR_Right);

l2 = !l2 ;

if(l1==1&&l2==1&&l3==1)   //feedback when comes all sensor on white
{
l1=f1;
l2=f2;
l3=f3;
}

if( ( l1==0&&l2==1&&l3==1) || ( l1==0&&l2==0&&l3==1 ) )   //left turn
  SharpLeft();
else if( ( l1==1&&l2==1&&l3==0 ) || ( l1==1&&l2==0&&l3==0 ) )  //right turn
  SharpRight();
else if ( (l1==1&&l2==0&&l3==1) || (l1 ==0 && l2==0 &&l3==0) )  //go forward straight
{
  Forward();
  analogWrite(en1,v);
  analogWrite(en2,v);
}

f1=l1;
f2=l2;
f3=l3;        //memory variables


d = sonar.ping_cm();
if ( d > 0 && d <= 15) 
{
     SharpRight();
     delay(400);
     f1 =1 ;
     f2 = 0;
     f3 = 1;
     stop();
     
}
} 

void LineFollower3()
{
l1=digitalRead(IR_Left);
l2=digitalRead(IR_Middle);
l3=digitalRead(IR_Right);

l2 = !l2 ;

if(l1==1&&l2==1&&l3==1)   //feedback when comes all sensor on white
{
l1=f1;
l2=f2;
l3=f3;
}

if( ( l1==0&&l2==1&&l3==1) || ( l1==0&&l2==0&&l3==1 ) )   //left turn
  SharpLeft();
else if( ( l1==1&&l2==1&&l3==0 ) || ( l1==1&&l2==0&&l3==0 ) )  //right turn
  SharpRight();
else if ( (l1==1&&l2==0&&l3==1) || (l1 ==0 && l2==0 &&l3==0) )  //go forward straight
{
  Forward();
  analogWrite(en1,v);
  analogWrite(en2,v);
}

f1=l1;
f2=l2;
f3=l3;        //memory variables


d = sonar.ping_cm();
if ( d > 0 && d <= 15) 
{
     SharpLeft();
     delay(400);
     f1 =1 ;
     f2 = 0;
     f3 = 1;
     stop();
     
}
} 
