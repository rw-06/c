#include "Testgame.h"
#include<time.h>
#include<stdlib.h>
#include"Block.h"
#include<conio.h>
#include<mmsystem.h>
#include<cstdio>
#include<iostream>
#include<fstream>
#pragma comment(lib,"winmm.lib")
#define MAX_level 5
#define record_file "recorder.txt"
using namespace std;
//const int speed_normal = 500;//单位是ms
const int speed_normal[5] = { 500,400,300,200,100 };
const int speed_quick = 50;
Testgame::Testgame(int rows, int cols, int left, int top, int blocksize)
{
	this->rows = rows;
	this->cols = cols;
	leftmargin = left;
	topmargin = top;
	this->blocksize = blocksize;
	for (int i = 0; i < rows; i++)
	{
		vector<int> maprows;
		for (int j = 0; j < cols; j++)
		{
			maprows.push_back(0);
		}
		map.push_back(maprows);
	}
}

void Testgame::inital()
{
	mciSendString("open res/myfavourite.mp3 alias music", 0, 0, 0);
	mciSendString("play  music repeat", 0, 0, 0);
	//设置相应速度
	delay = speed_normal[0];
	//配置随机种子
	srand(time(NULL));
	//创建游戏窗口
	initgraph(938, 896);
	//加载背景图片
	loadimage(&imabg, "res/bg2.png");
	loadimage(&overone, "res/over.png");

	//初始化游戏区中的数据
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < cols; j++)
			map[i][j] = 0;

	}
	score = 0;
	linecount = 0;
	level = 1;
	//初始化最高分
	ifstream file(record_file);
	if (!file.is_open())
	{
		cout << record_file << "打开失败" << endl;
		highestscore = 0;
	}
	else {
		file >> highestscore;
	}
	file.close();//关闭文件

	gameover = false;
}
void Testgame::play()
{
	inital();
	int timer = 0;
	nextblock = new Block;
	curblock = nextblock;
	nextblock = new Block;
	while (1)
	{

		keyevent();//接受用户输入
		timer += getdelay();
		if (timer > delay)
		{
			timer = 0;
			drop();
			update = true;
			//渲染画面
		}
		if (update)
		{
			update = false;
			updatewindow();// 渲染画面(更新游戏画面)
			clearline();                //更新游戏相关数据

		}
		if (gameover)
		{

			//保存分数
			savescore();
			//显示游戏更新界面
			displayover();
			system("pause");
			inital();//重新开局

		}
	}
}

void Testgame::keyevent()
{
	unsigned char ch;//char是-128,127,无符号是0,255
	bool rotateflag = false;
	int dx = 0;
	if (_kbhit())
	{
		ch = _getch();
		//如果按下方向键，会自动返回两个字符
		//如果按下 （向上）方向键，会先后返回：224 ，72
		//如果按下（向下）方向键，会先后返回：224,80
		// 向左，224,75
		// 向右，224,77
		if (ch == 224)
		{
			ch = _getch();
			switch (ch)
			{
			case 72:
				rotateflag = true;
				break;
			case 80:
				delay = speed_quick;
				break;
			case 75:
				dx = -1;
				break;
			case 77:
				dx = 1;
				break;
			default:break;
			}
		}
	}
	if (rotateflag)
	{
		rotate();
		update = true;
		//to do 旋转，待实现
	}
	if (dx != 0)
	{  //实现左右移动
		moveleftright(dx);
		update = true;
	}
}

void Testgame::updatewindow()
{
	putimage(0, 0, &imabg);//绘制背景图片
	IMAGE** imgs = Block::getimages();
	BeginBatchDraw();
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j] == 0)
				continue;
			int x = j * blocksize + leftmargin;
			int y = i * blocksize + topmargin;
			putimage(x, y, imgs[map[i][j] - 1]);
		}
	}
	curblock->draw(leftmargin, topmargin);
	nextblock->draw(leftmargin + 12 * blocksize, topmargin);
	drawScore();//绘制分数
	EndBatchDraw();
}
//第一次调用返回0
//返回距离上一次调用，间隔了多少ms
int Testgame::getdelay()
{
	static unsigned long long lasttime = 0;

	unsigned long long currentime = GetTickCount();
	if (lasttime == 0)
	{
		lasttime = currentime;
		return 0;
	}
	else {
		int ret = currentime - lasttime;
		lasttime = currentime;
		return ret;
	}
}

