#include "test_mddz.h"
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include "mddz.h"
#include "ddz_desk.h"
#include "mddzc.h"
#include "redis/myredis_pool.h"
#include "pptr_buffer.h"
#include "ddz_ai_strategies.h"
using namespace std;

#pragma warning(disable:4996)

/*
//测试对子
void test1()
{
    char cards[20]={0}; ddz_lz_item out={0};
    
    cards[0] = 0x4e;
    cards[1] = 0x13;

    char t = is_pair(cards, 2, 3, &out);

    cout << (int)t << endl;
}

//测试三条
void test2()
{
    char cards[20]={0}; ddz_lz_item out={0};

    cards[0] = 0x36;
    cards[1] = 0x26;
    cards[2] = 0x13;

}

//测试单顺
void test3()
{
    char cards[20]={0}; ddz_lz_item out={0};

    cards[0] = 0x09;
    cards[1] = 0x07;
    cards[2] = 0x08;
    cards[3] = 0x16;
    cards[4] = 0x05;

    
    char t = is_straight(cards, 5, 0, &out);

    cout << (int)t << endl;
}

//测试双顺
void test4()
{
    char cards[20]={0}; ddz_lz_item out={0};

    cards[0] = 0x13;
    cards[1] = 0x23;
    cards[2] = 0x33;
    cards[3] = 0x03;
    cards[4] = 0x05;
    cards[5] = 0x15;
    cards[6] = 0x08;
    cards[7] = 0x18;


    char t = is_two_straight(cards, 8, 3, &out);

    cout << (int)t << endl;
}

//测试三顺 测试不过
void test5()
{
    char cards[20]={0}; ddz_lz_item out={0};

    //cards[0] = 0x13;
    //cards[1] = 0x23;
    //cards[2] = 0x33;
    //cards[3] = 0x03;
    //cards[4] = 0x06;
    //cards[5] = 0x16;
    //cards[6] = 0x08;
    //cards[7] = 0x18;
    //cards[8] = 0x28;

    cards[0] = 0x16;
    cards[1] = 0x26;
    cards[2] = 0x36;
    cards[3] = 0x07;
    cards[4] = 0x17;
    cards[5] = 0x27;
    cards[6] = 0x08;
    cards[7] = 0x18;
    cards[8] = 0x28;

    char t = is_three_straight(cards, 9, 3, &out);

    cout << (int)t << endl;
}

//测试三带一
void test6()
{
    char cards[20]={0}; ddz_lz_item out={0};

    cards[0] = 0x13;
    cards[1] = 0x23;
    cards[2] = 0x35;
    cards[3] = 0x02;



    char t = is_three_with_one(cards, 4, 3, &out);

    cout << (int)t << endl;
    
}

//测试三带一对
void test7()
{
    char cards[20]={0}; ddz_lz_item out={0};

    cards[0] = 0x13;
    cards[1] = 0x23;
    cards[2] = 0x33;
    cards[3] = 0x02;
    cards[4] = 0x12;



    char t = is_three_with_pair(cards, 5, 0, &out);

    cout << (int)t << endl;
    
}

//测试飞机带翅膀
void test8()
{
    char cards[20]={0}; ddz_lz_item out={0};

    cards[0] = 0x13;
    cards[1] = 0x23;
    cards[2] = 0x03;
    cards[3] = 0x33;
    cards[4] = 0x4f;
    cards[5] = 0x01;
    cards[6] = 0x11;
    cards[7] = 0x21;


    char t = is_three_straight_with_n(cards, 8, 3, &out);

    cout << (int)t << endl;
    
}

//测试飞机带对子翅膀
void test9()
{
    char cards[20]={0}; ddz_lz_item out={0};

    cards[0] = 0x13;
    cards[1] = 0x23;
    cards[2] = 0x3d;
    cards[3] = 0x2b;
    cards[4] = 0x03;
    cards[5] = 0x01;
    cards[6] = 0x11;
    cards[7] = 0x21;
    cards[8] = 0x27;
    cards[9] = 0x1c;


    char t = is_three_straight_with_np(cards, 10, 3, &out);

    cout << (int)t << endl;

}

//测试四带2
void test10()
{
    char cards[20]={0}; ddz_lz_item out={0};

    cards[1] = 0x23;
    cards[2] = 0x33;
    cards[0] = 0x09;
    cards[3] = 0x02;
    cards[4] = 0x12;
    cards[5] = 0x22;



    char t = is_four_with_two(cards, 6, 3, &out);

    cout << (int)t << endl;

    
}

//测试四带2对
void test11()
{
    char cards[20]={0}; ddz_lz_item out={0};

    cards[1] = 0x23;
    cards[2] = 0x33;
    cards[0] = 0x03;
    cards[3] = 0x13;
    cards[4] = 0x12;
    cards[5] = 0x22;
    cards[6] = 0x19;
    cards[7] = 0x29;




    char t = is_four_with_twopair(cards, 8, 3, &out);

    cout << (int)t << endl;

}

//测试炸弹
void test12()
{
    char cards[20]={0}; ddz_lz_item out={0};

    cards[0] = 0x02;
    cards[1] = 0x23;
    cards[2] = 0x33;
    cards[3] = 0x32;




    char t = is_bomb(cards, 4, 3, &out);

    cout << (int)t << endl;

}

//测试火箭
void test13()
{
    char cards[20]={0};
    cards[1] = 0x3e;
    cards[0] = 0x3f;

    bool b = is_rocket(cards, 2);

    cout << b << endl;
}

//测试获取牌型表
void test14()
{
    //int ddz_get_cards_type_list(char* cards, int len, int lz, char maxcard[10], char outcard[10][20], int type[10], ddz_lz_item out[10]);
    char cards[20]={0}; ddz_lz_item out[10] = {0};
    char maxcard[10] = {0};
    char outcard[10][20] = {0};
    int  type[10] = {0};

    cards[0] = 0x07;
    cards[1] = 0x17;
    cards[2] = 0x18;
    cards[3] = 0x28;
    cards[4] = 0x19;
    cards[5] = 0x29;
    cards[6] = 0x13;
    cards[7] = 0x23;

    int size = ddz_get_cards_type_list(cards, 8, 3, maxcard, outcard, type, out);
    cout << size << endl;
}


//测试大小比较
void test15()
{
    //不测了, 没问题  :)
}

//测试最大单牌提示
void test16()
{
    char cards[20] = {0};
    char candidate[20] = {0};
    char ret[MAX_CANDIDATE][20] = {0};
    int  blen[MAX_CANDIDATE]={0};
    int  ot[MAX_CANDIDATE] = {0};
    int  lz = 3;
    ddz_lz_item lzout[MAX_CANDIDATE] = {0};

    cards[0] = 0x16;
    
    candidate[0] = 0x05;
    candidate[1] = 0x06;
    candidate[2] = 0x07;
    candidate[3] = 0x17;
    candidate[4] = 0x09;
    candidate[5] = 0x19;
    candidate[6] = 0x29;
    candidate[7] = 0x39;
    candidate[8] = 0x4e;
    candidate[9] = 0x4f;



    int count = ddz_get_bigger_compose(cards, 1, MDDZ_CT_ONE, candidate, 9, ret, blen, ot, lz, lzout);

    cout << count << endl;

}

//测试最大对子提示
void test17()
{
    char cards[20] = {0};
    char candidate[20] = {0};
    char ret[MAX_CANDIDATE][20] = {0};
    int  blen[MAX_CANDIDATE]={0};
    int  ot[MAX_CANDIDATE] = {0};
    int  lz = 3;
    ddz_lz_item lzout[MAX_CANDIDATE] = {0};

    cards[0] = 0x16;
    cards[1] = 0x06;

    candidate[0] = 0x05;
    candidate[1] = 0x06;
    candidate[2] = 0x07;
    candidate[3] = 0x17;
    candidate[4] = 0x09;
    candidate[5] = 0x19;
    candidate[6] = 0x29;
    candidate[7] = 0x39;
    candidate[8] = 0x4e;
    candidate[9] = 0x4f;

    candidate[10] = 0x0a;
    candidate[11] = 0x13;



    int count = ddz_get_bigger_compose(cards, 2, MDDZ_CT_PAIR, candidate, 12, ret, blen, ot, lz, lzout);

    cout << count << endl;
}

//测试最大三条提示
void test18()
{
    char cards[20] = {0};
    char candidate[20] = {0};
    char ret[MAX_CANDIDATE][20] = {0};
    int  blen[MAX_CANDIDATE]={0};
    int  ot[MAX_CANDIDATE] = {0};
    int  lz = 3;
    ddz_lz_item lzout[MAX_CANDIDATE] = {0};

    cards[0] = 0x16;
    cards[1] = 0x06;
    cards[2] = 0x26;

    candidate[0] = 0x05;
    candidate[1] = 0x06;
    candidate[2] = 0x07;
    candidate[3] = 0x17;
    candidate[4] = 0x09;
    candidate[5] = 0x19;
    candidate[6] = 0x29;
    candidate[7] = 0x39;
    candidate[8] = 0x4e;
    candidate[9] = 0x4f;

    candidate[10] = 0x0a;
    candidate[11] = 0x13;
    candidate[12] = 0x03;



    int count = ddz_get_bigger_compose(cards, 3, MDDZ_CT_THREE, candidate, 13, ret, blen, ot, lz, lzout);

    cout << count << endl;
}


//三代一提示
void test22()
{
    char cards[20] = {0};
    char candidate[20] = {0};
    char ret[MAX_CANDIDATE][20] = {0};
    int  blen[MAX_CANDIDATE]={0};
    int  ot[MAX_CANDIDATE] = {0};
    int  lz = 3;
    ddz_lz_item lzout[MAX_CANDIDATE] = {0};

    cards[0] = 0x16;
    cards[1] = 0x06;
    cards[2] = 0x26;
    cards[3] = 0x0d;

    candidate[0] = 0x05;
    candidate[1] = 0x06;
    candidate[2] = 0x07;
    candidate[3] = 0x17;
    candidate[4] = 0x09;
    candidate[5] = 0x19;
    candidate[6] = 0x29;
    candidate[7] = 0x39;
    candidate[8] = 0x4e;
    candidate[9] = 0x4f;

    candidate[10] = 0x0a;
    candidate[11] = 0x13;
    candidate[12] = 0x03;

    int count = ddz_get_bigger_compose(cards, 4, MDDZ_CT_THREE_WITH_ONE, candidate, 13, ret, blen, ot, lz, lzout);

    cout << count << endl;
}

//三代对提示
void test23()
{
    char cards[20] = {0};
    char candidate[20] = {0};
    char ret[MAX_CANDIDATE][20] = {0};
    int  blen[MAX_CANDIDATE]={0};
    int  ot[MAX_CANDIDATE] = {0};
    int  lz = 3;
    ddz_lz_item lzout[MAX_CANDIDATE] = {0};

    cards[0] = 0x16;
    cards[1] = 0x06;
    cards[2] = 0x26;
    cards[3] = 0x0d;
    cards[4] = 0x1d;

    candidate[0] = 0x05;
    candidate[1] = 0x06;
    candidate[2] = 0x07;
    candidate[3] = 0x17;
    candidate[4] = 0x07;
    candidate[5] = 0x19;
    candidate[6] = 0x29;
    candidate[7] = 0x39;
    candidate[8] = 0x4e;
    candidate[9] = 0x4f;
    candidate[10]= 0x0a;
    candidate[11]= 0x13;
    candidate[12]= 0x03;
    candidate[13]= 0x23;

    int count = ddz_get_bigger_compose(cards, 5, MDDZ_CT_THREE_WITH_PAIR, candidate, 13, ret, blen, ot, lz, lzout);

    cout << count << endl;
}

//测试提示四带2
void test24()
{
    char cards[20] = {0};
    char candidate[20] = {0};
    char ret[MAX_CANDIDATE][20] = {0};
    int  blen[MAX_CANDIDATE]={0};
    int  ot[MAX_CANDIDATE] = {0};
    int  lz = 3;
    ddz_lz_item lzout[MAX_CANDIDATE] = {0};

    cards[0] = 0x16;
    cards[1] = 0x06;
    cards[2] = 0x26;
    cards[3] = 0x0d;
    cards[4] = 0x1d;
    cards[5] = 0x36;

    candidate[0] = 0x05;
    candidate[1] = 0x06;
    candidate[2] = 0x07;
    candidate[3] = 0x17;
    candidate[4] = 0x07;
    candidate[5] = 0x19;
    candidate[6] = 0x29;
    candidate[7] = 0x39;
    candidate[8] = 0x4e;
    candidate[9] = 0x4f;
    candidate[10]= 0x0a;
    candidate[11]= 0x13;
    candidate[12]= 0x03;
    candidate[13]= 0x23;

    int count = ddz_get_bigger_compose(cards, 6, MDDZ_CT_FOUR_WITH_TWO, candidate, 13, ret, blen, ot, lz, lzout);

    cout << count << endl;
}

//提示四带2对
void test25()
{
    char cards[20] = {0};
    char candidate[20] = {0};
    char ret[MAX_CANDIDATE][20] = {0};
    int  blen[MAX_CANDIDATE]={0};
    int  ot[MAX_CANDIDATE] = {0};
    int  lz = 3;
    ddz_lz_item lzout[MAX_CANDIDATE] = {0};

    cards[0] = 0x16;
    cards[1] = 0x06;
    cards[2] = 0x26;
    cards[3] = 0x0d;
    cards[4] = 0x1d;
    cards[5] = 0x36;
    cards[6] = 0x11;
    cards[7] = 0x21;

    candidate[0] = 0x05;
    candidate[1] = 0x06;
    candidate[2] = 0x07;
    candidate[3] = 0x17;
    candidate[4] = 0x07;
    candidate[5] = 0x19;
    candidate[6] = 0x29;
    candidate[7] = 0x39;
    candidate[8] = 0x4e;
    candidate[9] = 0x4f;
    candidate[10]= 0x0a;
    candidate[11]= 0x13;
    candidate[12]= 0x03;
    candidate[13]= 0x23;

    int count = ddz_get_bigger_compose(cards, 8, MDDZ_CT_FOUR_WITH_TWOPAIR, candidate, 13, ret, blen, ot, lz, lzout);

    cout << count << endl;
}

//测试提示炸弹
void test26()
{
    char cards[20] = {0};
    char candidate[20] = {0};
    char ret[MAX_CANDIDATE][20] = {0};
    int  blen[MAX_CANDIDATE]={0};
    int  ot[MAX_CANDIDATE] = {0};
    int  lz = 3;
    ddz_lz_item lzout[MAX_CANDIDATE] = {0};

    cards[0] = 0x16;
    cards[1] = 0x06;
    cards[2] = 0x26;
    cards[3] = 0x26;
    cards[4] = 0x1d;
    cards[5] = 0x36;
    cards[6] = 0x11;
    cards[7] = 0x21;

    candidate[0] = 0x05;
    candidate[1] = 0x06;
    candidate[2] = 0x07;
    candidate[3] = 0x17;
    candidate[4] = 0x07;
    candidate[5] = 0x27;
    candidate[6] = 0x29;
    candidate[7] = 0x39;
    candidate[8] = 0x4e;
    candidate[9] = 0x4f;
    candidate[10]= 0x0a;
    candidate[11]= 0x13;
    candidate[12]= 0x03;
    candidate[13]= 0x23;

    int count = ddz_get_bigger_compose(cards, 4, MDDZ_CT_BOMB, candidate, 14, ret, blen, ot, lz, lzout);

    cout << count << endl;
}
//顺子提示
void test19()
{
    char cards[20] = {0};
    char candidate[20] = {0};
    char ret[MAX_CANDIDATE][20] = {0};
    int  blen[MAX_CANDIDATE]={0};
    int  ot[MAX_CANDIDATE] = {0};
    int  lz = 3;
    ddz_lz_item lzout[MAX_CANDIDATE] = {0};

    cards[0] = 0x04;
    cards[1] = 0x05;
    cards[2] = 0x06;
    cards[3] = 0x07;
    cards[4] = 0x08;
    cards[5] = 0x09;
    cards[6] = 0x0;
    cards[7] = 0x0;

    candidate[0] = 0x16;
    candidate[1] = 0x16;
    candidate[2] = 0x16;
    candidate[3] = 0x17;
    candidate[4] = 0x18;
    candidate[5] = 0x19;
    candidate[6] = 0x1a;
    candidate[7] = 0x1b;
    candidate[8] = 0x2b;
    candidate[9] = 0x21;
    candidate[10]= 0x11;
    candidate[11]= 0x13;
    candidate[12]= 0x03;
    candidate[13]= 0x23;
    candidate[14]= 0x33;

    int count = ddz_get_bigger_compose(cards, 6, MDDZ_CT_STRAIGHT, candidate, 13, ret, blen, ot, lz, lzout);

    cout << count << endl;
}
//双顺提示
void test20()
{
    char cards[20] = {0};
    char candidate[20] = {0};
    char ret[MAX_CANDIDATE][20] = {0};
    int  blen[MAX_CANDIDATE]={0};
    int  ot[MAX_CANDIDATE] = {0};
    int  lz = 3;
    ddz_lz_item lzout[MAX_CANDIDATE] = {0};

    cards[0] = 0x04;
    cards[1] = 0x14;
    cards[2] = 0x05;
    cards[3] = 0x15;
    cards[4] = 0x06;
    cards[5] = 0x16;
    cards[6] = 0x0;
    cards[7] = 0x0;

    candidate[0] = 0x16;
    candidate[1] = 0x2a;
    candidate[2] = 0x1a;
    candidate[3] = 0x2b;
    candidate[4] = 0x1c;
    candidate[5] = 0x2c;
    candidate[6] = 0x1d;
    candidate[7] = 0x2d;
    candidate[8] = 0x11;
    candidate[9] = 0x21;
    candidate[10]= 0x11;
    candidate[11]= 0x13;
    candidate[12]= 0x03;
    candidate[13]= 0x23;
    candidate[14]= 0x33;

    int count = ddz_get_bigger_compose(cards, 6, MDDZ_CT_TWO_STRAIGHT, candidate, 15, ret, blen, ot, lz, lzout);

    cout << count << endl;
}

//三顺提示
void test21()
{
    char cards[20] = {0};
    char candidate[20] = {0};
    char ret[MAX_CANDIDATE][20] = {0};
    int  blen[MAX_CANDIDATE]={0};
    int  ot[MAX_CANDIDATE] = {0};
    int  lz = 3;
    ddz_lz_item lzout[MAX_CANDIDATE] = {0};

    cards[0] = 0x04;
    cards[1] = 0x14;
    cards[2] = 0x04;
    cards[3] = 0x15;
    cards[4] = 0x05;
    cards[5] = 0x15;
    cards[6] = 0x0;
    cards[7] = 0x0;

    candidate[0] = 0x1a;
    candidate[1] = 0x2a;
    candidate[2] = 0x1a;
    candidate[3] = 0x2b;
    candidate[4] = 0x1b;
    candidate[5] = 0x2b;
    candidate[6] = 0x1d;
    candidate[7] = 0x2d;
    candidate[8] = 0x11;
    candidate[9] = 0x21;
    candidate[10]= 0x11;
    candidate[11]= 0x13;
    candidate[12]= 0x03;
    candidate[13]= 0x23;
    candidate[14]= 0x33;

    int count = ddz_get_bigger_compose(cards, 6, MDDZ_CT_THREE_STRAIGHT, candidate, 15, ret, blen, ot, lz, lzout);

    cout << count << endl;
}

//飞机带单翅膀提示
void test27()
{
    char cards[20] = {0};
    char candidate[20] = {0};
    char ret[MAX_CANDIDATE][20] = {0};
    int  blen[MAX_CANDIDATE]={0};
    int  ot[MAX_CANDIDATE] = {0};
    int  lz = 3;
    ddz_lz_item lzout[MAX_CANDIDATE] = {0};

    cards[0] = 0x15;
    cards[1] = 0x25;
    cards[2] = 0x05;
    cards[3] = 0x16;
    cards[4] = 0x26;
    cards[5] = 0x06;
    cards[6] = 0x14;
    cards[7] = 0x24;


    candidate[0] = 0x1a;
    candidate[1] = 0x2a;
    candidate[2] = 0x1a;
    candidate[3] = 0x2b;
    candidate[4] = 0x1b;
    candidate[5] = 0x2b;
    candidate[6] = 0x1d;
    candidate[7] = 0x2d;
    candidate[8] = 0x11;
    candidate[9] = 0x21;
    candidate[10]= 0x11;
    candidate[11]= 0x13;
    candidate[12]= 0x03;
    candidate[13]= 0x23;
    candidate[14]= 0x33;

    int count = ddz_get_bigger_compose(cards, 8, MDDZ_CT_THREE_STRAIGHT_WITH_N, candidate, 15, ret, blen, ot, lz, lzout);

    cout << count << endl;

}

//飞机带对子翅膀提示
void test28()
{
    char cards[20] = {0};
    char candidate[20] = {0};
    char ret[MAX_CANDIDATE][20] = {0};
    int  blen[MAX_CANDIDATE]={0};
    int  ot[MAX_CANDIDATE] = {0};
    int  lz = 3;
    ddz_lz_item lzout[MAX_CANDIDATE] = {0};

    cards[0] = 0x15;
    cards[1] = 0x25;
    cards[2] = 0x05;
    cards[3] = 0x16;
    cards[4] = 0x26;
    cards[5] = 0x06;
    cards[6] = 0x14;
    cards[7] = 0x24;
    cards[8] = 0x07;
    cards[9] = 0x17;


    candidate[0] = 0x1a;
    candidate[1] = 0x2a;
    candidate[2] = 0x1a;
    candidate[3] = 0x2b;
    candidate[4] = 0x1b;
    candidate[5] = 0x2b;
    candidate[6] = 0x1d;
    candidate[7] = 0x2d;
    candidate[8] = 0x11;
    candidate[9] = 0x21;
    candidate[10]= 0x11;
    candidate[11]= 0x13;
    candidate[12]= 0x03;
    candidate[13]= 0x23;
    candidate[14]= 0x33;

    int count = ddz_get_bigger_compose(cards, 10, MDDZ_CT_THREE_STRAIGHT_WITH_NP, candidate, 15, ret, blen, ot, lz, lzout);

    cout << count << endl;

}

void test_bug1()
{
    char cards[20] = {0};
    char maxcard[10];
    char outcard[10][20];
    int type[10];
    ddz_lz_item out[10];
    cards[0] = 0x03;
    cards[1] = 0x13;
    cards[2] = 0x23;
    cards[3] = 0x04;
    cards[4] = 0x14;
    cards[5] = 0x24;
    cards[6] = 0x05;
    cards[7] = 0x15;
    cards[8] = 0x25;
    int n = ddz_get_cards_type_list(cards, 9, 0, maxcard, outcard, type, out);

    cout << n << endl;
}

void test_bug2()
{
    char cards1[20];
    char cards2[20];
    cards1[0] = 0x04;
    cards1[1] = 0x14;
    cards2[0] = 0x18;
    cards2[1] = 0x28;

    bool b = ddz_compare(cards1, 2, 2, cards2, 2, 2, 3);

    cout << b << endl;
}

void test_bug3()
{
    //                
    char cards[20] = { 0x31,0x21,0x01,0x38,0x26,0x35,0x25,0x15,0x14,0x04,0x33,0x2c,0x2b,0x2a};
    sort_cards(cards, 14, 0);

    int c;
    c = 0;
}

void test_bug4()
{
    char cards[20] = {0x13,0x14,0x15,0x16,0x17,0x18};
    char card2[20] = {0x25,0x26,0x28,0x29,0x2a,0x21};
    bool b = ddz_compare(cards, 6, MDDZ_CT_STRAIGHT, card2, 6, MDDZ_CT_STRAIGHT, 1);

    cout << b << endl;
}

void test_bug5()
{
    char cards[20] = {0x14,0x15,0x16,0x17,0x18,0x19,0x11};
    char card2[20] = {0x25,0x26,0x28,0x29,0x2a,0x2b,0x21};
    bool b = ddz_compare(cards, 7, MDDZ_CT_STRAIGHT, card2, 7, MDDZ_CT_STRAIGHT, 1);

    cout << b << endl;
}

void test_bug6()
{
    char cards[20] = {0x22};
    char c2[20] = {0x4e};
    bool b = ddz_compare(cards, 1, MDDZ_CT_ONE, c2, 1, MDDZ_CT_ONE, 9);

    cout << b << endl;
}

void test_bug7()
{
    char cards[20] = {0x45, 0x03, 0x13, 0x33};
    cards_buf src;
    cards_buf buf[MAX_CANDIDATE];
    memset(&src, 0, sizeof(src));
    memset(&buf, 0, sizeof(buf));

    src.size = 4;
    memcpy(src.cards, cards, 4);
    int n = ddz_get_cards_type_list(src, 5, buf);

    cout << n << endl;
}

void test_bug8()
{
    char cards[20] = {0x11, 0x1a, 0x1b, 0x1c, 0x1d};
    cards_buf src;
    cards_buf buf[MAX_CANDIDATE];
    memset(&src, 0, sizeof(src));
    memset(&buf, 0, sizeof(buf));

    src.size = 5;
    memcpy(src.cards, cards, 5);
    int n = ddz_get_cards_type_list(src, 1, buf);

    cout << n << endl;
}
//int  ddz_get_bigger_compose(cards_buf& src, cards_buf& candidate, int lz, cards_buf out[MAX_CANDIDATE]);

void test_bug9()
{
    cards_buf src;
    cards_buf candidate;
    int lz = 3;

    cards_buf out[MAX_CANDIDATE];
    src.cards[0] = 0x18;
    src.size = 1;
    src.card_type = MDDZ_CT_ONE;

    candidate.cards[0] = 0x13;
    candidate.size = 1;

    int n = ddz_get_bigger_compose(src, candidate, lz, out);
    cout << n << endl;

}

void test_bug10()
{
    cards_buf src;
    cards_buf candidate;
    int lz = 3;

    cards_buf out[MAX_CANDIDATE];
    src.cards[0] = 0x18;
    src.cards[1] = 0x58;
    src.cards[2] = 0x58;
    src.cards[3] = 0x58;
    src.size = 4;
    src.card_type = MDDZ_CT_BOMB;

    candidate.cards[0] = 0x13;
    candidate.cards[1] = 0x19;
    candidate.cards[2] = 0x29;
    candidate.cards[3] = 0x39;
    candidate.cards[4] = 0x15;
    candidate.cards[5] = 0x25;
    candidate.cards[6] = 0x37;
    candidate.size = 7;



    int n = ddz_get_bigger_compose(src, candidate, lz, out);
    cout << n << endl;

}
//int  ddz_get_cards_type_list(cards_buf& src, int lz, cards_buf buf[MAX_CANDIDATE]);

void test_bug11()
{
    char select_cards[20] ={0x03, 0x13, 0x04, 0x24,  0x19};
    //char select_cards[20] ={0x03, 0x13, 0x04, 0x24, 0x15,0x25,0x16, 0x17,0x27, 0x19};
    cards_buf src;
    int lz = 9;
    cards_buf buf[MAX_CANDIDATE];
    memcpy(src.cards, select_cards, 5);
    src.size = 5;
    int n = ddz_get_cards_type_list(src, lz, buf);
    cout << n << endl;
    
}

void test_bug12()
{
    char last_out_cards[20] ={0x15,0x25};
    int last_out_len = 2;
    char select_cards[20] ={0x09, 0x19, 0x29, 0x4f};
    int select_cards_len = 4;

    cards_buf src;
    cards_buf candidate;
    int lz = 3;

    cards_buf out[MAX_CANDIDATE];

    memcpy(src.cards, last_out_cards, 2);
    src.size = 2;
    memcpy(candidate.cards, select_cards, 4);

    int n = ddz_get_bigger_compose(src, candidate, 9, out);
}

void test_bug13()
{
    char select_cards[20] = {0x07, 0x17, 0x18, 0x28, 0x0d,0x1d};
    cards_buf src;
    int lz = 13;
    cards_buf buf[MAX_CANDIDATE];
    memcpy(src.cards, select_cards, 6);
    src.size = 6;
    int n = ddz_get_cards_type_list(src, lz, buf);
    cout << n << endl;

}

void test_bug14()
{
    char select_cards[20] ={0x07, 0x17, 0x18, 0x28, 0x19, 0x29, 0x39, 0x0a, 0x1a, 0x1d};
    int select_cards_len = 10;

    cards_buf src;
    int lz = 13;
    cards_buf buf[MAX_CANDIDATE];
    memcpy(src.cards, select_cards, select_cards_len);
    src.size = select_cards_len;
    int n = ddz_get_cards_type_list(src, lz, buf);
    cout << n << endl;

}

void test_bug15()
{
    char select_cards[20] ={0x06, 0x16, 0x15, 0x25, 0x14, 0x23, 0x0a, 0x1a};
    int select_cards_len = 8;

    cards_buf src;
    int lz = 10;
    cards_buf buf[MAX_CANDIDATE];
    memcpy(src.cards, select_cards, select_cards_len);
    src.size = select_cards_len;
    int n = ddz_get_cards_type_list(src, lz, buf);
    cout << n << endl;

}

void test_bug16()
{
//char select_cards[20] ={0x06, 0x0a, 0x1a, 0x2a,0x3a};
//int select_cards_len = 5;
//癞子10
//ddz_get_cards_type_list只返回一种牌型 顺子，3带2没有

    char select_cards[20] ={0x06, 0x0a, 0x1a, 0x2a,0x3a};
    int select_cards_len = 5;

    cards_buf src;
    int lz = 10;
    cards_buf buf[MAX_CANDIDATE];
    memcpy(src.cards, select_cards, select_cards_len);
    src.size = select_cards_len;
    int n = ddz_get_cards_type_list(src, lz, buf);
    cout << n << endl;

}

void test_bug17()
{
    //char select_cards[20] ={0x04,0x05, 0x05, 0x16, 0x26,0x07,0x37, 0x08, 0x18,0x19, 0x1d,0x2d};
    //int select_cards_len = 12;
    //癞子是13
    //这个 是双顺 ddz_get_cards_type_list返回0

    char select_cards[20] ={0x04,0x05, 0x05, 0x16, 0x26,0x07,0x37, 0x08, 0x18,0x19, 0x1d,0x2d};
    int select_cards_len = 12;

    cards_buf src;
    int lz = 13;
    cards_buf buf[MAX_CANDIDATE];
    memcpy(src.cards, select_cards, select_cards_len);
    src.size = select_cards_len;
    int n = ddz_get_cards_type_list(src, lz, buf);
    cout << n << endl;

}

void test_bug18()
{
    //char select_cards[20] ={0x04, 0x14, 0x24, 0x05, 0x15, 0x25, 0x0a, 0x0b};
    //int select_cards_len = 8;
    //癞子13
    //现在飞机带翅膀ddz_get_cards_type_list返回0
    //char select_cards[20] ={0x04, 0x14, 0x24, 0x05, 0x15, 0x25, 0x0a, 0x0b};
    char select_cards[20] ={0x04, 0x14, 0x2d, 0x05, 0x15, 0x25, 0x0a, 0x0b};
    int select_cards_len = 8;

    cards_buf src;
    int lz = 13;
    cards_buf buf[MAX_CANDIDATE];
    memcpy(src.cards, select_cards, select_cards_len);
    src.size = select_cards_len;
    int n = ddz_get_cards_type_list(src, lz, buf);
    cout << n << endl;
}

void test_bug19()
{
    //char select_cards[20] ={0x01,0x11,0x21, 0x1d, 0x2d, 0x0b,0x1b, 0x0a,0x1a, 0x09,0x19, 0x06,0x16,0x15,0x05};
    //int select_cards_len = 15;
    //癞子是1
    //ddz_get_cards_type_list应该有3顺带3对 现在返回0
    
    char select_cards[20] ={0x01,0x11,0x21, 0x1d, 0x2d, 0x0b,0x1b, 0x0a,0x1a, 0x09,0x19, 0x06,0x16,0x15,0x05};
    int select_cards_len = 15;

    cards_buf src;
    int lz = 1;
    cards_buf buf[MAX_CANDIDATE];
    memcpy(src.cards, select_cards, select_cards_len);
    src.size = select_cards_len;
    int n = ddz_get_cards_type_list(src, lz, buf);
    cout << n << endl;

}

void test_bug20()
{
    //char last_out_cards[20] ={0x13,0x14,0x15,0x16,0x17,0x18};
    //int last_out_len = 6;
    //char select_cards[20] ={0x01,0x11, 0x4f, 0x02,0x1d, 0x1c, 0x2c, 0x1a,0x09,0x19,0x29, 0x08, 0x07,0x05,0x15,0x04,0x03};
    //int select_cards_len = 17;
    //癞子是1
    //ddz_get_bigger_compose后传出的顺子第5、6种里面包含0x02与0x4f
    char last_out_cards[20] ={0x13,0x14,0x15,0x16,0x17,0x18};
    int last_out_len = 6;
    char select_cards[20] ={0x01,0x11, 0x4f, 0x02,0x1d, 0x1c, 0x2c, 0x1a,0x09,0x19,0x29, 0x08, 0x07,0x05,0x15,0x04,0x03};
    int select_cards_len = 17;

    cards_buf src;
    cards_buf candidate;
    int lz = 1;
    cards_buf out[MAX_CANDIDATE];
    memcpy(src.cards, last_out_cards, last_out_len);src.size = last_out_len;src.card_type=MDDZ_CT_STRAIGHT;
    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;

    int n = ddz_get_bigger_compose(src, candidate, lz, out);
    cout << n << endl;

}

void test_bug21()
{
    char last_out_cards[20] ={0x11,0x21,0x31,0x16,0x17,0x19};
    int last_out_len = 6;
    char select_cards[20] ={0x06, 0x26, 0x12, 0x22, 0x09, 0x29};
    int select_cards_len = 6;

    cards_buf src;
    cards_buf candidate;
    int lz = 9;
    cards_buf out[MAX_CANDIDATE];
    memcpy(src.cards, last_out_cards, last_out_len);src.size = last_out_len;src.card_type=MDDZ_CT_FOUR_WITH_TWO;
    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;

    int n = ddz_get_bigger_compose(src, candidate, lz, out);
    cout << n << endl;

    candidate.card_type = MDDZ_CT_FOUR_WITH_TWO;

    bool b = ddz_compare(src, candidate, lz);
    cout << b << endl;

}

void test_bug22()
{
    char last_out_cards[20] ={0x04,0x21,0x31,0x16,0x17,0x19};
    int last_out_len = 1;
    char select_cards[20] ={0x0a, 0x1a, 0x2a, 0x3a, 0x09, 0x29};
    int select_cards_len = 4;

    cards_buf src;
    cards_buf candidate;
    int lz = 10;
    cards_buf out[MAX_CANDIDATE];
    memcpy(src.cards, last_out_cards, last_out_len);src.size = last_out_len;src.card_type=MDDZ_CT_ONE;
    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;

    int n = ddz_get_bigger_compose(src, candidate, lz, out);
    cout << n << endl;

}

void test_bug23()
{
    char select_cards[20] ={0x02, 0x12, 0x01, 0x11, 0x0d, 0x1d};
    int select_cards_len = 6;
    int lz = 2;

    cards_buf src;
    cards_buf buf[MAX_CANDIDATE];
    memcpy(src.cards, select_cards, select_cards_len);
    src.size = select_cards_len;
    int n = ddz_get_cards_type_list(src, lz, buf);
    cout << n << endl;

}

void test_bug24()
{
    char last_out_cards[20] ={0x05,0x15,0x25, 0x04, 0x14};
    int last_out_len = 5;
    char select_cards[20] ={0x01, 0x19, 0x29, 0x1a, 0x0a};
    int select_cards_len = 5;
    int lz = 1;

    cards_buf src;
    cards_buf candidate;

    cards_buf out[MAX_CANDIDATE];
    memcpy(src.cards, last_out_cards, last_out_len);src.size = last_out_len;src.card_type=MDDZ_CT_THREE_WITH_PAIR;
    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;

    int n = ddz_get_bigger_compose(src, candidate, lz, out);
    cout << n << endl;


    n = ddz_get_cards_type_list(candidate, lz, out);
    cout << n << endl;
}

void test_bug25()
{
    char select_cards[20] ={0x05, 0x02, 0x12, 0x11, 0x01, 0x0d, 0x1d, 0x2d};
    int select_cards_len = 8;
    int lz = 5;
    cards_buf src;

    cards_buf out[MAX_CANDIDATE];
    memcpy(src.cards, select_cards, select_cards_len);
    src.size = select_cards_len;

    int n = ddz_get_cards_type_list(src, lz, out);
    cout << n << endl;
}

void test_bug26()
{
    char select_cards[20] ={0x0d, 0x1d, 0x2d, 0x01, 0x11, 0x0c, 0x1c, 0x2c};
    int select_cards_len = 8;
    int lz = 13;
    cards_buf src;

    cards_buf out[MAX_CANDIDATE];
    memcpy(src.cards, select_cards, select_cards_len);
    src.size = select_cards_len;

    int n = ddz_get_cards_type_list(src, lz, out);
    cout << n << endl;
}

void test_bug27()
{
    char select_cards[20] ={0x02, 0x12, 0x22, 0x01, 0x11, 0x21};
    int select_cards_len = 6;
    int lz = 3;
    cards_buf src;

    cards_buf out[MAX_CANDIDATE];
    memcpy(src.cards, select_cards, select_cards_len);
    src.size = select_cards_len;

    int n = ddz_get_cards_type_list(src, lz, out);
    cout << n << endl;
}

void test_bug28()
{
    char select_cards[20] ={0x4e, 0x13, 0x23, 0x03, 0x12, 0x21};
    int select_cards_len = 6;
    int lz = 3;
    cards_buf src;

    cards_buf out[MAX_CANDIDATE];
    memcpy(src.cards, select_cards, select_cards_len);
    src.size = select_cards_len;

    int n = ddz_get_cards_type_list(src, lz, out);
    cout << n << endl;
}

void test_bug29()
{
    char last_out_cards[20] ={0x05,0x15,0x25, 0x04, 0x14};
    int last_out_len = 2;
    char select_cards[20] ={0x0a, 0x1a, 0x29, 0x1a, 0x0a};
    int select_cards_len = 2;
    int lz = 10;

    cards_buf src;
    cards_buf candidate;

    cards_buf out[MAX_CANDIDATE];
    memcpy(src.cards, last_out_cards, last_out_len);src.size = last_out_len;src.card_type=MDDZ_CT_PAIR;
    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;

    int n = ddz_get_bigger_compose(src, candidate, lz, out);
    cout << n << endl;

}

void test_bug30()
{
    char last_out_cards[20] ={0x09, 0x19, 0x29, 0x0a, 0x1a, 0x2a, 0x08, 0x18, 0x07, 0x27};
    int last_out_len = 10;
    char select_cards[20] =  {0x4e, 0x01, 0x11, 0x21, 0x0d, 0x1d, 0x2d, 0x06, 0x16, 0x05};
    int select_cards_len = 10;
    int lz = 5;

    cards_buf src;
    cards_buf candidate;

    cards_buf out[MAX_CANDIDATE];
    memcpy(src.cards, last_out_cards, last_out_len);src.size = last_out_len;src.card_type=MDDZ_CT_THREE_STRAIGHT_WITH_NP;
    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;

    int n = ddz_get_bigger_compose(src, candidate, lz, out);
    cout << n << endl;

    n = ddz_get_cards_type_list(candidate, lz, out);

    cout << n << endl;
}

void test_bug31()
{
    char last_out_cards[20] ={0x09, 0x19, 0x29, 0x0a, 0x1a, 0x2a, 0x08, 0x18, 0x07, 0x27};
    int last_out_len = 10;
    char select_cards[20] =  {0x05, 0x15, 0x11, 0x21, 0x0d, 0x07, 0x17, 0x27};
    int select_cards_len = 8;
    int lz = 5;

    cards_buf src;
    cards_buf candidate;

    cards_buf out[MAX_CANDIDATE];
    memcpy(src.cards, last_out_cards, last_out_len);src.size = last_out_len;src.card_type=MDDZ_CT_THREE_STRAIGHT_WITH_NP;
    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;


    int n = ddz_get_cards_type_list(candidate, lz, out);

    cout << n << endl;
}


void test_bug32()
{
    char select_cards[20] =  {0x03, 0x13, 0x04, 0x14, 0x06, 0x0d, 0x1d, 0x2d};
    int select_cards_len = 8;
    int lz = 13;

    cards_buf src;
    cards_buf candidate;

    cards_buf out[MAX_CANDIDATE];

    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;


    int n = ddz_get_cards_type_list(candidate, lz, out);

    cout << n << endl;
}

void test_bug33()
{
    //杨晓观(杨晓观) 11:46:06
    //char last_out_cards[20] ={0x0a, 0x1a, 0x09, 0x19, 0x0b,0x1b};
    //int last_out_len = 6;
    //char select_cards[20] ={0x02, 0x11, 0x2d, 0x3d, 0x2c,0x3b, 0x2a,0x29, 0x39,0x08, 0x18,0x28,0x07,0x06, 0x04, 0x14, 0x13};
    //int select_cards_len = 17;
    //癞子牌12
    //ddz_get_bigger_compose返回的牌比上家小

    char last_out_cards[20] ={0x0a, 0x1a, 0x09, 0x19, 0x0b,0x1b};
    int last_out_len = 6;
    char select_cards[20] ={0x02, 0x11, 0x2d, 0x3d, 0x2c,0x3b, 0x2a,0x29, 0x39,0x08, 0x18,0x28,0x07,0x06, 0x04, 0x14, 0x13};
    int select_cards_len = 17;
    int lz = 12;

    cards_buf src;
    cards_buf candidate;

    cards_buf out[MAX_CANDIDATE];
    memcpy(src.cards, last_out_cards, last_out_len);src.size = last_out_len;src.card_type=MDDZ_CT_TWO_STRAIGHT;
    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;

    int n = ddz_get_bigger_compose(src, candidate, lz, out);
    cout << n << endl;

}

void test_bug34()
{
    char select_cards[20] =  {0x01, 0x1d, 0x0d, 0x19, 0x09, 0x29, 0x1d, 0x2d};
    int select_cards_len = 6;
    int lz = 9;

    cards_buf src;
    cards_buf candidate;

    cards_buf out[MAX_CANDIDATE];

    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;


    int n = ddz_get_cards_type_list(candidate, lz, out);

    cout << n << endl;
}

void test_bug35()
{
    char last_out_cards[20] ={0x06, 0x16, 0x05, 0x15, 0x04, 0x14, 0x03, 0x11};
    int last_out_len = 8;
    char select_cards[20] ={0x09, 0x19, 0x08, 0x18, 0x26, 0x36, 0x11, 0x01};
    int select_cards_len = 8;
    int lz = 1;

    cards_buf src;
    cards_buf candidate;

    cards_buf out[MAX_CANDIDATE];
    memcpy(src.cards, last_out_cards, last_out_len);src.size = last_out_len;src.card_type=MDDZ_CT_TWO_STRAIGHT;
    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;

    int n = ddz_get_bigger_compose(src, candidate, lz, out);
    cout << n << endl;

}

void test_s1()
{
    char select_cards[20] ={0x11, 0x01, 0x09, 0x19, 0x08, 0x18, 0x26, 0x36};
    int select_cards_len = 6;
    int lz = 1;

    cards_buf src;
    cards_buf candidate;

    cards_buf out[MAX_CANDIDATE];
    memset(&src, 0, sizeof(src));
    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;

    int n = ddz_get_bigger_compose(src, candidate, lz, out);
    cout << n << endl;
}

void test_bug36()
{
    char last_out_cards[20] ={0x1a, 0x0a, 0x08, 0x18, 0x29,0x09};
    int last_out_len = 6;
    char select_cards[20] ={0x03, 0x13, 0x1d, 0x0d,0x2c, 0x1c};
    int select_cards_len = 6;

    int lz = 3;

    cards_buf src;
    cards_buf candidate;

    cards_buf out[MAX_CANDIDATE];
    memcpy(src.cards, last_out_cards, last_out_len);src.size = last_out_len;src.card_type=MDDZ_CT_TWO_STRAIGHT;
    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;

    int n = ddz_get_bigger_compose(src, candidate, lz, out);
    cout << n << endl;

}

void test_bug37()
{
    char last_out_cards[20] ={0x38, 0x28, 0x18, 0x24};
    int last_out_len = 4;


    char select_cards[20] ={0x02, 0x15, 0x25, 0x35};
    int select_cards_len = 4;
    bool b = ddz_compare(last_out_cards, last_out_len, 7, select_cards, select_cards_len, 13, 2);

    cout << b << endl;
}

void test_bug38()
{
    char last_out_cards[20] ={0x3a, 0x1a, 0x0a, 0x39, 0x29, 0x19, 0x25, 0x05, 0x34, 0x24};
    int last_out_len = 10;
    char select_cards[20] ={0x02, 0x12, 0x21, 0x31, 0x0d, 0x2d, 0x0c, 0x2c, 0x0b, 0x2b};
    int select_cards_len = 10;
    bool b = ddz_compare(last_out_cards, last_out_len, 10, select_cards, select_cards_len, 10, 11);

    cout << b << endl;
}

void test_bug39()
{
    //char select_cards[20] =  {0x02,0x0c,0x1c,0x2c,0x0b,0x1b,0x2b,0x0a,0x1a,0x2a,0x3a,0x08};
    char select_cards[20] =  {0x02,0x0c,0x1c,0x2c,0x3c};
    int  select_cards_len = 5;
    int  lz = 2;

    cards_buf src;
    cards_buf candidate;

    cards_buf out[MAX_CANDIDATE];

    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;


    int n = ddz_get_cards_type_list(candidate, lz, out);

    cout << n << endl;

}

void test_bug40()
{
    char select_cards[20] ={0x17, 0x07, 0x16, 0x36, 0x15, 0x25, 0x04, 0x03, 0x13, 0x23};
    int select_cards_len = 10;

    cards_buf src;
    cards_buf candidate;

    cards_buf out[MAX_CANDIDATE];

    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;


    int n = ddz_get_cards_type_list(candidate, 7, out);

    cout << n << endl;

}

void test_bug41()
{
    char select_cards[20] ={0x12, 0x02, 0x4e, 0x4f};
    int select_cards_len = 4;

    cards_buf src;
    cards_buf candidate;

    cards_buf out[MAX_CANDIDATE];

    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;


    int n = ddz_get_cards_type_list(candidate, 2, out);

    cout << n << endl;

}

void test_bug42()
{
    char select_cards[20] ={0x04, 0x14, 0x24, 0x33, 0x05};
    int select_cards_len = 4;

    cards_buf src;
    cards_buf candidate;

    cards_buf out[MAX_CANDIDATE];

    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;


    int n = ddz_get_cards_type_list(candidate, 4, out);

    cout << n << endl;

}

void test_bug43()
{
    char select_cards[20] ={0x03,0x23, 0x2c, 0x0c};
    int select_cards_len = 4;
    cards_buf src;
    cards_buf candidate;
    int lz = 0x0c;

    cards_buf out[MAX_CANDIDATE];

    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;


    int n = ddz_get_cards_type_list(candidate, lz, out);

    cout << n << endl;

}

void test_bug44()
{
    char select_cards[20] ={0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x03, 0x13, 0x23};
    int select_cards_len = 8;
    cards_buf src;
    cards_buf candidate;
    int lz = 3;

    cards_buf out[MAX_CANDIDATE];

    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;


    int n = ddz_get_cards_type_list(candidate, lz, out);

    cout << n << endl;

}

void test_bug45()
{
    char select_cards[20] ={0x08, 0x18, 0x06, 0x05, 0x03};
    int select_cards_len = 5;
    cards_buf src;
    cards_buf candidate;
    int lz = 8;

    cards_buf out[MAX_CANDIDATE];

    memcpy(candidate.cards, select_cards, select_cards_len);
    candidate.size = select_cards_len;


    int n = ddz_get_cards_type_list(candidate, lz, out);


    cout << n << endl;
}

void test_bug46()
{
    char selectedCards[20] = {0x02, 0x12};
    char lastOutCards[20] = {0x1c,0x2c};
    int lastOutCardsNum = 2;
    char cbLaiziCard = 2;

    int select_cards_len = 2;
    cards_buf src;
    cards_buf candidate;
    int lz = 2;

    cards_buf out[MAX_CANDIDATE];

    memcpy(candidate.cards, selectedCards, select_cards_len);
    candidate.size = select_cards_len;
    memcpy(src.cards, lastOutCards, 2);
    src.card_type = 2;
    src.size = 2;

    //int n = ddz_get_cards_type_list(candidate, lz, out);
    int n = ddz_get_bigger_compose(src, candidate, lz, out);

    cout << n << endl;
}

void test_bug47()
{
    char mycards[] = {0x3b, 0x2b, 0x1b, 0x0b, 0x21, 0x11,0x0d,0x1d,0x2d};
    char lz = 0x0b;
    cards_buf src;
    cards_buf buf[MAX_CANDIDATE];
    memset(&src, 0, sizeof(src));
    memset(buf, 0, sizeof(buf));
    memcpy(src.cards, mycards, 9);
    src.size = 9;
    int n = ddz_get_cards_type_list(src, lz, buf);
    cout << n << endl;
}

void test_bug48()
{
    char buf[] = {0x3a, 0x2a, 0x25, 0x34, 0x23};
    int size = 5;
    int lz = 10; 
    char maxcard[10];
    char outcard[10][20];
    int type[10];
    ddz_lz_item out[10];
    int n = ddz_get_cards_type_list(buf, size, lz, maxcard, outcard, type, out);
    cout << n << endl;
}
*/
void test_bug49()
{
    ddz_desk desk;
    //void coin_adjust(__int64 src[3], __int64 dst[3], int loader)
    __int64 src[3];
    __int64 dst[3];
    int loader = 0;
    src[0] = 3153;
    src[1] = 520;
    src[2] = 4623;

    dst[0] = -2400;
    dst[1] = 1200;
    dst[2] = 1200;

    desk.coin_adjust(src, dst, 0);

    int n = 0;
    cout << n << endl;
}

