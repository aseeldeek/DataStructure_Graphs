#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
/*
------------------------------------------- PROJECT FIVE ------------------------------------------------------
                                        NAME: ASEEL DEEK
                                        STUDENT'S ID : 1190587
                                        SECTION : 3
----------------------------------------------------------------------------------------------------------------

*/
struct Vertex  // store the vertex, weight, and  address of the next node << next vertex >>
{
    int dest; // distance vertex
    int weight; // the distention between the two vertexes
    char city[120]; // name of the road ( or city )
    struct Vertex* next; // pointer to the address of the next vertex
};
typedef struct Vertex*AdjVer;

struct Graph
{
    int V; // size of the graph
    AdjVer* array; // creating array of header << to point to all vertexes
};
typedef struct Graph* My_Ghraph;

/*----------------------------------------------------------------------------------------------------------*/
// creating a vertex to add to the graph
AdjVer newVertex(int dest, char*city, int weight)
{
    AdjVer createVer =(struct Vertex*)malloc(sizeof(struct Vertex)); // allocate this node in the memory
    createVer->dest = dest; // set the dist vertex
    createVer->weight = weight; // set the weight
    strcpy(createVer->city,city); // set the city name
    createVer->next = NULL; // make it pointing to null
    return createVer;
}
/*----------------------------------------------------------------------------------------------------------*/
// this function construct the graph
My_Ghraph createGraph(int V)
{
    My_Ghraph graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V; // assigning the size

    // create an array of adjacency List
    graph->array = (struct Vertex**)malloc(V * sizeof(struct Vertex));

    //  make each adjacency list as empty by assining head as NULL
    for (int i = 0; i < V; ++i)
        graph->array[i] = NULL;

    return graph;
}
/*----------------------------------------------------------------------------------------------------------*/
// adds an edge to an undirected graph (( creating the connection between the vertexes ))
// this implementation make it Undirected Graph
void addEdge(My_Ghraph graph, int src,int dest,char*city, int weight)
{
    /* first create a node from dest vertex to source vertex */
    AdjVer createVer =newVertex(dest, city, weight); // create the vertex
    createVer->next = graph->array[src]; // connected  to the array index
    graph->array[src] = createVer; // connect the index to it

    /* second repeat the process  but create a node from source vertex to dest vertex  */
    createVer = newVertex(src,city, weight);
    createVer->next = graph->array[dest];
    graph->array[dest] = createVer;
}
/*----------------------------------------------------------------------------------------------------------*/
/* printing the graph to make sure it's working */
void printGraph(My_Ghraph graph)
 {

    for (int v = 0; v < graph->V; v++)
    {

        AdjVer temp = graph->array[v];

        while (temp!=NULL)
        {
            printf("(%d -> %d), %s %d\n",v,temp->dest,temp->city, temp->weight);

            temp = temp->next;
        }
        printf("\n");
    }
  }
My_Ghraph graph; // creating a graph

/*----------------------------------------------------------------------------------------------------------*/

/* this function to find the size of the graph, by taking the largest value of the entered vertexes */
int FindSize()
  {
    char dash[1000];
    char *strAdd; // use to split the file contents
    int v1,v2;
    int maxSize=0; // initial value of  finding the size of the graph
    FILE* in= fopen("segments.txt", "r");  // read the input file
    while(fgets(dash, 1000, in)) // while moving among the inputs
    {
        int cAse=0; // will use in dividing the file

        strAdd= strtok(dash, " ");

        while (strAdd!=NULL) // split each line
        {
            if(cAse==0) // save first index in v1 ( vertex one )
            {
                v1= atoi(strAdd);
                if ( v1 > maxSize)
                {
                    /* compare the value of the first vertex id ,
                                       if it's larger than the given  max, update the max value */
                    maxSize = v1; /* make it equal to vertex value */
                }
            }
            if(cAse==1) // save second index in v2 ( vertex two )
            {
                v2= atoi(strAdd);
                if (v2 > maxSize)
                {
                    /* compare the value of the second vertex id ,
                                         if it's larger than the given  max, update the max value */
                    maxSize = v2;  /* make it equal to vertex value */
                }
            }
            strAdd= strtok(NULL, " ");
            cAse++;
        }
    }

    fclose(in);
    return maxSize ;  /*return the value of the new size  */
   }
/*----------------------------------------------------------------------------------------------------------*/

