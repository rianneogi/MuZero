#include "TicTacToe.h"

bool TicTacToe::isOver() const
{
	if(mState[0]==mState[1] && mState[0]==mState[2] && mState[0]!=2)
	{
		return true;
	}
	if(mState[3]==mState[4] && mState[3]==mState[5] && mState[3]!=2)
	{
		return true;
	}
	if(mState[6]==mState[7] && mState[6]==mState[8] && mState[6]!=2)
	{
		return true;
	}
	if(mState[0]==mState[3] && mState[0]==mState[6] && mState[0]!=2)
	{
		return true;
	}
	if(mState[1]==mState[4] && mState[1]==mState[7] && mState[1]!=2)
	{
		return true;
	}
	if(mState[2]==mState[5] && mState[2]==mState[8] && mState[2]!=2)
	{
		return true;
	}
	if(mState[0]==mState[4] && mState[0]==mState[8] && mState[0]!=2)
	{
		return true;
	}
	if(mState[2]==mState[4] && mState[2]==mState[6] && mState[2]!=2)
	{
		return true;
	}

	for(int i = 0;i<9;i++)
	{
		if(mState[i]==2)
		{
			return false;
		}
	}
	return true; //draw
}

int TicTacToe::getScore() const
{
	//Returns 1 if X wins, -1 if O wins and 0 is its a draw
	if(mState[0]==mState[1] && mState[0]==mState[2] && mState[0]!=2)
	{
		return mState[0]==0? 1 : -1;
	}
	if(mState[3]==mState[4] && mState[3]==mState[5] && mState[3]!=2)
	{
		return mState[3]==0? 1 : -1;
	}
	if(mState[6]==mState[7] && mState[6]==mState[8] && mState[6]!=2)
	{
		return mState[6]==0? 1 : -1;
	}
	if(mState[0]==mState[3] && mState[0]==mState[6] && mState[0]!=2)
	{
		return mState[0]==0? 1 : -1;
	}
	if(mState[1]==mState[4] && mState[1]==mState[7] && mState[1]!=2)
	{
		return mState[1]==0? 1 : -1;
	}
	if(mState[2]==mState[5] && mState[2]==mState[8] && mState[2]!=2)
	{
		return mState[2]==0? 1 : -1;
	}
	if(mState[0]==mState[4] && mState[0]==mState[8] && mState[0]!=2)
	{
		return mState[0]==0? 1 : -1;
	}
	if(mState[2]==mState[4] && mState[2]==mState[6] && mState[2]!=2)
	{
		return mState[2]==0? 1 : -1;
	}

	// for(int i = 0;i<9;i++)
	// {
	// 	if(mState[i]==2)
	// 	{
	// 		return 0;
	// 	}
	// }
	return 0;
}

void TicTacToe::restart()
{
	mTurn = 0;
	for(int i = 0;i<9;i++)
	{
		mState[i] = 2;
	}
	mNumMoves = 0;
}

std::vector<Action> TicTacToe::getActions() const
{
    std::vector<Action> v;
    for (int i = 0; i < 9; i++)
    {
        if(mState[i]==2)
        {
            v.push_back(i);
        }
    }
    return v;
}

float *TicTacToe::stateToTensor() const
{
	float *state = new float[28];
	stateToTensor(state);
	return state;
}

void TicTacToe::stateToTensor(float* res) const
{
	// float *state = new float[10];
	memset(res, 0, 28 * sizeof(float));
	for (int i = 0; i < 9; i++)
	{
		res[3*i+mState[i]] = 1;
	}
	res[27] = mTurn;
	// return state;
}

// int duel_tictactoe(Genotype* p1, Genotype* p2, Execution& exe, TicTacToe& game) //two individuals play tictactoe
// {
//     game.restart();
//     while(game.is_over()==-1)
//     {
//         if(game.mTurn==0)
//         {
//             exe.reset();
//             for(int i = 0;i<9;i++)
//             {
//                 exe.mRegisters[i+1] = game.mState[i];
//             }
//             exe.run_code(p1, 0);
//             if(exe.mRegisters[0]>=9) exe.mRegisters[0]=8;

