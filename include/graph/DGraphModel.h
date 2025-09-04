/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
#include "hash/xMap.h"
#include "stacknqueue/Queue.h"
#include "sorting/DLinkedListSE.h"

//////////////////////////////////////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
class DGraphModel: public AbstractGraph<T>
{
private:
public:
    DGraphModel(bool (*vertexEQ)(T&, T&),  string (*vertex2str)(T&) ): AbstractGraph<T>(vertexEQ, vertex2str) { }
    
    void connect(T from, T to, float weight = 0)
    {
        // TO DO
        typename AbstractGraph<T>::VertexNode* source = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* destination = this->getVertexNode(to);

        if (source == nullptr)
            throw VertexNotFoundException(this->vertex2str(from));

        if (destination == nullptr)
            throw VertexNotFoundException(this->vertex2str(to));

        source->connect(destination, weight);
    }

    void disconnect(T from, T to)
    {
        // TO DO
        typename AbstractGraph<T>::VertexNode* source = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* destination = this->getVertexNode(to);

        if (source == nullptr)
            throw VertexNotFoundException(this->vertex2str(from));

        if (destination == nullptr)
            throw VertexNotFoundException(this->vertex2str(to));

        typename AbstractGraph<T>::Edge* edge = source->getEdge(destination);
        if (edge == nullptr)
            throw EdgeNotFoundException("E(" + this->vertex2str(from) + "," + this->vertex2str(to) + ")");
        
        source->removeTo(destination);
        destination->removeTo(source);
    }

    void remove(T vertex)
    {
        // TO DO
        typename AbstractGraph<T>::VertexNode* node = this->getVertexNode(vertex);

        if (node == nullptr)
            throw VertexNotFoundException(this->vertex2str(vertex));

        for (auto it = this->nodeList.begin(); it != this->nodeList.end(); it++)
        {
            typename AbstractGraph<T>::VertexNode* vertexNode = *it;
            vertexNode->removeTo(node);
            node->removeTo(vertexNode);
        }

        this->nodeList.removeAt(this->nodeList.indexOf(node));
        delete node;
    }
    
    static DGraphModel<T>* create(T* vertices, int nvertices, Edge<T>* edges, int nedges, bool (*vertexEQ)(T&, T&), string (*vertex2str)(T&))
    {
        // TO DO
        DGraphModel<T>* graph = new DGraphModel<T>(vertexEQ, vertex2str);

        for (int i = 0; i < nvertices; i++)
            graph->add(vertices[i]);

        for (int i = 0; i < nedges; i++)
            graph->connect(edges[i].from, edges[i].to, edges[i].weight);

        return graph;
    }
};

#endif /* DGRAPHMODEL_H */