//int  ddz_get_cards_type_list(cards_buf& src, int lz, cards_buf buf[MAX_CANDIDATE]);
void test_bug50()
{
    cards_buf src;
    src.cards[0] = 0x05;
    src.cards[1] = 0x06;
    src.cards[2] = 0x16;
    src.cards[3] = 0x26;
    src.size = 4;

    int lz = 5;

    cards_buf buf[MAX_CANDIDATE];

    int n = ddz_get_cards_type_list2(src, lz, buf);



    cout << n << endl;

}

void test_bug51()
{
    cards_buf src;
    src.cards[0] = 0x06;
    src.cards[1] = 0x07;
    src.cards[2] = 0x18;
    src.cards[3] = 0x29;
    src.cards[4] = 0x3a;
    src.cards[5] = 0x3b;
    src.cards[6] = 0x3c;
    src.cards[7] = 0x3d;
    src.cards[8] = 0x31;

    src.size = 9;

    int lz = 0x0d;

    cards_buf buf[MAX_CANDIDATE];

    int n = ddz_get_cards_type_list2(src, lz, buf);

    cout << n << endl;

}

void test_bug52()
{
    cards_buf src;
    src.cards[0] = 0x0c;
    src.cards[1] = 0x1c;
    src.cards[2] = 0x2c;
    src.cards[3] = 0x29;
    src.cards[4] = 0x3b;

    src.size = 5;

    int lz = 0x0b;

    cards_buf buf[MAX_CANDIDATE];

    int n = ddz_get_cards_type_list2(src, lz, buf);

    cout << n << endl;
}

