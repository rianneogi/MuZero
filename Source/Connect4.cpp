#include "Connect4.h"

bool Connect4::isOver() const
{
	for (int i = 0; i < 7;i++)
	{
		for (int j = 0; j < 6;j++)
		{
			if(mState[i*6+j]==2)
			{
				continue;
			}

			for (int k = 1; k < 4; k++)
			{
				if(j+k >= 6)
				{
					break;
				}

				if(mState[i*6+j+k]!=mState[i*6+j+k-1])
				{
					break;
				}

				if(k==3)
				{
					return true;
				}
			}

			for (int k = 1; k < 4; k++)
			{
				if(j-k < 0)
				{
					break;
				}

				if(mState[i*6+j-k]!=mState[i*6+j-k+1])
				{
					break;
				}

				if(k==3)
				{
					return true;
				}
			}

			for (int k = 1; k < 4; k++)
			{
				if(i+k >= 7)
				{
					break;
				}

				if(mState[(i+k)*6+j]!=mState[(i+k-1)*6+j])
				{
					break;
				}

				if(k==3)
				{
					return true;
				}
			}

			for (int k = 1; k < 4; k++)
			{
				if(i-k < 0)
				{
					break;
				}

				if(mState[(i-k)*6+j]!=mState[(i-k+1)*6+j])
				{
					break;
				}

				if(k==3)
				{
					return true;
				}
			}

			for (int k = 1; k < 4; k++)
			{
				if(i+k >= 7 || j+k>=6)
				{
					break;
				}

				if(mState[(i+k)*6+j+k]!=mState[(i+k-1)*6+j+k-1])
				{
					break;
				}

				if(k==3)
				{
					return true;
				}
			}

			for (int k = 1; k < 4; k++)
			{
				if(i+k >= 7 || j-k<0)
				{
					break;
				}

				if(mState[(i+k)*6+j-k]!=mState[(i+k-1)*6+j-k+1])
				{
					break;
				}

				if(k==3)
				{
					return true;
				}
			}

			for (int k = 1; k < 4; k++)
			{
				if(i-k < 0 || j+k>=6)
				{
					break;
				}

				if(mState[(i-k)*6+j+k]!=mState[(i-k+1)*6+j+k-1])
				{
					break;
				}

				if(k==3)
				{
					return true;
				}
			}

			for (int k = 1; k < 4; k++)
			{
				if(i-k < 0 || j-k < 0)
				{
					break;
				}

				if(mState[(i-k)*6+j-k]!=mState[(i-k+1)*6+j-k+1])
				{
					break;
				}

				if(k==3)
				{
					return true;
				}
			}
		}
	}

	for (int i = 0; i < 6 * 7; i++)
	{
		if (mState[i] == 2)
		{
			return false;
		}
	}
	return true; //draw
}

int Connect4::getScore() const
{
	//Returns 1 if Player 1 wins, -1 if Player 2 wins and 0 is its a draw
	for (int i = 0; i < 7;i++)
	{
		for (int j = 0; j < 6;j++)
		{
			if(mState[i*6+j]==2)
			{
				continue;
			}

			for (int k = 1; k < 4; k++)
			{
				if(j+k >= 6)
				{
					break;
				}

				if(mState[i*6+j+k]!=mState[i*6+j+k-1])
				{
					break;
				}

				if(k==3)
				{
					return mState[i*6+j]==0? 1 : -1;
				}
			}

			for (int k = 1; k < 4; k++)
			{
				if(j-k < 0)
				{
					break;
				}

				if(mState[i*6+j-k]!=mState[i*6+j-k+1])
				{
					break;
				}

				if(k==3)
				{
					return mState[i*6+j]==0? 1 : -1;
				}
			}

			for (int k = 1; k < 4; k++)
			{
				if(i+k >= 7)
				{
					break;
				}

				if(mState[(i+k)*6+j]!=mState[(i+k-1)*6+j])
				{
					break;
				}

				if(k==3)
				{
					return mState[i*6+j]==0? 1 : -1;
				}
			}

			for (int k = 1; k < 4; k++)
			{
				if(i-k < 0)
				{
					break;
				}

				if(mState[(i-k)*6+j]!=mState[(i-k+1)*6+j])
				{
					break;
				}

				if(k==3)
				{
					return mState[i*6+j]==0? 1 : -1;
				}
			}

			for (int k = 1; k < 4; k++)
			{
				if(i+k >= 7 || j+k>=6)
				{
					break;
				}

				if(mState[(i+k)*6+j+k]!=mState[(i+k-1)*6+j+k-1])
				{
					break;
				}

				if(k==3)
				{
					return mState[i*6+j]==0? 1 : -1;
				}
			}

			for (int k = 1; k < 4; k++)
			{
				if(i+k >= 7 || j-k<0)
				{
					break;
				}

				if(mState[(i+k)*6+j-k]!=mState[(i+k-1)*6+j-k+1])
				{
					break;
				}

				if(k==3)
				{
					return mState[i*6+j]==0? 1 : -1;
				}
			}

			for (int k = 1; k < 4; k++)
			{
				if(i-k < 0 || j+k>=6)
				{
					break;
				}

				if(mState[(i-k)*6+j+k]!=mState[(i-k+1)*6+j+k-1])
				{
					break;
				}

				if(k==3)
				{
					return mState[i*6+j]==0? 1 : -1;
				}
			}

			for (int k = 1; k < 4; k++)
			{
				if(i-k < 0 || j-k < 0)
				{
					break;
				}

				if(mState[(i-k)*6+j-k]!=mState[(i-k+1)*6+j-k+1])
				{
					break;
				}

				if(k==3)
				{
					return mState[i*6+j]==0? 1 : -1;
				}
			}
		}
	}

	// for (int i = 0; i < 6 * 7; i++)
	// {
	// 	if (mState[i] == 2)
	// 	{
	// 		return false;
	// 	}
	// }
	return 0; //draw
}

void Connect4::restart()
{
	mTurn = 0;
	for(int i = 0;i<6*7;i++)
	{
		mState[i] = 2;
	}
	mNumMoves = 0;
}

std::vector<Action> Connect4::getActions() const
{
    std::vector<Action> v;
    for (int i = 0; i < 7; i++)
    {
		if(mState[i*6 + 5]==2)
		{
			v.push_back(i);
		}
    }
    return v;
}

float *Connect4::stateToTensor() const
{
	float *state = new float[6*7*3 + 1];
	stateToTensor(state);
	return state;
}

void Connect4::stateToTensor(float* res) const
{
	// float *state = new float[10];
	memset(res, 0, 6*7*3 * sizeof(float));
	for (int i = 0; i < 6*7; i++)
	{
		res[3*i+mState[i]] = 1;
	}
	res[6*7*3] = mTurn;
	// return state;
}