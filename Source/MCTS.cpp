#include "MCTS.h"

const float c_puct = 1.41;

float search(Node* s, Game* game, const Net& nn)
{
	assert(s->Qvalues != nullptr);
	assert(s->children != nullptr);
	assert(s->Nvalues != nullptr);
	// assert(s->Pvalues != nullptr);

	if(game->isOver())
	{
		float v = game->getScore();

		if(game->mTurn==1)
		{
			v = -v;
		}

		return -v;
	}

	if(s->visited==false)
	{
		s->visited = true;
		PolicyValuePair av = nn.predict(game);
		// printf("out of predict\n");
		assert(s->Pvalues == nullptr);
		s->Pvalues = av.policy;

		//normalize the Pvalues
		float sum_pvalues = 0;
		for (int i = 0; i < game->mMaxActions;i++)
		{
			if(game->isLegal(i))
			{
				sum_pvalues += s->Pvalues[i];
			}
			else
			{
				s->Pvalues[i] = 0;
			}
		}
		if(sum_pvalues==0)
		{
			//return a uniform distribution over legal actions
			std::vector<Action> actions = game->getActions();
			for (int i = 0; i < game->mMaxActions; i++)
			{
				if(game->isLegal(i))
				{
					s->Pvalues[i] = (1.0 / actions.size());
				}
				else
				{
					s->Pvalues[i] = 0;
				}
			}
		}
		else
		{
			for (int i = 0; i < game->mMaxActions;i++)
			{
				s->Pvalues[i] /= sum_pvalues;
			}
		}
	
		return -av.value;
	}
	assert(s->Pvalues != nullptr);
	float max_u = -10000;
	Action best_a = -1;
	int best_i = -1;
	std::vector<Action> actions = game->getActions();
	for (int i = 0; i < actions.size();i++)
	{
		Action a = actions[i];
		float u = s->Qvalues[a] + c_puct * s->Pvalues[a] * sqrt(s->Nsum) / (1 + s->Nvalues[a]);
		if (u > max_u)
		{
			max_u = u;
			best_a = a;
			best_i = i;
		}
	}
	Action a = best_a;
	Node *sp = nullptr;
	assert(s->children != nullptr);
	if (s->children[a] == nullptr)
	{
		sp = new Node(s, game->mMaxActions);
		s->addChild(sp, a);
	}
	else
	{
		sp = s->children[a];
	}
	assert(sp != nullptr);
	game->doAction(a);

	float v = search(sp, game, nn);
	game->undoAction(a);
	
	s->Qvalues[a] = (s->Nvalues[a] * s->Qvalues[a] + v) / (s->Nvalues[a] + 1);
	s->Nvalues[a] += 1;
	s->Nsum += 1;
	

	return -v;
}

void getActionProb(float* res, Game* game, const Net& nn, int num_sims, float temp)
{
	Node *root = new Node(nullptr, game->mMaxActions);
	for (int i = 0; i < num_sims; i++)
	{
		// printf("sim %d\n", i);
		search(root, game, nn);
	}

	std::vector<Action> actions = game->getActions();
	assert(actions.size() != 0);
	// float* output = new float[game->mMaxActions];

	if(temp==0) //get best action
	{
		Action best_a = -1;
		float best_v = -10000;
		for (int i = 0; i < actions.size();i++)
		{
			Action a = actions[i];
			if (root->Nvalues[a] > best_v)
			{
				best_v = root->Nvalues[a];
				best_a = a;
			}
		}

		memset(res, 0, game->mMaxActions * sizeof(float));
		res[best_a] = 1;
		// for (int i = 0; i < game->mMaxActions; i++)
		// {
		// 	if(i==best_a)
		// 		res[i] = 1;
		// 	else
		// 		res[i] = 0;
		// }
		// return best_v;
		// return res;
		return;
	}

	int sum_n = 0;
	for (int i = 0; i < game->mMaxActions; i++)
	{
		// Action a = actions[i];
		assert(root->Nvalues[i] == 0 || game->isLegal(i));
		res[i] = pow(root->Nvalues[i], temp);
		sum_n += root->Nvalues[i];
	}

	if(sum_n == 0)
	{
		//return a uniform distribution over legal actions
		// float* uniform = new float[game->mMaxActions];
		for (int i = 0; i < game->mMaxActions;i++)
		{
			if(game->isLegal(i))
			{
				res[i] = (1.0 / actions.size());
			}
			else
			{
				res[i] = 0;
			}
		}
		// return uniform;
	}

	for (int i = 0; i < game->mMaxActions; i++)
	{
		// Action a = actions[i];
		res[i] = ((res[i] * 1.0f) / sum_n);
	}

	delete root;
	// return root->Nvalues[best_a];
	// return res;
}