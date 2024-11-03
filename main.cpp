#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <string>
using namespace std;

//======================================
// Kích thước của bàn cờ
//======================================
const int width = 40;
const int height = 15;

//======================================
// Các biến toàn cục
//======================================
int x, y, fruitX, fruitY, score;
vector<pair<int, int>> snake; // Vecto chứa vị trí của rắn
char direction;               // Hướng di chuyển của rắn
bool gameOver;

void PrintTitle();
void StartGame(string &playerName);
void Setup();
void HideCursor();
void Input();
void Draw(HANDLE hConsole, const string &playerName);
void Logic();
int RestartGame();
void clearTerminal();
//======================================
// Hàm vẽ bàn cờ và rắn
//======================================
void PrintTitle()
{
    cout << " _____            _   _    _____         _   _   __  __  ____ _____ " << endl;
    cout << "|  __ \\     /\\   | \\ | |  / ____|  /\\   | \\ | | |  \\/  |/ __ \\_   _|" << endl;
    cout << "| |__) |   /  \\  |  \\| | | (___   /  \\  |  \\| | | \\  / | |  | || |  " << endl;
    cout << "|  _  /   / /\\ \\ | . ` |  \\___ \\ / /\\ \\ | . ` | | |\\/| | |  | || |  " << endl;
    cout << "| | \\ \\  / ____ \\| |\\  |  ____) / ____ \\| |\\  | | |  | | |__| || |_ " << endl;
    cout << "|_|  \\_\\/_/    \\_\\_| \\_| |_____/_/    \\_\\_| \\_| |_|  |_|\\____/_____|" << endl;
    cout << endl;
}

void Draw(HANDLE hConsole, const string &playerName)
{
    COORD coord;
    coord.X = 0;
    coord.Y = 0;                               // Bắt đầu từ tọa độ (0, 0)
    SetConsoleCursorPosition(hConsole, coord); // Đặt con trỏ tới vị trí bắt đầu
    PrintTitle();
    cout << "Xin chao : " << playerName << endl; // Hiển thị tên người chơi
    cout << "Diem : " << score << endl;          // Hiện điểm
    // Vẽ viền
    for (int i = 0; i < width + 2; i++)
        cout << "=";
    cout << endl;
    // Vẽ thân rắn
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
                cout << "|"; // Biên trái

            if (i == y && j == x)
            {
                cout << "O"; // Vẽ đầu rắn
            }
            else if (i == fruitY && j == fruitX)
            {
                cout << "*"; // Vẽ trái cây
            }
            else
            {
                bool isSnakeBody = false;
                for (const auto &s : snake)
                {
                    if (s.first == j && s.second == i)
                    {
                        cout << "o"; // Vẽ thân rắn
                        isSnakeBody = true;
                        break;
                    }
                }
                if (!isSnakeBody)
                    cout << " "; // Ô trống
            }

            if (j == width - 1)
                cout << "|"; // Biên phải
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "="; // Biên dưới
    cout << endl;
    cout << "Huong dan:" << endl;
    cout << "- Nhan A,W,S,D de di chuyen ran, tranh cham vao tuong và than ran" << endl;
    cout << "- Nhan (x) de dung tro choi" << endl;
}

//======================================
// Hàm thiết lập trò chơi
//======================================
void Setup()
{
    snake.clear();
    gameOver = false;
    direction = ' '; // Khởi tạo hướng di chuyển
    x = width / 2;   // Khởi tạo vị trí đầu rắn
    y = height / 2;
    fruitX = rand() % width; // Vị trí trái cây
    fruitY = rand() % height;
    score = 0;
    snake.push_back(make_pair(x, y)); // Đầu rắn
}
void clearTerminal()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}
// Hàm xử lý đầu vào
void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            if (direction == 'U' || direction == 'D' || direction == ' ')
                direction = 'L';
            break; // Trái
        case 'd':
            if (direction == 'U' || direction == 'D' || direction == ' ')
                direction = 'R';
            break; // Phải
        case 'w':
            if (direction == 'L' || direction == 'R' || direction == ' ')
                direction = 'U';
            break; // Lên
        case 's':
            if (direction == 'L' || direction == 'R' || direction == ' ')
                direction = 'D';
            break; // Xuống
        case 'x':
            gameOver = true;
            break; // Thoát game
        }
    }
}

