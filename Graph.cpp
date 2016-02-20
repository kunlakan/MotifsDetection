//------------------------------------------------------------------------------
//  Graph.h
//  Created by Kunlakan Cherdchusilp, Ngoc Luu, and Vuochly Ky
//------------------------------------------------------------------------------
// Graph is an undirected and unweigthed graph that is represented by adjacency
// list of vertices that are connected to each other.
// features are included:
//   -- allows adding and removing edges
//   -- allows displaying of the whole Graph with distance and path
//
// ASSUMPTIONS:
//   -- The graph will have no more than 100 vertices
//   -- Every of each VertexNode has ownership of its data stored
//   -- No graph ever share the same object (they could share the same value)
//   -- Input in the file and parameter for insert and remove will have a
//      range of 1 to size. Graph will ONLY work with the elements existing
//      from the range 0 to size - 1
//   -- file input must be properly formatted:
//          1   2
//          2   4       The frist number is the vertex from
//          3   1       The second number is the vertex to
//          0   0
//
//------------------------------------------------------------------------------


#include "Graph.h"

//--------------------------- A Default Constructor ----------------------------
// Default constructor for class Graph
// Preconditions: None
// Postconditions: - size is initialized to zero
//                 - Every VertexNode element in vertices will be initialized
//                   by its default constructor
Graph::Graph()
{
    size = 0;
}


//----------------------------- A Copy Constructor -----------------------------
// Copy constructor for class Graph
// Preconditions: - All of VertexNode.edgeHead in otherGraph's vertices points
//                  to NULL or an EdgeNode
//                - All of VertexNode.data in otherGraph's vertices points to
//                  NULL or a GraphData
// Postconditions: - size is equal to otherGraph.size
//                 - vertices will contains the same value and order as
//                   otherGraph.vertices but with its ownership of all data
Graph::Graph(const Graph &otherGraph)
{
    size = otherGraph.size;
    
    // copies adjacency list vertices
    copyAllVertices(otherGraph.vertices);
}

//-------------------------- PRIVATE: copyAllVertices --------------------------
// Copies every VertexNode element and all of its EdgeNode of othersVertices
// over to verticies sequentially vertices will have its ownership over all
// data and EdgeNode its stored.
// Preconditions: - size must be equal to size of otherVertices' graph
//                - otherVertices.edgeHead points to NULL or an EdgeNode
//                - otherVertices.data points to NULL or a GraphData
// Postcondition: vertices will contains the same value and structure as
//                otherGraph.vertices but with its ownership of all data
void Graph::copyAllVertices(const VertexNode *otherVertices)
{
    EdgeNode *otherEdgeHead = NULL;
    for(int i = 0; i < size; i++)
    {
        vertices[i].data = new GraphData(*otherVertices[i].data);
        
        otherEdgeHead = otherVertices[i].edgeHead;
        copyAllEdge(vertices[i].edgeHead, otherEdgeHead);
    }
}

//---------------------------- PRIVATE: copyAllEdge ----------------------------
// Creates a copy of every information recursively from otherEdge to thisEdge.
// thisEdge will have its ownership over all of its data stored.
// Preconditions: - otherEdge points to either NULL or an EdgeNode
//                - thisEdge points to either NULL or an EdgeNode
// Postcondition: Every thisEdge will have the same value and structure as
//                otherEdge with its ownership of all data stored
void Graph::copyAllEdge(EdgeNode *&thisEdge, EdgeNode *&otherEdge)
{
    if(otherEdge != NULL)
    {
        thisEdge = new EdgeNode(otherEdge->adjVertex);
        copyAllEdge(thisEdge->nextEdge, otherEdge->nextEdge);
    }
}

//--------------------------------- Destructor ---------------------------------
// Destructor for class Graph
// Preconditions: - All of VertexNode.edgeHead points to NULL or an EdgeNode
//                - All VertexNode.data points to NULL or a GraphData
// Postconditions: All VertexNode's data and edgeHead and its list of
//                 EdgeNode are deallocated
Graph::~Graph()
{
    for(int i = 0; i < size; i++)
    {
        removeAllEdge(vertices[i].edgeHead);
        delete(vertices[i].data);
        vertices[i].data = NULL;
    }
}

