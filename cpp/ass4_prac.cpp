#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <limits>
#include <queue>
#include <set>

using namespace std;
using namespace chrono;
struct QueensState{
    vector<int>queens;
    int row,bound;
    QueenState(int n)
    {
        queens.resize(n,-1);
        row=0;
        bound=0;
    }
    QueensState(const QueensState&other)
    {
        queens=other.queens;
        row=other.row;
        bound=other.bound;
    }
    bool operator >(const QueensState&other) const{
        return bound>other.bound;
    }

};
class NQueenSolver{
    private:
    int n;
    vector<int>solution;
    long long nodesExplored;
    bool solutionFound;
    public:
    NQueenSolver(int boardSize)
    {
        n=boardSize;
        nodesExplored=0;
        solutionFound=false;
        solution.resize(n,-1);
    }
    void solveBacktracking();
    bool isSafe(vector<int>&,int,int);
    void backtrack(vector<int>&,int);
    void printBoard(const vector<int>&);
    void solveConstraintBacktracking();
    void constraintBacktrack(vector<int>&,int,vector<bool>&,vector<bool>&,vector<bool>&);
};
void NQueenSolver::solveBacktracking()
{
    nodesExplored=0;
    vector<int>queens(n,-1);
    auto startTime=high_resolution_clock::now();
    backtrack(queens,0);
    auto endTime=high_resolution_clock::now();
    auto duration=duration_cast<milliseconds>(endTime-startTime).count();
    cout<<"Backtracking: "<<(solutionFound?"Solution found":"No solution")<<" in "<<duration<<" ms after exploring "<<nodesExplored<<" nodes.\n";
    if(solutionFound)
    {
        printBoard(solution);
    }

}
void NQueenSolver::printBoard(const vector<int>&queens)
{
    cout<<"+";
    for(int i=0;i<n;i++)
    {
        cout<<"---+";
    }
    cout<<"\n";
    for(int i=0;i<n;i++)
    {
        cout<<"|";
        for(int j=0;j<n;j++)
        {
            cout<<(queens[i]==j?" Q |":"   |");
        }
        cout<<"\n+";
        for(int j=0;j<n;j++)
        {
            cout<<"---+";
        }
        cout<<"\n";
    }
}
void NQueenSolver::backtrack(vector<int>&queens,int row)
{
    if(row==n)
    {
        solution=queens;
        solutionFound=true;
        return;
    }
    for(int col=0;col<n;col++)
    {
        nodesExplored++;
        if(isSafe(queens,row,col))
        {
            queens[row]=col;
            backtrack(queens,row+1);
            if(solutionFound) return;
            queens[row]=-1;
        }
    }
}
bool NQueenSolver::isSafe(vector<int>&queens,int row,int col)
{
    for(int i=0;i<row;i++)
    {
        if(queens[i]==col || queens[i]-i ==col-row || queens[i]+i==col+row)
        {
            return false;
        }
    }
    return true;
}
void NQueenSolver::solveConstraintBacktracking()
{
    nodesExplored=0;
    vector<int>queens(n,-1);
    vector<bool>col_used(n,false);
    vector<bool>diag1_used(2*n-1,false);
    vector<bool>diag2_used(2*n-1,false);
    auto startTime=high_resolution_clock::now();
    constraintBacktrack(queens,0,col_used,diag1_used,diag2_used);
    auto endTime=high_resolution_clock::now();
    auto duration=duration_cast<milliseconds>(endTime-startTime).count();
    cout<<"Constraint Backtracking: "<<(solutionFound?"Solution found":"No solution")<<" in "<<duration<<" ms after exploring "<<nodesExplored<<" nodes.\n";
    if(solutionFound)
    {
        printBoard(solution);
    }
}
void NQueenSolver::constraintBacktrack(vector<int>&queens,int row,vector<bool>&col_used,vector<bool>&diag1_used,vector<bool>&diag2_used)
{
    if(row==n)
    {
        solution=queens;
        solutionFound=true;
        return;
    }
    for(int col=0;col<n;col++)
    {
        nodesExplored++;
        int diag1_idx=row+col;
        int diag2_idx=row-col+n-1;
        if(!col_used[col] && !diag1_used[diag1_idx] && !diag2_used[diag2_idx])
        {
            queens[row]=col;
            col_used[col]=true;
            diag1_used[diag1_idx]=true;
            diag2_used[diag2_idx]=true;
            constraintBacktrack(queens,row+1,col_used,diag1_used,diag2_used);
            if(solutionFound) return;
            queens[row]=-1;
            col_used[col]=false;
            diag1_used[diag1_idx]=false;
            diag2_used[diag2_idx]=false;
        }
    }
}
int main()
{
    int n;
    cout<<"\nEnter the number of queens: ";
    cin>>n;
    if(n<=0)
    {
        cout<<"Invalid input. Number of queens must be positive.\n";
        return 1;
    }
    if (n == 2 || n == 3) {
        cout << "Warning: No solution exists for N=" << n << "\n";
    }

    NQueenSolver solver(n);
    solver.solveBacktracking();
    solver.solveConstraintBacktracking();
    return 0;
}