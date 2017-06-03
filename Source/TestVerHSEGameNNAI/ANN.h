// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include <algorithm>
#include <functional>
#include <list>
#include <random>
#include <vector>
#include <stack>
#include "ANN.generated.h"

typedef std::vector<float> TVector;
typedef std::vector<TVector> TWeights;

UCLASS(Blueprintable)
class TESTVERHSEGAMENNAI_API UANN : public UObject
{
	GENERATED_BODY()
public:
	UANN();

	unsigned ChooseAction(const TVector& state) const;
	void AddToQueue(const TVector& input, const TVector& desired);
	inline void Learn();

protected:
	// helpful methods
	void InitializeWeights();

	// send input and get not active hidden layer
	inline TVector SendInput(const TVector& input) const;
	// implemeent activative funtion and get active hidden layer
	inline TVector ActiveHidden(const TVector& hidden) const;
	// send active hid layer to output layer and get output layer
	inline TVector SendHidden(const TVector& hidden) const;
	// active output layer
	inline TVector ActiveOutput(const TVector& output) const;

private:
	float rate;

	// samples queue
	std::vector<TWeights> LearnQueue;
	unsigned QSize;
	std::vector<int> usable;

	// NN sizes
	size_t INP_SIZE;
	size_t HID_SIZE;
	size_t OUT_SIZE;
	
	// Weights
	TWeights InpHid;
	TWeights HidOut;

	// biases
	TVector biInp;
	TVector biHid;
	
	// activation functions
	float(*acHid)(float);
	float(*acOut)(float);
	// their derivatives
	float(*derHid)(float);
	float(*derOut)(float);
};

// activation functions samples
inline float Sigmoid(float x);
inline float derSigmoid(float x);
inline float BiSig(float x);
inline float derBiSig(float x);