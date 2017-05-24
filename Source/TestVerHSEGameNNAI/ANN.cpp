// Fill out your copyright notice in the Description page of Project Settings.

#include "TestVerHSEGameNNAI.h"
#include "ANN.h"

UANN::UANN()
{
	// initialize sizes
	sizes[0] = 8;
	sizes[1] = 8;
	sizes[2] = 2;

	input.resize(3);
	for (int i = 0; i < 3; ++i)
		input[i].resize(sizes[i]);

	weights.resize(sizes[1]);
	for (auto& vecs : weights) {
		vecs.resize(3);
		for (int i = 0; i < 3; ++i)
			vecs[i].SetNumZeroed(sizes[i]);
	}

	// fill weights with random nums
	for (auto& vecs : weights)
		for (int i = 0; i < 3; ++i)
			randomize(vecs[i]);

	// initialize parameters
	choice = FVector(0.33f, 0.33f, 0.33f);
	rate = FVector(0.33f, 0.33f, 0.33f);
	contribution = FVector(0.33f, 0.33f, 0.33f);
	vigilance = FVector(0.33f, 0.33f, 0.33f);
}

int UANN::ChooseAction(const TArray<float>& state)
{
	for (int i = 0; i < sizes[0]; ++i)
		input[0][i] = state[i];

	input[1].resize(sizes[1]);
	fill(input[1].begin(), input[1].end(), 1);

	input[2] = { 1, 0 };

	// calculating choose function for every neuron
	TVector cognitive(weights.size());
	for (int i = 0; i < weights.size(); ++i) {
		float sum = 0;
		for (int j = 0; j < 3; ++j) {
			float val = 0;
			float norm = 0;
			for (int k = 0; k < sizes[j]; ++k) {
				val += std::min(input[j][k], weights[i][j][k]);
				norm += weights[i][j][k];
			}
			sum += contribution[j] * val / (choice[j] + norm);
		}
		cognitive[i] = sum;
	}

	// find neuron with maximal CF
	bool indicator = true;
	int index;
	while (indicator) {
		auto iter = std::max_element(cognitive.begin(), cognitive.end());
		index = iter - cognitive.begin();
		indicator = false;
		for (int i = 0; i < 3; ++i) {
			float val = 0;
			float norm = 0;
			for (int j = 0; j < sizes[i]; ++j) {
				val += std::min(input[i][j], weights[index][i][j]);
				norm += input[i][j];
			}
			float m = val / norm;
			if (m < vigilance[i]) {
				indicator = true;
				*iter = 0;
				break;
			}
		}
	}

	// check if we chose it before
	bool ok = chosen.insert({ index, index }).second;
	if (ok) {
		std::vector<TArray<float>> new_weights(3);
		for (int i = 0; i < 3; ++i) {
			new_weights[i].SetNumZeroed(sizes[i]);
			randomize(new_weights[i]);
		}
		weights.push_back(new_weights);
	}

	// record currently chosen neuron
	chosen_neuron = index;

	// return action
	TVector actions(sizes[1]);
	for(int i = 0; i < sizes[1]; ++i)
		actions[i] = weights[index][1][i];

	int action = std::max_element(actions.begin(), actions.end()) - actions.begin();
	return action;
}

void UANN::WModify(float reward)
{
	input[2] = { reward, 1 - reward };
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < sizes[i]; ++j) {
			float val = weights[chosen_neuron][i][j];
			weights[chosen_neuron][i][j] = (1 - rate[i]) * val;
			weights[chosen_neuron][i][j] += rate[i] * std::min(input[i][j], val);
		}
	}
}

inline void UANN::randomize(TArray<float>& vec)
{

	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_real_distribution<float> dis(0.5, 1.0);
	auto gen = std::bind(dis, eng);
	std::generate(begin(vec), end(vec), gen);
}
