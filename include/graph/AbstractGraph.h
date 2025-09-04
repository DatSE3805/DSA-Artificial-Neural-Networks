/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AbstractGraph.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 17:48
 */

#ifndef ABSTRACTGRAPH_H
#define ABSTRACTGRAPH_H
#include "graph/IGraph.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

template<class T>
class AbstractGraph: public IGraph<T>
{
public:
    class Edge;         //forward declaration
    class VertexNode;   //forward declaration
    class Iterator;     //forward declaration
    
private:
protected:
    //Using the adjacent list technique, so need to store list of nodes (nodeList)
    DLinkedList<VertexNode*> nodeList; 
    bool (*vertexEQ)(T&, T&);   // to compare two vertices
    string (*vertex2str)(T&);   // to obtain string representation of vertices
    
    VertexNode* getVertexNode(T& vertex)
    {
        typename DLinkedList<VertexNode*>::Iterator it = nodeList.begin();

        while(it != nodeList.end())
        {
            VertexNode* node = *it;

            if(vertexEQ(node->vertex, vertex)) 
                return node;
            
            it++;
        }

        return 0;
    }

    string vertex2Str(VertexNode& node) { return vertex2str(node.vertex); }

    string edge2Str(Edge& edge)
    {
        stringstream os;

        os << "E(" << vertex2str(edge.from->vertex) << "," << vertex2str(edge.to->vertex) << ")";
        return os.str();
    }
    
public:
    AbstractGraph (bool (*vertexEQ)(T&, T&)=0, string (*vertex2str)(T&)=0)
    {
        this->vertexEQ = vertexEQ;
        this->vertex2str = vertex2str;
    }
    virtual ~AbstractGraph(){}
    typedef bool (*vertexEQFunc)(T&, T&);
    typedef string (*vertex2strFunc)(T&);
    vertexEQFunc getVertexEQ() { return this->vertexEQ; }
    vertex2strFunc getVertex2Str() { return this->vertex2str; }

//////////////////////////////////////////////////////////////////////
//////////////////// IMPLEMENTATION of IGraph API ////////////////////
//////////////////////////////////////////////////////////////////////
    /* connect, disconnect, and remove: will be overridden in:
     *  >> UGraphModel
     *  >> DGraphModel
     */
    virtual void connect(T from, T to, float weight = 0) = 0;
    virtual void disconnect(T from, T to) = 0;
    virtual void remove(T vertex) = 0;
    
    /* The following are common methods for UGraphModel and DGraphModel
     */
    virtual void add(T vertex)
    {
        // TO DO
        if (!contains(vertex))
        {
            VertexNode* node = new VertexNode(vertex, vertexEQ, vertex2str);
            nodeList.add(node);
        }
    }

    virtual bool contains(T vertex)
    {
        // TO DO
        VertexNode* node = getVertexNode(vertex);
        return node != nullptr;
    }

    virtual float weight(T from, T to)
    {
        // TO DO
        VertexNode* source = getVertexNode(from);
        VertexNode* destination = getVertexNode(to);

        if (source == nullptr)
            throw VertexNotFoundException(this->vertex2str(from));

        if (destination == nullptr)
            throw VertexNotFoundException(this->vertex2str(to));

        Edge* edge = source->getEdge(destination);

        if (edge == nullptr)
            throw EdgeNotFoundException("E(" + this->vertex2str(from) + "," + this->vertex2str(to) + ")");

        return edge->weight;
    }

    virtual DLinkedList<T> getOutwardEdges(T from)
    {
        // TO DO
        VertexNode* source = getVertexNode(from);

        if (source == nullptr)
            throw VertexNotFoundException(this->vertex2str(from));

        return source->getOutwardEdges();
    }
    
    virtual DLinkedList<T> getInwardEdges(T to)
    {
        //TO DO
        typename AbstractGraph<T>::VertexNode* destination = this->getVertexNode(to);

        if (destination == nullptr)
            throw VertexNotFoundException(this->vertex2str(to));

        DLinkedList<T> ListInwardEdges;

        typename DLinkedList<VertexNode*>::Iterator it = nodeList.begin();

        while(it != nodeList.end())
        {
            VertexNode* node = *it;
            if (node->getOutwardEdges().contains(to))
                ListInwardEdges.add(node->vertex);

            it++;
        }

        return ListInwardEdges;
    }
    
