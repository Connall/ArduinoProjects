#include <LiquidCrystal.h>
#define contrast 6
boolean checkReady();
boolean checkInput(int player);
boolean checkGame();
void readyPlayer();

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int switchState = 0;
int prevSwitchState = 0;
int reply;

const int LIGHT_RED = 10;
const int LIGHT_YELLOW = 9;
const int LIGHT_GREEN = 8;


const int STATUS_MENU = 0;
const int STATUS_PLAYER_1_READY = 1;
const int STATUS_PLAYER_2_READY = 2;
const int STATUS_ALL_READY = 3;
const int STATUS_GAMEPLAY = 4;
const int STATUS_PLAYER_1_WIN = 5;
const int STATUS_PLAYER_2_WIN = 6;
const int STATUS_PLAYER_1_EARLY = 7;
const int STATUS_PLAYER_2_EARLY = 8;
const int STATUS_GAME_OVER = 9;

int gameState = STATUS_MENU;

const int PLAYER_1 = 7;
const int PLAYER_2 = 6;

int player1Score = 0;
int player2Score = 0;

void setup()
{
    //Set up pins.
    pinMode(LIGHT_RED, OUTPUT);
    pinMode(LIGHT_YELLOW, OUTPUT);
    pinMode(LIGHT_GREEN, OUTPUT);
    pinMode(PLAYER_1, INPUT);
    pinMode(PLAYER_2, INPUT);
    
    
    Serial.begin(9600);
    Serial.print("Attempting LCD Print.");
    lcd.begin(16, 2);
    lcd.print("WELCOME PLAYERS!");
    lcd.setCursor(0, 1);
    lcd.print("Click to begin!");
    resetLights();
    
    randomSeed(analogRead(0));
}

void loop()
{
    if (gameState == STATUS_MENU)
    {
        mainMenu();
    }
    else if (gameState == STATUS_PLAYER_1_READY || gameState == STATUS_PLAYER_2_READY)
    {
        resetLights();
        readyPlayer();
    }
    else if (gameState == STATUS_GAMEPLAY)
    {
        startGame();
    }
    else if (gameState == STATUS_GAME_OVER)
    {
        gameOver();
    }
}

void mainMenu()
{
    resetLights();
    checkReady();
    digitalWrite(LIGHT_RED, HIGH);
    delay(150);
    checkReady();
    digitalWrite(LIGHT_RED, LOW);
    digitalWrite(LIGHT_YELLOW, HIGH);
    delay(150);
    checkReady();
    digitalWrite(LIGHT_YELLOW, LOW);
    digitalWrite(LIGHT_GREEN, HIGH);
    checkReady();
    delay(150);
    checkReady();
    
}

void gameOver()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("GAME OVER");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WINNER IS...");
    delay(3000);
    for (int i = 0; i < 5; i++) 
    {
        lcd.clear();
        delay(1000);
        lcd.setCursor(0, 0);
        
        if (player1Score > player2Score)
        {
            lcd.print("Player 1!");
        }
        else
        {
            lcd.print("Player 2!");
        }
        
        delay(1000);
    }
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Thanks for");
    lcd.setCursor(0, 1);
    lcd.print("Playing");
    delay(3000);
    
    player1Score = 0;
    player2Score = 0;
    gameState = STATUS_MENU;
    
    lcd.clear();
    lcd.print("WELCOME PLAYERS!");
    lcd.setCursor(0, 1);
    lcd.print("Click to begin!");
}

