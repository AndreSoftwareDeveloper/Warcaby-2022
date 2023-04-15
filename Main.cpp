#include <SFML/Graphics.hpp>
#include <iostream>
#include <locale.h>
#include <string>
#include "Windows.h"

using namespace sf;
using namespace std;

RenderWindow mainWindow(VideoMode(1024, 820, 32), "Warcaby 2022");


class Board
{
public:
    Board()
    {
        LoadTextures();
        CalculatePosition();

        for (int i = 0; i < 64; i++)
        {
            if ((i >= 0 && i < 8) || (i >= 16 && i < 24) || (i >= 32 && i < 40) || (i >= 48 && i < 56))
            {
                if (i % 2 == 0)
                    BoardFields[i].setTexture(Texture_WhiteSquare);
                else
                    BoardFields[i].setTexture(Texture_BlackSquare);
            }
            else
            {
                if (i % 2 == 0)
                    BoardFields[i].setTexture(Texture_BlackSquare);
                else
                    BoardFields[i].setTexture(Texture_WhiteSquare);
            }
        }
    }

    void ShowBoard()
    {
        for (int i = 0; i < 64; i++)
        {
            mainWindow.draw(BoardFields[i]);
        }
    }

private:
    Sprite BoardFields[64];
    int i_YAxisDistance = 0, i_XAxisDistance = 120;
    Texture Texture_WhiteSquare, Texture_BlackSquare;

    void LoadTextures()
    {
        if (!(Texture_WhiteSquare.loadFromFile("WhiteSquare.png")))
            cout << "Error while loading white square texture." << "\n";
        if (!(Texture_BlackSquare.loadFromFile("BlackSquare.png")))
            cout << "Error while loading black square texture." << "\n";
    }

    void CalculatePosition()
    {
        for (int i = 0; i < 64; i++)
        {
            BoardFields[i].setScale(0.39, 0.39);
            BoardFields[i].setPosition(i_XAxisDistance, i_YAxisDistance);
            i_XAxisDistance += 95;
            if (!((i + 1) % 8))
            {
                i_YAxisDistance += 95;
                i_XAxisDistance = 120;
            }
        }
    }
};

class Pawn
{
protected:
    Sprite Sprite_SinglePawn;
    Texture Texture_PawnTexture;

    Pawn()
    {
        Sprite_SinglePawn.setScale(0.39, 0.39);
        Sprite_SinglePawn.setPosition(215, 0);
    }

    Pawn(int x, int y)
    {
        Sprite_SinglePawn.setScale(0.39, 0.39);
        Sprite_SinglePawn.setPosition(x, y);
    }

    void MovePawn()
    {
        Sprite_SinglePawn.setPosition(310, 95);
    }
};

bool b_BeginPlayer = true;

class BlackPawn :Pawn
{
public:
    BlackPawn() :Pawn()
    {
        GetTexture();
    }

    BlackPawn(int x, int y) :Pawn(x, y)
    {
        GetTexture();
    }

    void ShowPawn()
    {
        mainWindow.draw(Sprite_SinglePawn);
    }
    void MovePawn()
    {
        Pawn::MovePawn();
    }
    void MovePawn(int x, int y)
    {
        Sprite_SinglePawn.setPosition(x, y);
        b_BeginPlayer = true;
    }
    Vector2f GetPawnPosition()
    {
        return Sprite_SinglePawn.getPosition();
    }

    void GetTexture()
    {
        if (!(Texture_PawnTexture.loadFromFile("PawnBlack.png")))
            cout << "Error while loading black pawn texture." << "\n";
        Sprite_SinglePawn.setTexture(Texture_PawnTexture);
    }
};

class WhitePawn :Pawn
{
public:
    WhitePawn() :Pawn()
    {
        GetTexture();
    }

    WhitePawn(int x, int y) :Pawn(x, y)
    {
        GetTexture();
    }

    void ShowPawn()
    {
        mainWindow.draw(Sprite_SinglePawn);
    }
    void MovePawn()
    {
        Sprite_SinglePawn.setPosition(310, 95);
    }
    void MovePawn(int x, int y)
    {
        Sprite_SinglePawn.setPosition(x, y);
        b_BeginPlayer = false;
    }

    Vector2f GetPawnPosition()
    {
        return Sprite_SinglePawn.getPosition();
    }

private:
    void GetTexture()
    {
        if (!(Texture_PawnTexture.loadFromFile("PawnWhite.png")))
            cout << "Error while loading white pawn texture." << "\n";
        Sprite_SinglePawn.setTexture(Texture_PawnTexture);
    }
};

