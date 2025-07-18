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
//const int speed_normal = 500;//��λ��ms
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
	//������Ӧ�ٶ�
	delay = speed_normal[0];
	//�����������
	srand(time(NULL));
	//������Ϸ����
	initgraph(938, 896);
	//���ر���ͼƬ
	loadimage(&imabg, "res/bg2.png");
	loadimage(&overone, "res/over.png");

	//��ʼ����Ϸ���е�����
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < cols; j++)
			map[i][j] = 0;

	}
	score = 0;
	linecount = 0;
	level = 1;
	//��ʼ����߷�
	ifstream file(record_file);
	if (!file.is_open())
	{
		cout << record_file << "��ʧ��" << endl;
		highestscore = 0;
	}
	else {
		file >> highestscore;
	}
	file.close();//�ر��ļ�

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

		keyevent();//�����û�����
		timer += getdelay();
		if (timer > delay)
		{
			timer = 0;
			drop();
			update = true;
			//��Ⱦ����
		}
		if (update)
		{
			update = false;
			updatewindow();// ��Ⱦ����(������Ϸ����)
			clearline();                //������Ϸ�������

		}
		if (gameover)
		{

			//�������
			savescore();
			//��ʾ��Ϸ���½���
			displayover();
			system("pause");
			inital();//���¿���

		}
	}
}

void Testgame::keyevent()
{
	unsigned char ch;//char��-128,127,�޷�����0,255
	bool rotateflag = false;
	int dx = 0;
	if (_kbhit())
	{
		ch = _getch();
		//������·���������Զ����������ַ�
		//������� �����ϣ�����������Ⱥ󷵻أ�224 ��72
		//������£����£�����������Ⱥ󷵻أ�224,80
		// ����224,75
		// ���ң�224,77
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
		//to do ��ת����ʵ��
	}
	if (dx != 0)
	{  //ʵ�������ƶ�
		moveleftright(dx);
		update = true;
	}
}

void Testgame::updatewindow()
{
	putimage(0, 0, &imabg);//���Ʊ���ͼƬ
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
	drawScore();//���Ʒ���
	EndBatchDraw();
}
//��һ�ε��÷���0
//���ؾ�����һ�ε��ã�����˶���ms
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
	bakBlock = *curblock;//���ݷ���������з���
	curblock->drop();
	if (!curblock->blockmap(map))//���˵�½��ĵط��������������Խ��
	{
		bakBlock.solidify(map);
		delay = speed_normal[level - 1];
		//��������顰�̻���
		delete curblock;
		curblock = nextblock;
		nextblock = new Block;

		//�����Ϸ�Ƿ����
		checkover();
	}
}

void Testgame::clearline()
{
	int lines = 0;
	int k = rows - 1;//�洢���ݵ�����
	for (int i = rows - 1; i >= 0; i--)
	{    //����i���Ƿ�����
		int count = 0;
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j])
			{
				count++;
			}
			map[k][j] = map[i][j];//һ��ɨ��һ�ߴ洢
		}

		if (count < cols)
		{
			k--;
			//��������
		}
		else {  //count==cols ����
			lines++;

		}
	}
	if (lines > 0)
		//����÷�
	{		// to do.
		int addscore[4] = { 10,30,60,80 };
		score += addscore[lines - 1];
		mciSendString("play res/xiaochu1.mp3", 0, 0, 0);
		update = true;
		//ÿ100��һ������
		level = (score + 99) / 100;//�ؿ�������
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
	gettextstyle(&f);//��ȡ��ǰ������
	f.lfHeight = 60;
	f.lfWeight = 30;
	f.lfQuality = ANTIALIASED_QUALITY;//��������Ϊ������ݡ�Ч��          
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));
	settextstyle(&f);
	setbkmode(TRANSPARENT);//͸��Ч��
	outtextxy(670, 727, scoretext);

	//���������˶�����
	sprintf_s(scoretext, sizeof(scoretext), "%d", linecount);
	gettextstyle(&f);
	int x = 224 - f.lfWidth * strlen(scoretext);
	outtextxy(x, 817, scoretext);

	//���Ƶ�ǰ�ǵڼ���
	sprintf_s(scoretext, sizeof(scoretext), "%d", level);
	outtextxy(224 - 30, 727, scoretext);
	//д��߷�
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
