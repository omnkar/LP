#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <queue>
#include <stack>
#include <functional>

using namespace std;

class Graph {
private:
    // Adjacency list representation
    unordered_map<int, vector<int>> adjacencyList;

public:
    // Add a vertex to the graph
    void addVertex(int vertex) {
        if (adjacencyList.find(vertex) == adjacencyList.end()) {
            adjacencyList[vertex] = vector<int>();
        }
    }

    // Add an edge between two vertices (undirected)
    void addEdge(int vertex1, int vertex2) {
        // Add vertices if they don't exist
        addVertex(vertex1);
        addVertex(vertex2);
        
        // Add edge in both directions (undirected graph)
        adjacencyList[vertex1].push_back(vertex2);
        adjacencyList[vertex2].push_back(vertex1);
    }

    // Print the graph
    void printGraph() {
        cout << "Graph representation (Adjacency List):" << endl;
        for (const auto& pair : adjacencyList) {
            cout << pair.first << " -> ";
            for (int neighbor : pair.second) {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }

    // Recursive DFS helper function
    void dfsRecursiveHelper(int vertex, set<int>& visited, vector<int>& result) {
        // Mark the current vertex as visited
        visited.insert(vertex);
        result.push_back(vertex);
        
        // Recur for all adjacent vertices
        for (int neighbor : adjacencyList[vertex]) {
            if (visited.find(neighbor) == visited.end()) {
                dfsRecursiveHelper(neighbor, visited, result);
            }
        }
    }

    // DFS traversal starting from a given vertex (recursive implementation)
    vector<int> dfsRecursive(int startVertex) {
        set<int> visited;
        vector<int> result;
        
        // Call the recursive helper function
        dfsRecursiveHelper(startVertex, visited, result);
        
        return result;
    }

    // DFS traversal starting from a given vertex (non-recursive implementation)
    vector<int> dfsNonRecursive(int startVertex) {
        vector<int> result;
        set<int> visited;
        stack<int> stack;
        
        // Push the start vertex to the stack
        stack.push(startVertex);
        
        while (!stack.empty()) {
            // Pop a vertex from stack
            int currentVertex = stack.top();
            stack.pop();
            
            // Skip if already visited
            if (visited.find(currentVertex) != visited.end()) {
                continue;
            }
            
            // Mark as visited and add to result
            visited.insert(currentVertex);
            result.push_back(currentVertex);
            
            // Push all unvisited neighbors to stack
            // Note: We push in reverse order to match the recursive DFS traversal
            for (int i = adjacencyList[currentVertex].size() - 1; i >= 0; i--) {
                int neighbor = adjacencyList[currentVertex][i];
                if (visited.find(neighbor) == visited.end()) {
                    stack.push(neighbor);
                }
            }
        }
        
        return result;
    }

    // BFS traversal starting from a given vertex
    vector<int> bfs(int startVertex) {
        vector<int> result;
        set<int> visited;
        queue<int> queue;
        
        // Mark the start vertex as visited and enqueue it
        visited.insert(startVertex);
        queue.push(startVertex);
        
        while (!queue.empty()) {
            // Dequeue a vertex from queue
            int currentVertex = queue.front();
            queue.pop();
            result.push_back(currentVertex);
            
            // Get all adjacent vertices of the dequeued vertex
            // If an adjacent has not been visited, mark it visited and enqueue it
            for (int neighbor : adjacencyList[currentVertex]) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    queue.push(neighbor);
                }
            }
        }
        
        return result;
    }
    
    // Depth-Limited Search recursive helper function
    bool dlsRecursiveHelper(int vertex, int target, int depthLimit, set<int>& visited, 
                          vector<int>& result) {
        // If target is found, return true
        if (vertex == target) {
            result.push_back(vertex);
            return true;
        }
        
        // If depth limit is reached, return false
        if (depthLimit <= 0) {
            return false;
        }
        
        // Mark current vertex as visited
        visited.insert(vertex);
        result.push_back(vertex);
        
        // Recur for all adjacent vertices with reduced depth limit
        for (int neighbor : adjacencyList[vertex]) {
            if (visited.find(neighbor) == visited.end()) {
                bool found = dlsRecursiveHelper(neighbor, target, depthLimit - 1, visited, result);
                if (found) {
                    return true;
                }
            }
        }
        
        // Target was not found in this path, backtrack
        result.pop_back();
        visited.erase(vertex);
        
        return false;
    }
    
    // Depth-Limited Search - searches up to a given depth limit
    vector<int> depthLimitedSearch(int startVertex, int target, int depthLimit) {
        set<int> visited;
        vector<int> result;
        
        bool found = dlsRecursiveHelper(startVertex, target, depthLimit, visited, result);
        
        // If target was found, result contains the path
        // If not found, return an empty result
        if (!found) {
            result.clear();
        }
        
        return result;
    }
    
