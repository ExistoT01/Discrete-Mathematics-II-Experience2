#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include "Matrix.h"
#include <bitset>
using namespace std;

const int maxn = 50;


class solution {
public:
	//变量
	vector<vector<int>> adjMatrix;//邻接矩阵
	vector<vector<int>> incMatrix;//关联矩阵
	vector<vector<int>> spaAdjMatrix;//生成树邻接矩阵
	int eList[maxn];
	int eListCNT;
	int n;//顶点数
	int m;//边数
	int numOfSpanningTree;
	vector<vector<int>> basedLoopSpace;//环路空间
	vector<vector<int>> fragmentationSpace;//断集空间
	int edgeList[maxn] = { 0 };//边集,1为生成树的边，0为剩余边
	int numOfBasedLoop;//环路数
	int numOfFragmentation;//断集数
	int numOfIndex;
	vector<vector<int>> myIndex;

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
		numOfIndex = 0;
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
	int getEdge(int v1, int v2);
	void generateIndex(int n);
};

int main()
{
	solution method;

	method.setN();
	method.setAdjMatrix();
	method.InitalIncMatrix();

	method.displayMatrix(method.incMatrix, method.n, method.m, 1);

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
		if (i != 1)
			start *= 10;
		j++;
		i--;
	}

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

	int cnt = 0;

	for (int i = start; i <= end; i++)
	{
		if (checkNumOK(i))
			eList[++cnt] = i;
	}

	eListCNT = cnt;

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
	int eListNum = 0;
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

		for (int i = 1; i <= n; i++)
		{
			for (int j = 1; j <= n - 1; j++)
			{
				spanningTree[i][j] = incMatrix[i][list[j]];
			}
		}

		if (valueOfMatrix(n - 1, toDeterminant(spanningTree)) % 2) {
			numOfSpanningTree++;

			samMatrix.assign(spanningTree.begin(), spanningTree.end());

			for (int j = 1; j <= n - 1; j++) {
				samList[j] = list[j];
			}

		}

	}

	cout << endl;
	cout << "#==========计算生成树数量" << endl;
	cout << endl;
	cout << "共有" << numOfSpanningTree << "生成树" << endl;

	cout << endl;
	cout << "#===============================其中一棵生成树" << endl;
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

		for (int j = 1; j <= n - 1; j++)
		{
			printf("%5d", samMatrix[i][j]);
		}
		cout << endl;
	}

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
		for (int j = 1; j <= n - 1; j++)
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

	//求解
	for (int index = 1; index <= m; index++)
	{

		//如果是生成树中的边
		if (edgeList[index]) {

			numOfFragmentation++;
			vector<int> fragmentation;
			fragmentation.push_back(index);

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
							if ((edgeList[j] == 1 && j != index) || j == i)
								tem.push_back(incMatrix[ii][j]);
						}

						samMatrix.push_back(tem);
					}

					//G连通  <=> r(M(G))=r(Mf(G))=n-1. 
					if (rankOfDeterminant(n - 1, samMatrix) == n - 1) {
						fragmentation.push_back(i);
					}

				}

			}

			sort(fragmentation.begin(), fragmentation.end());
			fragmentationSpace.push_back(fragmentation);
		}

		//如果是剩余的边
		else {

			numOfBasedLoop++;
			vector<int> basedLoop;

			vector<int> list;

			//先找出连接的两个点
			int v1, v2;
			v1 = v2 = 0;

			samMatrix.clear();
			samMatrix.assign(incMatrix.begin(), incMatrix.end());

			for (int j = 1; j <= m; j++)
			{
				if (edgeList[j] == 0 && j != index) {
					for (int i = 1; i <= n; i++)
					{
						samMatrix[i][j] = 0;
					}
				}
			}

			for (int i = 1; i <= n; i++)
			{
				if (samMatrix[i][index]) {
					if (v1 == 0) {
						v1 = i;
					}
					else
					{
						v2 = i;
					}
				}
			}

			//设置vis数组遍历
			bool vis[maxn] = { 0 };
			int dis[maxn] = { 0 };
			int parent[maxn] = { 0 };
			for (int i = 1; i <= n; i++)
			{
				vis[i] = false;
				dis[i] = 0x3f3f3f;
				parent[i] = -1;
			}

			int cur = v1;
			dis[cur] = 0;
			while (vis[v2] != true)
			{
				vis[cur] = true;

				int minDis = 0x3f3f3f;
				int nextCur = -1;
				for (int i = 1; i <= n; i++)
				{
					if (spaAdjMatrix[cur][i] && !vis[i]) {
						dis[i] = min(dis[i], dis[cur] + 1);

						if (dis[i] < minDis) {
							minDis = dis[i];
							nextCur = i;
						}
					}
				}

				if (nextCur == -1) {
					cur = parent[cur];
				}

				else {
					parent[nextCur] = cur;
					cur = nextCur;
				}
			}

			cur = v2;
			while (cur != v1) {
				list.push_back(cur);
				cur = parent[cur];
			}
			list.push_back(v1);

			for (int i = 0; i < list.size(); i++)
			{

				if (i == list.size() - 1) {
					basedLoop.push_back(getEdge(list[i], list[0]));
				}

				else {
					basedLoop.push_back(getEdge(list[i], list[i + 1]));

				}
			}

			sort(basedLoop.begin(), basedLoop.end());
			basedLoopSpace.push_back(basedLoop);
		}
	}


	//输出

	cout << endl;
	cout << "#==========输出基本回路系统" << endl;
	cout << endl;

	cout << "{ ";
	for (int i = 0; i < numOfBasedLoop; i++)
	{
		for (int j = 0; j < basedLoopSpace[i].size(); j++)
		{
			cout << "e" << basedLoopSpace[i][j];
		}
		if (i != numOfBasedLoop - 1)
			cout << ", ";
	}
	cout << " }" << endl;

	//环路空间

	cout << endl;
	cout << "#==========输出环路空间" << endl;
	cout << endl;

	cout << "{ Φ, ";

	for (int i = 0; i < numOfBasedLoop; i++)
	{
		cout << "{ ";
		for (int j = 0; j < basedLoopSpace[i].size(); j++)
		{
			cout << "e" << basedLoopSpace[i][j];
		}
		cout << " }, ";
	}

	generateIndex(numOfBasedLoop);

	vector<int> tem;
	vector<int> sameItem;

	for (int i = 0; i < numOfIndex; i++)
	{
		vector<int>().swap(tem);
		for (int j = 0; j < myIndex[i].size(); j++)
		{
			tem.insert(tem.end(),
				basedLoopSpace[myIndex[i][j]].begin(),
				basedLoopSpace[myIndex[i][j]].end());
		}

		sort(tem.begin(), tem.end());
		for (vector<int>::iterator it = tem.begin() + 1; it < tem.end(); it++)
		{
			if (*it == *(it - 1))
				sameItem.push_back(*it);
		}

		vector<int>::iterator itEnd = unique(tem.begin(), tem.end());		
		tem.erase(itEnd, tem.end());

		itEnd = unique(sameItem.begin(), sameItem.end());
		sameItem.erase(itEnd, sameItem.end());

		vector<int>::iterator ptr;
		for (vector<int>::iterator it = sameItem.begin(); it < sameItem.end(); it++)
		{
			ptr = find(tem.begin(), tem.end(), *it);
			if (ptr != tem.end())
				tem.erase(ptr);
		}

		cout << "{ ";
		for (int j = 0; j < tem.size(); j++)
		{
			cout << "e" << tem[j];
		}
		cout << " }";
		if (i != numOfIndex - 1)
			cout << ", ";
	}

	cout << " }" << endl;


	cout << endl;
	cout << "#==========输出基本割集系统" << endl;
	cout << endl;

	cout << "{ ";
	for (int i = 0; i < numOfFragmentation; i++)
	{
		cout << "{ ";
		for (int j = 0; j < fragmentationSpace[i].size(); j++)
		{
			cout << "e" << fragmentationSpace[i][j];
			if (j != fragmentationSpace[i].size() - 1)
				cout << ", ";
		}

		cout << " }";
		if (i != numOfFragmentation - 1)
			cout << ", ";
	}
	cout << " }" << endl;

	cout << endl;
	cout << "#==========输出断集空间" << endl;
	cout << endl;

	cout << "{ Φ, ";

	for (int i = 0; i < numOfFragmentation; i++)
	{
		cout << "{ ";
		for (int j = 0; j < fragmentationSpace[i].size(); j++)
		{
			cout << "e" << fragmentationSpace[i][j];
			if (j != fragmentationSpace[i].size() - 1)
				cout << ", ";
		}

		cout << " }, ";
	}

	generateIndex(numOfFragmentation);

	for (int i = 0; i < numOfIndex; i++)
	{
		vector<int>().swap(tem);
		for (int j = 0; j < myIndex[i].size(); j++)
		{
			tem.insert(tem.end(),
				fragmentationSpace[myIndex[i][j]].begin(),
				fragmentationSpace[myIndex[i][j]].end());
		}


		sort(tem.begin(), tem.end());

		vector<int>().swap(sameItem);
		for (vector<int>::iterator it = tem.begin() + 1; it < tem.end(); it++)
		{
			if (*it == *(it - 1))
				sameItem.push_back(*it);
		}

		vector<int>::iterator itEnd = unique(tem.begin(), tem.end());
		tem.erase(itEnd, tem.end());

		itEnd = unique(sameItem.begin(), sameItem.end());
		sameItem.erase(itEnd, sameItem.end());

		vector<int>::iterator ptr;
		for (vector<int>::iterator it = sameItem.begin(); it < sameItem.end(); it++)
		{
			ptr = find(tem.begin(), tem.end(), *it);
			if (ptr != tem.end())
				tem.erase(ptr);
		}

		cout << "{ ";
		for (int j = 0; j < tem.size(); j++)
		{
			cout << "e" << tem[j];
			if (j != tem.size() - 1)
				cout << ", ";
		}
		cout << " }";
		if (i != numOfIndex - 1)
			cout << ", ";
	}

	cout << " }" << endl;

}

int solution::getEdge(int v1, int v2)
{
	for (int j = 1; j <= m; j++)
	{
		if (incMatrix[v1][j] && incMatrix[v2][j])
			return j;
	}

	return 0;
}

void solution::generateIndex(int n)
{
	vector<vector<int>>().swap(myIndex);
	numOfIndex = 0;

	for (int k = 2; k <= n; k++)
	{
		long long comb = (1 << k) - 1;

		while (comb < (1 << n)) {
			vector<int> tem;
			bitset<8> bs(comb);
			//cout << bs << endl;
			for (int i = 0; i < n; i++)
			{
				if (bs[i] == 1)
					tem.push_back(i);
			}
			int x = comb & -comb, y = comb + x;
			comb = ((comb & ~y) / x >> 1) | y;

			numOfIndex++;

			myIndex.push_back(tem);
		}

	}
}