void test_bug53()
{
    cards_buf buf[MAX_CANDIDATE];
    cards_buf src;
    cards_buf candidate;

    candidate.cards[0] = 0x4f;
    candidate.cards[1] = 0x4e;
    candidate.size = 2;

    src.cards[0] = 0x09;
    src.cards[1] = 0x19;
    src.cards[2] = 0x29;
    src.cards[3] = 0x39;
    src.card_type = MDDZ_CT_BOMB;
    src.size = 4;

    int n = ddz_get_bigger_compose2(src, candidate, 9, buf);
    cout << n << endl;
}

void test_bug54()
{
    ddz_desk desk;
    //void coin_adjust(__int64 src[3], __int64 dst[3], int loader)
    __int64 src[3];
    __int64 dst[3];
    int loader = 0;
    src[0] = 100;
    src[1] = 300;
    src[2] = 400;

    dst[0] = 2400;
    dst[1] = -1200;
    dst[2] = -1200;

    desk.coin_adjust(src, dst, 0);

    int n = 0;
    cout << n << endl;
}

void test_bug55()
{
    myredis_pool::instance()->init(10, 0);

    myredis_helper redis(myredis_pool::instance(), "127.0.0.1", 6379, 6);
    if(redis.is_null())
        return;
    char key[64];
    sprintf(key, "cb_%d_%d", 5, 111);
    pptr_buffer<128, 4> field;
    pptr_buffer<128, 4> value;
    int n = 0;

    int seconds = -1;
    int level = -1;

    field.add("second");
    field.add("level");
    //if(redis->exists(key))
    //{
    //    redis_bulk* out;
    //    int n = redis->hmget(key, field.pptr, &out);
    //    if(n == 2)
    //    {
    //        seconds = atoi(out[0].data);
    //        level = atoi(out[0].data);
    //    }
    //}
    //else
    {
        value.add("3");
        value.add("4");
        n = redis->hmset(key, field.pptr, value.pptr);
    }


    cout << n << endl;
}

