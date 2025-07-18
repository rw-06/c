#pragma once
#include<graphics.h>
#include<vector>
using namespace std;
struct point {
	int row;
	int col;
};
class Block
{
public: Block();
	  void rerverse();//��ת
	  void drop();//�½�
	  void moveleftright(int offset);//�����ƶ�
	  void draw(int leftmargin, int topmargin);
	  static IMAGE** getimages();
	  void solidify(vector<vector<int>>& map);//�̻�
	  Block& operator=(const Block& other);//��д�����

	  bool blockmap(const vector<vector<int>>& map);
	  //point* getsmallblocks();
	  int getblocktype();

private:
	int blocktype;//���������
	point smallblocks[4];//4��С����ֱ��ڵڼ��еڼ���
	IMAGE* img;


	static IMAGE* imgs[7];//���ص�������IMAGE*
	static int size;
};

