// Fill out your copyright notice in the Description page of Project Settings.

#include "TestVerHSEGameNNAI.h"
#include "ANN.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

UANN::UANN()
{
	// initialize sizes
	sizes[0] = 8;
	sizes[1] = 8;
	sizes[2] = 2;

	weights.resize(sizes[1]);
	for (auto& vecs : weights) {
		vecs.resize(3);
		for (int i = 0; i < 3; ++i)
			vecs[i].SetNumZeroed(sizes[i]);
	}

	// fill weights with random nums
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_real_distribution<float> dis(0.5, 1.0);
	auto gen = std::bind(dis, eng);

	for (auto& vecs : weights)
		for (int i = 0; i < 3; ++i)
			std::generate(begin(vecs[i]), end(vecs[i]), gen);

	// initialize parameters
	choice = FVector(0.33f, 0.33f, 0.33f);
	rate = FVector(0.33f, 0.33f, 0.33f);
	contribution = FVector(0.33f, 0.33f, 0.33f);
	vigilance = FVector(0.33f, 0.33f, 0.33f);
}

TArray<int> UANN::ChooseAction(const TArray<float>& state)
{
	std::vector<TVector> input = Extract(state);

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
	
	// add new neuron
	if (ok) {
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_real_distribution<float> dis(0.5, 1.0);
		auto gen = std::bind(dis, eng);

		std::vector<TArray<float>> new_weights(3);
		for (int i = 0; i < 3; ++i) {
			new_weights[i].SetNumZeroed(sizes[i]);
			std::generate(begin(new_weights[i]), end(new_weights[i]), gen);
		}
		weights.push_back(new_weights);
	}

	// record currently chosen neuron
	TArray<int> result;
	result.Add(index);

	// choose action
	TVector actions(sizes[1]);
	for(int i = 0; i < sizes[1]; ++i)
		actions[i] = weights[index][1][i];
	int action = std::max_element(actions.begin(), actions.end()) - actions.begin();

	result.Add(action);
	return result;
}

void UANN::WModify()
{	
	// check if there is data to modify weights
	if (modification_queue.empty())
		return;
	
	// extracting data from queue
	std::pair<int, TArray<float>> cur_elem = modification_queue.front();
	modification_queue.pop();

	int neuron = cur_elem.first;
	std::vector<TVector> respond = Extract(cur_elem.second);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
		FString::Printf(TEXT("%f, %f"),
			respond[2][0], respond[2][1]));

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < sizes[i]; ++j) {
			float val = weights[neuron][i][j];
			weights[neuron][i][j] = (1 - rate[i]) * val;
			weights[neuron][i][j] += rate[i] * std::min(respond[i][j], val);
		}
	}
}

inline int UANN::AddToQueue(int neuron, TArray<float> respond)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
		//FString::Printf(TEXT("%u"), modification_queue.size()));

	std::pair<int, TArray<float>> new_elem(neuron, respond);
	modification_queue.push(new_elem);
	return 0;
}

inline std::vector<TVector> UANN::Extract(const TArray<float>& data)
{
	std::vector<TVector> respond(3);
	for (int i = 0; i < data.Num(); ++i) {
		int type = 0;
		if (i == data.Num() - 1)
			type = 2;
		respond[type].push_back(data[i]);
		respond[type].push_back(1 - data[i]);
	}
	respond[1].resize(sizes[1]);
	fill(respond[1].begin(), respond[1].end(), 1.0);
	return respond;
}
