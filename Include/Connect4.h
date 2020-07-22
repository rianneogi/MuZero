#pragma once

#include "TicTacToe.h"

class Connect4 : public Game
{
public:
	// int mTurn;
	int *mState;
	int mNumMoves;

	Connect4()
    {
        mTurn = 0;
        mState = new int[6*7];
        for(int i = 0;i<6*7;i++)
        {
            mState[i] = 2;
        }
        mNumMoves = 0;
    }
    
    ~Connect4()
    {
        delete[] mState;
    }

    Connect4(const TicTacToe& other)
    {
        mTurn = other.mTurn;
        mNumMoves = other.mNumMoves;
        mState = new int[6*7];
        memcpy(mState, other.mState, 6*7 * sizeof(int));
    }

    bool isLegal(int pos) const
    {
		if(pos>=7 || pos < 0)
		{
			return false;
		}
		if(mState[pos*6 + 5]==2)
            return true;
        return false;
    }
    
    void doAction(int pos)
    {
		int ht = 0;
		for (int i = 0; i < 6; i++)
		{
			if(mState[pos*6+i]==2)
			{
				ht = i;
				break;
			}
		}
		mState[pos*6+ht] = mTurn;
        mTurn = (mTurn+1)%2;
        mNumMoves++;
    }

    void undoAction(int pos)
    {
		int ht = 0;
		for (int i = 0; i < 6; i++)
		{
			if(mState[pos*6+i]==2)
			{
				ht = i-1;
				break;
			}
		}
		mState[pos * 6 + ht] = 2;
		mTurn = (mTurn + 1) % 2;
        mNumMoves--;
    }

    bool isOver() const;
    
    void restart();
    
    void print() const
    {
        for(int i = 0;i<7*6;i++)
        {
            if(mState[i]==0)
            {
                printf("x ");
            }
            else if(mState[i]==1)
            {
                printf("o ");
            }
            else
            {
                printf("- ");
            }
            // printf("%d ", mState[i]);
            if((i+1)%6==0)
            {
                printf("\n");
            }
        }
    }

    int getScore() const;

    float *stateToTensor() const;
    void stateToTensor(float *res) const;

    std::vector<Action> getActions() const;
};