WhitePawn Sprite_WhitePawns[12];
BlackPawn Sprite_BlackPawns[12];
Board Board_CheckersBoard;

enum ProgramStates
{
    mainMenu = 0,
    Credits = 1,
    Play = 2,
    GameOver = 3
};

ProgramStates ProgramState;

class Player
{
protected:
    int i_Score = 0;
    virtual void AddPoint() = 0;
};

class WhitePlayer :Player
{
public:
    Vector2f WhitePawnsPositions[12];    
    void AddPoint()
    {
        i_Score++;
        if (i_Score == 12)
        {
            cout << "Zwyciestwo gracza BIALEGO";
            ProgramState = GameOver;            
        }
    }
};

class BlackPlayer :Player
{
public:
    Vector2f BlackPawnsPositions[12];    
    void AddPoint()
    {
        i_Score++;
        if (i_Score == 12)
        {            
            cout << "Zwyciestwo gracza CZARNEGO";
            ProgramState = GameOver;
        }
    }
};

WhitePlayer WhitePlayer1;
BlackPlayer BlackPlayer1;

void PrintAgainstTheRulesMessage()
{
    cout << "To niezgodne z zasadami!\n";
    Sleep(5000);
}

void TakeWhitePawn(Vector2f WhitePawnToTakePosition, BlackPawn& BlackPawnToMove, int i_DestinationX, int i_DestinationY)
{
    for (int k = 0; k < 12; k++)
    {
        if (Sprite_WhitePawns[k].GetPawnPosition() == WhitePawnToTakePosition)
        {
            Sprite_WhitePawns[k].MovePawn(9999, 9999);
            BlackPlayer1.AddPoint();
        }
    }
    BlackPawnToMove.MovePawn(i_DestinationX, i_DestinationY);
}

void TakeBlackPawn(Vector2f BlackPawnToTakePosition)
{
    for (int k = 0; k < 12; k++)
    {
        if (Sprite_BlackPawns[k].GetPawnPosition() == BlackPawnToTakePosition)
        {
            Sprite_BlackPawns[k].MovePawn(9999, 9999);
            WhitePlayer1.AddPoint();
        }
    }    
}

