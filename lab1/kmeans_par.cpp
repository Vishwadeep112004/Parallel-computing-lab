#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

int main(int argc,char** argv){
    MPI_Init(&argc,&argv);

    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    int N=8,K=2,D=2,ITER=10;
    vector<vector<double>> points={
        {1,2},{1,4},{1,0},{10,2},
        {10,4},{10,0},{5,5},{6,6}
    };

    vector<vector<double>> centroids={{1,2},{10,2}};
    vector<int> label(N);

    int per=N/size;
    int l=rank*per;
    int r=(rank==size-1?N:l+per);

    for(int it=0;it<ITER;it++){
        MPI_Bcast(&centroids[0][0],K*D,MPI_DOUBLE,0,MPI_COMM_WORLD);

        vector<vector<double>> localSum(K,vector<double>(D,0));
        vector<int> localCnt(K,0);

        for(int i=l;i<r;i++){
            double best=1e18;int id=0;
            for(int k=0;k<K;k++){
                double dist=0;
                for(int d=0;d<D;d++)
                    dist+=(points[i][d]-centroids[k][d])*
                          (points[i][d]-centroids[k][d]);
                if(dist<best){best=dist;id=k;}
            }
            localCnt[id]++;
            for(int d=0;d<D;d++)
                localSum[id][d]+=points[i][d];
        }

        vector<vector<double>> globalSum(K,vector<double>(D));
        vector<int> globalCnt(K);

        MPI_Reduce(&localSum[0][0],&globalSum[0][0],
                   K*D,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

        MPI_Reduce(&localCnt[0],&globalCnt[0],
                   K,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

        if(rank==0){
            for(int k=0;k<K;k++)
                for(int d=0;d<D;d++)
                    centroids[k][d]=globalSum[k][d]/max(1,globalCnt[k]);
        }
    }

    if(rank==0)
        for(auto &c:centroids)
            cout<<c[0]<<" "<<c[1]<<"\n";

    MPI_Finalize();
}
