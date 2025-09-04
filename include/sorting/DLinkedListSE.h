/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DLinkedListSE.h
 * Author: LTSACH
 *
 * Created on 31 August 2020, 14:13
 */

#ifndef DLINKEDLISTSE_H
#define DLINKEDLISTSE_H
#include "list/DLinkedList.h"
#include "sorting/ISort.h"

template<class T>
class DLinkedListSE: public DLinkedList<T>
{
public:
    
    DLinkedListSE(void (*removeData)(DLinkedList<T>*)=0, bool (*itemEQ)(T&, T&)=0 ) : DLinkedList<T>(removeData, itemEQ) { };
    
    DLinkedListSE(const DLinkedList<T>& list) { this->copyFrom(list); }
    
    void sort(int(*comparator)(T&, T&) = 0)
    {
        // You should implement the merge sort algorithm
        int size = this->size();
        T* array = new T[size];

        typename DLinkedList<T>::Iterator it = this->begin();
        for(int i = 0; i < size; i++)
        {
            array[i] = *it;
            it++;
        }

        mergeSort(array, 0, size - 1, comparator);

        this->clear();
        for(int i = 0; i < size; i++)
            this->add(array[i]);

        delete[] array;
    }

    void merge(T* array, int left, int mid, int right, int (*comparator)(T&, T&)) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        T* leftArray = new T[n1];
        T* rightArray = new T[n2];

        // Copy data to temporary arrays
        for (int i = 0; i < n1; i++) leftArray[i] = array[left + i];
        for (int i = 0; i < n2; i++) rightArray[i] = array[mid + 1 + i];

        // Merge the arrays back into the original array
        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (comparator)
            {
                if (comparator(leftArray[i], rightArray[j]) <= 0)
                    array[k++] = leftArray[i++];
                else
                    array[k++] = rightArray[j++];
            } 
            else
            {
                if (leftArray[i] <= rightArray[j])
                    array[k++] = leftArray[i++];
                else
                    array[k++] = rightArray[j++];
            }
        }

        // Copy remaining elements of leftArray and rightArray, if any
        while (i < n1) array[k++] = leftArray[i++];
        while (j < n2) array[k++] = rightArray[j++];

        delete[] leftArray;
        delete[] rightArray;
    }

    void mergeSort(T* array, int left, int right, int (*comparator)(T&, T&)) {
        if (left >= right) return;

        int mid = left + (right - left) / 2;

        // Sort the left and right halves
        mergeSort(array, left, mid, comparator);
        mergeSort(array, mid + 1, right, comparator);

        // Merge the sorted halves
        merge(array, left, mid, right, comparator);
    }
protected:
    static int compare(T& lhs, T& rhs, int (*comparator)(T&,T&)=0)
    {
        if(comparator != 0)
            return comparator(lhs, rhs);
        else
        {
            if(lhs < rhs) return -1;
            else if(lhs > rhs) return +1;
            else return 0;
        }
    }
};

#endif /* DLINKEDLISTSE_H */