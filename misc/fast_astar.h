#ifndef __FAST_A_STAR_H__
#define __FAST_A_STAR_H__
struct node_item
{
	char type;
	char state;
	short x;
	short y;
	float G;
	float H;
	node_item* next;
	node_item* parent;
};

struct node_position
{
	int x;
	int y;
};

class fast_astar
{
public:
	fast_astar(void);
	~fast_astar(void);
	
	bool init(int w, int h);
	void reset();
	bool cal_short_path(char* map, int sx, int sy, int ex, int ey, bool dir);
	int get_steps();
	bool out_path(node_position* pbuf, int size);
private:
	bool extend_node(char* map, int x, int y, int px, int py, float G);

private:
	int step;
	int end_index;
	int start_index;
	node_item* open_list;
	node_item* map_node;
	int width;
	int height;
	int map_size;
};



#endif//__FAST_A_STAR_H__
