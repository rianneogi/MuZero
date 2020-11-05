#pragma once

#include "Node.h"

struct PolicyValuePair
{
	float* policy;
	float value;

	// ActionValuePair(Action a, float v): action(a),value(v) {}
};

struct TrainingExample
{
	float *position;
	int player;
	float *policy;
	float value;

	TrainingExample() {}
	TrainingExample(float *_position, int _player, float *_policy, float _value) : position(_position), player(_player), policy(_policy), value(_value) {}
};

class Net
{
public:
	Board* board;

	Blob *inputBlob;
	Blob *conv1Blob;
	Blob *conv2Blob;
	Blob *conv3Blob;
	Blob *fc1Blob;
	Blob *act1Blob;
	Blob *fc2Blob;
	Blob *act2Blob;
	Blob *outputPolicyFCBlob;
	Blob *outputPolicyBlob;
	Blob *outputValueFCBlob;
	Blob *outputValueBlob;

	Tensor gameState;
	Tensor targetValue;
	Tensor targetPolicy;
	Tensor predictStateTensor;
	Tensor predictValueTensor;
	Tensor predictPolicyTensor;

	uint64_t GameStateSize;
	uint64_t GameActionSize;
	uint64_t BatchSize;

	// ConvNeuron *conv1Neuron;
	// ConvNeuron *conv2Neuron;
	// ConvNeuron *conv3Neuron;
	// FullyConnectedNeuron *fc1Neuron;
	// LeakyReLUNeuron *act1Neuron;
	// FullyConnectedNeuron *fc2Neuron;
	// LeakyReLUNeuron *act2Neuron;
	// SoftmaxNeuron *outputPolicyNeuron;
	// FullyConnectedNeuron *outputValueFCNeuron;
	// TanhNeuron *outputValueNeuron;

	ErrorFunction *errorFunctionValue;
	ErrorFunction *errorFunctionPolicy;
	AdamOptimizer *optimizer;
	Initializer *initializer;

	void init();
	PolicyValuePair predict(Game* pos) const;
	void train(const std::vector<TrainingExample>& examples, uint64_t num_epochs);
	void copyFrom(const Net& other);
	void save(const std::string& s);
	void load(const std::string& s);
	void copyToCPU();
	void copyToGPU();
};