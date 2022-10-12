#include<iostream>
#include<vector>
using namespace std;
#define ty  long long int
#define lli long long int
vector<ty> segment_tree;
vector<ty> tag;
vector<ty> arr;
vector<ty> min1;
void build(lli id, lli L, lli R) {
    if (L == R) {
        segment_tree[id]= arr[L];
        min1[id] = arr[L];
        return;
    }
    lli M = (L + R) / 2;
    build(id*2, L, M);
    build(id*2+1,M+1, R);
    min1[id] = min(min1[id*2], min1[id * 2+1]);
    segment_tree[id] = segment_tree[id * 2] + segment_tree[id * 2 + 1];
}
//id=1,L=0,R=arr.size()-1,i=update target, val = update val
void update(lli id, lli L, lli R,lli i,ty val) {
    if (L == R) {
        segment_tree[id] += val;
        return;
    }
    lli M = (L + R) / 2;
    if (i <= M) {
        segment_tree[id] += val;
        update(id * 2, L, M, i, val);
    }
    else {
        segment_tree[id] += val;
        update(id * 2+1, M+1,R, i, val);
    }
    min1[id] = min(min1[id * 2 + 1], min1[id * 2]);
}
//id=1,L=0,R=arr.size()-1,ql = query left index,qr  = query right index
ty getsum(lli id, lli L, lli R, lli ql, lli qr) {
    int M = (L + R) / 2;
    if (ql == L && R == qr) {
        return segment_tree[id];
    }
    if (qr <= M) {//all in left
        return getsum(id * 2, L, M, ql, qr);
    }
    if (M < ql) {//all in right
        return getsum(id * 2 + 1, M + 1, R, ql, qr);
    }
    return getsum(id * 2, L, M, ql, M) + getsum(id * 2 + 1, M + 1, R, M + 1, qr);
}

ty getmin(lli id, lli L, lli R, lli ql, lli qr) {
    int M = (L + R) / 2;
    if (ql == L && R == qr) {
        return min1[id];
    }
    if (qr <= M) {//all in left
        return  getmin(id * 2, L, M, ql, qr);
    }
    if (M < ql) {//all in right
        return  getmin(id * 2 + 1, M + 1, R, ql, qr);
    }
    return  min(getmin(id * 2, L, M, ql, M) , getmin(id * 2 + 1, M + 1, R, M + 1, qr));
}



void push(lli id,lli L,lli R) {
    segment_tree[id] += tag[id] * (R - L + 1);
    min1[id] += tag[id];
    if (L != R) {//還能往下鑽
        tag[id*2] += tag[id];
        tag[id * 2+1] += tag[id];
    }
    tag[id] = 0;
}
//id=1,L=0,R=arr.size()-1,ml = modify left index,qr  = modify right index , val = modify val
lli modify(lli id, lli L, lli R, lli ml, lli mr, ty val) {
    if (tag[id]) {
        push(id, L, R);
    }
    segment_tree[id] += val * (mr - ml + 1);
    int M = (L + R) / 2;
    if (ml == L && R == mr) {
        if (L != R) {
            tag[id * 2] += val;
            tag[id * 2 + 1] += val;
        }
        min1[id] += val;
    }
    else {
        if (mr <= M) {//all in left
            lli left = modify(id * 2, L, M, ml, mr, val);
            lli right = modify(id * 2+1, M+1, R, M + 1, R, 0);
            min1[id] = min(left, right);
        }
        else if (M < ml) {//all in right
            lli left = modify(id * 2, L, M, L, M, 0);
            lli right = modify(id * 2 + 1, M + 1, R, ml, mr, val);
            min1[id] = min(right, left);
        }
        else {
            lli left = modify(id * 2, L, M, ml, M, val);
            lli right = modify(id * 2 + 1, M + 1, R, M + 1, mr, val);
            min1[id] = min(left, right);
        }
    }
    return min1[id];
}
//id=1,L=0,R=arr.size()-1,ql = query left index,qr  = query right index
ty getsum_with_tag(lli id, lli L, lli R, lli ql, lli qr) {
    if (tag[id]) {
        push(id, L, R);
    }
    int M = (L + R) / 2;
    if (ql == L && R == qr) {
        return segment_tree[id];
    }
    if (qr <= M) {//all in left
        return getsum_with_tag(id * 2, L, M, ql, qr);
    }
    if (M < ql) {//all in right
        return getsum_with_tag(id * 2 + 1, M + 1, R, ql, qr);
    }
    return  getsum_with_tag(id * 2, L, M, ql, M) + getsum_with_tag(id * 2 + 1, M + 1, R, M + 1, qr);
}
ty getmin_with_tag(lli id, lli L, lli R, lli ql, lli qr) {
    if (tag[id]) {
        push(id, L, R);
    }
    int M = (L + R) / 2;
    if (ql == L && R == qr) {
        return min1[id];
    }
    if (qr <= M) {//all in left
        return  getmin_with_tag(id * 2, L, M, ql, qr);
    }
    if (M < ql) {//all in right
        return  getmin_with_tag(id * 2 + 1, M + 1, R, ql, qr);
    }
    return  min(getmin_with_tag(id * 2, L, M, ql, M), getmin_with_tag(id * 2 + 1, M + 1, R, M + 1, qr));
}

void fuc(int l, int r, int v) {
    for (int i = l; i <= r; i++) {
        arr[i] += v;
    }
}
int fuc1(int l, int r) {
    int ans = 0;
    for (int i = l; i <= r; i++) {
        ans += arr[i];
    }
    return ans;
}
int getmin1(int l, int r) {
    lli min1 = 10000000;
    for (int i = l; i <= r; i++) {
        min1 = min(arr[i], min1);
    }
    return min1;
}
bool test2(int seed) {
    srand(seed);
    int size = 16;
    arr = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
    min1.assign(size * 4, 0);
    tag.assign(size * 4, 0);
    segment_tree.assign(size * 4, 0);
    build(1,0,size-1);
    int testtime = 10;
    while (testtime--) {
        int l = rand() % size;
        int r = l + rand() % (size - l);
            int val = rand() % 12;
            modify(1, 0, arr.size() - 1, l, r, val);
            fuc(l, r, val);
            l = rand() % size;
            r = l + rand() % (size - l);
            if (getmin_with_tag(1, 0, arr.size() - 1, l, r) == getmin1(l, r)&&getsum_with_tag(1,0,size-1,l,r) ==fuc1(l,r)){

            }
            else {
                return 0;
            }
        
    }
    return 1;
}
bool test(int seed) {
    srand(seed);
    int size = rand() % 100000+1;
    arr.assign(size, 0);
    min1.assign(size * 4, 0);
    tag.assign(size * 4, 0);
    segment_tree.assign(size * 4, 0);
    build(1, 0, size - 1);
    int testtime = 1000;
    while (testtime--) {
        int f = rand() % 2;
        int l = rand() % size;
        int r = l + rand() % (size - l);
        if (f) {
            int val = rand() % 123;
            modify(1, 0, arr.size() - 1, l, r, val);
            fuc(l, r, val);
        }
        else {
            if (getmin_with_tag(1, 0, arr.size() - 1, l, r) == getmin1(l, r) && getsum_with_tag(1, 0, size - 1, l, r) == fuc1(l, r)) {

            }
            else {
                return 0;
            }
        }
    }
    return 1;
}
void testf() {
    int er = 1000;
    for (int i = 1; i <= 1000; i++) {
        if (test(i)) {
            cout << "good" << endl;
            er--;
        }
        else {
            cout << "bad" << endl;
        }
    }
    cout << "error: " << er << endl;
}

int main() {
    testf();
}