void MovePawnByPlayer(char c_SourceX, char c_SourceY, char c_DestinationX, char c_DestinationY)
{
    int i_SourceX = ((int)c_SourceX - 65) * 95 + 120;
    int i_SourceY = (int)c_SourceY - 49;
    i_SourceY = 0 + i_SourceY * 95;
    bool b_CanBeMoved = false;

    int i_DestinationX = ((int)c_DestinationX - 65) * 95 + 120;
    int i_DestinationY = (int)c_DestinationY - 49;
    i_DestinationY = 0 + i_DestinationY * 95;

    Vector2f DestinationVector;
    DestinationVector.x = i_DestinationX;
    DestinationVector.y = i_DestinationY;

    for (int i = 0; i < 12; i++)
    {
        WhitePlayer1.WhitePawnsPositions[i] = Sprite_WhitePawns[i].GetPawnPosition();
        BlackPlayer1.BlackPawnsPositions[i] = Sprite_BlackPawns[i].GetPawnPosition();
    }

    for (int i = 0; i < 12; i++)
    {
        if (b_BeginPlayer)
        {
            if (Sprite_WhitePawns[i].GetPawnPosition().x == i_SourceX && Sprite_WhitePawns[i].GetPawnPosition().y == i_SourceY)
            {
                if (i_DestinationY == i_SourceY + 95)
                {
                    if (i_DestinationX == i_SourceX + 95 || i_DestinationX == i_SourceX - 95)
                    {
                        for (int j = 0; j < 12; j++)
                        {
                            if ((i != j && WhitePlayer1.WhitePawnsPositions[j] == DestinationVector) || (BlackPlayer1.BlackPawnsPositions[j] == DestinationVector))
                            {
                                b_CanBeMoved = false;
                                PrintAgainstTheRulesMessage();
                                break;
                            }
                            b_CanBeMoved = true;
                        }
                        if (b_CanBeMoved)
                        {
                            Sprite_WhitePawns[i].MovePawn(i_DestinationX, i_DestinationY);
                            b_CanBeMoved = true;                            
                        }
                    }
                    else
                        if (!b_CanBeMoved)
                            PrintAgainstTheRulesMessage();
                }
                else
                    if (i_DestinationY == i_SourceY + 190 || i_DestinationY == i_SourceY - 190)
                    {
                        if (i_DestinationX == i_SourceX + 190)
                        {
                            for (int j = 0; j < 12; j++)
                            {
                                if ((i != j && WhitePlayer1.WhitePawnsPositions[j] == DestinationVector) || (BlackPlayer1.BlackPawnsPositions[j] == DestinationVector))
                                    b_CanBeMoved = false;
                                if (i_DestinationY == i_SourceY + 190)
                                {
                                    if ((BlackPlayer1.BlackPawnsPositions[j].x == i_SourceX + 95) && (BlackPlayer1.BlackPawnsPositions[j].y == i_DestinationY - 95))
                                    {
                                        TakeBlackPawn(BlackPlayer1.BlackPawnsPositions[j]);
                                        Sprite_WhitePawns[i].MovePawn(i_DestinationX, i_DestinationY);
                                        b_CanBeMoved = true;                                        
                                        continue;
                                    }
                                }
                                else
                                    if (i_DestinationY == i_SourceY - 190)
                                    {
                                        if ((BlackPlayer1.BlackPawnsPositions[j].x == i_SourceX + 95) && (BlackPlayer1.BlackPawnsPositions[j].y == i_DestinationY + 95))
                                        {
                                            TakeBlackPawn(BlackPlayer1.BlackPawnsPositions[j]);
                                            Sprite_WhitePawns[i].MovePawn(i_DestinationX, i_DestinationY);
                                            b_CanBeMoved = true;                                            
                                            continue;
                                        }
                                    }
                            }
                            if (!b_CanBeMoved)
                                PrintAgainstTheRulesMessage();
                        }
                        else
                            if (i_DestinationX == i_SourceX - 190)
                            {
                                for (int j = 0; j < 12; j++)
                                {
                                    if ((i != j && WhitePlayer1.WhitePawnsPositions[j] == DestinationVector) || (BlackPlayer1.BlackPawnsPositions[j] == DestinationVector))
                                        b_CanBeMoved = false;

                                    if (i_DestinationY == i_SourceY + 190)
                                    {
                                        if ((BlackPlayer1.BlackPawnsPositions[j].x == i_SourceX - 95) && (BlackPlayer1.BlackPawnsPositions[j].y == i_DestinationY - 95))
                                        {
                                            TakeBlackPawn(BlackPlayer1.BlackPawnsPositions[j]);
                                            Sprite_WhitePawns[i].MovePawn(i_DestinationX, i_DestinationY);
                                            b_CanBeMoved = true;                                           
                                            continue;
                                        }
                                    }
                                    else
                                        if (i_DestinationY == i_SourceY - 190)
                                        {
                                            if ((BlackPlayer1.BlackPawnsPositions[j].x == i_SourceX - 95) && (BlackPlayer1.BlackPawnsPositions[j].y == i_DestinationY + 95))
                                            {
                                                TakeBlackPawn(BlackPlayer1.BlackPawnsPositions[j]);
                                                Sprite_WhitePawns[i].MovePawn(i_DestinationX, i_DestinationY);
                                                b_CanBeMoved = true;                                                
                                                continue;
                                            }
                                        }
                                }
                                if (!b_CanBeMoved)
                                    PrintAgainstTheRulesMessage();
                            }
                            else
                                PrintAgainstTheRulesMessage();
                    }
            }
        }
        else
        {
            if (Sprite_BlackPawns[i].GetPawnPosition().x == i_SourceX && Sprite_BlackPawns[i].GetPawnPosition().y == i_SourceY)
            {
                if (i_DestinationY == i_SourceY - 95)
                {
                    if (i_DestinationX == i_SourceX + 95 || i_DestinationX == i_SourceX - 95)
                    {
                        for (int j = 0; j < 12; j++)
                        {
                            if ((i != j && BlackPlayer1.BlackPawnsPositions[j] == DestinationVector) || (WhitePlayer1.WhitePawnsPositions[j] == DestinationVector))
                            {
                                b_CanBeMoved = false;
                                PrintAgainstTheRulesMessage();
                                break;
                            }
                            b_CanBeMoved = true;
                        }
                        if (b_CanBeMoved)
                        {
                            Sprite_BlackPawns[i].MovePawn(i_DestinationX, i_DestinationY);
                            b_CanBeMoved = true;                            
                        }
                    }
                    else
                        if (!b_CanBeMoved)
                            PrintAgainstTheRulesMessage();
                }
                else
                    if (i_DestinationY == i_SourceY - 190 || i_DestinationY == i_SourceY + 190)
                    {
                        if (i_DestinationX == i_SourceX + 190)
                        {
                            for (int j = 0; j < 12; j++)
                            {
                                if ((i != j && BlackPlayer1.BlackPawnsPositions[j] == DestinationVector) || (WhitePlayer1.WhitePawnsPositions[j] == DestinationVector))
                                    b_CanBeMoved = false;

                                if (i_DestinationY == i_SourceY - 190)
                                {
                                    if ((WhitePlayer1.WhitePawnsPositions[j].x == i_SourceX + 95) && (WhitePlayer1.WhitePawnsPositions[j].y == i_DestinationY + 95))
                                    {
                                        TakeWhitePawn(WhitePlayer1.WhitePawnsPositions[j], Sprite_BlackPawns[i], i_DestinationX, i_DestinationY);                                        
                                        b_CanBeMoved = true;                                        
                                        continue;
                                    }
                                }
                                else
                                    if (i_DestinationY == i_SourceY + 190)
                                    {
                                        if ((WhitePlayer1.WhitePawnsPositions[j].x == i_SourceX + 95) && (WhitePlayer1.WhitePawnsPositions[j].y == i_DestinationY - 95))
                                        {
                                            TakeWhitePawn(WhitePlayer1.WhitePawnsPositions[j], Sprite_BlackPawns[i], i_DestinationX, i_DestinationY);
                                            b_CanBeMoved = true;                                            
                                            continue;
                                        }
                                    }
                            }
                            if (!b_CanBeMoved)
                                PrintAgainstTheRulesMessage();
                        }
                        else
                            if (i_DestinationX == i_SourceX - 190)
                            {
                                for (int j = 0; j < 12; j++)
                                {
                                    if ((i != j && BlackPlayer1.BlackPawnsPositions[j] == DestinationVector) || (WhitePlayer1.WhitePawnsPositions[j] == DestinationVector))
                                        b_CanBeMoved = false;

                                    if (i_DestinationY == i_SourceY - 190)
                                    {
                                        if ((WhitePlayer1.WhitePawnsPositions[j].x == i_SourceX - 95) && (WhitePlayer1.WhitePawnsPositions[j].y == i_DestinationY + 95))
                                        {
                                            TakeWhitePawn(WhitePlayer1.WhitePawnsPositions[j], Sprite_BlackPawns[i], i_DestinationX, i_DestinationY);
                                            b_CanBeMoved = true;                                            
                                            continue;
                                        }
                                    }
                                    else
                                        if (i_DestinationY == i_SourceY + 190)
                                        {
                                            if ((WhitePlayer1.WhitePawnsPositions[j].x == i_SourceX - 95) && (WhitePlayer1.WhitePawnsPositions[j].y == i_DestinationY - 95))
                                            {
                                                TakeWhitePawn(WhitePlayer1.WhitePawnsPositions[j], Sprite_BlackPawns[i], i_DestinationX, i_DestinationY);
                                                b_CanBeMoved = true;                                                
                                                continue;
                                            }
                                        }
                                }
                                if (!b_CanBeMoved)
                                    PrintAgainstTheRulesMessage();
                            }
                            else
                                PrintAgainstTheRulesMessage();
                    }
            }
        }
    }
}
void ArrangeThePawns()
{    
    for (int i_Counter = 0, i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++, i_Counter++)
        {
            if (!(i % 2))
            {
                Sprite_WhitePawns[i_Counter].MovePawn(215 + 190 * j, 0 + 95 * i);
                Sprite_BlackPawns[i_Counter].MovePawn(120 + 190 * j, 475 + 95 * i);
            }                          
            else
            {
                Sprite_WhitePawns[i_Counter].MovePawn(120 + 190 * j, 0 + 95 * i);
                Sprite_BlackPawns[i_Counter].MovePawn(215 + 190 * j, 475 + 95 * i);
            }                                      
        }
    }
}