//--------------------------- PRIVATE: removeAllEdge ---------------------------
// Removes all EdgeNode recursively
// Preconditions: currentEdge points to either NULL or an EdgeNode
// Postconditions: currentEdge->nextEdge, and currentEdge will be deallocated
//                 and set to NULL
void Graph::removeAllEdge(EdgeNode *&currentEdge)
{
    if(currentEdge != NULL)
    {
        removeAllEdge(currentEdge->nextEdge);
        delete(currentEdge->nextEdge);
        currentEdge->nextEdge = NULL;
        delete(currentEdge);
        currentEdge = NULL;
    }
}

//--------------------------------- buildGraph ---------------------------------
// Builds a graph by reading data from an ifstream
// Preconditions:  infile has been successfully opened and the file contains
//                 properly formated data (according to the program specs)
// Postconditions: A graph is read from infile and stored in the object
void Graph::buildGraph(ifstream& infile)
{
    infile >> size;                          // data member stores array size
    
    if (infile.eof())
        return;
    infile.ignore();                         // throw away '\n' go to next line
    
    char tempString[MAX_CHAR_LENGTH];

    for (int v = 0; v < size; v++)
    {
        infile.getline(tempString, MAX_CHAR_LENGTH, '\n');
  
		vertices[v].data = new GraphData(tempString);
    }
    
    int src = 1, dest = 1;
    for (;;) {
        infile >> src >> dest;
        if (src == 0 || infile.eof())
            break;
        insertEdge(src, dest);
    }
}

//--------------------------------- insertEdge ---------------------------------
// Insert an EdgeNode into the graph
// Preconditions: - source and destination should be within the input range
//                - vertices[vertexFrom] and its data must exist
// Postconditions: - If tempEdge does not exist between the two vertices,
//                   tempEdge will be inserted
//                 - If source and destination are not in its range, the
//                   function will end
void Graph::insertEdge(const int &source, const int &destination)
{
    int vertexFrom = source - 1;
    int vertexTo = destination - 1;
    
    if(vertexFrom != vertexTo && areInRange(vertexFrom, vertexTo))
    {
        EdgeNode *tempEdge = new EdgeNode(vertexTo);
        insertHelper(vertices[vertexFrom].edgeHead, tempEdge);
        tempEdge = NULL;
    }
}

//---------------------------- PRIVATE: insertHelper ---------------------------
// Recursively looking if the edge already exist inside the list. If so, ignore
// the insertion. Otherwise, inserts edge at the end of the list.
// Preconditions: current and edge points to NULL or a first EdgeNode
// Postconditions: - If edge does not already exist in the list, it will be
//                   inserted to the end of the list
//                 - If edge already exist in the list, the insertion will be
//                   ignored
void Graph::insertHelper(EdgeNode *&current, EdgeNode *&edge)
{
    if(current == NULL)
    {
        edge->nextEdge = current;
        current = edge;
    }
    else
        insertHelper(current->nextEdge, edge);
}

//--------------------------------- removeEdge ---------------------------------
// Remove an EdgeNode out of the graph
// Preconditions: - source and destination should be within the input range
//                - vertices[vertexFrom] and its data must exist
// Postcondition: - If EdgeNode of destination does not exist in the list,
//                  nothing will happen
//                - If EdgeNode of destination is found, it gets removed
void Graph::removeEdge(const int &source, const int &destination)
{
    int vertexFrom = source - 1;
    int vertexTo = destination - 1;
    
    if(vertexFrom != vertexTo && areInRange(vertexFrom, vertexTo))
    {
        removeHelper(vertices[vertexFrom].edgeHead, vertexTo);
    }
}

//--------------------------- PRIVATE: removeHelper ----------------------------
// Recursively looking for the EdgeNode containing destination to remove
// Preconditions: current points to NULL or an EdgeNode
// Postconditions: If EdgeNode containing destination is found, it will be
//                 deallocated. Otherwise, funtions ends
void Graph::removeHelper(EdgeNode *&current, int destination)
{
    if(current == NULL)
        return;
    else if(current->adjVertex == destination)
    {
        EdgeNode *temp = current;
        current = temp->nextEdge;
        delete(temp);
        temp = NULL;
    }
    else
        removeHelper(current->nextEdge, destination);
}

