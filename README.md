# 离散数学 II 实验二 (实验报告)

20020007095-叶鹏

盛艳秀-老师

## 实验题目

生成树、环路空间、断集空间的求解

## 实验目的

1. 掌握无向连通图生成树的求解方法
2. 掌握基本回路系统和环路空间的求解方法
3. 掌握基本割集系统和断集空间的求解方法
4. 了解生成树、环路空间和断集空间的实际应用

## 实验要求

1. 给定一无向简单连通图的相邻矩阵
2. 输出此图的关联矩阵 $M$
3. 求此图所有生成树个数
4. 输出其中任意一颗生成树的相邻矩阵和关联矩阵
5. 求此生成树对应的基本回路系统
6. 求此生成树对应的环路空间
7. 求此生成树对应的基本割集系统
8. 求此生成树对应的断集空间

## 实验内容和实验步骤

### 需求分析

1. 输入的形式和输入值的范围

   - 输入形式为：输入邻接矩阵的阶数 $n$ ，接着以矩阵的形式输入邻接矩阵
   - 输入值范围：因为使用可扩展数组，理论上 $n$ 没有限制，但是考虑到之后产生`index`序列的关系，`n <= 10`时程序最稳定

2. 输出的形式

   - 输出形式：
     - 相邻矩阵：默认第 $i$ 行对应顶点 $v_i$
     - 关联矩阵：默认第 $i$ 行对应顶点 $v_i$，第 $j$ 列对应边 $e_j$
     - 基本回路系统：输出形式如：${\{e1e4e3,e2e5e3\}}$
     - 环路空间：输出形式如：$ {\{Φ,e1e4e3,e2e5e3,e1e4e5e2\}}$
     - 基本割集系统：输出形式如： $\{\{e1,e4\},\{e2,e5\},\{e3,e4,e5\}\}$
     - 断集空间：输出形式如：$ \{ Φ ,\{e1,e4\},\{e2,e5\},\{e3,e4,e5\},\{e1,e2,e4,e5\},\{e1,e3,e5\},\{e2,e3,e4\},\{ e1,e2,e3\}\}$

3. 程序所能实现的功能

   给定一无向简单连通图的相邻矩阵，输出此图的关联矩阵 $M$，求所有生成树个数，输出其中任意一颗生成树的相邻矩阵和关联矩阵，求此生成树对应的基本回路系统、环路空间、基本割集系统、断集空间。

### 概要设计

- 考虑到给定`Matrix.h`头文件中矩阵参数为二维`vector`形式，我们也采用二维`vector`储存矩阵，计划定义一个`solution`类，所有的成员函数与成员变量都定义在类中

```c++
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
```

- 接着我们就在`main`函数中实例化类，并调用类的相关函数即可

### 详细设计

#### 输出关联矩阵 $M$

1. 读入邻接矩阵，根据邻接矩阵的性质，对角线任一侧`1`的数量既是边数`m`，接着打印出邻接矩阵。

```c++
//读入邻接矩阵函数
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

//打印矩阵函数
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
```

2. 根据之前的统计我们知道现在图中有`m`条边，那么我们就从`1`到`m`依次设置边连接的两个点，如果在邻接矩阵中`[i][j]`为`1`，则在关联矩阵中将 $e_{cnt}$ 的两个点设为 $i$ 和 $j$ ，然后输出此关联矩阵即可

```c++
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
```

#### 求所有生成树个数

1. 我们知道，可以用关联矩阵求生成树，求 $M_f$ 中的所有 $n-1$ 阶子方阵,计算行列式,行列式的值**模2非0**的是生成树，所以我们把我们的关联矩阵去掉一行，找出所有 $n-1$ 阶子方阵，送进`valueOfMatrix`计算矩阵的值(模2)，然后统计非0的个数，既是生成树个数的结果。保存最后一个非0的子方针，作为我们选中的一棵生成树，输出它的邻接矩阵以及关联矩阵，

```c++
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
```

