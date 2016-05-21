#include "fast_astar.h"
#include <string.h>
#include <iostream>
using namespace std;

#define sqrt2 1.41421f

enum NODE_ITEM_TYPE
{
	FA_START = 100,
	FA_END
};

enum NODE_ITEM_STATE
{
	FA_NULL,
	FA_OPEN,
	FA_CLOSE
};


#define abs(x) ((x)>0?(x):(-(x)))

float get_H(int cur_i, int cur_j, int end_i, int end_j)
{
	return (float)(abs(end_j - cur_j) + abs(end_i - cur_i));
}

float get_G(node_item* cur_node)
{
	node_item* p = cur_node->parent;
	if (abs(p->x - cur_node->x) + abs(p->y - cur_node->y) > 1)
		return sqrt2 + p->G;
	else
		return 1.0f + p->G;
}

#define get_f(x)(x->G + x->H)


node_item* pop_node(node_item** p)
{
	node_item* ret = *p;

	if(ret)
	{
		*p = (*p)->next;
		ret->next = 0;
	}

	return ret;
}

void put_node(node_item** p, node_item* n)
{
	if(!(*p))
	{
		*p = n;
		return;
	}
	node_item* q = *p;
	node_item* t = 0;
	while(1)
	{
		t = q;
		q = q->next;
		if(!q)
		{
			t->next = n;
			break;
		}
		else if(get_f(n) < get_f(q))
		{
			t->next = n;
			n->next = q;
			break;
		}
	}
}

void remove_node(node_item** p, node_item* n)
{
	node_item* q;
	if((*p) == n)
	{
		(*p) = (*p)->next;
		return;
	}
	node_item* pp = (*p);
	while(pp)
	{
		q = pp;
		pp = pp->next;
		if(pp == n)
		{
			q->next = pp->next;
			pp->next = 0;
			return;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
fast_astar::fast_astar(void)
{
	open_list = 0;
	map_node = 0;
	map_size = 0;
	width = 0;
	height = 0;
}

fast_astar::~fast_astar(void)
{
	if(map_node)
	{
		delete[] map_node;
		map_node = 0;
	}
}

bool fast_astar::init(int w, int h)
{
	width = w;
	height = h;
	map_size = width * height;
	if(map_node)
	{
		delete map_node;
		map_node = 0;
	}
	map_node = new node_item[map_size];
	memset(map_node, 0, sizeof(node_item) * map_size);
	
	open_list = 0;
	return true;
}

void fast_astar::reset()
{
	if(!map_node)
		return;
	memset(map_node, 0, sizeof(node_item) * map_size);
	open_list = 0;
}

bool fast_astar::extend_node(char* map, int x, int y, int nx, int ny, float G)
{
	if(nx < 0 || nx >= width || ny < 0 || ny>= height)
		return false;

	int index = ny * width + nx;
	int pindex= y * width + x;
	switch(map_node[index].state)
	{
	case FA_NULL:
		{
			if(!map[index])
				return false;
			map_node[index].x = nx;
			map_node[index].y = ny;
			map_node[index].parent = &map_node[y*width+x];
			map_node[index].next = 0;
			map_node[index].G = G;
			map_node[index].H = get_H(map_node[end_index].x, map_node[end_index].y, nx, ny);
			map_node[index].state = FA_OPEN;
			put_node(&open_list, &map_node[index]);
		}
		return true;
	case FA_OPEN:
		{
			float nG = get_G(&map_node[index]);
			if(nG < map_node[index].G)
			{
				map_node[index].parent = &map_node[pindex];
				remove_node(&open_list, &map_node[index]);
				put_node(&open_list, &map_node[index]);		
			}
		}
		return true;
	case FA_CLOSE:
		return true;
	}

	return false;
}

bool fast_astar::cal_short_path(char* map, int sx, int sy, int ex, int ey, bool dir)
{
	start_index = sy * width + sx;
	if(start_index >= map_size)
		return false;
	map_node[start_index].type = FA_START;
	map_node[start_index].state= FA_OPEN;
	map_node[start_index].x = sx;
	map_node[start_index].y = sy;
	map_node[start_index].H = get_H(sx, sy, ex, ey);
	map_node[start_index].G = 0;
	map_node[start_index].next = 0;

	end_index = ey * width + ex;
	if(end_index >= map_size)
		return false;
	map_node[end_index].type = FA_END;
	map_node[end_index].x = ex;
	map_node[end_index].y = ey;
	map_node[end_index].H = 0;
	map_node[end_index].G = 99999;

	open_list = &map_node[start_index];

	node_item* cur = pop_node(&open_list);

	while(cur)
	{
		if(cur->type == FA_END)
		{
			return true;
		}
		int x,y;
		x = cur->x;
		y = cur->y;
		
		int nx, ny;
		nx = x;
		ny = y-1;
		bool b_top = extend_node(map, x, y, nx, ny, 1);
		ny = y+1;
		bool b_bottom = extend_node(map, x, y, nx, ny, 1);
		nx = x - 1;
		ny = y;
		bool b_left = extend_node(map, x, y, nx, ny, 1);
		nx = x + 1;
		bool b_right = extend_node(map, x, y, nx, ny, 1);
		
		if(dir)
		{
			if(b_left && b_top)
			{
				nx = x - 1;
				ny = y - 1;
				extend_node(map, x, y, nx, ny, sqrt2);
			}
			if(b_right && b_top)
			{
				nx = x + 1;
				ny = y - 1;
				extend_node(map, x, y, nx, ny, sqrt2);
			}
			if(b_left && b_bottom)
			{
				nx = x - 1;
				ny = y + 1;
				extend_node(map, x, y, nx, ny, sqrt2);
			}
			if(b_right && b_bottom)
			{
				nx = x + 1;
				ny = y + 1;
				extend_node(map, x, y, nx, ny, sqrt2);
			}
		}

		cur->state = FA_CLOSE;
		cur = pop_node(&open_list);
	}
	return false;
}

int fast_astar::get_steps()
{
	step = 0;
	if(map_node[end_index].parent == 0)
		return 0;
	node_item* p = &map_node[end_index];
	while(p && p->type != FA_START)
	{
		step ++;
		p = p->parent;
	}
	return step;
}

bool fast_astar::out_path(node_position* pbuf, int size)
{
	if(size != step)
		return false;
	node_item* p = &map_node[end_index];
	int count = 0;
	while(p && p->type != FA_START)
	{
		pbuf[step - count - 1].x = p->x;
		pbuf[step - count - 1].y = p->y;
		p = p->parent;
		count ++;
		if(count >= step)
			return false;
	}
	return true;
}