// read from file and insert to the graph
void ReadFromFile()
 {

     char dash[1000]; // to read the line
     int v1,v2,w;
     char CN1[100]; // save the city of the edge
     char CN2[100];

    graph = createGraph(FindSize()+1); // creating the graph with size equal to the largest value of a vertex  +1
    // plus one here to make sure we have enough vertexes for all the entered one

    FILE* in= fopen("segments.txt", "r"); // read the file

    if( in == NULL)  // if the file is not existing
    {
        printf("Warning: there is a problem in your file try again.. \n");
    }
    else   // giving a message that the file has been read
    {
        printf("File opened, your data has been loaded!\n");
    }

    /*  start splitting the file  to source vertex, distance vertex, city name and the weight ( distention )  */
    while(fgets(dash, 1000, in))
    {
        int cAse=0;

        char *strAdd = strtok(dash, " ");

        while (strAdd!=NULL)
        {
            if(cAse==0) /* case 0:  store the first part in the line in source vertex*/
            {
                v1= atoi(strAdd);   /* this function : convert the string to integer */
            }
            if(cAse==1) /* case 1:  store the second part in the line in distance vertex*/
            {
                v2= atoi(strAdd);
            }
            if(cAse==2) /* case 2:  store the city name part in city string */
            {
                strcpy(CN1,strAdd); // copy strAdd to CN1
                strAdd=strtok(NULL," ");
                cAse++;
                strcpy(CN2,strAdd);
                while(isdigit(CN2[0])==0) // this statement check if the character is digit or not
                    // it will return 0 if it's a character
                {
                    strcat(CN1," "); // concatenate it with space
                    strcat(CN1,CN2); // keep concatenate with the other string
                    strAdd=strtok(NULL," ");
                    strcpy(CN2,strAdd);

                }
            }

            if(cAse==3) /* case 3:  storing the weight- part  */
            {
                w= atoi(strAdd);

            }
            strAdd= strtok(NULL, " ");
            cAse++;
        } /** end of splitting each line */

        /* pos1 */ addEdge(graph,v1,v2,CN1,w);
    } /** end of reading the file line by line */

    fclose(in); // close the file

  }
/*
------------------------------------------------------------------------------------------------------------------------------------
                                                   Dijkstra Algorithm
------------------------------------------------------------------------------------------------------------------------------------
*/
// heaps are like completed Binary tree in structure ( has two children)
// and stored in array

struct MinHeapNode   // to hold data
 {
    int source; // source vertex
    int dist;  // weight of vertex

 };
typedef struct MinHeapNode*Hnode; // create an alias name
/*----------------------------------------------------------------------------------------------------------*/
// represent the entire min heap
struct MinHeap_struct
 {
    int Size;  // number of current nodes in the heap
    int capacity;   // maximum  Size of  the heap
    int *Loc;  //  create an array  of integers to save the position of heapnodes
    Hnode*array;  //  create an array  of nodes
 };
typedef struct MinHeap_struct *MinHe;
/*----------------------------------------------------------------------------------------------------------*/
// creating Min Heap Node
Hnode Creat_MinHeapNode(int source,int dist)
 {
    Hnode minNode =(struct MinHeapNode*)malloc(sizeof(Hnode)); // allocate space for one node
    minNode->source = source; /* assigning values*/
    minNode->dist = dist;
    return minNode;
 }
/*----------------------------------------------------------------------------------------------------------*/
// create a Min Heap
MinHe Create_MinHe(int capacity)
 {
    MinHe minH =(struct MinHeap_struct *)malloc(sizeof(MinHe));
    minH->Loc = (int *)malloc(capacity * sizeof(int)); // creating integer array for position
    minH->Size = 0; // initializing the Size to zero
    minH->capacity = capacity; // assign the capacity of heap to the entered value
    minH->array =(struct MinHeapNode**)malloc(capacity *sizeof(struct MinHeapNode*)); // create array of min heap nodes

    return minH;
 }
/*----------------------------------------------------------------------------------------------------------*/
// Check if the heap is empty or not
int isEmpty(MinHe H)
 {
    return H->Size == 0;
 }
/*----------------------------------------------------------------------------------------------------------*/
/* since insertion in heap done by comparing the entered node with the current nodes
   if it's less << in min heap case >> swap these two values */
void swapNode(Hnode* Node1,Hnode* Node2)
 {
    // swapping the nodes with each others
     Hnode temp = *Node1;
     *Node1 = *Node2;
     *Node2 = temp;
 }