#### 求生成树对应的基本回路系统

1. 求回路系统，先找到非生成树中的边，使用`dijkstra`算法求该边从一个顶点到另一个顶点的最短路径，然后保存此路径经过的边，这些边构成一条基本回路

```c++
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
```

#### 求生成树对应的环路空间

1. 环路空间就是求出的所有回路的合集以及 $n$ 条回路中 $2\sim n$ 条回路的对称差运算，再输出结果。这里我使用了`stl`库函数中的`unique`函数与`erase`函数实现 $n$ 条回路相加后结果的去重以及删除重复元素的运算，至于具体选择哪条回路运算，我使用了**“从n个数字选取k个数进行组合且符合升序”**算法生成一个索引序列`index`，`index`中储存下标，然后对回路集合中下标为`index`中的元素的回路进行相加以及对称差运算

```c++
//生成index序列
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
```

#### 求生成树对应的基本割集系统

1. 求基本割集系统，先找到生成树中的边，然后遍历非生成树中的边。根据**定理10.2的推论2**

​																$G$ 连通 ⇔ $r(M(G))=r(M_f(G))=n-1$

我们只要**判断生成树删去该边，再加上非生成树上的边 $e_i$ 构成的图的关联矩阵的雉是否为 $n-1$** ，便可判断出此图是否连通，若连通，则为一个割集系统。

```c++
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
```

#### 求生成树对应的断集空间

与求环路空间类似

```c++
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
```



## 

## 实验测试数据、代码及相关结果分析

### 源代码

```c++
//Matrix.h
#include <vector>

using namespace std;

// 传入的参数依次为行列式的阶数和行列式数组,，返回值为行列式的值
int valueOfMatrix(int n, vector<vector<int> >a)
{
    // 使用递归算法计算行列式的值
    if (n == 1)
    {
        return a[0][0];
    }
    else if(n == 2)
        return a[0][0]*a[1][1]-a[0][1]*a[1][0];
    else
    {
        int sum = 0;
        for(int k=0; k<n; k++)
        {
            vector<vector<int> > b;
            for(int i=1; i<n; i++)
            {
                vector<int>c;
                for(int j=0; j<n; j++)
                {
                    if(j == k)
                        continue;
                    c.push_back(a[i][j]);
                }
                b.push_back(c);
            }
            sum = k%2==0 ? sum+a[0][k]*valueOfMatrix(n-1, b) : sum-a[0][k]*valueOfMatrix(n-1, b);
        }
        return sum;
    }
}

// 传入的参数为行列式的阶数和行列式数组，返回值为矩阵的秩
int rankOfDeterminant(int n, vector<vector<int> > matrix)
{
    // 求二进制矩阵的秩，即消元，最后看斜对角线上有几个1
    int row = 0;
    for(int col=0; col < n && row < n; col++, row++)  // 从每一列开始，将每一列消到只有 1 个 1 或者全 0
    {
        int i = 0;
        for(i = row; i < n; ++i)  // 寻找这一列第一个非 0 的行
        {
            if(matrix[i][col] != 0)
                break;
        }
        if(n == i)
            --row;
        else
        {
            swap(matrix[row], matrix[i]);
            for(int k = i+1; k < n; k++)
            {
                if(0 != matrix[k][col])
                {
                    for(int j = col; j < n; j++)
                    {
                        matrix[k][j] ^= matrix[row][j];// 用第 r 行的 1 消除这一列上其余全部行的 1
                    }
                }
            }
        }
    }
    return row;
}

```



```c++
//main.cpp
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

```

### 测试数据

#### case 1

`input`

```
4
0 1 1 1
1 0 0 1
1 0 0 1
1 1 1 0
```

`output`

