#include <iostream>
#include <vector>
#include <string>
#include "Matrix.h"
using namespace std;

const int maxn = 50;


class solution {
public:
	//变量
	vector<vector<int>> adjMatrix;//邻接矩阵
	vector<vector<int>> incMatrix;//关联矩阵
	int eList[maxn];
	int eListCNT;
	int n;//顶点数
	int m;//边数
	int numOfSpanningTree;
	string basedLoopSpace[maxn];//环路空间
	string fragmentationSpace[maxn];//断集空间
	int edgeList[maxn] = { 0 };//边集,1为生成树的边，0为剩余边
	int numOfBasedLoop;//环路数
	int numOfFragmentation;//断集数


	//函数

	//constructor
	solution()
	{
		this->n = 0;
		this->m = 0;
		eListCNT = 0;
		numOfSpanningTree = 0;
		numOfBasedLoop = 0;
		numOfFragmentation = 0;
		basedLoopSpace[0] = "";
		fragmentationSpace[0] = "";

	}

	solution(int n) {
		this->n = n;
		this->m = 0;
		eListCNT = 0;
		numOfSpanningTree = 0;
		numOfBasedLoop = 0;
		numOfFragmentation = 0;
	}

	void setN();
	void setAdjMatrix();
	void displayMatrix(vector<vector<int>> matrix, int row, int col, int type);//打印参数矩阵,type表示类型,0为邻接矩阵,1为关联矩阵
	void InitalIncMatrix();//关联矩阵初始化
	bool checkNumOK(int num);
	void buildEList();//生成生成树序列
	void calcSpanningTree();//求生成树
	vector<vector<int>> toDeterminant(vector<vector<int>> matrix);
	vector<vector<int>> toDeterminant(vector<vector<int>> matrix, int row, int col);
	void calcSpace();//求环路空间与断集空间
};

