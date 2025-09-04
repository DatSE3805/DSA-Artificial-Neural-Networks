/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TopoSorter.h
 * Author: ltsach
 *
 * Created on July 11, 2021, 10:21 PM
 */

#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"
#include "hash/xMap.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"

template<class T>
class TopoSorter
{
public:
    static int DFS;
    static int BFS; 
    
protected:
    DGraphModel<T>* graph;
    int (*hash_code)(T&, int);
    
public:
    TopoSorter(DGraphModel<T>* graph, int (*hash_code)(T&, int)=0)
    {
        // TO DO
        this->graph = graph;
        this->hash_code = hash_code;
    }

    // sorted = true -> sort the vertex list in ascending
    DLinkedList<T> sort(int mode=0, bool sorted=true)
    {
        // TO DO
        if (mode == DFS)
            return this->dfsSort(sorted);
        else if (mode == BFS)
            return this->bfsSort(sorted);
        else
            return DLinkedList<T>();
    }

    DLinkedList<T> bfsSort(bool sorted=true)
    {
        // TO DO
        xMap<T, int> inDegrees = this->vertex2inDegree(this->hash_code);
        DLinkedListSE<T> zeroInDegrees = this->listOfZeroInDegrees();
        DLinkedListSE<T> sortedList;
        Queue<T> queue;

        if (sorted)
            zeroInDegrees.sort();

        typename DLinkedList<T>::Iterator it = zeroInDegrees.begin();
        while(it != zeroInDegrees.end())
        {
            T vertex = *it;
            queue.push(vertex);
            it++;
        }

        while(!queue.empty())
        {
            T vertex = queue.pop();
            sortedList.add(vertex);

            DLinkedList<T> outwardEdges = this->graph->getOutwardEdges(vertex);
            typename DLinkedList<T>::Iterator it = outwardEdges.begin();
            while(it != outwardEdges.end())
            {
                T outwardVertex = *it;
                int degree = inDegrees.get(outwardVertex);
                inDegrees.put(outwardVertex, degree - 1);
                if (degree - 1 == 0)
                    queue.push(outwardVertex);
                it++;
            }
        }

        return sortedList;
    }

    DLinkedList<T> dfsSort(bool sorted = true)
    {
        // TO DO
        xMap<T, int> inDegrees = this->vertex2inDegree(this->hash_code);
        DLinkedListSE<T> zeroInDegrees = this->listOfZeroInDegrees();
        DLinkedListSE<T> sortedList;
        Stack<T> stack;

        if (sorted)
            zeroInDegrees.sort();

        typename DLinkedList<T>::Iterator it = zeroInDegrees.begin();
        while(it != zeroInDegrees.end())
        {
            T vertex = *it;
            stack.push(vertex);
            it++;
        }

        while(!stack.empty())
        {
            T vertex = stack.pop();
            sortedList.add(vertex);

            DLinkedList<T> outwardEdges = this->graph->getOutwardEdges(vertex);
            typename DLinkedList<T>::Iterator it = outwardEdges.begin();
            while(it != outwardEdges.end())
            {
                T outwardVertex = *it;
                int degree = inDegrees.get(outwardVertex);
                inDegrees.put(outwardVertex, degree - 1);
                if (degree - 1 == 0)
                    stack.push(outwardVertex);
                it++;
            }
        }

        return sortedList;
    }

protected:
    //Helper functions
    xMap<T, int> vertex2inDegree(int (*hash)(T&, int));
    xMap<T, int> vertex2outDegree(int (*hash)(T&, int));
    DLinkedList<T> listOfZeroInDegrees();
};

template<class T>
int TopoSorter<T>::DFS = 0;
template<class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////

#endif /* TOPOSORTER_H */

template<class T>
xMap<T, int> TopoSorter<T>::vertex2inDegree(int (*hash)(T&, int))
{
    xMap<T, int> inDegrees(hash);
    DLinkedList<T> vertices = this->graph->vertices();

    for (auto it = vertices.begin(); it != vertices.end(); it++)
    {
        T vertex = *it;
        inDegrees.put(vertex, 0);
    }

    for (auto it = vertices.begin(); it != vertices.end(); ++it)
    {
        T vertex = *it;
        DLinkedList<T> outwardEdges = this->graph->getOutwardEdges(vertex);

        for (auto edgeIt = outwardEdges.begin(); edgeIt != outwardEdges.end(); ++edgeIt)
        {
            T outwardVertex = *edgeIt;
            int degree = inDegrees.get(outwardVertex);
            inDegrees.put(outwardVertex, degree + 1);
        }
    }

    return inDegrees;
}

template<class T>
DLinkedList<T> TopoSorter<T>::listOfZeroInDegrees()
{
    DLinkedList<T> zeroInDegrees;
    xMap<T, int> inDegrees = this->vertex2inDegree(this->hash_code);
    DLinkedList<T> vertices = this->graph->vertices();

    for (auto it = vertices.begin(); it != vertices.end(); it++)
    {
        T vertex = *it;

        if (inDegrees.get(vertex) == 0)
            zeroInDegrees.add(vertex);
    }

    return zeroInDegrees;
}