void ShowAllPawns()
{
    for (int i = 0; i < 12; i++)
    {
        Sprite_WhitePawns[i].ShowPawn();
        Sprite_BlackPawns[i].ShowPawn();
    }
}

void WindowColorOnClickEvents(RenderWindow& thatWindow, Text& ButtonText)
{
    Event UserEvent;
    while (thatWindow.pollEvent(UserEvent))
    {
        if (UserEvent.type == Event::Closed)
            thatWindow.close();
        if (UserEvent.type == Event::KeyPressed && UserEvent.key.code == Keyboard::Escape)
            thatWindow.close();
    }
    if (ButtonText.getGlobalBounds().contains(Mouse::getPosition(thatWindow).x, Mouse::getPosition(thatWindow).y))
    {
        ButtonText.setFillColor(Color::Red);
        if (UserEvent.type == Event::MouseButtonPressed)
        {
            string TextOnButton = ButtonText.getString();
            if (TextOnButton == "Nowa gra")
                ProgramState = Play;
            if (TextOnButton == "Autorzy")
                ProgramState = Credits;
            else if (TextOnButton == "Powrót do menu")
                ProgramState = mainMenu;
            else if (TextOnButton == "Koniec")
                thatWindow.close();
        }
    }
    else
        ButtonText.setFillColor(Color::Yellow);
}