int main()
{
	solution method;

	method.setN();
	method.setAdjMatrix();
	method.InitalIncMatrix();

	method.displayMatrix(method.incMatrix,method.n,method.m, 1);

	method.calcSpanningTree();

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

bool solution::checkNumOK(int num)
{

	int list[maxn] = { 0 };
	int cnt = 0;
	int i = n - 1;
	while (num)
	{
		cnt++;
		list[i] = num % 10;
		num /= 10;
		i--;
	}
	for (int i = 1; i <= cnt; i++)
	{
		if (list[i] > m)
			return false;
		if (list[i] <= list[i - 1])
			return false;
	}
	return true;
}

void solution::buildEList()
{
	int start, end;
	start = end = 0;
	int i = n - 1;
	int j = 1;

	while (i)
	{
		start += j;
		if(i != 1)
			start *= 10;
		j++;
		i--;
	}

	//cout << "start = " << start << endl;

	i = n - 1;
	j = m;
	int base = 1;

	while (i)
	{
		end += j * base;
		base *= 10;
		j--;
		i--;
	}

	//cout << "end = " << end << endl;

	int cnt = 0;

	for (int i = start; i <= end; i++)
	{
		if (checkNumOK(i))
			eList[++cnt] = i;
	}

	eListCNT = cnt;

	//for (int i = 1; i <= cnt; i++)
	//{
	//	cout << eList[i] << " ";
	//}


}




void solution::calcSpanningTree()
{
	vector<vector<int>> spanningTree;

	vector<vector<int>> samMatrix(spanningTree);
	int* samList = new int[maxn];

	//初始化tem
	for (int i = 0; i <= n; i++)
	{
		vector<int> tem;
		for (int j = 0; j <= n - 1; j++)
		{
			tem.push_back(0);
		}
		spanningTree.push_back(tem);
	}

	buildEList();

	//关联矩阵求最小生成树,删除v1
	int list[maxn];
	int eListNum;
	for (int index = 1; index <= eListCNT; index++)
	{
		eListNum = eList[index];
		int j = n - 1;
		while (eListNum)
		{
			list[j] = eListNum % 10;
			j--;
			eListNum /= 10;
		}


		//for (int k = 1; k <= n-1; k++)
		//{
		//	cout << list[k] << " ";
		//}

		for (int i = 1; i <= n; i++)
		{
			for (int j = 1; j <= n-1; j++)
			{
				spanningTree[i][j] = incMatrix[i][list[j]];
			}
		}
		//displayMatrix(spanningTree, 4, n - 1, 1);
		//cout << valueOfMatrix(n - 1, toDeterminant(spanningTree)) << " " << endl;
		if (valueOfMatrix(n - 1, toDeterminant(spanningTree)) % 2) {
			numOfSpanningTree++;

			samMatrix.assign(spanningTree.begin(), spanningTree.end());
			
			for (int j = 1; j <= n - 1; j++) {
				samList[j] = list[j];
			}
			//cout << numOfSpanningTree << " " << endl;
		}
			
	}

	cout << endl;
	cout << "#==========计算生成树数量" << endl;
	cout << endl;	
	cout << "共有" << numOfSpanningTree << "生成树" << endl;
	
	cout << endl;
	cout << "#============其中一棵生成树" << endl;
	cout << endl;

	cout << endl;
	cout << "#==========打印关联矩阵" << endl;
	cout << endl;
	printf("%-5s\t", "index");
	for (int j = 1; j <= n - 1; j++) {
		printf("%4c%d", 'e', samList[j]);
		edgeList[samList[j]] = 1;
	}
	cout << endl;

	for (int i = 1; i <= n; i++)
	{
		printf("%4c%d\t", 'v', i);

		for (int j = 1; j <= n-1; j++)
		{
			printf("%5d", samMatrix[i][j]);
		}
		cout << endl;
	}

	vector<vector<int>> spaAdjMatrix;

	for (int i = 0; i <= n; i++)
	{
		vector<int> tem;
		for (int j = 0; j <= n; j++)
		{
			tem.push_back(0);
		}
		spaAdjMatrix.push_back(tem);
	}

	int v1, v2;
	
	for (int j = 1; j <= n - 1; j++)
	{
		v1 = v2 = 0;
		for (int i = 1; i <= n; i++)
		{
			if (samMatrix[i][j])
			{
				if (!v1)
					v1 = i;
				else
					v2 = i;
			}
				
		}
		spaAdjMatrix[v1][v2] = 1;
		spaAdjMatrix[v2][v1] = 1;
	}

	displayMatrix(spaAdjMatrix, n, n, 0);

	//开始求环路空间与断集空间
	calcSpace();


}

vector<vector<int>> solution::toDeterminant(vector<vector<int>> matrix)
{
	vector<vector<int>> ans;
	for (int i = 2; i <= n; i++)
	{
		vector<int> tem;
		for (int j = 1; j <= n-1; j++)
		{
			tem.push_back(matrix[i][j]);
		}
		ans.push_back(tem);
	}
	return ans;
}

vector<vector<int>> solution::toDeterminant(vector<vector<int>> matrix, int row, int col)
{
	vector<vector<int>> ans;
	for (int i = 1; i <= row; i++)
	{
		vector<int> tem;
		for (int j = 1; j <= col; j++)
		{
			tem.push_back(matrix[i][j]);
		}
		ans.push_back(tem);
	}
	return ans;
}

void solution::calcSpace()
{
	vector<vector<int>> samMatrix;
	int cnt = 0;
	int v[maxn] = { 0 };
	for (int index = 1; index <= m; index++)
	{
		
		//如果是生成树中的边
		if (edgeList[index]) {
			numOfFragmentation++;
			char* tem = new char[maxn];
			_itoa(index, tem, 10);
			fragmentationSpace[numOfBasedLoop] = tem;

			//找不是生成树中的边
			for (int i = 1; i <= m; i++)
			{
				if (edgeList[i] == 0) {
					samMatrix.clear();
					//如果加上这条边，图连通，就放到割集里
					for (int ii = 2; ii <= n; ii++)//默认删除第二行
					{
						vector<int> tem;
						for (int j = 1; j <= m; j++)
						{
							if ((edgeList[j] == 1 && j != index) || j == i) {
								tem.push_back(incMatrix[ii][j]);
								//cout << incMatrix[ii][j] << " ";
							}

						}
						//cout << endl;
						samMatrix.push_back(tem);
					}

					//for (int iii = 0; iii < n-1; iii++)
					//{
					//	for (int jjj = 0; jjj < n-1; jjj++)
					//	{
					//		cout << samMatrix[iii][jjj] << " ";
					//	}
					//	cout << endl;
					//}

					//cout << "*" << rankOfDeterminant(n - 1, samMatrix) << endl;

					//G连通  <=> r(M(G))=r(Mf(G))=n-1. 
					if (rankOfDeterminant(n - 1, samMatrix) == n - 1) {
						_itoa(i, tem, 10);
						fragmentationSpace[numOfBasedLoop] += tem;
					}

				}

				
			}

			
			cout << fragmentationSpace[numOfBasedLoop] << endl;








			/*cnt = 0;
			int target;
			bool isTargerFind = false;*/

			////找到与其他点都无边的点
			//for (int row = 1; row <= n && !isTargerFind; row++)
			//{
			//	for (int col = 1; col <= m; col++)
			//	{
			//		if (edgeList[col]) {
			//			if (col != index) {
			//				v[row] += incMatrix[row][col];
			//			}
			//		}
			//	}
			//	if (v[row] == 0) {
			//		target = row;
			//		isTargerFind = true;
			//	}
			//}

			//for (int i = 1; i <= m; i++)
			//{
			//	if (incMatrix[target][i] && i != index) {
			//		_itoa(i, tem, 10);
			//		fragmentationSpace[numOfBasedLoop] += tem;
			//	}
			//}
			//cout << "index = " << index << endl;
			//cout << fragmentationSpace[numOfBasedLoop];

		}
		//如果是剩余的边
		else {

		}
	}
}
