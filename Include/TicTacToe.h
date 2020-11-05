#pragma once

#include "Game.h"

class TicTacToe : public Game
{
public:
    // int mTurn;
    int* mState;
    int mNumMoves;
    
    TicTacToe()
    {
        mTurn = 0;
        mState = new int[9];
        for(int i = 0;i<9;i++)
        {
            mState[i] = 2;
        }
        mNumMoves = 0;
        mMaxActions = 9;
        mStateSize = 28;
    }

    ~TicTacToe()
    {
        delete[] mState;
    }

    TicTacToe(const TicTacToe& other)
    {
        mTurn = other.mTurn;
        mNumMoves = other.mNumMoves;
        mState = new int[9];
        memcpy(mState, other.mState, 9 * sizeof(int));
    }

    bool isLegal(Action action) const
    {
        if(mState[action]==2)
            return true;
        return false;
    }
    
    void doAction(Action action)
    {
        mState[action] = mTurn;
        mTurn = (mTurn+1)%2;
        mNumMoves++;
    }

    void undoAction(int pos)
    {
        mState[pos] = 2;
        mTurn = (mTurn + 1) % 2;
        mNumMoves--;
    }

    bool isOver() const;
    
    void restart();
    
    void print() const
    {
        for(int i = 0;i<9;i++)
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
            if((i+1)%3==0)
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

// int play_tictactoe(Genotype* indi, Execution& exe, TicTacToe& game);
// int duel_tictactoe(Genotype* p1, Genotype* p2, Execution& exe, TicTacToe& game);
// void play_tictactoe_human(Genotype* indi, Execution& exe);
