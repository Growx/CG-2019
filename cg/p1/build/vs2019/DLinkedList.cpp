// OVERVIEW: DLinkedList.cpp
// ========
// Source file for generic scene node.
//
// Author(s): Jean Carvalho Ortiz
// Last revision: 20/09/2019

#include "DLinkedList.h"
#include <cstdarg>
#include <cstdio>

namespace cg
{ // begin namespace cg


/////////////////////////////////////////////////////////////////////
//
// Doubly Linked List implementation
// =========
	DLinkedList::DLinkedList()
	{
		this->first = NULL;
		this->minimum = NULL;
	}

	bool DLinkedList::isEmpty()
	{
		return (this->first == NULL);
	}

	void DLinkedList::insert(Node* x)
	{
		if (isEmpty())
		{
			this->first = x;
			this->minimum = x;
			x->left = x;
			x->right = x;
		}
		else
		{
			Node* last = this->first->left;
			last->right = x;
			x->left = last;
			this->first->left = x;
			x->right = this->first;
			if (x->key < this->minimum->key) this->minimum = x;
		}
	}

	Node* DLinkedList::getMinimum()
	{
		return this->minimum;
	}

	Node* DLinkedList::extractMinimum()
	{
		if (isEmpty()) return NULL;
		Node* ret = this->minimum;
		if (ret->right == ret)
		{
			this->first = NULL;
			this->minimum = NULL;
		}
		else
		{
			Node* prev = ret->left;
			Node* next = ret->right;
			prev->right = next;
			next->left = prev;
			if (ret == this->first)
			{
				this->first = next;
			}
			this->minimum = this->first;
			Node* curr = this->first->right;
			while (curr != this->first)
			{
				if (curr->key < this->minimum->key) this->minimum = curr;
				curr = curr->right;
			}
		}
		return ret;
	}

	void DLinkedList::decreaseKey(Node* x, int newKey)
	{
		x->key = newKey;
		if (x->key < this->minimum->key) this->minimum = x;
	}

	void DLinkedList::Delete(Node* x)
	{
		if (this->minimum == x) extractMinimum();
		else
		{
			Node* prev = x->left;
			Node* next = x->right;
			prev->right = next;
			next->left = prev;
			if (x == this->first)
			{
				this->first = next;
			}
		}
	}

	void DLinkedList::merge(DLinkedList* cdll)
	{
		if (cdll->isEmpty()) return;
		if (this->isEmpty())
		{
			this->first = cdll->first;
			this->minimum = cdll->minimum;
			return;
		}
		Node* first1 = this->first;
		Node* last1 = this->first->left;
		Node* first2 = cdll->first;
		Node* last2 = cdll->first->left;
		first1->left = last2;
		last1->right = first2;
		first2->left = last1;
		last2->right = first1;
		if (cdll->minimum->key < this->minimum->key) this->minimum = cdll->minimum;
	}

	void DLinkedList::print()
	{
		if (isEmpty()) { printf("\n"); return; }
		else
		{
			if (this->first == this->minimum) printf("*%d*", this->first->key);
			else printf("%d", this->first->key);
			Node* curr = this->first;
			do
			{
				curr = curr->right;
				if (this->minimum == curr) printf(" <-> *%d*", curr->key);
				else printf(" <-> %d", curr->key);
			} while (curr != this->first);
			printf("\n");
		}
	}

} // end namespace cg
