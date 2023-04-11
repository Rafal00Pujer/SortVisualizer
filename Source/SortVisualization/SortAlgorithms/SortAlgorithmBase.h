// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SortAlgorithmBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(SortAlgorithm, Log, All);

UENUM()
enum EOperationType
{
	Info,
	Compare,
	SwapOperation,
	MarkAsSorted
};

USTRUCT(BlueprintType)
struct FOperationInfo
{
	GENERATED_BODY();

public:
	FOperationInfo();
	FOperationInfo(EOperationType InOperationType, FText InDescription, int32 InFirstIndex, int32 InSecondIndex);
	FOperationInfo(FText InDescription);

	UPROPERTY(VisibleAnywhere)
		TEnumAsByte<EOperationType> OperationType;

	UPROPERTY(VisibleAnywhere)
		FText Description;

	UPROPERTY(VisibleAnywhere)
		int32 FirstIndex;

	UPROPERTY(VisibleAnywhere)
		int32 SecondIndex;
};

/**
 *
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class SORTVISUALIZATION_API USortAlgorithmBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		virtual TArray<FOperationInfo> Sort(TArray<int32>& ValuesToSort);
};
