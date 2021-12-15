#include <iostream>
#include <vector>
#include "Matrix.h"
using namespace std;

const int maxn = 50;


class solution {
public:
	//变量
	vector<vector<int>> adjMatrix;//邻接矩阵
	vector<vector<int>> incMatrix;//关联矩阵
	int n;//顶点数
	int m;//边数



	//函数

	//constructor
	solution()
	{
		this->n = 0;
		this->m = 0;
	}

	solution(int n) {
		this->n = n;
		this->m = 0;
	}

	void setN();
	void setAdjMatrix();
	void displayMatrix(vector<vector<int>> matrix, int row, int col, int type);//打印参数矩阵,type表示类型,0为邻接矩阵,1为关联矩阵
	void InitalIncMatrix();//关联矩阵初始化
};

int main()
{
	solution method;

	method.setN();
	method.setAdjMatrix();
	method.InitalIncMatrix();

	method.displayMatrix(method.incMatrix,method.n,method.m, 1);

	return 0;
}

void solution::setN()
{
	cout << endl;
	cout << "#==========读入邻接矩阵n" << endl;
	cout << endl;

	cout << "n = ";
	cin >> n;

	this->n = n;
}

void solution::setAdjMatrix()
{
	cout << endl;
	cout << "#==========读入邻接矩阵" << endl;
	cout << endl;

	int input;

	for (int i = 0; i <= n; i++)
	{
		vector<int> tem;
		for (int j = 0; j <= n; j++)
		{
			if (i == 0 || j == 0) {
				tem.push_back(-1);
			}
			else {
				cin >> input;
				//读入的时候顺便统计边数
				if (j > i && input)m++;
				tem.push_back(input);
			}			
		}
		adjMatrix.push_back(tem);
	}

	displayMatrix(adjMatrix, n, n, 0);
}

void solution::displayMatrix(vector<vector<int>> matrix, int row, int col, int type)
{
	char ch = 'v';

	cout << endl;
	if (type == 0) {
		cout << "#==========打印邻接矩阵" << endl;
		ch = 'v';
	}
		
	else if (type == 1) {
		cout << "#==========打印关联矩阵" << endl;
		ch = 'e';
	}
		
	cout << endl;

	for (int i = 0; i <= row; i++)
	{
		if (i == 0) {
			printf("%-5s\t", "index");
		}
		else
			printf("%4c%d\t", 'v', i);

		for (int j = 1; j <= col; j++)
		{
			if (i == 0)
				printf("%4c%d", ch, j);
			else
				printf("%5d", matrix[i][j]);
		}
		cout << endl;
	}
}

void solution::InitalIncMatrix()
{
	//初始化为0
	for (int i = 0; i <= n; i++)
	{
		vector<int> tem;
		for (int j = 0; j <= m; j++)
		{
			tem.push_back(0);
		}
		incMatrix.push_back(tem);
	}

	//初始化
	int cnt = 0;
	for (int i = 1; i <= n; i++)
	{
		for (int j = i + 1; j <= n; j++)
		{
			if (adjMatrix[i][j]) {
				cnt++;
				incMatrix[i][cnt] = 1;
				incMatrix[j][cnt] = 1;
			}
		}
	}
}
