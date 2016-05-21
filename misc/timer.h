#ifndef __TIMER_H__
#define __TIMER_H__

#include "base_thread.h"
#include "mutex.h"
#include <map>
#include "singleton.h"
#include "safe_queue.h"
using namespace std;

class i_timer_service
{
public:
	virtual void on_timer(unsigned short id1, unsigned short id2, unsigned short id3, unsigned short tid, int data) = 0;
};

struct time_item
{
	int delay;
	int count;
    int data;
};

typedef map<unsigned long long, time_item> timer_map;
typedef timer_map::iterator timer_iterator;


struct set_timer_item
{
    int type;
	unsigned long long key;
	int delay;
    int data;
};

typedef safe_queue<set_timer_item> set_timer_queue;

class timer : public base_thread
{
public:
	timer(void);
	~timer(void);

	virtual int svc();

	bool init(i_timer_service* service);
	bool start();
	bool stop();
	void set_timer(unsigned short id1, unsigned short id2, unsigned short id3, unsigned short tid, int delay, int data);
	void kill_timer(unsigned short id1, unsigned short id2, unsigned short id3, unsigned short tid);
private:
	void check_timer();
	void check_set_queue();
private:
	bool   m_brun;
	thread_mutex m_mutex;
    set_timer_queue m_set_queue;


	timer_map m_timer_map;
	i_timer_service* m_p_service;
};

typedef singleton<timer> timer_singleton;
#define timer_inst timer_singleton::instance()
struct on_timer_data
{
    unsigned short id1;
    unsigned short id2;
    unsigned short id3;
    unsigned short tid;
    int data;
};
#endif//__TIMER_H__