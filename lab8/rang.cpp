#include <cstdio>

class TTestSet {
public:
	double *string;
	int cost;
	int pos;
	bool operator > (const TTestSet& a) const {
		return cost > a.cost;
	}
};

template <class T> void insertHeap(T* a,int N){
	int i = 0;
	int r, l, cur, next;
	while (i < N) {
		l = 2 * ( i + 1 ) - 1;
		r = 2 * ( i + 1 );
		next = i;
		if (r < N) {
			if (a[l] > a[r]) {
				cur = l;
			}
			else {
				cur = r;
			}
			if (a[cur] > a[i]) {
				T tmp = a[i];
				a[i] = a[cur];
				a[cur] = tmp;
				next = cur;
			}
		}else if ((l < N) && (a[l] > a[i])) {
				T tmp = a[i];
				a[i] = a[l];
				a[l] = tmp;
				next = l;
		}
		if (i == next) {
			break;
		} else {
			i = next;
		}
	}
}

template <class T> void makeHeap(T* a,int N){
	for (int i = N - 1;i > 0;i--)  {
		insertHeap(a+i,N-i);
	}
}

template <class T> void heapSort(T* a,int N){
	makeHeap(a,N);
	T tmp;
	for (int i = 0;i < N - 1;i++){
		insertHeap(a,N - i);
		tmp = a[N - i - 1];
		a[N - i - 1] = a[0];
		a[0] = tmp;
	}
}

bool IsZero (TTestSet* str, int n) {
	bool res = true;
	for (int i = 0; i < n;++i) {
		if (str->string[i] != 0) {
			res = false;
			break;
		}
	}
	return res;
}

int FindBadString(int n, TTestSet **matrix) { // 
	for (int cur = 0; cur < n; ++cur) {
		int temp = cur;
		for ( ; temp < n && matrix[temp]->string[cur] == 0; ++temp); // look for non zero element
		if (temp < n) { 
			if (temp != cur) { // shift lines down
				TTestSet *tempLine = matrix[temp];
				for (int j = temp; j > cur; j--) {
					matrix[j] = matrix[j - 1];
				}
				matrix[cur] = tempLine;
			}
			for (int j = cur + 1; j < n; ++j) {
				if (matrix[j]->string[cur] == 0) {
					continue;
				}
				double k = matrix[j]->string[cur] / matrix[cur]->string[cur];
				matrix[j]->string[cur] = 0;
				for (int l = cur + 1; l < n; ++l) {
					matrix[j]->string[l] -= matrix[cur]->string[l] * k;
				}
			}
		}
		else {
			int maxCost = cur;
			for (int i = cur; i < n; ++i) {
//				if (IsZero(matrix[i], n)) {
//					return i + 1;
//				}
				if (matrix[i]->cost > matrix[maxCost]->cost) {
					maxCost = i;
				}
			}
			return maxCost + 1;
		}
	}
	return 0;
}

int main() {
    int n,m;
    scanf("%d %d",&n,&m);
    TTestSet *set = new TTestSet [n];
    for (int i = 0;i < n;++i) {
    	set[i].string = new double [m];
    	set[i].pos = i + 1;
        for (int j = 0;j < m;++j) {
            scanf("%lf",set[i].string + j);
        }
        scanf("%d",&(set[i].cost));
    }
    heapSort(set, n);
    TTestSet **optimalSet = new TTestSet*[n];
    for (int i = 0; i < m; ++i) {
    	optimalSet[i] = &(set[i]);
    }
    int cur = m;
    while (cur - 1 < n) {
    	int badString = FindBadString(m, optimalSet);
    	if (!badString) {
    		break;
    	}
    	optimalSet[badString - 1] = &(set[cur++]);
    }
    if (cur > n) {
    	printf("-1\n");
    } else {
    	int* ans = new int [m];
    	for (int i = 0;i < m; ++i) {
    		ans[i] = optimalSet[i]->pos;
    	}
    	heapSort(ans, m);
    	for (int i = 0; i < m - 1; ++i) {
    		printf("%d ",ans[i]);
    	}
    	printf("%d\n",ans[m - 1]);
    	delete [] ans;
    }
    delete [] set;
    delete [] optimalSet;
    return 0;
}