void test_bug56()
{
    //void split_optimal_cards(char* c, int len, split_info& split)
    char c[20] = {0x11, 0x21, 0x22, 0x12, 0x03, 0x13};
    split_info split;
    split_optimal_cards(c, 6, split);
    int n = 0;
    cout << n << endl;
}

void test_57()
{
    char c[20] = {0x1, 0x22, 0x12, 0x03, 0x13, 0x04, 0x14, 0x05, 0x15, 0x06, 0x07};
    split_info split;
    split_optimal_cards(c, 11, split);
    int n = 0;
    cout << n << endl;
}

void test_bug58()
{
    //char c[20] = {0x11, 0x22, 0x33, 0x02, 0x12, 0x22};
    char c[20] = {0x1d, 0x2d, 0x3d, 0x0c, 0x1c, 0x2c};
    split_info split;
    split_optimal_cards(c, 6, split);

    char buf[20];
    int n = split.get_cards(buf, 20);

    cout << n << endl;
}

void test_59()
{
    char c[10] = {0x09};
    split_info split;
    split_optimal_cards(c, 1, split);
    int turns;
    int power;

    cal_power(split, power, turns);

    cout << turns << endl;

}

void test_60()
{
    char c[10] = {0x01, 0x11, 0x21, 0x02, 0x12, 0x32};
    split_info split;
    split_optimal_cards(c, 6, split);
    int turns;
    int power;

    cal_power(split, power, turns);

    cout << turns << endl;

}

