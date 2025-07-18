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
	void keyevent();//�����û�����
	void updatewindow();//��Ⱦ����
	int getdelay();//���ؾ�����һ�ε��øú���������˶���ʱ��(ms)
	void drop();//ͼ���½�
	void clearline();//���һ��
	void moveleftright(int offset);
	void rotate();//������ת
	void drawScore();//���ƹ��ڷ����ĺ���
	void checkover();//�����Ϸ�Ƿ����
	void savescore();//������߷���
	void displayover();//��ʤ����������ʧ�ܽ���
private:
	int delay;
	bool update;//�Ƿ����
	vector<vector<int>>map;// 0,�հף�û���κη���
	// 5���ǵ����ֶ���˹����w
	int rows;
	int cols;
	int leftmargin;
	int topmargin;
	int blocksize;
	int score;//��ǰ����
	IMAGE imabg;
	int level;//��ǰ�Ĺؿ�
	int linecount;//��ǰ��������
	int highestscore;//��߷�
	bool gameover;//��Ϸ�Ƿ��Ѿ�����
	Block* curblock;
	Block* nextblock;//Ԥ��ķ���
	Block bakBlock;//��ǰ���齵������У�����������һ�ݷ���ķ��顣
	IMAGE overone;
};

