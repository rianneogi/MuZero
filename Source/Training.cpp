#include "Training.h"

int sample_action_prob(float* p, int count)
{
	//samples an action from the vector p
	float r = ((rand() % 10000) * 1.0f) / 10000;
	float sum = 0.0f;
	int last_nonzero = -1;
	for (int i = 0; i < count; i++)
	{
		sum += p[i];
		if(r <= sum)
		{
			return i;
		}
		if(p[i]>0)
		{
			last_nonzero = i;
		}
	}
	return last_nonzero;
}

void executeEpisode(std::vector<TrainingExample>& examples, const Net& nn)
{
	//Execute one episode of self-play and append collected data to examples
	TicTacToe game;
	// Net nn;
	// nn.init();

	int episodeStep = 0;
	int EPISODE_MAX_STEP = 250;

	int num_sims = 100;
	float temp = 1.0f;

	int start_example_size = examples.size();
	// float *action_probs = new float[MAX_ACTIONS];
	while (true)
	{
		episodeStep += 1;
		
		float *p = new float[MAX_ACTIONS];
		getActionProb(p, game, nn, num_sims, temp);
		
		std::vector<Action> actions = game.getActions();

		Action a = sample_action_prob(p, MAX_ACTIONS);
		// memcpy(p, action_probs, MAX_ACTIONS * sizeof(float));
		
		examples.push_back(TrainingExample(game.stateToTensor(), game.mTurn==0? 1 : -1, p, 0));

		game.doAction(a);
		
		if(game.isOver())
		{
			for (int i = start_example_size; i < examples.size();i++)
			{
				//update the training data with the result of the game
				examples[i].value = (game.getScore()==examples[i].player)? 1.0 : -1.0;
			}
			break;
		}

		if(episodeStep >= EPISODE_MAX_STEP)
		{
			break;
		}
	}
	// delete action_probs;
}

float pit(const Net& net1, const Net& net2, int num_games)
{
	//Pits net1 and net2 against each other and returns the winrate of net1
	int firstPlayer = 0;
	TicTacToe game;
	int score1 = 0;
	int score2 = 0;
	int EPISODE_MAX_STEP = 250;
	int num_sims = 600;
	float temp = 1.0f;

	int numwins1 = 0;
	int numwins2 = 0;
	int numdraws = 0;

	float *action_probs = new float[MAX_ACTIONS];
	for (int i = 0; i < num_games; i++)
	{
		int episodeStep = 0;
		game.restart();
		firstPlayer = i%2;
		while (true)
		{
			episodeStep += 1;

			// float *action_probs = nullptr;
			
			if((firstPlayer+game.mTurn)%2==0)
			{
				getActionProb(action_probs, game, net1, num_sims, temp);
			}
			else
			{
				getActionProb(action_probs, game, net2, num_sims, temp);
			}

			// std::vector<Action> actions = game.get_actions();
			Action a = sample_action_prob(action_probs, MAX_ACTIONS);

			// examples.push_back(TrainingExample(game.state_to_tensor(),game.mTurn,action_probs,0));

			game.doAction(a);

			if(game.isOver())
			{
				// for (int i = start_example_size; i < examples.size();i++)
				// {
				// 	//update the training data with the result of the game
				// 	examples[i].value = (game.get_score()==examples[i].player)? 1.0 : -1.0;
				// }
				if(game.getScore()==1)
				{
					if(firstPlayer==0)
					{
						numwins1 += 1;
						score1 += 2;
					}
					else
					{
						numwins2 += 1;
						score2 += 2;
					}
				}
				else if(game.getScore()==-1)
				{
					if(firstPlayer==0)
					{
						numwins2 += 1;
						score2 += 2;
					}
					else
					{
						numwins1 += 1;
						score1 += 2;
					}
				}
				else
				{
					numdraws += 1;
					score1 += 1;
					score2 += 1;
				}
				break;
			}

			if(episodeStep >= EPISODE_MAX_STEP)
			{
				//its a draw
				score1 += 1;
				score2 += 1;
				break;
			}

			// turn = (turn + 1) % 2;
		}
	}
	delete action_probs;
	printf("Pit results: Wins %d, Losses %d, Draws %d\n", numwins1, numwins2, numdraws);
	return (score1 * 1.0f) / (score1 + score2);
}

void shuffle(std::vector<TrainingExample>& v1)
{
	int rem = v1.size();
	TrainingExample tmp;
	for (int i = 0; i < v1.size(); i++)
	{
		int r = i + (rand() % rem);
		tmp = v1[i];
		v1[i] = v1[r];
		v1[r] = tmp;
		rem--;
	}
}

