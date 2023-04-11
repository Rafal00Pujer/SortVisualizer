// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SortAlgorithms/SortAlgorithmBase.h"
#include "SortVisualizerGameMode.generated.h"

class AColumn;
class USortAlgorithmBase;
class USortingControlWidget;

DECLARE_LOG_CATEGORY_EXTERN(SortVisualizer, Log, All);

/**
 *
 */
UCLASS()
class SORTVISUALIZATION_API ASortVisualizerGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Exec)
		void GenerateColumns();

	UFUNCTION(BlueprintCallable, Exec)
		void Sort();

#pragma region Columns
private:
	UPROPERTY(EditDefaultsOnly, Category = "Columns")
		TSubclassOf<AActor> RowStartReferenceActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Columns")
		TSubclassOf<AColumn> ColumnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Columns", meta = (ClampMin = "1"))
		int32 NumberOfColumns = 20;

	UPROPERTY(EditDefaultsOnly, Category = "Columns")
		int32 MinColumnValue = 20;

	UPROPERTY(EditDefaultsOnly, Category = "Columns")
		int32 MaxColumnValue = 90;

	UPROPERTY(VisibleInstanceOnly, Category = "Columns")
		TArray<AColumn*> Columns;

	TArray<AColumn*> SpawnColumns(TSubclassOf<AColumn> ColumnClassToSpawn, FTransform FirstColumnSpawnTransform, int32 NumberOfColumnsToSpawn, int32 MinRandColumnValue, int32 MaxRandColumnValue) const;

	void DestroyColumns(const TArray<AColumn*>& ColumnsToDestroy) const;
#pragma endregion

#pragma region Sort Algorithm
private:
	UPROPERTY(EditDefaultsOnly, Category = "Sort Algorithm")
		TSubclassOf<USortAlgorithmBase> SortAlgorithmClass;

	UPROPERTY(VisibleInstanceOnly, Category = "Sort Algorithm")
		TObjectPtr<USortAlgorithmBase> SortAlgorithm;
#pragma endregion

#pragma region Visualization of sorting operations
private:
	UPROPERTY(VisibleInstanceOnly, Category = "Visualization of sorting operations")
		TArray<FOperationInfo> SortingOperations;

	UPROPERTY(VisibleInstanceOnly, Category = "Visualization of sorting operations")
		int32 CurrentSortingOperationIndex;

	UPROPERTY(EditDefaultsOnly, Category = "Visualization of sorting operations", meta = (ClampMin = "0.1"))
		float StepTime;

	FTimerHandle StepTimer;

	void ShowNextOperation();

	void FinishShowingCurrentOperation();
#pragma endregion

#pragma region UI
private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<USortingControlWidget> SortingControlWidgetClass;

	UPROPERTY(VisibleInstanceOnly, Category = "UI")
		TObjectPtr<USortingControlWidget> SortingControlWidget;
	void SetupUI();
#pragma endregion
};