    virtual int size()
    {
        // TO DO
        return nodeList.size();
    }

    virtual bool empty()
    {
        // TO DO
        return nodeList.size() == 0;
    }

    virtual void clear()
    {
        // TO DO
        nodeList.clear();
    }

    virtual int inDegree(T vertex)
    {
        // TO DO
        VertexNode* destination = getVertexNode(vertex);

        if (destination == nullptr)
            throw VertexNotFoundException(this->vertex2str(vertex));

        return destination->inDegree_;
    }

    virtual int outDegree(T vertex)
    {
        // TO DO
        VertexNode* source = getVertexNode(vertex);

        if (source == nullptr)
            throw VertexNotFoundException(this->vertex2str(vertex));

        return source->outDegree_;
    }
    
    virtual DLinkedList<T> vertices()
    {
        // TO DO
        DLinkedList<T> ListVertices;

        typename DLinkedList<VertexNode*>::Iterator it = nodeList.begin();

        while(it != nodeList.end())
        {
            VertexNode* node = *it;
            ListVertices.add(node->vertex);
            it++;
        }

        return ListVertices;
    }

    virtual bool connected(T from, T to)
    {
        // TO DO
        VertexNode* source = getVertexNode(from);
        VertexNode* destination = getVertexNode(to);

        if (source == nullptr)
            throw VertexNotFoundException(this->vertex2str(from));

        if (destination == nullptr)
            throw VertexNotFoundException(this->vertex2str(to));

        Edge* edge = source->getEdge(destination);

        if (edge == nullptr)
            return false;

        return true;
    }

    void println() { cout << this->toString() << endl; }

    virtual string toString()
    {
        string mark(50, '=');
        stringstream os;
        os << mark << endl;
        os << left << setw(12) << "Vertices:" << endl;

        typename DLinkedList<VertexNode*>::Iterator nodeIt = nodeList.begin();

        while(nodeIt != nodeList.end())
        {
            VertexNode* node = *nodeIt;
            os << node->toString() << endl;
            nodeIt++;
        }

        string mark2(30, '-');
        os << mark2 << endl;
        
        os << left << setw(12) << "Edges:" << endl;
        
        nodeIt = nodeList.begin();
        while(nodeIt != nodeList.end())
        {
            VertexNode* node = *nodeIt;
            
            typename DLinkedList<Edge*>::Iterator edgeIt = node->adList.begin();
            while(edgeIt != node->adList.end())
            {
                Edge* edge = *edgeIt;
                os << edge->toString() << endl;
                
                edgeIt++;
            }
            
            nodeIt++;
        }

        os << mark << endl;
        return os.str();
    }
    
    /* begin, end: will be used to traverse on graph's vertices
     * example: assume that "graph" is a graph.
     * Code:
     *      AbstractGraph<T>::Iterator it;
     *      for(it = graph.begin(); it != graph.end(); it++){
     *          T vertex = *it; 
     *          //continue to process vertex here!
     *      }
     */
    Iterator begin(){ return Iterator(this, true); }
    Iterator end(){ return Iterator(this, false); }

//////////////////////////////////////////////////////////////////////
////////////////////////  INNER CLASSES DEFNITION ////////////////////
//////////////////////////////////////////////////////////////////////
    
public:
//BEGIN of VertexNode    
    class VertexNode
    {
    private:
        template<class U>
        friend class UGraphModel;
        T vertex;
        int inDegree_, outDegree_;
        DLinkedList<Edge*> adList; 
        friend class Edge;
        friend class AbstractGraph;
        
        bool (*vertexEQ)(T&, T&);
        string (*vertex2str)(T&);
        
