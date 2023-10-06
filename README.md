# S-DES
## 作业报告
#### 1.基本测试
在本关卡中，我们一共进行了三组测试，明文加密后得到密文，密文解密后又得到了相同的明文，该关卡测试通过，以下是测试截图。

第一组加密：

![image](第一组加密.png)

第一组解密：

![image](第一组解密.png)

第二组加密：

![image](第二组加密.png)

第二组解密：

![image](第二组解密.png)

第三组加密：

![image](第三组加密.png)

第三组解密：

![image](第三组解密.png)

#### 2.交叉测试
在本关卡中，本组与对照组都采用相同的明文和密钥来进行加密操作，观察生成的密文是否相同，效果如下。

本组加密：

![image](交叉测试1.png)

对照组加密：

![image](交叉测试2.png)

如上图所示，结果有些许差异。

#### 3.扩展功能
在本关卡中，我们输入一个ASCⅡ码字符，然后对其进行加密，将得到的密文再进行解密，得到了输入的ASCⅡ码，本关卡成功，效果如下图。

加密：

![image](ASC加密.png)

解密：

![image](ASC解密.png)

#### 4.暴力破解
我们通过输入三组明密文对，来进行暴力破解，该关卡成功通关，以下是效果图：

![image](暴力破解.png)

#### 5.封闭测试
根据第四关的结果，我们不难发现，对于随机选择的明密文对，有不止一个key。

因此，我们又通过对一个选定的明文01010001，我们用可用的所有10bit密钥（共1024个）对其进行加密，并统计所出现的重复密文以及重复次数，结果如下图：

![image](封闭测试.png)

## 开发手册
#### 1.密钥扩展置设计
```
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
```

在主函数中调用：
```
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
```

通过以上方式获得子密钥k1、k2

#### 2.初始置换盒、最终置换盒、轮函数设计
```
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
```

在主函数中调用：
```
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
```

通过以上方式，将输入的明文，结合之前得到子密钥k1、k2，即可获得密文ciphertext。

采用以上设计，将k1与k2的使用位置对换（即将fk(ip, k1, f)与fk(ip, k2, f)这两行代码的位置对换），即可完成密文解密为明文。

#### 3.扩展功能实现

#### 4.暴力破解实现：
输入明密文对：
```
string plaintexts[3];  // 存储明文
string ciphertexts[3]; // 存储密文

for (int i = 0; i < 3; i++) {
    cout << "请输入第 " << i + 1 << " 对明文 (8位二进制)：";
    cin >> plaintexts[i];
    cout << "请输入对应的密文 (8位二进制)：";
    cin >> ciphertexts[i];
}
```

设置时间戳记录破解时间：
```
// 记录开始时间
auto start_time = high_resolution_clock::now();

// 记录结束时间
auto end_time = high_resolution_clock::now();

// 计算耗时（以毫秒为单位）
auto duration = duration_cast<milliseconds>(end_time - start_time);
```

尝试所有可能的key，如果加密后的密文与输入的密文匹配，将其输出：
```
for (int key = 0; key < 1024; key++)

// 逐一比较密文和c1
bool match = true;
for (int j = 0; j < 8; j++) {
    if (c[j] != c1[j]) {
        match = false;
        break;
    }
}

// 如果匹配，打印密钥
if (match) {
    std::bitset<10> binary(key);
    cout << "匹配的密钥: ";
    for (int j = 0; j < 10; j++) {
        cout << binary[j];
    }
    cout << endl;
}
```

#### 5.封闭测试实现：
