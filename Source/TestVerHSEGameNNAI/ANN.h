// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include <algorithm>
#include <functional>
#include <random>
#include <vector>
#include <unordered_map>
#include "ANN.generated.h"

typedef std::vector<float> TVector;

UCLASS(Blueprintable)
class TESTVERHSEGAMENNAI_API UANN : public UObject
{
	GENERATED_BODY()
public:
	UANN();

	UFUNCTION(BlueprintCallable, Category=Action)
	int ChooseAction(const TArray<float>& state);

	// weights' modification
	UFUNCTION(BlueprintCallable, Category=Action)
	void WModify(float reward);

protected:
	// parameters
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Parameters)
		FVector choice;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Parameters)
		FVector rate;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Parameters)
		FVector contribution;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Parameters)
		FVector vigilance;

private:
	int chosen_neuron;
	int sizes[3];
	std::vector<std::vector<TArray<float>>> weights;
	inline void randomize(TArray<float>& vec);
	std::unordered_map<int, int> chosen;
	std::vector<TVector> input;
};