//======================================
// Hàm cập nhật trạng thái
//======================================
void Logic()
{
    int prevX = snake[0].first;
    int prevY = snake[0].second;
    int prev2X, prev2Y;
    snake[0].first = x;
    snake[0].second = y;

    for (size_t i = 1; i < snake.size(); i++)
    {
        prev2X = snake[i].first;
        prev2Y = snake[i].second;
        snake[i].first = prevX;
        snake[i].second = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (direction)
    {
    case 'L':
        x--;
        break;
    case 'R':
        x++;
        break;
    case 'U':
        y--;
        break;
    case 'D':
        y++;
        break;
    }

    if (x >= width || x < 0 || y >= height || y < 0)
    {
        gameOver = true;
        return;
    }

    for (size_t i = 1; i < snake.size(); i++)
    {
        if (snake[i].first == x && snake[i].second == y)
        {
            gameOver = true;
            return;
        } // Tự đâm vào
    }

    if (x == fruitX && y == fruitY)
    {
        score += 10;             // Tăng điểm
        fruitX = rand() % width; // Vị trí mới cho trái cây
        fruitY = rand() % height;
        snake.push_back(make_pair(-1, -1)); // Thêm thân rắn
    }
}
void clearLines(int numLines)
{
    for (int i = 0; i < numLines; ++i)
    {
        std::cout << "\033[1A"; // Move cursor up by 1 line
        std::cout << "\033[2K"; // Clear the entire line
    }
}
//======================================
// Hàm nhập tên và bắt đầu game
//======================================
void StartGame(string &playerName)
{
    clearTerminal();
    PrintTitle();
    cout << "Thanh vien nhom: " << endl;
    cout << "1. Vo Dang Truong - MSSV 24730075 " << endl;
    cout << "2. Do Ngoc Minh Tien - MSSV 24730070" << endl;
    cout << "3. Ly Duc Minh - MSSV 24730045" << endl;
    cout << endl;
    cout << "Dang tai... ";
    for (int i = 1; i <= 100; i++)
    {
        Sleep(5);
        cout << "=";
    }
    cout << " 100%" << endl;
    cout << endl;
    while (playerName.empty())
    {
        cout << "Nhap ten cua ban va nhan [ENTER] de bat dau choi: ";
        getline(cin, playerName);
    }
    clearTerminal();
}

//======================================
// Hàm ẩn con trỏ
//======================================
void HideCursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false; // Ẩn con trỏ
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
//======================================
// Hàm bắt đầu chơi
//======================================
void Play(string &playerName)
{
    StartGame(playerName);
    Setup();
    HideCursor();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    while (!gameOver)
    {
        Input();
        Draw(hConsole, playerName);
        Logic();
        Sleep(110);
    }
    cout << endl;
    cout << "=====================================" << endl;
    cout << "   TRO CHOI KET THUC.!!!             " << endl;
    cout << "=====================================" << endl;
    cout << "   DIEM CUA BAN LA :" << score << endl;
    cout << "=====================================" << endl;
    cout << "   Nhan (r) de choi lai" << endl;
    cout << "=====================================" << endl;
    cout << "   Nhan (x) de thoat game" << endl;
    cout << "=====================================" << endl;
    while (gameOver)
    {
        int isRestart = RestartGame();
        if (isRestart == 1)
        {
            Play(playerName);
            break;
        }
        else if (isRestart == -1)
        {
            return;
        }
    }
}
//======================================
// Hàm chơi lại từ đầu
// Đầu ra: (int)
//  1 = Chơi lại
// -1 = Thoát
//  0 = Bắt đầu lại vòng lặp, hỏi user có muốn chơi lại hay không
//======================================
int RestartGame()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'r':
            return 1;
        case 'x':
            return -1;
        }
    }
    return 0;
}

//======================================
// Hàm chính
//======================================
int main()
{

    string playerName;
    Play(playerName);
    return 0;
}