//             if(game.is_legal(exe.mRegisters[0]))
//             {
//                 game.make_move(exe.mRegisters[0]);
//             }
//             else 
//             {
//                 return game.mNumMoves;
//             }
//         }
//         else
//         {
//             exe.reset();
//             for(int i = 0;i<9;i++)
//             {
//                 exe.mRegisters[i+1] = game.mState[i];
//             }
//             exe.run_code(p2, 0);
//             if(exe.mRegisters[0]>=9) exe.mRegisters[0]=8;

//             if(game.is_legal(exe.mRegisters[0]))
//             {
//                 game.make_move(exe.mRegisters[0]);
//             }
//             else 
//             {
//                 return 30-game.mNumMoves;
//             }
//         }
//     }

//     if(game.is_over()==2)
//         return 15;
//     if(game.is_over()==0)
//         return 30-game.mNumMoves;
//     if(game.is_over()==1)
//         return game.mNumMoves;
    
//     return game.is_over();
// }

// int play_tictactoe(Genotype* indi, Execution& exe, TicTacToe& game) //individual plays tictactoe against a random opponent
// {
//     game.restart();
//     while(game.is_over()==-1)
//     {
//         if(game.mTurn==0)
//         {
//             exe.reset();
//             memcpy(exe.mRegisters, game.mState, 9*sizeof(int));

//             // for(int i = 0;i<9;i++)
//             // {
//             //     exe.mRegisters[i+1] = game.mState[i];
//             // }
//             exe.run_code(indi, 0);
//             if(exe.mRegisters[0]>=9) exe.mRegisters[0]=8;

//             if(game.is_legal(exe.mRegisters[0]))
//             {
//                 game.make_move(exe.mRegisters[0]);
//             }
//             else 
//             {
//                 return game.mNumMoves;
//             }
//         }
//         else
//         {
//             while(true)
//             {
//                 int r = rand()%9;
//                 if(game.is_legal(r))
//                 {
//                     game.make_move(r);
//                     break;
//                 }
//                 // else 
//                 // {
//                 //     return 30-game.mNumMoves;
//                 // }
//             }
            
//             // for(int i = 0;i<9;i++)
//             // {
//             //     if(game.is_legal(i))
//             //     {
//             //         game.make_move(i);
//             //         break;
//             //     }
//             // }
//         }
//     }
        
//     if(game.is_over()==2)
//         return 15;
//     if(game.is_over()==0)
//         return 30-game.mNumMoves;
//     if(game.is_over()==1)
//         return game.mNumMoves;
    
//     return game.is_over();
// }


// void play_tictactoe_human(Genotype* indi, Execution& exe)
// {
//     TicTacToe game;
    
//     while(game.is_over()==-1)
//     {
//         if(game.mTurn==0)
//         {
//             exe.reset();
//             for(int i = 0;i<9;i++)
//             {
//                 exe.mRegisters[i+1] = game.mState[i];
//             }
//             exe.run_code(indi, 0);
//             if(exe.mRegisters[0]>=9) exe.mRegisters[0]=8;

//             if(game.is_legal(exe.mRegisters[0]))
//             {
//                 game.make_move(exe.mRegisters[0]);
//             }
//             else 
//             {
//                 printf("Illegal Move played by Agent: %d\n", exe.mRegisters[0]);
//                 break;
//             }
//         }
//         else
//         {
//             int x;
//             std::cin >> x;
//             if(game.is_legal(x))
//             {
//                 game.make_move(x);
//             }
//             else
//             {
//                 printf("Illegal move!\n");
//             }
//         }
//         game.print();
//         printf("\n");
//     }
    
//     printf("%d is the winner!\n", game.is_over());
// }