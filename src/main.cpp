#include <Arduino.h>
#include <LiquidCrystal.h>

const int RS = 8;
const int EN = 9;
const int d4 = 4;
const int d5 = 5;
const int d6 = 6;
const int d7 = 7;

LiquidCrystal lcd(RS,EN, d4,  d5,  d6,  d7);

char player = '+';
int jumprate = -1;
byte mobs[2][16] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,1,0,0,0,0}};


void displayLeft(){
    for(int i = 0;i < 16;i++)
        lcd.scrollDisplayRight();
}
void displayRight(){
    for(int i = 0;i < 16;i++)
        lcd.scrollDisplayLeft();
}
void updatePlayerPos(int &x,int &y){
    int moves = analogRead(A0);
    if(moves >= 0 && moves <= 60){
        //Right
        x+=1;
        x = min(15,x);
    }else if(moves <= 200){
        //jump
        y = 0;
        jumprate = 3;
    }else if(moves <= 400){
        //Down (( crouch))
        y=1;
        player = '_';
    }else if(moves <= 600){
        //left
        x-=1;
        x = max(0,x);
    }else if(moves <= 800){

    }
    if(moves > 400 || moves <= 200){
        player = '+';
    }
    lcd.setCursor(x ,y);
}

void renderPlayer(int x,int y){
    lcd.setCursor(x,y);
    lcd.print(player);
}
void updateMobs(byte mobs[2][16]){
    for(int y = 0;y < 2;y++){
        for(int x = 1;x < 16;x++){
            mobs[y][x-1] = mobs[y][x] ;
        }
        mobs[y][15] = 0;
    }
    bool prob = rand()%4 == 0;
    if(prob && ((mobs[1][14] & mobs[1][13] & mobs[1][12]) == 0)){
        mobs[1][15] = 1;     
    }
}
void renderMobs(byte mobs[2][16]){
    for(int y = 0;y < 2;y++){
        for(int x = 0;x < 15;x++){
            lcd.setCursor(x,y);
            byte enemy = mobs[y][x];
            if(enemy == 1){
                lcd.print("|");
            }
        }
    }
}


void setup(){
    lcd.begin(16,2);
    lcd.setCursor(0,0);
    renderPlayer(0,1);
    renderMobs(mobs);
    delay(2000);
}


void loop(){
    static int x = 0;
    static int y = 1;
    static int score = 1;
    static bool alive = true;
    if(alive){
        delay(250);
        lcd.clear();
        lcd.setCursor(10,0);
        String sc = String(score);
        lcd.print(sc.c_str());
        if(jumprate == 0)
            y = 1;
        if(jumprate == -1){
            y = 1;
            updatePlayerPos(x,y);
        }else{
            jumprate--;
        }
        updateMobs(mobs);
        renderPlayer(x,y);
        renderMobs(mobs);
        if(mobs[y][x] == 1){
            alive = false;
        }else{
            score+=1;
        }
    }else{
        lcd.setCursor(10,0);
        String sc = String(score);
        lcd.print(sc.c_str());
        lcd.setCursor(0,0);
        lcd.print('D');
        delay(1000);
    }
}