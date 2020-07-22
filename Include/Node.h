#pragma once

#include "Connect4.h"

extern const int MAX_ACTIONS;

class Node
{
public:
	float *Qvalues;
	float *Pvalues;
	int *Nvalues;
	int Nsum;

	bool visited;
	int NumActions;

	Node *parent;
	Node **children;

	Node() : Qvalues(nullptr),Nvalues(nullptr),Pvalues(nullptr),visited(false),Nsum(0),parent(nullptr), children(nullptr) {}

	~Node()
	{
		for (int i = 0; i < NumActions;i++)
		{
			delete children[i];
		}
		delete Qvalues;
		delete Pvalues;
		delete Nvalues;
		delete children;
	}

	Node(Node* p, int max_actions)
	{
		NumActions = max_actions;
		Qvalues = new float[max_actions];
		Nvalues = new int[max_actions];
		// Pvalues = new float[max_actions];
		Pvalues = nullptr;
		children = new Node *[max_actions];
		visited = false;
		Nsum = 0;
		parent = p;

		for (int i = 0; i < max_actions;i++)
		{
			Qvalues[i] = 0;
			Nvalues[i] = 0;
			// Pvalues[i] = 0;
			children[i] = nullptr;
		}
	}

	void addChild(Node* n, Action a)
	{
		// children.push_back(n);
		children[a] = n;
	}
};