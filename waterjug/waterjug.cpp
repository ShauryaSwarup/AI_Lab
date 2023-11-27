#include <bits/stdc++.h>
using namespace std;
class Jug{
    int maxCap,curCap;
    public:
    Jug(int maxCap){
        this->maxCap=maxCap;
        this->curCap=0;
    }
    void fill(){
        this->curCap=this->maxCap;
    }
    void fillOther(Jug other){
       if(other.curCap==other.maxCap) return;
       int transfer = min(other.maxCap-other.curCap,this->curCap);
       other.curCap+=transfer;
       this->curCap-=transfer;
    }
    void empty(){
        this->curCap=0;
    }
};
int BFS(Jug A,Jug B,int target){
    Jug StoreA=A,StoreB=B;
    queue<pair<Jug,Jug>> q;
    q.push(make_pair(A,B));
    while(q.empty()){
        int n=q.size();
        for(int i=0;i<n;i++){
            //fill A
            A.fill();
            q.push(make_pair(A,B));
            A=StoreA;
            //fill B
            B.fill();
            q.push(make_pair(A,B));
            B=StoreB;
            //empty A
            A.empty();
            q.push(make_pair(A,B));
            A=StoreA;
            //empty B
            B.empty();
            q.push(make_pair(A,B));
            B=StoreB;
            //transfer A to B
            A.fillOther(B);
            q.push(make_pair(A,B));
            A=StoreA;
            B=StoreB;
            //transfer B to A
            B.fillOther(A);
            q.push(make_pair(A,B));
            A=StoreA;
            B=StoreB;

        }
    }
}
int main(){
	Jug A(6),B(4);
    
	return 0;
}