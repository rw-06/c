#include "Block.h"
#include<stdlib.h>
IMAGE* Block::imgs[7] = { NULL, };
int Block::size = 36;

int blocks[7][4] = {
	1,3,5,7,//1
	2,4,5,7,//
	3,5,4,6,//
	3,5,4,7,//
	2,3,5,7,//
	3,5,7,6,//
	2,3,4,5//
};//随机生成一种俄罗斯方块
Block::Block()
{
	if (imgs[0] == NULL)
	{
		IMAGE imgtmp;
		loadimage(&imgtmp, "res/tiles.png");

		SetWorkingImage(&imgtmp);
		for (int i = 0; i < 7; i++)
		{
			imgs[i] = new IMAGE;
			getimage(imgs[i], i * size, 0, size, size);
		}
		SetWorkingImage();//恢复工作区
	}
	blocktype = rand() % 7;//0到6（七个类型）
	//初始化smallblocks
	for (int i = 0; i < 4; i++)
	{
		int value = blocks[blocktype][i];
		smallblocks[i].row = value / 2;
		smallblocks[i].col = value % 2;
	}
	img = imgs[blocktype];
}

void Block::rerverse()
{
	point p = smallblocks[1];
	for (int i = 0; i < 4; i++)
	{
		point tep = smallblocks[i];
		smallblocks[i].col = p.col - tep.row + p.row;
		smallblocks[i].row = p.row + tep.col - p.col;
	}
}

void Block::drop()
{
	for (int i = 0; i < 4; i++)
	{
		smallblocks[i].row++;
	}
	//for(auto x:smallblocks){x.row++;};
}

void Block::moveleftright(int offset)
{
	for (int i = 0; i < 4; i++)
	{
		smallblocks[i].col += offset;
	}

}

void Block::draw(int leftmargin, int topmargin)
{
	for (int i = 0; i < 4; i++)
	{
		int x = leftmargin + smallblocks[i].col * size;
		int y = topmargin + smallblocks[i].row * size;
		putimage(x, y, img);
	}//
}

IMAGE** Block::getimages()
{
	return imgs;
}

void Block::solidify(vector<vector<int>>& map)
{
	for (int i = 0; i < 4; i++)
	{  //设置标记，“固化”对应的位置
		map[smallblocks[i].row][smallblocks[i].col] = blocktype + 1;//不能让这个值为1
	}
}

Block& Block::operator=(const Block& other)
{
	if (this == &other)
		return *this;
	this->blocktype = other.blocktype;
	for (int i = 0; i < 4; i++)
	{
		this->smallblocks[i] = other.smallblocks[i];
	}
	this->img = imgs[blocktype];  // 添加img复制
	return *this; 
}
bool Block::blockmap(const vector<vector<int>>& map)
{
	for (int i = 0; i < 4; i++)
	{
		int rows = map.size();
		int cols = map[0].size();
		if (smallblocks[i].col < 0 || smallblocks[i].col >= cols || smallblocks[i].row < 0
			|| smallblocks[i].row >= rows || map[smallblocks[i].row][smallblocks[i].col] != 0)
		{
			return false;
		}
	}
	return true;
}

int Block::getblocktype()
{
	return blocktype;
}
