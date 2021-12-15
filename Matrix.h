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
