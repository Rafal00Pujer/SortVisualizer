// Fill out your copyright notice in the Description page of Project Settings.


#include "BubbleSort.h"

TArray<FOperationInfo> UBubbleSort::Sort(TArray<int32>& ValuesToSort)
{
	UE_LOG(SortAlgorithm, Log, TEXT("%s"), *FString(__FUNCTION__));

	TArray<FOperationInfo> SortingOperations;

	AddBeginOperation(SortingOperations, ValuesToSort.Num());

	int32 i;
	int32 j;
	bool bSwapped;

	for (i = 0; i < ValuesToSort.Num() - 1; i++)
	{
		bSwapped = false;

		int32 LastIndexOfInnerLoop = ValuesToSort.Num() - 1 - i;

		for (j = 0; j < LastIndexOfInnerLoop; j++)
		{
			int32 FirstIndex = j;
			int32 SecondIndex = j + 1;

			AddCompareOperation(SortingOperations, ValuesToSort, FirstIndex, SecondIndex);

			if (ValuesToSort[FirstIndex] > ValuesToSort[SecondIndex])
			{
				AddSwapOperation(SortingOperations, ValuesToSort, FirstIndex, SecondIndex);

				int32 Temp = ValuesToSort[FirstIndex];
				ValuesToSort[FirstIndex] = ValuesToSort[SecondIndex];
				ValuesToSort[SecondIndex] = Temp;

				bSwapped = true;
			}
		}

		// Don't add 1 to j because the last iteration of the inner loop did it.
		AddMarkAsSortedOperation(SortingOperations, ValuesToSort, j/* + 1*/);

		if (!bSwapped)
		{
			MarkRemainingValuesAsSorted(SortingOperations, ValuesToSort, LastIndexOfInnerLoop - 1);

			break;
		}
	}

	AddEndOperation(SortingOperations);

	return SortingOperations;
}

void UBubbleSort::AddBeginOperation(TArray<FOperationInfo>& SortingOperations, int32 NumberOfValuesToSort)
{
	FText FormatedText = FText::Format(BeginSortingText, NumberOfValuesToSort);

	FOperationInfo BeginOperation(FormatedText);

	SortingOperations.Add(BeginOperation);
}

void UBubbleSort::AddEndOperation(TArray<FOperationInfo>& SortingOperations)
{
	//FText FormatedText = FText::Format(EndSortingText);

	FOperationInfo EndOperation(EndSortingText);

	SortingOperations.Add(EndOperation);
}

void UBubbleSort::AddCompareOperation(TArray<FOperationInfo>& SortingOperations, const TArray<int32>& ValuesToSort, int32 FirstIndex, int32 SecondIndex)
{
	FText FormatedText = FText::Format(CompareText, ValuesToSort[FirstIndex], FirstIndex, ValuesToSort[SecondIndex], SecondIndex);

	FOperationInfo CompareOperation(EOperationType::Compare, FormatedText, FirstIndex, SecondIndex);

	SortingOperations.Add(CompareOperation);
}

void UBubbleSort::AddSwapOperation(TArray<FOperationInfo>& SortingOperations, const TArray<int32>& ValuesToSort, int32 FirstIndex, int32 SecondIndex)
{
	FText FormatedText = FText::Format(SwapText, ValuesToSort[FirstIndex], FirstIndex, ValuesToSort[SecondIndex], SecondIndex);

	FOperationInfo SwapOperation(EOperationType::SwapOperation, FormatedText, FirstIndex, SecondIndex);

	SortingOperations.Add(SwapOperation);
}

void UBubbleSort::AddMarkAsSortedOperation(TArray<FOperationInfo>& SortingOperations, const TArray<int32>& ValuesToSort, int32 FirstIndex)
{
	FText FormatedText = FText::Format(MarkAsSortedText, ValuesToSort[FirstIndex], FirstIndex);

	FOperationInfo MarkAsSortedOperation(EOperationType::MarkAsSorted, FormatedText, FirstIndex, -1);

	SortingOperations.Add(MarkAsSortedOperation);
}

void UBubbleSort::MarkRemainingValuesAsSorted(TArray<FOperationInfo>& SortingOperations, const TArray<int32>& ValuesToSort, int32 LastIndex)
{
	for (int32 i = 0; i <= LastIndex; i++)
	{
		AddMarkAsSortedOperation(SortingOperations, ValuesToSort, i);
	}
}