void startGame()
{
    resetLights();
    
    int lightRedTime = random(1, 10001);
    int lightYellowTime = random(1, 10001);
    int lightGreenTime = random(1, 10001);
    int target = 0;
    
    countdown();
    
    while(target != lightRedTime)
    {
        if (checkGame())
        {
            return;
        }
        
        target = random(1, 10001);
    }
    
    digitalWrite(LIGHT_RED, HIGH);
    
    while (target != lightYellowTime)
    {
        if (checkGame())
        {
            return;
        }
        
        target = random(1, 10001);
    }
    
    digitalWrite(LIGHT_YELLOW, HIGH);
    
    while(target != lightGreenTime)
    {
        if (checkGame())
        {
            return;
        }
        
        target = random(1, 10001);
    }
    
    digitalWrite(LIGHT_GREEN, HIGH);
    
    while (gameState == STATUS_GAMEPLAY)
    {
        if (checkInput(PLAYER_1))
        {
            gameState = STATUS_PLAYER_1_WIN;
        }
        else if (checkInput(PLAYER_2))
        {
            gameState = STATUS_PLAYER_2_WIN;
        }
                
    }
            
    roundOver();
    
}

boolean checkGame()
{
    if (checkInput(PLAYER_1))
    {
        gameState = STATUS_PLAYER_1_EARLY;
        roundOver();
        return true;
    }
    else if(checkInput(PLAYER_2))
    {
        gameState = STATUS_PLAYER_2_EARLY;
        roundOver();
        return true;
    }
    
    return false;
}

void roundOver()
{
    resetLights();
    
    if (gameState == STATUS_PLAYER_1_EARLY)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Player 1 Early!");
        lcd.setCursor(0, 1);
        lcd.print("Player 2 Wins!");
        
        player2Score++;
    }
    else if (gameState == STATUS_PLAYER_2_EARLY)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Player 2 Early!");
        lcd.setCursor(0, 1);
        lcd.print("Player 1 Wins!");
        
        player1Score++;
    }
    else if (gameState == STATUS_PLAYER_1_WIN)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Player 1 Wins!");
        player1Score++;
    }
    else if(gameState == STATUS_PLAYER_2_WIN)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Player 2 Wins!");
        player2Score++;
    }
    
    delay(3000);
    roundSummary();
}

void roundSummary()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Player 1: ");
    lcd.print(player1Score);
    lcd.setCursor(0, 1);
    lcd.print("Player 2: ");
    lcd.print(player2Score);
    delay(3000);
    
    if (player1Score == 3 || player2Score == 3)
    {
        gameState = STATUS_GAME_OVER;
    }
    else 
    {
        gameState = STATUS_GAMEPLAY;
    }
}

void readyPlayer()
{
    if (gameState == STATUS_PLAYER_1_READY)
    {
        digitalWrite(LIGHT_RED, HIGH);
        if (checkInput(PLAYER_2))
        {
            gameState = STATUS_ALL_READY;
            digitalWrite(LIGHT_GREEN, HIGH);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("All Ready!");
            delay(1000);
            gameState = STATUS_GAMEPLAY;
        }
    }
    else
    {
        digitalWrite(LIGHT_GREEN, HIGH);
        if (checkInput(PLAYER_1))
        {
            gameState = STATUS_ALL_READY;
            digitalWrite(LIGHT_RED, HIGH);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("All Ready!");
            delay(1000);
            gameState = STATUS_GAMEPLAY;
        }
    }
}

void countdown()
{
    resetLights();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Begins In:");
    
    for (int i = 5; i > 0; i--)
    {
        lcd.setCursor(0, 1);
        lcd.print(i);
        delay(1000);
    }
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Begin!");
    delay(1000);
    lcd.clear();
}
        
        

boolean checkInput(int player)
{
    if (player == PLAYER_1)
    {
        if (digitalRead(PLAYER_1) == HIGH)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (player == PLAYER_2)
    {
        if (digitalRead(PLAYER_2) == HIGH)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;    
    }
}

boolean checkReady()
{
    if (checkInput(PLAYER_1))
    {
        gameState = STATUS_PLAYER_1_READY;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Player 1: READY");
        delay(500);
    }
    else if (checkInput(PLAYER_2))
    {
        resetLights();
        gameState = STATUS_PLAYER_2_READY;
        digitalWrite(LIGHT_GREEN, HIGH);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Player 2: READY");
        delay(500);
    }
}


void resetLights()
{
    digitalWrite(LIGHT_RED, LOW);
    digitalWrite(LIGHT_GREEN, LOW);
    digitalWrite(LIGHT_YELLOW, LOW);
}
