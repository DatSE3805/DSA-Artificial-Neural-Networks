/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"
#include "stacknqueue/Queue.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T>
{
private:
public:
    // class UGraphAlgorithm;
    // friend class UGraphAlgorithm;

    UGraphModel(bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str) { }

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
        destination->connect(source, weight);
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

        typename DLinkedList<typename AbstractGraph<T>::VertexNode*>::Iterator it = this->nodeList.begin();

        while (it != this->nodeList.end())
        {
            typename AbstractGraph<T>::VertexNode* vertexNode = *it;
            vertexNode->removeTo(node);
            node->removeTo(vertexNode);
            it++;
        }

        this->nodeList.removeAt(this->nodeList.indexOf(node));
        delete node;
    }

    static UGraphModel<T> *create(T *vertices, int nvertices, Edge<T> *edges, int nedges, bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &))
    {
        // TO DO
        UGraphModel<T> *graph = new UGraphModel<T>(vertexEQ, vertex2str);

        for (int i = 0; i < nvertices; i++)
            graph->add(vertices[i]);

        for (int i = 0; i < nedges; i++)
            graph->connect(edges[i].from, edges[i].to, edges[i].weight);

        return graph;
    }
};

#endif /* UGRAPHMODEL_H */