    // Iterative Deepening Depth-First Search
    vector<int> iterativeDeepeningDFS(int startVertex, int target, int maxDepth) {
        vector<int> result;
        
        // Try DLS with increasing depth
        for (int depth = 0; depth <= maxDepth; depth++) {
            cout << "Trying depth limit: " << depth << endl;
            result = depthLimitedSearch(startVertex, target, depth);
            
            // If target is found at this depth, return the result
            if (!result.empty()) {
                return result;
            }
        }
        
        // Target was not found within maxDepth
        return result;
    }
    
    // For testing purposes: DLS and IDDFS that print all visited nodes
    vector<int> exploreDepthLimitedSearch(int startVertex, int depthLimit) {
        set<int> visited;
        vector<int> result;
        
        // Helper function for exploration DLS
        function<void(int, int)> exploreHelper = [&](int vertex, int currentDepth) {
            if (currentDepth > depthLimit) return;
            
            visited.insert(vertex);
            result.push_back(vertex);
            
            for (int neighbor : adjacencyList[vertex]) {
                if (visited.find(neighbor) == visited.end()) {
                    exploreHelper(neighbor, currentDepth + 1);
                }
            }
        };
        
        exploreHelper(startVertex, 0);
        return result;
    }
    
    // IDDFS that explores all nodes at each depth
    vector<vector<int>> exploreIterativeDeepeningDFS(int startVertex, int maxDepth) {
        vector<vector<int>> results;
        
        for (int depth = 0; depth <= maxDepth; depth++) {
            vector<int> result = exploreDepthLimitedSearch(startVertex, depth);
            results.push_back(result);
        }
        
        return results;
    }
};

// Function to print traversal results
void printTraversal(const vector<int>& traversal, const string& traversalType) {
    cout << traversalType << " traversal result: ";
    for (int vertex : traversal) {
        cout << vertex << " ";
    }
    cout << endl;
}

int main() {
    Graph g;
    int vertices, edges;
    
    // Get number of vertices and edges
    cout << "Enter number of vertices: ";
    cin >> vertices;
    
    cout << "Enter number of edges: ";
    cin >> edges;
    
    // Input edges
    cout << "Enter edges (format: vertex1 vertex2):" << endl;
    for (int i = 0; i < edges; i++) {
        int vertex1, vertex2;
        cin >> vertex1 >> vertex2;
        g.addEdge(vertex1, vertex2);
    }
    
    // Print the graph
    g.printGraph();
    
    // Get start vertex for traversals
    int startVertex;
    cout << "\nEnter starting vertex for traversals: ";
    cin >> startVertex;
    
    // Perform DFS traversal (recursive)
    vector<int> dfsRecursiveResult = g.dfsRecursive(startVertex);
    printTraversal(dfsRecursiveResult, "DFS (recursive)");
    
    // Perform DFS traversal (non-recursive)
    vector<int> dfsNonRecursiveResult = g.dfsNonRecursive(startVertex);
    printTraversal(dfsNonRecursiveResult, "DFS (non-recursive)");
    
    // Perform BFS traversal
    vector<int> bfsResult = g.bfs(startVertex);
    printTraversal(bfsResult, "BFS");
    
    // Depth-Limited Search
    int depthLimit;
    cout << "\nEnter depth limit for Depth-Limited Search: ";
    cin >> depthLimit;
    
    // Explore DLS (visits all nodes within depth limit)
    vector<int> dlsResult = g.exploreDepthLimitedSearch(startVertex, depthLimit);
    printTraversal(dlsResult, "Depth-Limited Search (exploration mode, depth limit: " + to_string(depthLimit) + ")");
    
    // Target search with DLS
    int targetVertex;
    cout << "\nEnter target vertex for DLS target search: ";
    cin >> targetVertex;
    
    vector<int> dlsTargetResult = g.depthLimitedSearch(startVertex, targetVertex, depthLimit);
    if (dlsTargetResult.empty()) {
        cout << "Target " << targetVertex << " not found within depth limit " << depthLimit << endl;
    } else {
        printTraversal(dlsTargetResult, "Depth-Limited Search path to target " + to_string(targetVertex));
    }
    
    // Iterative Deepening DFS
    int maxDepth;
    cout << "\nEnter maximum depth for Iterative Deepening DFS: ";
    cin >> maxDepth;
    
    vector<int> iddfsResult = g.iterativeDeepeningDFS(startVertex, targetVertex, maxDepth);
    if (iddfsResult.empty()) {
        cout << "Target " << targetVertex << " not found within maximum depth " << maxDepth << endl;
    } else {
        printTraversal(iddfsResult, "Iterative Deepening DFS path to target " + to_string(targetVertex));
    }
    
    // Demonstration of IDDFS exploration mode
    cout << "\nDemonstration of Iterative Deepening DFS exploration:" << endl;
    auto iddfsExplorationResults = g.exploreIterativeDeepeningDFS(startVertex, min(3, maxDepth));
    for (size_t depth = 0; depth < iddfsExplorationResults.size(); depth++) {
        printTraversal(iddfsExplorationResults[depth], "IDDFS exploration at depth " + to_string(depth));
    }
    
    return 0;
}