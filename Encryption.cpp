#include <iostream>
#include <bitset>
#include<string>
#include <Windows.h>
using namespace std;
const int BUTTON_ID = 1001;
const int EDIT_INPUT_PLAINTEXT_ID = 1002;
const int EDIT_INPUT_KEY_ID = 1003;
const int EDIT_OUTPUT_CIPHERTEXT_ID = 1004;

HWND hWndButton;
HWND hWndEditInputPlaintext;
HWND hWndEditInputKey;
HWND hWndEditOutputCiphertext;

int a, b;
// P10置换表
int P10[] = { 3, 5, 2, 7, 4, 10, 1, 9, 8, 6 };

// P8置换表
int P8[] = { 6, 3, 7, 4, 8, 5, 10, 9 };

// 左移位数
int left_shift1[] = { 1, 1 };
int left_shift2[] = { 2, 2 };

// 左循环移位函数
bitset<5> leftCircularShift(const bitset<5>& bits, int shift) {
    bitset<5> shifted_bits;
    for (int i = 0; i < 5; i++) {
        int new_pos = (i + shift) % 5;
        shifted_bits[i] = bits[new_pos];
    }
    return shifted_bits;
}

// P8置换函数
bitset<8> performP8(const bitset<10>& key) {
    bitset<8> new_key;
    for (int i = 0; i < 8; i++) {
        new_key[i] = key[P8[i] - 1];
    }
    return new_key;
}

void replace_start(int p[], int c1[])//初始置换
{

    int rs[8] = { 2,6,3,1,4,8,5,7 };//初始置换盒
    for (int i = 0; i < 8; i++)
    {
        c1[i] = p[rs[i] - 1];
    }
}
void replace_end(int c[], int p1[])//最终置换
{

    int re[8] = { 4,1,3,5,7,2,8,6 };//最终置换盒
    for (int i = 0; i < 8; i++)
    {
        p1[i] = c[re[i] - 1];
    }

}
// 左移一位
void leftShift(int arr[]) {
    int temp = arr[0]; // 保存第一个元素的值

    for (int i = 0; i < 3; i++) {
        arr[i] = arr[i + 1]; // 将后一个元素的值赋给当前元素
    }

    arr[3] = temp; // 将保存的第一个元素的值赋给最后一个元素
}

