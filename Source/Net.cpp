#include "Net.h"

void Net::init()
{
	//State Size for TicTacToe: 28
	//Action Size for TicTacToe: 9
	//State Size for Connect 4: 7*6*3+1
	//Action Size for Connect 4: 7

	GameStateSize = 7*6*3+1;
	GameActionSize = 7;
	BatchSize = 32;

	// int input_size = 9;
	// int action_size = 9;

	float learning_rate = 0.01;

	board = new Board();

	initializer = new RangeInitializer();

	optimizer = new AdamOptimizer(learning_rate);
	board->setOptimizer(optimizer);
	
	inputBlob = board->newBlob(make_shape(BatchSize, GameStateSize));
	fc1Blob = board->addFCNeuron(inputBlob, 15);
	act1Blob = board->addLeakyReLUNeuron(fc1Blob, 0.05f);
	fc2Blob = board->addFCNeuron(act1Blob, 9);
	act2Blob = board->addLeakyReLUNeuron(fc2Blob, 0.05f);
	outputValueFCBlob = board->addFCNeuron(act2Blob, 1);
	outputValueBlob = board->addTanhNeuron(outputValueFCBlob);
	outputPolicyFCBlob = board->addFCNeuron(act2Blob, GameActionSize);
	outputPolicyBlob = board->addSoftmaxNeuron(outputPolicyFCBlob);

	// fc1Neuron = new FullyConnectedNeuron(inputBlob,fc1Blob,initializer);
	// board->addNeuron(fc1Neuron);
	// fc2Neuron = new FullyConnectedNeuron(inputBlob, fc2Blob, initializer);
	// board->addNeuron(fc2Neuron);
	// outputNeuron = new FullyConnectedNeuron(inputBlob,outputBlob,initializer);
	// board->addNeuron(outputNeuron);

	errorFunctionValue = new MeanSquaredError(outputValueBlob);
	errorFunctionPolicy = new CategoricalCrossEntropyError(outputPolicyBlob);
	board->addErrorFunction(errorFunctionValue);
	board->addErrorFunction(errorFunctionPolicy);

	board->addPlaceholder(&(inputBlob->Data));
	board->addPlaceholder(&errorFunctionValue->mTarget);
	board->addPlaceholder(&errorFunctionPolicy->mTarget);

	assert(board->mOptimizer != nullptr);
	board->setUp();

	predictStateTensor = Tensor(make_shape(BatchSize, GameStateSize));
	predictPolicyTensor = Tensor(make_shape(BatchSize, GameActionSize));
	predictValueTensor = Tensor(make_shape(BatchSize, 1));
}

PolicyValuePair Net::predict(Game* pos) const
{
	// pos.state_to_tensor(predictStateTensor.mData);
	// Tensor state = Tensor(state_tensor, make_shape(BatchSize, GameStateSize), false);
	// Tensor policy = Tensor(make_shape(BatchSize, GameActionSize));
	// Tensor value = Tensor(make_shape(BatchSize, 1));

	// memcpy(state.mData, state_tensor, GameStateSize*sizeof(float));

	// std::vector<Tensor> v;
	// v.push_back(predictStateTensor);
	// v.push_back(predictValueTensor);
	// v.push_back(predictPolicyTensor);

	// board->forward(predictStateTensor);

	pos->stateToTensor(inputBlob->Data.mData);
	inputBlob->copyToGPU();
	for (int i = 0; i < board->mNeurons.size(); i++)
	{
		board->mNeurons[i]->forward();
	}
	outputPolicyBlob->copyToCPU();
	outputValueBlob->copyToCPU();

	PolicyValuePair pv;
	pv.value = outputValueBlob->Data(0);
	pv.policy = new float[pos->mMaxActions];
	memcpy(pv.policy, outputPolicyBlob->Data.mData, GameActionSize * sizeof(float));
	// for (int i = 0; i < game->mMaxActions; i++)
	// {
	// 	pv.policy[i] = policy[i];
	// }
	// printf("here\n");
	// delete state_tensor;
	// printf("here2\n");
	return pv;
}

void Net::train(const std::vector<TrainingExample>& examples, uint64_t num_epochs)
{
	uint64_t num_examples = examples.size();
	gameState = Tensor(make_shape(num_examples, GameStateSize));
	targetPolicy = Tensor(make_shape(num_examples, GameActionSize));
	targetValue = Tensor(make_shape(num_examples, 1));

	for (int i = 0; i < num_examples;i++)
	{
		memcpy(&(gameState(i, 0)), examples[i].position, GameStateSize * sizeof(float));
		memcpy(&(targetPolicy(i, 0)), examples[i].policy, GameActionSize * sizeof(float));
		targetValue(i) = examples[i].value;
	}

	std::vector<Tensor> inputs;
	inputs.push_back(gameState);
	inputs.push_back(targetValue);
	inputs.push_back(targetPolicy);
	board->train(inputs, num_epochs, BatchSize);
}

void Net::copyFrom(const Net& other)
{
	// int size = optimizer->Variables.size();
	// for (int i = 0; i < optimizer->Variables.size();i++)
	// {
	// 	Tensor t = optimizer->Variables[i]->Data;
	// 	memcpy(t.mData, other.optimizer->Variables[i]->Data.mData, t.mSize * sizeof(Float));
	// }

	board->copy_variables(other.board);
}

void Net::save(const std::string& s)
{
	// board->copyVariablesToCPU();
	board->save_variables(s);
}

void Net::load(const std::string& s)
{
	board->load_variables(s);
	// board->copyVariablesToGPU();
}

void Net::copyToGPU()
{
	board->copyVariablesToGPU();
}

void Net::copyToCPU()
{
	board->copyVariablesToCPU();
}