    public:
        VertexNode():adList(&DLinkedList<Edge*>::free, &Edge::edgeEQ){ }
        VertexNode(T vertex, bool (*vertexEQ)(T&, T&), string (*vertex2str)(T&)) : adList(&DLinkedList<Edge*>::free, &Edge::edgeEQ)
        {
            this->vertex = vertex;
            this->vertexEQ = vertexEQ;
            this->vertex2str = vertex2str;
            this->outDegree_ = this->inDegree_ = 0;
        }

        T& getVertex(){ return vertex; }

        void connect(VertexNode* to, float weight = 0)
        {
            // TO DO
            Edge* existingEdge = getEdge(to);
            if (existingEdge != nullptr) 
                existingEdge->weight = weight;
            else 
            {
                Edge* edge = new Edge(this, to, weight);
                adList.add(edge);
                outDegree_++;
                to->inDegree_++;
            }
        }

        DLinkedList<T> getOutwardEdges()
        {
            // TO DO
            DLinkedList<T> ListOutwardEdges;

            typename DLinkedList<Edge*>::Iterator it = adList.begin();

            while(it != adList.end())
            {
                Edge* edge = *it;
                ListOutwardEdges.add(edge->to->vertex);
                it++;
            }

            return ListOutwardEdges;
        }

        Edge* getEdge(VertexNode* to)
        {
            // TO DO
            typename DLinkedList<Edge*>::Iterator it = adList.begin();

            while(it != adList.end())
            {
                Edge* edge = *it;
                if (vertexEQ(edge->to->vertex, to->vertex))
                    return edge;

                it++;
            }

            return nullptr;
        }

        bool equals(VertexNode* node) { return vertexEQ(this->vertex, node->vertex); }
        
        void removeTo(VertexNode* to)
        {
            // TO DO
            typename DLinkedList<Edge*>::Iterator it = adList.begin();

            while(it != adList.end())
            {
                Edge* edge = *it;
                if (vertexEQ(edge->to->vertex, to->vertex))
                {
                    adList.removeAt(adList.indexOf(edge));
                    outDegree_--;    
                    to->inDegree_--;
                    delete edge;
                    break;
                }

                it++;
            }
        }

        int inDegree(){ return inDegree_; }
        int outDegree(){ return outDegree_; }

        string toString()
        {
            stringstream os;
            os << "V(" << this->vertex << ", " << "in: " << this->inDegree_ << ", " << "out: " << this->outDegree_ << ")";
            return os.str();
        }
    };
//END of VertexNode    
    
//BEGIN of Edge
    class Edge
    {
    private:
        VertexNode* from;
        VertexNode* to;
        float weight;
        friend class VertexNode;
        friend class AbstractGraph;
        
    public:
        Edge(){}
        Edge(VertexNode* from, VertexNode* to, float weight = 0)
        {
            this->from = from;
            this->to = to;
            this->weight = weight;
        }

        bool equals(Edge* edge)
        {
            // TO DO
            return this->from->equals(edge->from) && this->to->equals(edge->to);
        }

        static bool edgeEQ(Edge*& edge1, Edge*& edge2) { return edge1->equals(edge2); }

        string toString()
        {
            stringstream os;
            os << "E(" << this->from->vertex << "," << this->to->vertex << "," << this->weight << ")";
            return os.str();
        }
    };
 //END of Edge

//BEGIN of Iterator
public:
    class Iterator
    {
    private:
        typename DLinkedList<VertexNode*>::Iterator nodeIt;
        
    public:
        Iterator(AbstractGraph<T>* pGraph=0, bool begin=true)
        {
            if (begin)
            {
                if(pGraph != 0)
                    nodeIt = pGraph->nodeList.begin();
            }
            else
            {
                if(pGraph != 0)
                    nodeIt = pGraph->nodeList.end();
            }
        }

        Iterator& operator=(const Iterator& iterator)
        {
            this->nodeIt = iterator.nodeIt;
            return *this;
        }
        
        T& operator*() { return (*nodeIt)->vertex; }
        
        bool operator!=(const Iterator& iterator) { return nodeIt != iterator.nodeIt; }

        // Prefix ++ overload 
        Iterator& operator++()
        {
            nodeIt++;
            return *this; 
        }

        // Postfix ++ overload 
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };
//END of Iterator
};

#endif /* ABSTRACTGRAPH_H */