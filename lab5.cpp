#include <cstdio>
#include <map>
#include <cstring>

template <class T> class TVector {

private:
    static const int MinCapacity = 4;
    int Size;
    int Capacity;
    T *Data;
    int GetOptimalCapacity (int newCap,int oldCap) {
        int res = oldCap;
        while (res < newCap) {
            res *= 2;
        }
        while ( (res / 2 > newCap) && (res > MinCapacity) ) {
            res /= 2;
        }
        return res;
    }

public:
    T& operator [] (int idx) const {
        if (idx >= Size || idx < 0) {
            printf("LOL: idx = %d ; size = %d\n",idx,Size );
        }
        return Data[idx];
    }
    T* Begin() const {
        return Data;
    }
    void operator = (TVector <T>& vector) {
        Resize(vector.GetSize());
        memcpy(Data,vector.Begin(),Size * sizeof(T));
    }
    TVector () {
        Size = 0;
        Capacity = MinCapacity;
        Data = new T [Capacity];
    }
    void PushBack (T value){
        Resize (Size + 1);
        Data[Size - 1] = value;
    }
    int GetSize () const{
        return Size;
    }
    void Resize (int newSize) {
        int optimalCap = GetOptimalCapacity (newSize,Capacity);
        if (optimalCap > Capacity){
            T *tmp = new T [optimalCap];
            for (int i = 0;i < Capacity;i++) {
                tmp[i] = Data[i];
            }
            Capacity = optimalCap;
            delete [] Data;
            Data = tmp;
        } else if (optimalCap<Capacity) {
            T *tmp = new T [optimalCap];
            for (int i = 0;i < newSize;i++) {
                tmp[i] = Data[i];
            }
            Capacity = optimalCap;
            delete [] Data;
            Data = tmp;
        }
        Size = newSize;
    }
    ~TVector () {
        delete [] Data;
        Capacity = Size = 0;
    }
};

typedef unsigned long long int TInt;

class TSuffTree {
private:
	class TSuffNode {
	public:
//		std::map <char, TSuffNode*> next;
		TSuffNode* next[26];
		TSuffNode* link;
		bool leaf;
		TInt left;
		TInt right;
		TSuffNode(bool tleaf, TInt tleft, TInt tright, TSuffNode* tlink) {
			leaf = tleaf;
			left = tleft;
			right = tright;
			link = tlink;
			memset(next, 0, 26 * sizeof(TSuffNode*));
		}
		TSuffNode* getNext(char c) {
//			TSuffNode* res = next.find(c) -> second;
//			return res;
			return next[c - 'a'];
		}
		void add(char c, TSuffNode* value) {
//			next[c] = value;
			next[c - 'a'] = value;
		}
		bool check(char c) {
			return next[c - 'a'];
		}
		TInt len() {
			return right - left;
		}
		TSuffNode* getMin() {
			for (int i = 0; i < 26;++i) {
				if (next[i]) {
					return next[i];
				}
			}
			return NULL;
		}
	};

	TSuffNode* dummy;
	TSuffNode* iter;
	TInt cur;
	TVector <char> text;
	TInt pathLen;
	TSuffNode* root;

	TSuffNode* split() {
		TSuffNode* child = iter -> getNext(text[cur - pathLen - 1]);
		TSuffNode* tmp = new TSuffNode(false, child -> left, child -> left + pathLen, NULL);
		iter -> add(text[cur - pathLen - 1], tmp);
		child -> left += pathLen;
		tmp -> add(text[child -> left], child);
		return tmp;
	}
	TSuffNode* getNew(char c) {
		if (pathLen == 0) {
			return iter -> check(c)?NULL:iter;
		}
		return text[iter -> getNext(text[cur - pathLen - 1]) -> left + pathLen] == c? NULL: split();
	}
	void go(TInt from, TInt len) {
		TSuffNode* tmp;
		while (len) {
			tmp = iter -> getNext(text[from]);
			if (tmp -> len() > len || tmp -> leaf) {
				pathLen = len;
				break;
			}
			len -= tmp -> len();
			from += tmp -> len();  
			iter = tmp;
		}
		if (len == 0) pathLen = 0;
	}
public:
	TSuffTree() {
		dummy = new TSuffNode(false, 0, 0, NULL);
		root = new TSuffNode(false, 0, 1, dummy);
		for (char c = 'a'; c <= 'z'; ++c) {
			dummy -> add(c, root);
		}
		iter = root;
		pathLen = 0;
		cur = 0;
	}
	void add(char c) {
		text.PushBack(c);
		cur++;
		TSuffNode* prev = dummy;
		TSuffNode* tmp;
		while((tmp = getNew(c)) != NULL) {
			tmp -> add(c, new TSuffNode (true, cur - 1, cur, NULL));
			prev -> link = tmp;
			prev = tmp;
			iter = iter -> link;
			go(prev -> left, pathLen);
		}
		prev -> link = iter;
		go(cur - pathLen - 1, pathLen + 1);
	}
	void doubleStr() {
		TInt size = cur;
		for (TInt i = 0; i < size; ++i) {
			add(text[i]);
		}
	}
	void printMin() {
		TInt size = cur / 2;
		TSuffNode* tmp = root;
		while (size) {
			tmp = tmp -> getMin();
			TInt beg = tmp -> left;
			if (size < tmp -> len() || tmp -> leaf) {
				for (TInt i = 0; i < size; ++i) {
					printf("%c",text[beg + i]);
				}
				break;
			}
			else {
				for (TInt i = beg; i < tmp -> right; ++i) {
					printf("%c",text[i]);
				}
				size -= tmp -> len();
			}
		}
		printf("\n");
	}
};

int main() {
    char c;
    TSuffTree tree;
    while((c = getchar()) != '\n' && c != EOF) {
        tree.add(c);
    }
    tree.doubleStr();
    tree.printMin();
    return 0;
}