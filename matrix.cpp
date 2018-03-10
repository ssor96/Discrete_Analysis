#include <cstdio>

int main() {
    int n,m;
    scanf("%d %d",&n,&m);
    long long int** matrix = new long long int*[n];
    char** res = new char*[n];
    for (int i = 0; i < n;++i) {
        res[i] = new char[m];
        matrix[i] = new long long int[m];
        for (int j = 0;j < m;++j) {
            scanf("%lld",matrix[i] + j);
        }
    }
/*
    for (int i = 0; i < n;++i) {
        for (int j = 0;j < m;++j) {
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
*/
    for (int i = n - 2;i >= 0;--i) {
        int best;
        if (matrix[i + 1][0] < matrix[i + 1][1]) {
            best = 0;
        }
        else {
            best = 1;
        }
        matrix[i][0] += matrix[i + 1][best];
        res[i][0] = best;
        for (int j = 1; j < m - 1;++j) {
            if (matrix[i + 1][j - 1] < matrix[i + 1][j]) {
                best = -1;
            }
            else {
                best = 0;
            }
            if (matrix[i + 1][j + 1] < matrix[i + 1][j + best]) {
                best = 1;
            }
            matrix[i][j] += matrix[i + 1][j + best];
            res[i][j] = best;
        }
        if (matrix[i + 1][m - 1] < matrix[i + 1][m - 2]) {
            best = 0;
        }
        else {
            best = -1;
        }
        matrix[i][m - 1] += matrix[i + 1][m - 1 + best];
        res[i][m - 1] = best;
    }
    int max = 0;
    for (int i = 1; i < m;++i) {
        if (matrix[0][i] < matrix[0][max]) {
            max = i;
        }
    }
    int cur = max;
    printf("%lld\n",matrix[0][cur]);
    for (int i = 0; i < n - 1;++i) {
        printf("(%d,%d) ",i + 1,cur + 1);
        cur += res[i][cur];
        delete [] matrix[i];
        delete [] res[i];
    }
    delete [] matrix[n - 1];
    delete [] res[n-1];
    delete [] res;
    delete [] matrix;
    printf("(%d,%d)\n",n,cur + 1);
    return 0;
}
