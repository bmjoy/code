#include "ddz_desk.h"
#include "config_manager.h"
#include "myrandom.h"
#include "cflog.h"
#include "mddz.h"
#include "mddz_util.h"
#include "myrandom.h"

void ddz_desk::shuffle2()
{
	if (config_manager::instance()->get_game_config().surport_lz == 0)
	{
		m_lz = 0;
	}
	else
	{
		m_lz = random_int(1, 13);
	}

    shuffle_config& config = cfg_inst->get_shuffle_config();
    int rate[3];
    rate[0] = config.normal_rate;
    rate[1] = rate[0] + config.str_rate;
    rate[2] = rate[1] + config.bomb_rate;

    int r = random_int(0, rate[2]);
    if(r < rate[0])
    {
        shuffle1();
        return;
    }
    if(r < rate[1])
    {
        shuffle_straight();
        return;
    }
    shuffle_bomb();
}

void ddz_desk::shuffle_bomb()
{
    shuffle_config& config = cfg_inst->get_shuffle_config();
    int nb = random_int(config.bomb_from, config.bomb_to);
    if(nb == 0){shuffle1(); return;}
    if(nb>5)nb = 5;

    char buf[3][17];
    int  size[3];
    for(int i=0; i<3; i++)size[i] = 0;
    
    int num[13];
    for(int i=0; i<13; i++) num[i] = i+1;

    for(int i=0; i<13; i++)
    {
        int t = random_int(0, 13);
        if(t >= 13) t = 12;
        if(t == i) continue;
        int c = num[t];
        num[t] = num[i];
        num[i] = c;
    }

    for(int i=0; i<nb; i++)
    {
        int seatid = random_int(0, 2);
        if(size[seatid]>=16){seatid++;if(seatid>2)seatid=0;}
        buf[seatid][size[seatid]] = 0x30 | num[i]; size[seatid]++;
        buf[seatid][size[seatid]] = 0x20 | num[i]; size[seatid]++;
        buf[seatid][size[seatid]] = 0x10 | num[i]; size[seatid]++;
        buf[seatid][size[seatid]] =        num[i]; size[seatid]++;
    }
    char restbuf[54];
    int  restlen = 0;
    for(int i=nb; i<13; i++)
    {
        restbuf[restlen] = 0x30 | num[i]; restlen ++;
        restbuf[restlen] = 0x20 | num[i]; restlen ++;
        restbuf[restlen] = 0x10 | num[i]; restlen ++;
        restbuf[restlen] =        num[i]; restlen ++;
    }
    restbuf[restlen] = 0x4e; restlen ++;
    restbuf[restlen] = 0x4f; restlen ++;

    for(int i=0; i<restlen; i++)
    {
        int t = random_int(0, restlen-1);
        if(i == t) continue;
        char c = restbuf[t];
        restbuf[t] = restbuf[i];
        restbuf[i] = c;
    }
    int nshf = 0;
    for(int i=0; i<3; i++)
    {
        int nl = 17 - size[i];
        for(int j=0; j<nl; j++)
        {
            buf[i][size[i] + j] = restbuf[nshf+j];
        }
        nshf += nl;
    }
    if((restlen - nshf) != 3)
    {
        LOG_ERR_LINE();
        shuffle1();
        return;
    }
    memcpy(m_back, &restbuf[nshf], 3);
    for (int i=0; i<3; i++)
    {
        memcpy(m_hand[i].cards, buf[i], 17);
        m_hand[i].len = 17;
        change_12_2_A2(m_hand[i].cards, m_hand[i].len);
        sort_by_num(m_hand[i].cards, m_hand[i].len);
        change_A2_2_12(m_hand[i].cards, m_hand[i].len);
    }
    if(config_manager::instance()->get_game_config().surport_lz == 1)
    {
        m_lz = random_int(1, 13);
    }
}

void ddz_desk::shuffle_straight()
{
    char buf[3][17];
    int  size[3];
    for(int i=0; i<3; i++)size[i] = 0;

    char str_buf[4][11];
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<11; j++)
        {
            str_buf[i][j] = (i << 4) | (j+3);
        }
    }
    for(int i=0; i<11; i++)
    {
        for(int j=0; j<4; j++)
        {
            int r = random_int(0, 3);
            if(r == j) continue;
            char c = str_buf[r][i];
            str_buf[r][i] = str_buf[j][i];
            str_buf[j][i] = c;
        }
    }
    for(int i=0; i<3; i++)
    {
        memcpy(buf[i], str_buf[i], 11);
        size[i] = 11;
    }
    char restbuf[54];
    int  restlen = 0;
    for(int i=0; i<3; i++)
    {
        restbuf[restlen] = 0x30 | (i+1); restlen ++;
        restbuf[restlen] = 0x20 | (i+1); restlen ++;
        restbuf[restlen] = 0x10 | (i+1); restlen ++;
        restbuf[restlen] =        (i+1); restlen ++;
    }
    memcpy(&restbuf[8], str_buf[3], 11);
    restbuf[19] = 0x4e; restbuf[20] = 0x4f;
    for(int i=0; i<21; i++)
    {
        int r = random_int(0, 20);
        if(i == r) continue;
        char c = restbuf[i];
        restbuf[i] = restbuf[r];
        restbuf[r] = c;
    }
    for(int i=0; i<3; i++)
    {
        memcpy(&buf[i][11], &restbuf[i*6], 6);
    }
    memcpy(m_back, &restbuf[18], 3);
    for (int i=0; i<3; i++)
    {
        memcpy(m_hand[i].cards, buf[i], 17);
        m_hand[i].len = 17;
        change_12_2_A2(m_hand[i].cards, m_hand[i].len);
        sort_by_num(m_hand[i].cards, m_hand[i].len);
        change_A2_2_12(m_hand[i].cards, m_hand[i].len);
    }
    if(config_manager::instance()->get_game_config().surport_lz == 1)
    {
        m_lz = random_int(1, 13);
    }
}