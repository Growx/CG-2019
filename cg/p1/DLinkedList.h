// OVERVIEW: DLinkedList.h
// ========
// Class definition for doubly linked list component.
//
// Author(s): Jean Carvalho Ortiz e Lucas Bassani
// Last revision: 20/08/2019

#ifndef __DLinkedList_h
#define __DLinkedList_h

#include <stdio.h>

namespace cg
{
	template<class T>
	class DLLNode {
		friend class SceneObject;
	public:
		T info;
		DLLNode* next, * prev;
		DLLNode() {
			next = prev = nullptr;
		}
		DLLNode(const T& el, DLLNode* n = nullptr, DLLNode* p = nullptr) {
			info = el; next = n; prev = p;
		}
	};

	template<class T>
	class DLinkedList {
		friend class SceneObject;
	public:

		DLinkedList() {
			head = tail = nullptr;
		}
		void addToDLLTail(const T&);
		T deleteFromDLLTail();
		~DLinkedList() {
			clear();
		}
		bool isEmpty() const {
			return head == nullptr;
		}
		void clear();
		void setToNull() {
			head = tail = nullptr;
		}
		void addInMiddle(const T&);
		void addToDLLHead(const T&);
		T deleteFromDLLHead();
		T& firstEl();

	protected:
		DLLNode<T>* head, * tail;
	};

	template<class T>
	void DLinkedList<T>::addToDLLHead(const T& el) {
		if (head != nullptr) {
			head = new DLLNode<T>(el, head, nullptr);
			head->next->prev = head;
		}
		else head = tail = new DLLNode<T>(el);
	}

	template<class T>
	void DLinkedList<T>::addToDLLTail(const T& el) {
		if (tail != nullptr) {
			tail = new DLLNode<T>(el, nullptr, tail);
			tail->prev->next = tail;
		}
		else head = tail = new DLLNode<T>(el);
	}

	template<class T>
	T DLinkedList<T>::deleteFromDLLHead() {
		T el = head->info;
		if (head == tail) { 
			delete head;
			head = tail = nullptr;
		}
		else {             
			head = head->next;
			delete head->prev;
			head->prev = nullptr;
		}
		return el;
	}

	template<class T>
	T DLinkedList<T>::deleteFromDLLTail() {
		T el = tail->info;
		if (head == tail) { 
			delete head;
			head = tail = nullptr;
		}
		else {           
			tail = tail->prev;
			delete tail->next;
			tail->next = nullptr;
		}
		return el;
	}

	template<class T>
	void DLinkedList<T>::addInMiddle(const T& el) {
		if (head != nullptr) {
			if (head->next != nullptr) {
				int i = 1;
				DLLNode<T>* tmp;
				for (tmp = head; tmp->next != nullptr; i++, tmp = tmp->next);
				for (tmp = head, i = i / 2; i; i--, tmp = tmp->next);
				tmp->prev = tmp->prev->next = new DLLNode<T>(el, tmp, tmp->prev);
			}
			else head->next = tail = new DLLNode<T>(el, nullptr, head);
		}
		else head = tail = new DLLNode<T>(el);
	}

	template<class T>
	T& DLinkedList<T>::firstEl() {
		return head->info;
	}

	template<class T>
	void DLinkedList<T>::clear() {
		for (DLLNode<T>* tmp; head != nullptr; ) {
			tmp = head;
			head = head->next;
			delete tmp;
		}
	}
} // end namespace cg

#endif // __DLinkedList_h