void test_61()
{
    char c[10] = {0x01, 0x11, 0x21, 0x31, 0x02, 0x12, 0x32};
    split_info split;
    split_optimal_cards(c, 7, split);
    int turns;
    int power;

    cal_power(split, power, turns);

    cout << turns << endl;

}

void test_bug62()
{
    char cards[20] = {0};
    char candidate[20] = {0};
    int  lz = 3;
    cards_buf out[MAX_CANDIDATE];

    cards[0] = 0x16;
    cards[1] = 0x06;

    candidate[0] = 0x05;
    candidate[1] = 0x06;
    candidate[2] = 0x07;
    candidate[3] = 0x17;
    candidate[4] = 0x09;
    candidate[5] = 0x19;
    candidate[6] = 0x29;
    candidate[7] = 0x39;
    candidate[8] = 0x4e;
    candidate[9] = 0x4f;

    candidate[10] = 0x0a;
    candidate[11] = 0x13;

    cards_buf src(cards, 2, MDDZ_CT_PAIR);
    cards_buf cand(candidate, 12);

    int count = ddz_get_bigger_compose(src, cand, lz, out);

    cout << count << endl;
}

void test_bug63()
{
    char cards[20] = {0x04,0x14,0x24,0x3B};
    char candidate[20] = {0x06,0x16,0x1b,0x2b};
    int  lz = 0x0b;
    cards_buf out[MAX_CANDIDATE];

    cards_buf src(cards, 4, MDDZ_CT_BOMB);
    cards_buf cand(candidate, 4);

    int count = ddz_get_bigger_compose(src, cand, lz, out);

    cout << count << endl;
}

void test_mddz()
{
    test_bug63();
    _getche();
}