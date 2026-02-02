    #include <bits/stdc++.h>
    #include <mpi.h>
    using namespace std;

    int main(int argc,char** argv)
    {
        MPI_Init(&argc,&argv);

        int rank,size;
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);
        MPI_Comm_size(MPI_COMM_WORLD,&size);

        bool MANUAL=false;
        int n,k,D=2,ITER=20;

        vector<pair<int,int>> points;
        vector<int> centroids;

        double start,end;

        /* ---------- DATA GENERATION (ROOT ONLY) ---------- */
        if(rank==0)
        {
            if(MANUAL)
            {
                n=8;
                points={{2,3},{3,4},{5,3},{8,7},{7,8},{9,6},{1,2},{4,5}};
                k=3;
                centroids={2,3,5,3,8,7};
            }
            else
            {
                srand(time(0));
                int L=10000,R=100000;
                n=L+rand()%(R-L+1);

                points.resize(n);
                for(int i=0;i<n;i++)
                    points[i]={rand()%1000,rand()%1000};

                k=6;
                centroids.resize(k*D);
                for(int i=0;i<k;i++)
                {
                    auto p=points[rand()%n];
                    centroids[i*D]=p.first;
                    centroids[i*D+1]=p.second;
                }
            }
        }

        MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&k,1,MPI_INT,0,MPI_COMM_WORLD);

        vector<int> flat_points;
        if(rank==0)
        {
            flat_points.resize(n*D);
            for(int i=0;i<n;i++)
            {
                flat_points[i*D]=points[i].first;
                flat_points[i*D+1]=points[i].second;
            }
        }
        else
        {
            flat_points.resize(n*D);
            points.resize(n);
            centroids.resize(k*D);
        }

        MPI_Bcast(flat_points.data(),n*D,MPI_INT,0,MPI_COMM_WORLD);

        if(rank!=0)
            for(int i=0;i<n;i++)
                points[i]={flat_points[i*D],flat_points[i*D+1]};

        int per=n/size;
        int l=rank*per;
        int r=(rank==size-1?n:l+per);

        MPI_Barrier(MPI_COMM_WORLD);
        start=MPI_Wtime();

        /* ---------- K-MEANS ---------- */
        for(int it=0;it<ITER;it++)
        {
            MPI_Bcast(centroids.data(),k*D,MPI_INT,0,MPI_COMM_WORLD);

            vector<int> localSum(k*D,0),localCnt(k,0);

            for(int i=l;i<r;i++)
            {
                int mn=INT_MAX,id=0;
                for(int j=0;j<k;j++)
                {
                    int dx=points[i].first-centroids[j*D];
                    int dy=points[i].second-centroids[j*D+1];
                    int d=dx*dx+dy*dy;
                    if(d<mn){mn=d;id=j;}
                }
                localCnt[id]++;
                localSum[id*D]+=points[i].first;
                localSum[id*D+1]+=points[i].second;
            }

            vector<int> globalSum(k*D,0),globalCnt(k,0);

            MPI_Reduce(localSum.data(),globalSum.data(),
                    k*D,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

            MPI_Reduce(localCnt.data(),globalCnt.data(),
                    k,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

            if(rank==0)
            {
                for(int i=0;i<k;i++)
                {
                    if(globalCnt[i]==0) continue;
                    centroids[i*D]=globalSum[i*D]/globalCnt[i];
                    centroids[i*D+1]=globalSum[i*D+1]/globalCnt[i];
                }
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);
        end=MPI_Wtime();

        if(rank==0)
        {
            ofstream fout("analysis_mpi.txt",ios::app);
            fout<<size<<" "<<(end-start)<<"\n";
            fout.close();
        }

        MPI_Finalize();
        return 0;
    }