void setTextParameters(Text& ButtonText, Font& MenuFont, string TextToDisplay = "ERROR", int PositionX = 450, int PositionY = 300, int CharacterSize = 40, Color TextColor = Color::Yellow)
{
    ButtonText.setString(TextToDisplay);
    ButtonText.setPosition(PositionX, PositionY);
    ButtonText.setCharacterSize(CharacterSize);
    ButtonText.setFillColor(TextColor);
    ButtonText.setFont(MenuFont);
    ButtonText.setStyle(Text::Bold);
}

int main()
{
    Font MenuFont;
    if (MenuFont.loadFromFile("melianes.ttf"))
        cout << "Font loaded properly.\n";
    else
        cout << "Eror while loading font.\n";
    Texture BackgroundFile;
    if (BackgroundFile.loadFromFile("BoardBackground.jpg"))
        cout << "Texture loaded properly.\n";
    else
        cout << "Eror while loading texture.\n";
    Sprite MenuBackground;
    MenuBackground.setTexture(BackgroundFile);
    MenuBackground.setScale(0.75, 0.75);
    MenuBackground.setPosition(40, 40);

    Text Text_PlayButton, Credits, AUTOR, PreviousScreen, ExitButton, GameOver;
    setTextParameters(Text_PlayButton, MenuFont, "Nowa gra", 440); //dać automatyczne wyśrodkowanie zamiast ręcznego podawania położenia w osi X     
    setTextParameters(Credits, MenuFont, "Autorzy", 450, 350);
    setTextParameters(AUTOR, MenuFont, "autor jest tylko jeden :)", 350, 300, 30, Color::Green);
    setTextParameters(PreviousScreen, MenuFont, "Powrót do menu", 100, 600, 30, Color::Blue);
    setTextParameters(ExitButton, MenuFont, "Koniec", 477, 400);
    setTextParameters(GameOver, MenuFont, "KONIEC GRY", 430, 300, 40, Color::Green);

    Sprite Sprite_OnBoardPosition;
    Texture Texture_OnBoardPosition;
    Texture_OnBoardPosition.loadFromFile("numbers.png");
    Sprite_OnBoardPosition.setTexture(Texture_OnBoardPosition);
    Sprite_OnBoardPosition.setPosition(70, 15);
    
    ArrangeThePawns();
    char c_SourceX, c_SourceY, c_DestinationX, c_DestinationY;

    while (mainWindow.isOpen())
    {
        mainWindow.clear(Color(255, 255, 255));
        switch (ProgramState)
        {
         case 0: mainWindow.draw(MenuBackground);
            mainWindow.draw(Credits);
            mainWindow.draw(Text_PlayButton);
            mainWindow.draw(ExitButton); break;
         case 1: mainWindow.draw(MenuBackground);
            mainWindow.draw(AUTOR);
            mainWindow.draw(PreviousScreen); break;
         case 2:
            mainWindow.draw(Sprite_OnBoardPosition);
            Board_CheckersBoard.ShowBoard();
            system("cls");
            if (b_BeginPlayer)
                cout << "Ruch gracza BIALEGO\nKtory pionek chcesz przesunac?\nPozycja o osi X:";
            else
                cout << "Ruch gracza CZARNEGO\nKtory pionek chcesz przesunac?\nPozycja o osi X:";
            cin >> c_SourceX;
            cout << "Pozycja w osi Y:";
            cin >> c_SourceY;
            cout << "Na jaka pozycje chcesz go przesunac?\nPozycja w osi X:";
            cin >> c_DestinationX;
            cout << "Pozycja w osi Y:";
            cin >> c_DestinationY;
            MovePawnByPlayer(c_SourceX, c_SourceY, c_DestinationX, c_DestinationY);
            ShowAllPawns(); break;
         case 3:
            mainWindow.draw(MenuBackground);
            mainWindow.draw(GameOver);
            mainWindow.draw(ExitButton); break;
        }
        mainWindow.display();
        WindowColorOnClickEvents(mainWindow, Credits);
        WindowColorOnClickEvents(mainWindow, PreviousScreen);
        WindowColorOnClickEvents(mainWindow, ExitButton);
        WindowColorOnClickEvents(mainWindow, Text_PlayButton);
    }
    return 0;
}