/*----------------------------------------------------------------------------------------------------------*/
// determining the  position for each node in the heap , takes O(log n) since it's binary tree
void  min_Heapify(MinHe H,int i)
 {
    int SM, LF, RH;
     SM = i; // root
     LF  = 2*i+1; // smallest values ( left)
     RH = 2*i+2; // larger values ( right)

    /* start inserting from left Always*/

    // if the left value of the vertex is less than than the root put is as the root
    if (LF < H->Size && H->array[LF]->dist < H->array[SM]->dist )
        SM = LF;
    // if the right value of the vertex is less than than the root  put is as the root
    if (RH < H->Size && H->array[RH]->dist <H->array[SM]->dist )
        SM = RH;
    //  after updating the values  swap it's locations
    if (SM !=  i)
    {
        /// The nodes will be swapped in minheap
        Hnode SMNode = H->array[SM];
        Hnode Nnode = H->array[i];

        /// Swap Locations
        H->Loc[SMNode->source] =  i;
        H->Loc[Nnode->source] = SM;

        ///Swap nodes
        swapNode(&H->array[SM],&H->array[ i]);

        min_Heapify(H, SM);  /// Keep repeating this process till it's has the priority
    }
 }
 /*----------------------------------------------------------------------------------------------------------*/
/// deletion takes O(nlog n) since we we will look to the minimum vertex
Hnode RemoveMin( MinHe H)
 {
    if (isEmpty(H))
    {
    return NULL;
    }
    // Store the root node in other node
       Hnode Root =H->array[0];
    //  printf(" the index will be %d: \n", H->Loc[Root->source]); // will be zero
    // Replace Root node with last node
        Hnode lastNode = H ->array[H->Size - 1]; // Size is 7, after shrinking it
        H->array[0] = lastNode;

    // update position of last node
       H->Loc[Root->source] = H->Size-1;
    // printf("Source is %d \n", H->Size -1);// answer for the first loop is 7
       H->Loc[lastNode->source] = 0; // it becomes the Root

    //decrease heap Size and insert Root
      --H->Size;
     min_Heapify(H,0); // to maintain the heap property  call  min_Heapify  after removing Root.

    return Root;
 }
 /*----------------------------------------------------------------------------------------------------------*/
void Decrease_Key(MinHe H,int source, int dist)
 {
    // Get the index of source in heap array
      int i = H->Loc[source];
    // printf("index position: %d\n", i); will be 1
    /// Get the node and update its dist value
       H->array[i]->dist = dist;
    // Travel up while the complete tree is not hepified.

    // This is a O(Log n) loop
    while (i && H->array[i]->dist < H->array[(i - 1) / 2]->dist) // this condition checks if the weight of the current node
        // is less than it's parent weight if i = 1 it's parent is 0
    {

        // Swap this node with its parent << Changed the index location >>
        H->Loc[H->array[i]->source] =(i-1)/2;  // position of the source vertex is now the position of the parent
        H->Loc[H->array[(i-1)/2]->source] = i; // position of the parent is now the position of source vertex

        swapNode(&H->array[i],&H->array[(i - 1) / 2]);
        // move to parent index
        i = (i - 1) / 2; // to reach the root for the heap
    }
 }
/*----------------------------------------------------------------------------------------------------------*/
//  check if a vertex is in min heap or not
int isInMinHeap(MinHe H, int v)
 {
     if (H->Loc[v] < H->Size)
        return 1;
    return 0;
 }
/*----------------------------------------------------------------------------------------------------------*/
// function used to print the solution ( shortest distance )
void PrintWeight(int dist[], int n)
 {
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        if (dist[i] == INT_MAX )  // if the weight is infinity  print it back
        {
            printf("%d\t\tInfinite\n",i);
        }
        else                     // if not print it's distance
            printf("%d \t\t %d\n", i, dist[i]);
 }
/*----------------------------------------------------------------------------------------------------------*/

// function to print the path from point A to point B, and print the shortest path
void print_Route( int dist[], int i, int d,char city[1000][1000])
 {
    if ( dist[d] ==INT_MAX)
    {
        printf("You Can't Reach This Distention < %d > From This Source < %d >\nNo Path Between Them!\n",d,i);
    }
    else
    {
        printf("\n(%d -> %d), the Shortest path is : [ %d ]\nThe route of the shortest distance is:\n%s\n", i, d, dist[d],city[d]);
    }

 }
 /*----------------------------------------------------------------------------------------------------------*/

// function to print the path from point A to point B, and print the shortest path into the file
void WriteToFile(int dist[], int i, int d,char city[1000][1000])
 {
    FILE *fp = fopen("route.txt","a");

    if ( dist[d] ==INT_MAX)
    {
        fprintf(fp,"You Can't Reach This Distention < %d > From This Source < %d >\nNo Path Between Them!\n",d,i);
        fputs("\n",fp);
    }
    else
    {
        fprintf(fp,"\n(%d -> %d), the Shortest path is : [ %d ]\nThe route of the shortest distance is:\n%s\n", i, d, dist[d],city[d]);
        fputs("\n",fp);
    }

    fclose(fp); //closing the file
 }
 /*----------------------------------------------------------------------------------------------------------*/

