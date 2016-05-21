#ifndef __KEY_WORDS_TOOL_H__
#define __KEY_WORDS_TOOL_H__

/*
优化关键字库文件
将字库文件过滤排重，调整顺序，提高加载速度与检索速度。
成功返回0
*/
extern int kw_optimize(const char* srcfilename, const char* outfile);


/*
加载初始化词库
成功返回0
*/
extern int kw_init(const char* kw_file);


/*
检测是否存在关键字
返回值：0  无关键字冲突
        -1 存在关键字冲突
        hint: 冲突的关键字
*/
extern int kw_check(const char* word, char* hint, int* hintlen);


/*
和谐字符串
将src中出现关键字的替换成**，每个关键字替换两个**。
返回值：0   无关键字冲突
        -1  存在关键字冲突，
*/
extern int kw_hexie(char* src, int* hexielen);

#endif//__KEY_WORDS_TOOL_H__