void alphaZero(int num_iters, int num_eps, float updateThreshold)
{
	//Performs num_iters iterations with num_eps episodes of self-play in each iteration.
	//After every iteration, it retrains neural network with training data from self-play
	//Then it pits the new network with the old and accepts it if it plays better

	int MAX_EXAMPLES_SIZE = 30000;
	int num_pitting_games = 20;
	int training_epochs = 20;

	std::vector<TrainingExample> examples;


	Net nn;
	Net nn_old;
	printf("Initializing net 1\n");
	nn.init();
	printf("Initializing net 2\n");
	nn_old.init();
	printf("Net initialization complete\n");

	nn.load("tictactoe.bin");
	bool loaded_from_old_network = true;
	nn.copyToGPU();

	for (int iteration = 0; iteration != num_iters; iteration++)
	{
		printf("Iteration %d\n", iteration);

		for (int i = 0; i < num_eps;i++)
		{
			if(i%10==0)
				printf("Executing episode %d\n", i);
			executeEpisode(examples, nn);
		}

		if(examples.size() >= MAX_EXAMPLES_SIZE)
		{
			//delete oldest examples, but this actually deletes random examples due to the shuffle
			while(examples.size() > MAX_EXAMPLES_SIZE)
			{
				examples.pop_back();
			}
		}

		printf("Num examples: %d\n", examples.size());
		shuffle(examples);
		nn.train(examples, training_epochs);

		bool accept = true;
		float wr = 0;
		if (iteration >= 1 || loaded_from_old_network)
		{
			wr = pit(nn, nn_old, num_pitting_games);
			if (wr < updateThreshold)
				accept = false;
		}
		
		if(accept)
		{
			printf("Accepting new network, wr: %f\n", wr);
			nn.copyToCPU();
			nn_old.copyFrom(nn);
			nn_old.copyToGPU();
			nn.save("tictactoe.bin");
		}
		else
		{
			printf("Rejecting new network, wr: %f\n", wr);
			nn_old.copyToCPU();
			nn.copyFrom(nn_old);
			nn.copyToGPU();
		}
	}
}

Action getHumanInput(const TicTacToe& game, const Net& nn)
{
	while(true)
	{
		printf("Enter a move\n");
		int x;
		std::cin >> x;

		if(x==-1)
		{
			PolicyValuePair pv = nn.predict(game);
			for (int i = 0; i < MAX_ACTIONS;i++)
			{
				printf("%f ", pv.policy[i]);
			}
			printf("\nValue: %f\n", pv.value);
		}

		if(game.isLegal(x))
		{
			return x;
		}
		printf("Illegal move, try again\n");
	}
}

void playVsHuman(int num_sims)
{
	TicTacToe game;

	int firstPlayer = rand() % 2;
	game.print();
	float *action_probs = new float[MAX_ACTIONS];
	float temp = 0;

	Net nn;
	nn.init();
	nn.load("tictactoe.bin");
	nn.copyToGPU();

	while (true)
	{
		Action a;
		if ((firstPlayer+game.mTurn)%2 == 0)
		{
			printf("Human to play...\n");
			// getActionProb(action_probs, game, nn, num_sims, temp);
			a = getHumanInput(game, nn);
		}
		else
		{
			printf("Computer is thinking...\n");
			getActionProb(action_probs, game, nn, num_sims, temp);
			// std::vector<Action> actions = game.get_actions();
			a = sample_action_prob(action_probs, MAX_ACTIONS);
			for (int i = 0; i < MAX_ACTIONS;i++)
			{
				printf("%f ", action_probs[i]);
			}
			printf("\n");
			for (int i = 0; i < MAX_ACTIONS; i++)
			{
				assert(action_probs[i] == 0 || game.isLegal(i));
			}
		}


		// examples.push_back(TrainingExample(game.state_to_tensor(),game.mTurn,action_probs,0));
		assert(game.isLegal(a));
		game.doAction(a);
		game.print();

		if(game.isOver())
		{
			if(game.getScore()==1)
			{
				if(firstPlayer==0)
					printf("Human wins!\n");
				else
					printf("Computer wins!\n");
			}
			else if(game.getScore()==-1)
			{
				if(firstPlayer==0)
					printf("Computer wins!\n");
				else
					printf("Human wins!\n");
			}
			else
			{
				printf("Its a draw!\n");
			}
			break;
		}

		// if(episodeStep >= EPISODE_MAX_STEP)
		// {
		// 	//its a draw
		// 	score1 += 1;
		// 	score2 += 1;
		// 	break;
		// }

		// firstPlayer = (toPlay + 1) % 2;
	}
	delete action_probs;
}