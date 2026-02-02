#include <bits/stdc++.h>
#include <omp.h>
using namespace std;
#define int long long

int dist(pair<int,int>& a,pair<int,int>& b)
{
    int dx=a.first-b.first;
    int dy=a.second-b.second;
    return dx*dx+dy*dy;
}

int32_t main(int argc,char** argv)
{
    if(argc<2)
    {
        cout<<"Usage: ./kmeans_omp <threads>\n";
        return 0;
    }

    int T=atoi(argv[1]);

    omp_set_dynamic(0);      
    omp_set_num_threads(T);  

    bool MANUAL=false;

    int n,k,ITER=10;
    vector<pair<int,int>> points,centroids;

    if(MANUAL)
    {
        n=8;
        points={{2,3},{3,4},{5,3},{8,7},{7,8},{9,6},{1,2},{4,5}};
        k=3;
        centroids={{2,3},{5,3},{8,7}};
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
        centroids.resize(k);
        for(int i=0;i<k;i++)
            centroids[i]=points[rand()%n];
    }

    double start=omp_get_wtime();

    for(int it=0;it<ITER;it++)
    {
        vector<int> cnt(k,0),sx(k,0),sy(k,0);

        #pragma omp parallel
        {
            vector<int> lc(k,0),lsx(k,0),lsy(k,0);

            #pragma omp for nowait
            for(int i=0;i<n;i++)
            {
                int mn=LLONG_MAX,id=0;
                for(int j=0;j<k;j++)
                {
                    int d=dist(points[i],centroids[j]);
                    if(d<mn){mn=d;id=j;}
                }
                lc[id]++;
                lsx[id]+=points[i].first;
                lsy[id]+=points[i].second;
            }

            #pragma omp critical
            {
                for(int i=0;i<k;i++)
                {
                    cnt[i]+=lc[i];
                    sx[i]+=lsx[i];
                    sy[i]+=lsy[i];
                }
            }
        }

        for(int i=0;i<k;i++)
        {
            if(cnt[i]==0) continue;
            centroids[i].first=sx[i]/cnt[i];
            centroids[i].second=sy[i]/cnt[i];
        }
    }

    double end=omp_get_wtime();

    ofstream fout("analysis_omp.txt",ios::app);
    fout<<T<<" "<<(end-start)<<"\n";
    fout.close();

    return 0;
}
