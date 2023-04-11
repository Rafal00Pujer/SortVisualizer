// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SortAlgorithmBase.h"
#include "BubbleSort.generated.h"

/**
 *
 */
UCLASS()
class SORTVISUALIZATION_API UBubbleSort : public USortAlgorithmBase
{
	GENERATED_BODY()

public:
	virtual TArray<FOperationInfo> Sort(TArray<int32>& ValuesToSort) override;

private:
	UPROPERTY(EditDefaultsOnly)
		FText BeginSortingText;

	UPROPERTY(EditDefaultsOnly)
		FText EndSortingText;

	UPROPERTY(EditDefaultsOnly)
		FText CompareText;

	UPROPERTY(EditDefaultsOnly)
		FText SwapText;

	UPROPERTY(EditDefaultsOnly)
		FText MarkAsSortedText;

	void AddBeginOperation(TArray<FOperationInfo>& SortingOperations, int32 NumberOfValuesToSort);

	void AddEndOperation(TArray<FOperationInfo>& SortingOperations);

	void AddCompareOperation(TArray<FOperationInfo>& SortingOperations, const TArray<int32>& ValuesToSort, int32 FirstIndex, int32 SecondIndex);

	void AddSwapOperation(TArray<FOperationInfo>& SortingOperations, const TArray<int32>& ValuesToSort, int32 FirstIndex, int32 SecondIndex);

	void AddMarkAsSortedOperation(TArray<FOperationInfo>& SortingOperations, const TArray<int32>& ValuesToSort, int32 FirstIndex);

	void MarkRemainingValuesAsSorted(TArray<FOperationInfo>& SortingOperations, const TArray<int32>& ValuesToSort, int32 LastIndex);
};
