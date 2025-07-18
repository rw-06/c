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
	  void rerverse();//翻转
	  void drop();//下降
	  void moveleftright(int offset);//左右移动
	  void draw(int leftmargin, int topmargin);
	  static IMAGE** getimages();
	  void solidify(vector<vector<int>>& map);//固化
	  Block& operator=(const Block& other);//重写运算符

	  bool blockmap(const vector<vector<int>>& map);
	  //point* getsmallblocks();
	  int getblocktype();

private:
	int blocktype;//方块的类型
	point smallblocks[4];//4个小方块分别在第几行第几列
	IMAGE* img;


	static IMAGE* imgs[7];//返回的类型是IMAGE*
	static int size;
};

