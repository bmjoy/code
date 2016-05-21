#include "timer.h"
#include "cflog.h"

struct timer_key
{
	union{
		struct{
            unsigned short id1;
            unsigned short id2;
			unsigned short id3;
			unsigned short tid;
		};
		unsigned long long key;
	};
};

#define TYPE_SET_TIMER 1
#define TYPE_KILL_TIMER 2

timer::timer(void):base_thread("timer")
{
	m_brun = false;
	m_p_service = 0;
    //一秒最大内并发设置timer
    m_set_queue.init(100000);
}

timer::~timer(void)
{
}

bool timer::init(i_timer_service* service)
{
	if(!service)
		return false;
	m_p_service = service;
	return true;
}

int timer::svc()
{
	while(m_brun)
	{
#ifdef _WIN32
		Sleep(1000);
#else
	    usleep(1000000);
#endif
        check_set_queue();

		if(!m_brun)
			break;
		check_timer();
	}
	m_timer_map.clear();
	return 0;
}

bool timer::start()
{
	if(!m_p_service || m_brun)
		return false;
	m_brun = true;
	activate();
	return true;
}

bool timer::stop()
{
	if(!m_brun)
		return false;
	m_brun = false;
#ifdef _WIN32
	Sleep(1500);
#else
	usleep(1000);
#endif
	return true;
}

void timer::set_timer(unsigned short id1, unsigned short id2, unsigned short id3, unsigned short tid, int delay, int data)
{
	if(!m_brun)
		return;
	timer_key tk;
	tk.id1 = id1;
	tk.id2 = id2;
    tk.id3 = id3;
	tk.tid = tid;

    set_timer_item s;
    s.type = TYPE_SET_TIMER;
    s.key = tk.key;
	s.delay = delay;
    s.data = data;

    {
    //    guard g(m_mutex);
        m_set_queue.put(s);
    }
}

void timer::kill_timer(unsigned short id1, unsigned short id2, unsigned short id3, unsigned short tid)
{
    timer_key tk;
    tk.id1 = id1;
    tk.id2 = id2;
    tk.id3 = id3;
    tk.tid = tid;

    set_timer_item s;
    s.type = TYPE_KILL_TIMER;
    s.key = tk.key;
    s.delay = 0;
    s.data = 0;

    {
      //  guard g(m_mutex);
        m_set_queue.put(s);
    }
}

void timer::check_timer()
{
	timer_iterator i;

	for(i=m_timer_map.begin(); i!=m_timer_map.end();)
	{
		i->second.count --;
		if(i->second.count <= 0)
		{
			timer_key tk;
			tk.key = i->first;

			m_p_service->on_timer(tk.id1, tk.id2, tk.id3, tk.tid, i->second.data);
#ifdef _WIN32
            i = m_timer_map.erase(i);
#else
            m_timer_map.erase(i);
            i++;
#endif
		}
		else
			i++;
	}
}

void timer::check_set_queue()
{
   // guard g(m_mutex);
	set_timer_item s;
    timer_iterator i;

	while(m_set_queue.pop(s))
	{
        if(s.type == TYPE_SET_TIMER)
        {
            time_item item;
            item.count = s.delay;
            item.delay = s.delay;
            item.data = s.data;
            m_timer_map[s.key] = item;
        }
        else if(s.type = TYPE_KILL_TIMER)
        {
            i = m_timer_map.find(s.key);
            if(i != m_timer_map.end())
                m_timer_map.erase(i);
        }
	}
}