```

#==========读入邻接矩阵n

n = 4

#==========读入邻接矩阵

0 1 1 1
1 0 0 1
1 0 0 1
1 1 1 0

#==========打印邻接矩阵

index      v1   v2   v3   v4
   v1       0    1    1    1
   v2       1    0    0    1
   v3       1    0    0    1
   v4       1    1    1    0

#==========打印关联矩阵

index      e1   e2   e3   e4   e5
   v1       1    1    1    0    0
   v2       1    0    0    1    0
   v3       0    1    0    0    1
   v4       0    0    1    1    1

#==========计算生成树数量

共有8生成树

#===============================其中一棵生成树


#==========打印关联矩阵

index      e3   e4   e5
   v1       1    0    0
   v2       0    1    0
   v3       0    0    1
   v4       1    1    1

#==========打印邻接矩阵

index      v1   v2   v3   v4
   v1       0    0    0    1
   v2       0    0    0    1
   v3       0    0    0    1
   v4       1    1    1    0

#==========输出基本回路系统

{ e1e3e4, e2e3e5 }

#==========输出环路空间

{ Φ, { e1e3e4 }, { e2e3e5 }, { e1e2e4e5 } }

#==========输出基本割集系统

{ { e1, e2, e3 }, { e1, e4 }, { e2, e5 } }

#==========输出断集空间

{ Φ, { e1, e2, e3 }, { e1, e4 }, { e2, e5 }, { e2, e3, e4 }, { e1, e3, e5 }, { e1, e2, e4, e5 }, { e3, e4, e5 } }
```

#### case 2

`input`

```
5
0 1 1 0 1
1 0 1 0 1
1 1 0 1 0
0 0 1 0 1
1 1 0 1 0
```

`output`

```

#==========读入邻接矩阵n

n = 5

#==========读入邻接矩阵

0 1 1 0 1
1 0 1 0 1
1 1 0 1 0
0 0 1 0 1
1 1 0 1 0

#==========打印邻接矩阵

index      v1   v2   v3   v4   v5
   v1       0    1    1    0    1
   v2       1    0    1    0    1
   v3       1    1    0    1    0
   v4       0    0    1    0    1
   v5       1    1    0    1    0

#==========打印关联矩阵

index      e1   e2   e3   e4   e5   e6   e7
   v1       1    1    1    0    0    0    0
   v2       1    0    0    1    1    0    0
   v3       0    1    0    1    0    1    0
   v4       0    0    0    0    0    1    1
   v5       0    0    1    0    1    0    1

#==========计算生成树数量

共有24生成树

#===============================其中一棵生成树


#==========打印关联矩阵

index      e3   e5   e6   e7
   v1       1    0    0    0
   v2       0    1    0    0
   v3       0    0    1    0
   v4       0    0    1    1
   v5       1    1    0    1

#==========打印邻接矩阵

index      v1   v2   v3   v4   v5
   v1       0    0    0    0    1
   v2       0    0    0    0    1
   v3       0    0    0    1    0
   v4       0    0    1    0    1
   v5       1    1    0    1    0

#==========输出基本回路系统

{ e1e3e5, e2e3e6e7, e4e5e6e7 }

#==========输出环路空间

{ Φ, { e1e3e5 }, { e2e3e6e7 }, { e4e5e6e7 }, { e1e2e5e6e7 }, { e1e4e6e7 }, { e2e4 }, { e1e2e4 } }

#==========输出基本割集系统

{ { e1, e2, e3 }, { e1, e4, e5 }, { e2, e4, e6 }, { e2, e4, e7 } }

#==========输出断集空间

{ Φ, { e1, e2, e3 }, { e1, e4, e5 }, { e2, e4, e6 }, { e2, e4, e7 }, { e2, e3, e4, e5 }, { e1, e3, e4, e6 }, { e1, e2, e5, e6 }, { e1, e3, e4, e7 }, { e1, e2, e5, e7 }, { e6, e7 }, { e3, e5, e6 }, { e3, e5, e7 }, { e1, e3, e6, e7 }, { e1, e5, e6, e7 }, { e3, e5, e6, e7 } }
```

#### case 3

`input`

```
6
0 1 1 0 0 0
1 0 1 0 0 0
1 1 0 1 0 0
0 0 1 0 1 1
0 0 0 1 0 1
0 0 0 1 1 0
```