// Dijkstra algorithm that calculate the shortest path between source node and all the nodes
void Dijkstra_Algo( int src, int d) // passing the source and the distention vertex
  {

    int V = graph->V;  // Get the number of vertices in graph

    int weight[V]; // this array to save weight to the vertexes

    MinHe H = Create_MinHe(V); // minHeap represents set E


    // Initialize min heap with all vertexes weight value to infinite ( creating the table )
    for (int v = 0; v < V; ++v)
    {
        weight[v] = INT_MAX; // make the weight for all vertex is infinity  since INT_MAX represents infinity in c-language
        H->array[v] = Creat_MinHeapNode(v,weight[v]); // create array of nodes using heap
        H->Loc[v] = v; // create array for position
    }
    // Make weight value of src ( source ) vertex as 0 so that it is remove first from heap

    H->array[src] =Creat_MinHeapNode(src, weight[src]); // weight is inif
    H->Loc[src] = src;
    weight[src] = 0;


    Decrease_Key(H, src, weight[src]); // remove it from the heap
    H->Size = V;

    // Initially Size of min heap is equal to V
    /// printf("number of vertexes in heap : %d\n",V ); will give 7

    // this loop to find the shortest path for all vertexes in the min heap
    char Parent[1000][1000]; // point to cities
    for ( int i = 0 ; i < V ; i++)
    {
        strcpy(Parent[i],"");
    }
    /* while the heap id not empty
    -------------------------------------------------------
       1) find the adjacency vertexes to that extracted vertex
       2) chose the minimum one
       3) update the weight value for that vertex
       4) repeat that process again till the heap is empty ( till reaching all nodes)  */

    while (!isEmpty(H))
    {
        // remove the vertex with minimum distance value
        Hnode minHeapNode =RemoveMin(H); // from the first iteration return 1, with weight of 0

        // Store the removed vertex source number
        int u = minHeapNode->source; // wi11 be 1
        //printf(" u is %d\n", u);//  asn : 1,2,3,4,5,6,7,0

        // traverse through all adjacent vertexes  of u (the extracted vertex) and update their distance values
        AdjVer vPtr =graph->array[u];

        // while that vertex has  connected vertexes find it's value and update it
        while (vPtr != NULL)
        {
            int v = vPtr->dest; // the first distention of this vertex << first iteration is vertex  3 >>
            /// printf(" the first dis for the vertex : %d\n", v); //this line to prove it's 3 // note : not the weight, but distant vertex
            // If distance to v TO u is less than The previous calculation --> Update the weight
            if ( isInMinHeap(H, v) && weight[u] != INT_MAX && vPtr->weight + weight[u] < weight[v])
            {
                weight[v] = weight[u] + vPtr->weight; // update the wight to the new value
            /* these lines to save the path for the distention vertex  */
                strcpy(Parent[v],"");
                strcat(Parent[v],Parent[u]);
                strcat(Parent[v],"-> ");
                strcat(Parent[v],vPtr->city);
                strcat(Parent[v],"\n");

                // update weight value in min heap
                Decrease_Key(H, v, weight[v]);
            }
            vPtr = vPtr->next; // go for the next node for the first head
        }
      } // keep looping till reaching all the nodes

    /* these two methods make sure that the distentions are calculated right */
    // PrintWeight(weight, V);

    /* these two methods to print the Route of the entered indexes into the console and into the file */
    print_Route(weight,src,d,Parent);
    WriteToFile(weight,src,d,Parent);
  }

/*----------------------------------------------------------------------------------------------------------*/
// Main Method
int main()
{
     /**<< creating a Menu  that list what operations can be done in the tree >>*/
        int Choice;
        printf("MENU: \n");
        printf("...............");
        printf("\n1.Read the file and load the data\n2.Enter 2 points to compute the shortest path between them"
                "and save the route of the shortest distance to a file\n3.Eixt\n");
   while(1){
        printf("Enter a choice: \n");
        scanf("%d", &Choice);

    switch(Choice)
        {

     case 1 : /* case 1 read file */
        {
              ReadFromFile();
        }
        break;

     case 2 : /* case 2 load data to find the shortest path  */
        {
            if ( graph != NULL){ // check the condition if the file has been read or no

           int s,d;
                printf("Enter the source Vertex ID :");
                scanf("%d", &s);
                printf("Enter the distention Vertex ID :");
                scanf("%d", &d);
                Dijkstra_Algo(s,d);
        }
         else  // if the file is not read yet, print a statement  as a warning message
            {
         printf("WARNING: if you Please Read file ( by entering 1 before 2) to find the shortest path!\n");
            }
        }
        break;

    case 3 :     /* case 3 Exit the program */
       {
          exit(1);
       }
        break;

    } // end of switch operation

  } // end of while loop

    return 0;
}

