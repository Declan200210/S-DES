#include <iostream>
#include <bitset>
#include<string>
using namespace std;
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
int main()
{

    int p[8];
    string input_key_str;
    bitset<10> initial_key;
    string str1;
    cout << "输入明文:";
    cin >> str1;
    if (str1.length() == 1) {//输入一个字符转换8bit数据 
        char ch = str1[0];
        bitset<8> binary(ch);  // 将字符 ch 转换为 8 位二进制数
        for (int i = 0; i < 8; ++i) {
            p[i] = binary[i];   // 将二进制数按位存入数组
        }
        int start = 0;
        int end = 7;

        while (start < end) {
            // 交换数组元素
            int temp = p[start];
            p[start] = p[end];
            p[end] = temp;

            // 更新指针
            start++;
            end--;
        }
    }
    else {
        for (int i = 0; i < 8; i++) {
            p[i] = str1[i] - 48;
        }
    }
    //cout << p[1];
    cout << endl;
    // 循环提示用户输入直到输入为10位二进制数
    while (true) {
        cout << "请输入10位密钥（只能输入0或1）：";
        cin >> input_key_str;

        bool valid_input = true;

        // 检查输入位数是否为10，并验证每个字符是0或1
        if (input_key_str.length() == 10) {
            for (char c : input_key_str) {
                if (c != '0' && c != '1') {
                    valid_input = false;
                    break;
                }
            }
            if (valid_input) {
                string initial_key1 = string(input_key_str.rbegin(), input_key_str.rend());
                initial_key = bitset<10>(initial_key1);
                break;
            }
        }

        cout << "密钥必须是10位二进制数，请重新输入。\n";
    }

    // 对初始密钥进行P10置换
    bitset<10> key_p10;
    for (int i = 0; i < 10; i++) {
        key_p10[i] = initial_key[P10[i] - 1];
    }

    // 将P10置换后的密钥分为左右各5位的两个部分
    bitset<5> left_half = (key_p10 >> 5).to_ulong();
    bitset<5> right_half = (key_p10 & bitset<10>("0000011111")).to_ulong();

    // 生成子密钥k1（左移1位）
    left_half = leftCircularShift(left_half, left_shift1[0]);
    right_half = leftCircularShift(right_half, left_shift1[1]);
    bitset<10> key1 = (left_half.to_ulong() << 5) | right_half.to_ulong();
    bitset<8> subkey1 = performP8(key1);

    // 生成子密钥k2（左移2位）
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

    cout << "子密钥k1: ";
    for (int i = 0; i < 8; i++) {
        cout << k1[i];
    }
    cout << endl;
    cout << "子密钥k2: ";
    for (int i = 0; i < 8; i++) {
        cout << k2[i];
    }
    cout << endl;

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
    if (str1.length() == 1)
    {
        //bitset<8> binary(0);  // 创建一个初始值为0的8位二进制数

        //for (int i = 0; i < 8; ++i) {
           // binary[i] = c[i];   // 将数组中的二进制数据按位存入二进制数中
        //}
        string binaryStr;
        for (int i = 0; i < 8; i++) {
            binaryStr += to_string(c[i]);
        }

        for (int i = 0; i < 8; i++)
        {
            cout << c[i];
        }
        cout << endl;
        //char ch = static_cast<char>(binary.to_ulong());  // 将二进制数转换为字符
        int num = stoi(binaryStr, nullptr, 2);

        // 将整数转换为对应的字符
        char ch = static_cast<char>(num);
        cout << "密文字符为: " << ch << endl;
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            cout << c[i];
        }
    }

}