`output`

```

#==========读入邻接矩阵n

n = 6

#==========读入邻接矩阵

0 1 1 0 0 0
1 0 1 0 0 0
1 1 0 1 0 0
0 0 1 0 1 1
0 0 0 1 0 1
0 0 0 1 1 0

#==========打印邻接矩阵

index      v1   v2   v3   v4   v5   v6
   v1       0    1    1    0    0    0
   v2       1    0    1    0    0    0
   v3       1    1    0    1    0    0
   v4       0    0    1    0    1    1
   v5       0    0    0    1    0    1
   v6       0    0    0    1    1    0

#==========打印关联矩阵

index      e1   e2   e3   e4   e5   e6   e7
   v1       1    1    0    0    0    0    0
   v2       1    0    1    0    0    0    0
   v3       0    1    1    1    0    0    0
   v4       0    0    0    1    1    1    0
   v5       0    0    0    0    1    0    1
   v6       0    0    0    0    0    1    1

#==========计算生成树数量

共有9生成树

#===============================其中一棵生成树


#==========打印关联矩阵

index      e2   e3   e4   e6   e7
   v1       1    0    0    0    0
   v2       0    1    0    0    0
   v3       1    1    1    0    0
   v4       0    0    1    1    0
   v5       0    0    0    0    1
   v6       0    0    0    1    1

#==========打印邻接矩阵

index      v1   v2   v3   v4   v5   v6
   v1       0    0    1    0    0    0
   v2       0    0    1    0    0    0
   v3       1    1    0    1    0    0
   v4       0    0    1    0    0    1
   v5       0    0    0    0    0    1
   v6       0    0    0    1    1    0

#==========输出基本回路系统

{ e1e2e3, e5e6e7 }

#==========输出环路空间

{ Φ, { e1e2e3 }, { e5e6e7 }, { e1e2e3e5e6e7 } }

#==========输出基本割集系统

{ { e1, e2 }, { e1, e3 }, { e4 }, { e5, e6 }, { e5, e7 } }

#==========输出断集空间

{ Φ, { e1, e2 }, { e1, e3 }, { e4 }, { e5, e6 }, { e5, e7 }, { e2, e3 }, { e1, e2, e4 }, { e1, e3, e4 }, { e1, e2, e5, e6 }, { e1, e3, e5, e6 }, { e4, e5, e6 }, { e1, e2, e5, e7 }, { e1, e3, e5, e7 }, { e4, e5, e7 }, { e6, e7 }, { e2, e3, e4 }, { e2, e3, e5, e6 }, { e1, e2, e4, e5, e6 }, { e1, e3, e4, e5, e6 }, { e2, e3, e5, e7 }, { e1, e2, e4, e5, e7 }, { e1, e3, e4, e5, e7 }, { e1, e2, e6, e7 }, { e1, e3, e6, e7 }, { e4, e6, e7 }, { e2, e3, e4, e5, e6 }, { e2, e3, e4, e5, e7 }, { e2, e3, e6, e7 }, { e1, e2, e4, e6, e7 }, { e1, e3, e4, e6, e7 }, { e2, e3, e4, e6, e7 } }
```



## 实验总结

本次实验感觉到难度较大，主要困难都出现在编程实现的部分，用到了一些算法比如说`dijkstra`求最短路径，从n个数字选取k个数进行组合且符合升序来创建`index`值，数组的去重、合并、排序。在锻炼代码能力的过程中也巩固复习了离散数学的知识，对图的矩阵表示尤其是邻接矩阵与关联矩阵在图论中的性质有了更深刻的认识，比如说基本关联矩阵可以判断图是否连通，求生成树、学习到了邻接矩阵与关联矩阵之间的相互转化。比起上一次实验采用静态二维数组，本次使用了动态二维数组`vector`来储存矩阵，体验到了`vector`本身封装的函数的强大之处，受益匪浅。

