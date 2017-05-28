// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include <algorithm>
#include <functional>
#include <utility>
#include <random>
#include <vector>
#include <unordered_map>
#include <queue>
#include "ANN.generated.h"

typedef std::vector<float> TVector;

UCLASS(Blueprintable)
class TESTVERHSEGAMENNAI_API UANN : public UObject
{
	GENERATED_BODY()
public:
	UANN();

	UFUNCTION(BlueprintCallable, Category=Action)
	TArray<int> ChooseAction(const TArray<float>& state);

	// weights' modification
	UFUNCTION(BlueprintCallable, Category=Action)
	void WModify();

	UFUNCTION(BlueprintCallable, Category = Action)
	inline int AddToQueue(int neuron, TArray<float> respond);

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
	int sizes[3];
	std::vector<std::vector<TArray<float>>> weights;
	std::unordered_map<int, int> chosen;
	std::queue<std::pair<int, TArray<float>>> modification_queue;
	inline std::vector<TVector> Extract(const TArray<float>& data);
};
