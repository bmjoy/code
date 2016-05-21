#include "mddz_util.h"
#include "mddz.h"
#include <string.h>
#ifdef _WIN32
#include "myrandom.h"
#endif

void zerom(char* s, int len)
{
    for(int i=0; i<len; i++)s[i] = 0;
}

void change_12_2_A2(char* buf, int len)
{
    if(!buf || len<= 0 || len > 20)
        return;
    for(int i=0; i<len; i++)
    {
        if((buf[i]& 0x0f) <= 2)
        {
            buf[i] += 11;
        }
        else if((buf[i] & 0xf0) != 0x40)
        {
            buf[i] -= 2;
        }
    }
}

char change_12_2_A2(char c)
{
    if(c == 0)
        return 0;
    if((c & 0xf0) > 0x50)
        return c;
    if((c & 0x0f) <= 2)
    {
        c += 11;
    }
    else if((c & 0xf0) != 0x40)
    {
        c -= 2;
    }
    return c;
}

void change_A2_2_12(char* buf, int len)
{
    if(!buf || len<=0 || len > 20)
        return;
    for(int i=0; i<len; i++)
    {
        if((buf[i] & 0x0f) >= CARD_A)
        {
            if((buf[i] & 0xf0) != 0x40)
                buf[i] -= 11;
        }
        else
        {
            buf[i] += 2;
        }
    }
}

void change_A2_2_12(ddz_lz_change_item* lz, int size)
{
    if(!lz || size<=0 || size>4)
        return;
    for(int i=0; i<size; i++)
    {
        lz[i].card = change_A2_2_12(lz[i].card);
        lz[i].change = change_A2_2_12(lz[i].change);
    }
}

char change_A2_2_12(char c)
{
    if((c & 0xf0) > 0x50)
        return c;
    if((c & 0x0f) >= CARD_A)
    {
        if((c & 0xf0) != 0x40)
            c -= 11;
    }
    else
    {
        c += 2;
    }
    return c;
}

//扑克数据
const char CARD_DATA1[44]=
{
    0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,//方块 a - k
    0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,//梅花 a - k
    0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,//红桃 a - k
    0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,//黑桃 a - k
};
const char CARD_DATA2[10]=
{
    0x0c,0x0d,	
    0x1c,0x1d,	
    0x2c,0x2d,	
    0x3c,0x3d,	
    0x4e,0x4f
};

void shuffle_buf(char* c, int size)
{
    for(int i=0; i<size; i++)
    {
        int n = random_int(0, size-1);
        if(n == i)continue;
        char t = c[n];
        c[n] = c[i];
        c[i] = t;
    }
}

#ifdef _WIN32
void shuffle(char* c1, char* c2, char* c3, char* b, bool* robot)
{
    do 
    {
        break;
        if(robot)
        {
            char big[10];
            char normal[44];
            int nr = 0;
            int len[3] = {0,0,0};
            for(int i=0; i<3; i++)
            {
                if(robot[i])
                    nr ++;
            }
            if(nr ==0 || nr == 3)
                break;
            
            memcpy(big, CARD_DATA2, 10);
            memcpy(normal, CARD_DATA1, 44);
            shuffle_buf(big, 10);
            shuffle_buf(normal, 44);
            switch (nr)
            {
            case 1:
                {
                    for(int i=0; i<3; i++)
                    {
                        if(robot[i])
                        {
                            len[i] = 5;
                            break;
                        }
                    }
                    int rn[2];
                    rn[0] = random_int(1,4);
                    rn[1] = 5 - rn[0];
                    int ii = 0;
                    for(int i=0; i<3; i++)
                    {
                        if(len[i]>0)continue;
                        len[i] = rn[ii]; ii++;
                    }
                }
                break;
            case 2:
                {
                    int nn = random_int(1, 3);
                    for(int i=0; i<3; i++)
                    {
                        if(robot[i]) continue;
                        len[i] = nn;
                        break;
                    }
                    int rn[2];
                    rn[0] = random_int(3, 5);
                    rn[1] = 10 - rn[0] - nn;
                    int ii = 0;
                    for(int i=0; i<3; i++)
                    {
                        if(len[i]>0)continue;
                        len[i] = rn[ii]; ii++;
                    }
                }
                break;
            default:
                break;
            }
            int l1 = 0;
            int l2 = 0;
            memcpy(c1, &big[l1], len[0]); l1 += len[0];
            memcpy(&c1[len[0]], &normal[l2], 17-len[0]); l2+=(17-len[0]);

            memcpy(c2, &big[l1], len[1]); l1 += len[1];
            memcpy(&c2[len[1]], &normal[l2], 17-len[1]); l2+=(17-len[1]);

            memcpy(c3, &big[l1], len[2]); l1 += len[2];
            memcpy(&c3[len[2]], &normal[l2], 17-len[2]); l2+=(17-len[2]);

            memcpy(b, &normal[l2], 3); l2 += 3;
            return;
        }
    } while (false);
    char buf[CARD_COUNT];
    memcpy(buf, (char*)CARD_DATA, CARD_COUNT);

    for(int i=0; i<CARD_COUNT; i++)
    {
        int r = random_int(0, CARD_COUNT-1);
        if(r == i)continue;
        buf[i] ^= buf[r] ^= buf[i] ^= buf[r];
    }
    int index =0;
    for(int i=0; i<17; i++)
    {
        c1[i] = buf[index++];
        c2[i] = buf[index++];
        c3[i] = buf[index++];
    }
    for(int i=0; i<3; i++) b[i] = buf[index++];
}
#else
void shuffle(char* c1, char* c2, char* c3, char* b){}
#endif
