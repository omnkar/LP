#include<bits/stdc++.h>
using namespace std;

#define INF 9999;
class Graph
{
    public:
    int graph[10][10];
    int n,isDirected,isWeighted;
    Graph()
    {
        cout<<"Enter number of vertices: ";
        cin>>n;
        cout<<"Is the graph directed? (1 for yes, 0 for no): ";
        cin>>isDirected;
        cout<<"Is the graph weighted? (1 for yes, 0 for no): ";
        cin>>isWeighted;
        // cout<<"Enter the adjacency matrix:\n";
        // for(int i=0;i<n;i++)
        //     for(int j=0;j<n;j++)
        //         cin>>graph[i][j];
    }
    void printGraph()
    {
        cout<<"Adjacency Matrix:\n";
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
                cout<<graph[i][j]<<" ";
            cout<<endl;
        }
    }
    void readGraphByEdge()
    {
        int u,v,w;
        while(1)
        {
            cout<<"Enter edge (u v w) or -1 to stop: ";
            cin>>u;
            if(u==-1 || v==-1) break;
            cin>>v>>w;
            graph[u][v]=w;
            if(!isDirected)
                graph[v][u]=w;
        }
    }
    void prims()
    {
        int **cost;
        cost=new int *[n];
        for(int i=0;i<n;i++)
            cost[i]=new int[n];

        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(graph[i][j]==0)
                {
                    cost[i][j]=INF;
                }  
                else{
                    cost[i][j]=graph[i][j];
                }
            }
        }
        int *visited,*dist,*from;
        visited=new int[n];
        dist=new int[n];
        from=new int[n];

        int src=0;
        for(int i=0;i<n;i++)
        {
            visited[i]=0;
            dist[i]=cost[src][i];
            from[i]=src;
        }
        visited[src]=1;
        dist[src]=0;
        int u,v;
        int minCost=0;
        int ne=n-1;
        while(ne>0)
        {
            int minDist=INF;
            for(int i=0;i<n;i++)
            {
                if(!visited[i] && dist[i]<minDist)
                {
                    minDist=dist[i];
                    v=i;
                }
            }
            u=from[v];
            visited[v]=1;
            cout<<"Edge: "<<u<<" - "<<v<<" Weight: "<<cost[u][v]<<endl;
            minCost+=cost[u][v];
            for(int i=0;i<n;i++)
            {
                if(!visited[i] && cost[v][i]<dist[i])
                {
                    dist[i]=cost[v][i];
                    from[i]=v;
                }
            }
            ne--;
        }
        cout<<"Minimum Cost: "<<minCost<<endl;
    }
    void dijkstras()
    {
        int **cost;
        cost=new int *[n];
        for(int i=0;i<n;i++)
            cost[i]=new int[n];

        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(graph[i][j]==0){
                    cost[i][j]=INF;
                }
                else{
                    cost[i][j]=graph[i][j];
                }
            }
        }
        int *visited,*dist,*from;
        int src=0;
        visited=new int[n];
        dist=new int[n];
        from=new int[n];
        for(int i=0;i<n;i++)
        {
            visited[i]=0;
            dist[i]=graph[src][i];
            from[i]=src;
        }
        visited[src]=1;
        dist[src]=0;
        int u,v;
        int minCost=0;
        int ne=n-1;
        while(ne>0)
        {
            int minDist=INF;
            for(int i=0;i<n;i++)
            {
                if(!visited[i] && dist[i]<minDist)
                {
                    minDist=dist[i];
                    v=i;
                }
            }
            u=from[v];
            visited[v]=1;
            cout<<"Edge: "<<u<<" - "<<v<<" Weight: "<<cost[u][v]<<endl;
            // minCost+=cost[u][v];
            for(int i=0;i<n;i++)
            {
                if(!visited[i] && dist[i]>dist[v]+cost[v][i])
                {
                    dist[i]=cost[v][i]+dist[v];
                    from[i]=v;
                }
            }
            ne--;
        }

        //printing the shortest path from source to all other vertices
        cout<<"Shortest Path from source "<<src<<endl;
        for(int i=0;i<n;i++)
        {
            cout<<"To "<<i<<": ";
            int j=i;
            while(j!=src)
            {
                cout<<j<<"<-";
                j=from[j];
            }
            cout<<src<<endl;
        }
        cout<<"Minimum Cost: "<<minCost<<endl;
    }
};
int main()
{
    Graph g;
    g.readGraphByEdge();
    g.prims();
    g.dijkstras();
    return 0;
}