// 右移一位
void rightShift(int arr[]) {
    int temp = arr[3]; // 保存最后一个元素的值

    for (int i = 3; i > 0; i--) {
        arr[i] = arr[i - 1]; // 将前一个元素的值赋给当前元素
    }

    arr[0] = temp; // 将保存的最后一个元素的值赋给第一个元素
}
int bitOr(int a, int b) {//相或
    return (a ^ b); // 使用按位或运算符实现
}
void x_y(int a, int b, int& c)//s盒依据拓展的特定位数判断盒中的横纵
{
    if (a == 1 && b == 1)
    {
        c = 3;
    }
    else if (a == 1 && b == 0)
    {
        c = 2;
    }
    else if (a == 0 && b == 1)
    {
        c = 1;
    }
    else if (a == 0 && b == 0)
    {
        c = 0;
    }
}
void tr(int a, int& b, int& c)// 依据s盒中数据给出01值
{
    if (a == 0)
    {
        b = 0; c = 0;
    }
    else if (a == 1)
    {
        b = 0; c = 1;
    }
    else if (a == 2)
    {
        b = 1; c = 0;
    }
    else if (a == 3)
    {
        b = 1; c = 1;
    }
}
void fk(int ip[], int k[], int f[])//轮函数fk
{
    //int left[4];
    int right[4];//取出右半数组
    //for (int i = 0; i < 4; i++)
    //{
        //left[i] = ip[i];
   // }
    for (int i = 4; i < 8; i++)
    {
        right[i - 4] = ip[i];
    }
    int right_e[8];
    rightShift(right);
    for (int i = 0; i < 4; i++)
    {
        right_e[i] = right[i];
    }
    leftShift(right);
    leftShift(right);
    for (int i = 4; i < 8; i++)
    {
        right_e[i] = right[i - 4];
    }//拓展置换
    //相或
    for (int i = 0; i < 8; i++)
    {
        right_e[i] = bitOr(right_e[i], k[i]);
    }
    int s1[4][4] = { {1,0,3,2},
                     {3,2,1,0},
                     {0,2,1,3},
                     {3,1,0,2} };
    int s2[4][4] = { {0,1,2,3},
                     {2,3,1,0},
                     {3,0,1,2},
                     {2,1,0,3} };//定义s盒1和2
    int x, y;
    x_y(right_e[0], right_e[3], x);
    x_y(right_e[1], right_e[2], y);
    //cout << right_e[0] <<"   "<< right_e[3] << endl;
    //cout << 10 << endl;
    int S[4];//一个用来存储s盒改变完后的数组 
    tr(s1[x][y], a, b);
    S[0] = a;
    S[1] = b;
    x_y(right_e[4], right_e[7], x);
    x_y(right_e[5], right_e[6], y);
    tr(s2[x][y], a, b);
    S[2] = a;
    S[3] = b;
    int sp[4] = { 2,4,3,1 };
    //最终出现的轮函数F
    for (int i = 0; i < 4; i++)
    {
        f[i] = S[sp[i] - 1];
    }

}
void rearr(int f[], int ip[])//pi左半与F进行或操作
{
    for (int i = 0; i < 4; i++)
    {
        ip[i] = bitOr(f[i], ip[i]);
    }
}
void exchange(int ip[])//数组交换左右部分
{
    for (int i = 0; i < 4; i++) {
        int temp = ip[i]; // 保存左半部分的元素
        ip[i] = ip[i + 4]; // 将右半部分的元素复制到左半部分
        ip[i + 4] = temp; // 将左半部分的元素复制到右半部分
    }
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED) {
            if ((HWND)lParam == hWndButton) {
                char plaintext[9];
                GetWindowTextA(hWndEditInputPlaintext, plaintext, sizeof(plaintext));

                char key[11];
                GetWindowTextA(hWndEditInputKey, key, sizeof(key));

                // 将明文和密钥转换为二进制数组

                int p[8];
                for (int i = 0; i < 8; i++) {
                    p[i] = plaintext[i] - '0';
                }

                bitset<10>  k;
                for (int i = 0; i < 10; i++) {
                    k[i] = key[P10[i] - 1] - '0';
                }
                //cout << k[1] << endl;
                bitset<10> initial_key(k);
                bitset<5> left_half = (initial_key >> 5).to_ulong();
                bitset<5> right_half = (initial_key & bitset<10>("0000011111")).to_ulong();

                left_half = leftCircularShift(left_half, left_shift1[0]);
                right_half = leftCircularShift(right_half, left_shift1[1]);
                bitset<10> key1 = (left_half.to_ulong() << 5) | right_half.to_ulong();
                bitset<8> subkey1 = performP8(key1);

                left_half = leftCircularShift(left_half, left_shift2[0]);
                right_half = leftCircularShift(right_half, left_shift2[1]);
                bitset<10> key2 = (left_half.to_ulong() << 5) | right_half.to_ulong();
                bitset<8> subkey2 = performP8(key2);

                int k1[8];
                for (int i = 0; i < 8; i++) {
                    k1[i] = subkey1[i];
                }

                int k2[8];
                for (int i = 0; i < 8; i++) {
                    k2[i] = subkey2[i];
                }

                // 执行 S-DES 算法加密过程
                int ip[8];//中间存储数组
                replace_start(p, ip);
                int f[4];//F
                fk(ip, k1, f);
                rearr(f, ip);
                exchange(ip);
                fk(ip, k2, f);
                rearr(f, ip);
                int c[8];//最终密文
                replace_end(ip, c);
                // 将密文转换为字符串
                char ciphertext[9];
                for (int i = 0; i < 8; i++) {
                    ciphertext[i] = c[i] + '0';
                }
                ciphertext[8] = '\0';

                // 将密文输出到控件
                SetWindowTextA(hWndEditOutputCiphertext, ciphertext);
            }
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"SDESWindowClass";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(0, CLASS_NAME, L"S-DES", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, nullptr, nullptr, hInstance, nullptr);
    if (hWnd == nullptr) {
        return 0;
    }

    // 创建输入明文的编辑框
    hWndEditInputPlaintext = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 10, 10, 200, 30, hWnd, (HMENU)EDIT_INPUT_PLAINTEXT_ID, nullptr, nullptr);
    if (hWndEditInputPlaintext == nullptr) {
        return 0;
    }

    // 创建输入密钥的编辑框
    hWndEditInputKey = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 10, 50, 200, 30, hWnd, (HMENU)EDIT_INPUT_KEY_ID, nullptr, nullptr);
    if (hWndEditInputKey == nullptr) {
        return 0;
    }

    // 创建按钮
    hWndButton = CreateWindow(L"BUTTON", L"加密", WS_CHILD | WS_VISIBLE, 10, 100, 80, 30, hWnd, (HMENU)BUTTON_ID, nullptr, nullptr);
    if (hWndButton == nullptr) {
        return 0;
    }

    // 创建输出密文的编辑框
    hWndEditOutputCiphertext = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY | ES_AUTOHSCROLL, 10, 150, 200, 30, hWnd, (HMENU)EDIT_OUTPUT_CIPHERTEXT_ID, nullptr, nullptr);
    if (hWndEditOutputCiphertext == nullptr) {
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}