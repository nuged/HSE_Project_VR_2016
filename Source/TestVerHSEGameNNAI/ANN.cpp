// Fill out your copyright notice in the Description page of Project Settings.

#include "TestVerHSEGameNNAI.h"
#include "ANN.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

UANN::UANN()
{
	rate = 0.127;

	// set sizes
	INP_SIZE = 2;
	HID_SIZE = 6;
	OUT_SIZE = 8;

	// initialize weights' and biases' sizes
	InpHid.resize(HID_SIZE);
	for (auto& vec : InpHid)
		vec.resize(INP_SIZE);

	HidOut.resize(OUT_SIZE);
	for (auto& vec : HidOut)
		vec.resize(HID_SIZE);

	biInp.resize(HID_SIZE);
	biHid.resize(OUT_SIZE);

	InitializeWeights();

	// set activation functions
	acHid = acOut = BiSig;
	derHid = derOut = derBiSig;
}

unsigned UANN::ChooseAction(const TVector& state) const
{
	TVector output = ActiveOutput(SendHidden(ActiveHidden(SendInput(state))));
	unsigned NumAction = std::max_element(output.begin(), output.end()) - output.begin();
	return NumAction;
}

void UANN::AddToQueue(const TVector & input, const TVector & desired)
{
	if (LearnQueue.size() > 100)
		return;
	std::vector<TVector> temp(2);
	temp[0] = input;
	temp[1] = desired;
	LearnQueue.push(temp);
}

void UANN::Learn()
{
	if (LearnQueue.empty())
		return;

	TVector Input = LearnQueue.top()[0];
	TVector Target = LearnQueue.top()[1];
	LearnQueue.pop();

	//if (GEngine)
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%u"), LearnQueue.size()));


	// get layers
	TVector HidLayer = SendInput(Input); // not activated hidden
	TVector acHidden = ActiveHidden(HidLayer); // activated hidden
	TVector OutLayer = SendHidden(acHidden); // not activated output

	// get values for correting output weights
	{
		TVector Output = ActiveOutput(OutLayer);
		for (size_t i = 0; i < OUT_SIZE; ++i) {
			OutLayer[i] = (Target[i] - Output[i]) * (*derOut)(OutLayer[i]);
		}
	}

	// get values for correcting hidden weigths
	{
		TVector temp(HID_SIZE);
		for (size_t i = 0; i < HID_SIZE; ++i) {
			float sum = 0;
			for (size_t j = 0; j < OUT_SIZE; ++j) {
				sum += OutLayer[j] * HidOut[j][i];
			}
			temp[i] = sum;
		}
		for (size_t i = 0; i < HID_SIZE; ++i) {
			HidLayer[i] = temp[i] * (*derHid)(HidLayer[i]);
		}
	}

	// correct weights
	for (size_t i = 0; i < OUT_SIZE; ++i) {
		for (size_t j = 0; j < HID_SIZE; ++j) {
			HidOut[i][j] += rate * OutLayer[i] * acHidden[j];
		}
		biHid[i] += rate * OutLayer[i];
	}
	for (size_t i = 0; i < HID_SIZE; ++i) {
		for (size_t j = 0; j < INP_SIZE; ++j) {
			InpHid[i][j] += rate * HidLayer[i] * Input[j];
		}
		biInp[i] += rate * HidLayer[i];
	}
	//rate *= 0.9993;
}

void UANN::InitializeWeights()
{
	// randomly fill Input-Hidden connections with values between -0.5 and 0.5
	std::random_device rd;
	std::mt19937_64 generator(rd());
	std::uniform_real_distribution<float> distribution(-0.5, 0.5);
	for (size_t i = 0; i < HID_SIZE; ++i) {
		for (size_t j = 0; j < INP_SIZE; ++j) {
			InpHid[i][j] = distribution(generator);
		}
	}
	for (size_t i = 0; i < OUT_SIZE; ++i) {
		for (size_t j = 0; j < HID_SIZE; ++j) {
			HidOut[i][j] = distribution(generator);
		}
	}
	for (auto& val : biInp) {
		val = distribution(generator);
	}
	for (auto& val : biHid) {
		val = distribution(generator);
	}
}

inline TVector UANN::SendInput(const TVector & input) const
{
	TVector HiddenLayer(HID_SIZE);
	for (size_t i = 0; i < HID_SIZE; ++i) {
		float sum = biInp[i];
		for (size_t j = 0; j < INP_SIZE; ++j) {
			sum += input[j] * InpHid[i][j];
		}
		HiddenLayer[i] = sum;
	}
	return HiddenLayer;
}

inline TVector UANN::ActiveHidden(const TVector & hidden) const
{
	TVector acHidden(HID_SIZE);
	for (size_t i = 0; i < HID_SIZE; ++i) {
		acHidden[i] = (*acHid)(hidden[i]);
	}
	return acHidden;
}

inline TVector UANN::SendHidden(const TVector & hidden) const
{
	TVector Output(OUT_SIZE);
	for (size_t i = 0; i < OUT_SIZE; ++i) {
		float sum = biHid[i];
		for (size_t j = 0; j < HID_SIZE; ++j) {
			sum += hidden[j] * HidOut[i][j];
		}
		Output[i] = sum;
	}
	return Output;
}

inline TVector UANN::ActiveOutput(const TVector & output) const
{
	TVector acOutput(OUT_SIZE);
	for (size_t i = 0; i < OUT_SIZE; ++i) {
		acOutput[i] = (*acOut)(output[i]);
	}
	return acOutput;
}

inline float Sigmoid(float x)
{
	return 1 / (1 + exp(-x));
}

inline float derSigmoid(float x)
{
	return Sigmoid(x) * (1 - Sigmoid(x));
}

inline float BiSig(float x)
{
	float temp = 2 / (1 + exp(-x)) - 1;
	return temp;
}

inline float derBiSig(float x)
{
	return 0.5 * (1 + BiSig(x)) * (1 - BiSig(x));
}
