#pragma once
using namespace std;
#include<graphics.h>
#include<vector>
#include"Block.h"
class Testgame
{
public: Testgame(int rows, int cols, int left, int top, int blocksize);
	  void inital();
	  void play();
private:
	void keyevent();//接受用户收入
	void updatewindow();//渲染画面
	int getdelay();//返回距离上一次调用该函数，间隔了多少时间(ms)
	void drop();//图像下降
	void clearline();//清除一行
	void moveleftright(int offset);
	void rotate();//方块旋转
	void drawScore();//绘制关于分数的函数
	void checkover();//检查游戏是否结束
	void savescore();//保存最高分数
	void displayover();//是胜利结束还是失败结束
private:
	int delay;
	bool update;//是否更新
	vector<vector<int>>map;// 0,空白，没有任何方块
	// 5，是第五种俄罗斯方块w
	int rows;
	int cols;
	int leftmargin;
	int topmargin;
	int blocksize;
	int score;//当前分数
	IMAGE imabg;
	int level;//当前的关卡
	int linecount;//当前消除行数
	int highestscore;//最高分
	bool gameover;//游戏是否已经结束
	Block* curblock;
	Block* nextblock;//预告的方块
	Block bakBlock;//当前方块降落过程中，用来备份上一份方块的方块。
	IMAGE overone;
};