void Testgame::drop()
{
	bakBlock = *curblock;//备份方块等于现有方块
	curblock->drop();
	if (!curblock->blockmap(map))//如果说下降的地方有其他方块或者越界
	{
		bakBlock.solidify(map);
		delay = speed_normal[level - 1];
		//把这个方块“固化”
		delete curblock;
		curblock = nextblock;
		nextblock = new Block;

		//检查游戏是否结束
		checkover();
	}
}

void Testgame::clearline()
{
	int lines = 0;
	int k = rows - 1;//存储数据的行数
	for (int i = rows - 1; i >= 0; i--)
	{    //检查第i行是否满行
		int count = 0;
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j])
			{
				count++;
			}
			map[k][j] = map[i][j];//一边扫描一边存储
		}

		if (count < cols)
		{
			k--;
			//不是满行
		}
		else {  //count==cols 满行
			lines++;

		}
	}
	if (lines > 0)
		//计算得分
	{		// to do.
		int addscore[4] = { 10,30,60,80 };
		score += addscore[lines - 1];
		mciSendString("play res/xiaochu1.mp3", 0, 0, 0);
		update = true;
		//每100分一个级别
		level = (score + 99) / 100;//关卡如何设计
		if (level > MAX_level)
		{
			gameover = true;
		}
		linecount += lines;

	}
}

void Testgame::moveleftright(int offset)
{
	bakBlock = *curblock;
	curblock->moveleftright(offset);
	if (!curblock->blockmap(map))
	{
		*curblock = bakBlock;
	}
}

void Testgame::rotate()
{
	if (curblock->getblocktype() == 6)
	{
		return;
	}
	bakBlock = *curblock;
	curblock->rerverse();
	if (!curblock->blockmap(map))
	{
		*curblock = bakBlock;
	}
}

void Testgame::drawScore()
{
	char scoretext[32];
	sprintf_s(scoretext, sizeof(scoretext), "%d", score);
	setcolor(RGB(180, 180, 180));
	LOGFONT f;
	gettextstyle(&f);//获取当前的字体
	f.lfHeight = 60;
	f.lfWeight = 30;
	f.lfQuality = ANTIALIASED_QUALITY;//设置字体为“抗锯齿”效果          
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));
	settextstyle(&f);
	setbkmode(TRANSPARENT);//透明效果
	outtextxy(670, 727, scoretext);

	//绘制消除了多少行
	sprintf_s(scoretext, sizeof(scoretext), "%d", linecount);
	gettextstyle(&f);
	int x = 224 - f.lfWidth * strlen(scoretext);
	outtextxy(x, 817, scoretext);

	//绘制当前是第几关
	sprintf_s(scoretext, sizeof(scoretext), "%d", level);
	outtextxy(224 - 30, 727, scoretext);
	//写最高分
	sprintf_s(scoretext, sizeof(scoretext), "%d", highestscore);
	outtextxy(670, 817, scoretext);
}

void Testgame::checkover()
{
	gameover = (curblock->blockmap(map) == false);


}

void Testgame::savescore()
{
	if (score > highestscore)
	{
		highestscore = score;
		ofstream file(record_file);
		file << highestscore;
		file.close();

	}
}

void Testgame::displayover()
{
	mciSendString("stop music", 0, 0, 0);
	if (level <= MAX_level)
	{
		putimage(262, 361, &overone);
		mciSendString("play res/over.mp3", 0, 0, 0);
	}
	else {

		mciSendString("play res/win.mp3", 0, 0, 0);
	}
}