//--------------------------------- displayAll ---------------------------------
// Display a all detailed path
// Preconditions: vertices[vertexFrom] and its data must exist
// Postconditions: all detailed path is displayed
void Graph::displayAll() const
{
    cout << "Description\t\t\t\t\t";
    cout << "From\t";
    cout << "To\t\t";

    for(int i = 0; i < size; i++)
    {
        diplayAllHelper(i);
    }
}

//------------------------- PRIVATE: displayAllHelper --------------------------
// Recursively display a all detailed path
// Preconditions: vertices[vertexFrom] and its data must exist
// Postconditions: All detailed path is displayed
void Graph::diplayAllHelper(const int &source) const
{
    cout << *vertices[source].data << endl;

    for(int i = 0; i < size; i++)
    {
        if(source != i){
            cout << "\t\t\t\t\t\t\t";
            cout << (source+1) << "\t\t";
            cout << (i+1) << "\t\t";
        }
    }
}

//----------------------------------- display ----------------------------------
// Display a single detailed path
// Preconditions: - source and destination should be within the input range
//                - vertices[vertexFrom] and its data must exist
// Postconditions: - If source and destination is not in the range, an error
//                   message is shown
//                 - a single detailed path is displayed with its locations'
//                   description
void Graph::display(const int &source, const int &destination) const
{
    int vertexFrom = source - 1;
    int vertexTo = destination - 1;

    
    if(!areInRange(vertexFrom, vertexTo))
        cout << "DISPLAY ERROR: No path exists" << endl;
    else
    {
        cout << (source) << "\t";
        cout << (destination) << "\t";
    }
}

//----------------------------- PRIVATE: areInRange ----------------------------
// Checks if source and destination are in the range of 0 to size
// Preconditions: None
// Postconditions: true is returned if the source and destination are in the
//                 range. Otherwise, false is returned
bool Graph::areInRange(const int &source , const int &destination) const
{
    bool sourceInRange = (0 <= source) && (source < size);
    bool destInRange = (0 <= destination) && (destination < size);
    
    return sourceInRange && destInRange;
}



void Graph::enumerateSubgraph(const int &k)
{
    for(int i = 0; i <= size; i++)
    {
        vector<int> Vsubgraph;
        Vsubgraph.push_back(i);
        list<int> Vextension = getExtension(i, Vextension);
        
        extendSubgraph(Vsubgraph, Vextension, i, k);
    }
}

void Graph::extendSubgraph(vector<int> Vsubgraph, list<int> &Vextension, int v, const int &k)
{
    if(Vsubgraph.size() == k)
    {
        for(int i = 0; i < Vsubgraph.size(); i++)
            cout << Vsubgraph[i] + 1 << " ";
        cout << endl;
        return;
    }
    
    while(Vextension.size() != 0 && Vsubgraph.size() < k)
    {
        int w = Vextension.front();
        
        Vextension.pop_front();
        Vsubgraph.push_back(w);
        
        list<int> Vextension2 = getExtension(w, Vextension);
        extendSubgraph(Vsubgraph, Vextension2, v, k);
        
        Vsubgraph.pop_back();
    }
}

list<int> Graph::getExtension(const int &v, const list<int>& Vextension) const{
    list<int> newExtension = Vextension;
    
    for(EdgeNode *w = vertices[v].edgeHead; w != NULL; w = w->nextEdge)
    {
        if(w->adjVertex > v)
        {
            if(!isDuplicate(w->adjVertex, newExtension))
                newExtension.push_back(w->adjVertex);
        }
    }

    return newExtension;
}

bool Graph::isDuplicate(const int &target, const list<int>& Vextension) const
{
    for(list<int>::const_iterator it = Vextension.begin(); it != Vextension.end(); it++)
    {
        if(target == *it)
            return true;
    }
    return false;
}
