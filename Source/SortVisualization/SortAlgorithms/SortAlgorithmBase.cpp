// Fill out your copyright notice in the Description page of Project Settings.


#include "SortAlgorithmBase.h"

DEFINE_LOG_CATEGORY(SortAlgorithm);

TArray<FOperationInfo> USortAlgorithmBase::Sort(TArray<int32>& ValuesToSort)
{
	//UE_LOG(SortAlgorithm, Log, TEXT("%s"), *FString(__FUNCTION__));

	unimplemented();

	return TArray<FOperationInfo>();
}

FOperationInfo::FOperationInfo()
{
	OperationType = EOperationType::Info;
	Description = FText::GetEmpty();
	FirstIndex = -1;
	SecondIndex = -1;
}

FOperationInfo::FOperationInfo(EOperationType InOperationType, FText InDescription, int32 InFirstIndex, int32 InSecondIndex)
{
	OperationType = InOperationType;
	Description = InDescription;
	FirstIndex = InFirstIndex;
	SecondIndex = InSecondIndex;
}

FOperationInfo::FOperationInfo(FText InDescription)
{
	OperationType = EOperationType::Info;
	Description = InDescription;
	FirstIndex = -1;
	SecondIndex = -1;
}