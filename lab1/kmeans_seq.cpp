#include<bits/stdc++.h>
using namespace std;

int main(){
    int N=8,K=2,D=2,ITER=10;
    vector<vector<double>> points={
        {1,2},{1,4},{1,0},{10,2},
        {10,4},{10,0},{5,5},{6,6}
    };

    vector<vector<double>> centroids={{1,2},{10,2}};
    vector<int> label(N);

    for(int it=0;it<ITER;it++){
        // Assignment step
        for(int i=0;i<N;i++){
            double best=1e18;
            for(int k=0;k<K;k++){
                double dist=0;
                for(int d=0;d<D;d++)
                    dist+=(points[i][d]-centroids[k][d])*
                          (points[i][d]-centroids[k][d]);
                if(dist<best){
                    best=dist;
                    label[i]=k;
                }
            }
        }

        // Update step
        vector<vector<double>> newC(K,vector<double>(D,0));
        vector<int> cnt(K,0);

        for(int i=0;i<N;i++){
            cnt[label[i]]++;
            for(int d=0;d<D;d++)
                newC[label[i]][d]+=points[i][d];
        }

        for(int k=0;k<K;k++)
            for(int d=0;d<D;d++)
                centroids[k][d]=newC[k][d]/max(1,cnt[k]);
    }

    for(auto &c:centroids)
        cout<<c[0]<<" "<<c[1]<<"\n";
}
