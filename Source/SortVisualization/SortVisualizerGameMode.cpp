// Fill out your copyright notice in the Description page of Project Settings.

#include "SortVisualizerGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Column.h"
#include "UI/SortingControlWidget.h"

DEFINE_LOG_CATEGORY(SortVisualizer);

void ASortVisualizerGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetupUI();
}

void ASortVisualizerGameMode::GenerateColumns()
{
	UE_LOG(SortVisualizer, Log, TEXT("%s"), *FString(__FUNCTION__));

	checkf(RowStartReferenceActorClass, TEXT("Row Start Reference Actor Class is null!"));

	TArray<AActor*> RowStartActors;
	UGameplayStatics::GetAllActorsOfClass(this, RowStartReferenceActorClass, RowStartActors);

	checkf(RowStartActors.Num() > 0, TEXT("Row Start Reference Actor not found!"));

	if (RowStartActors.Num() > 1)
	{
		UE_LOG(SortVisualizer, Warning, TEXT("More than 1 Row Start Reference Actor found! The first actor will be used."));
	}

	const FTransform& FirstColumnTransform = RowStartActors[0]->GetActorTransform();

	// Stop sorting.
	SortingOperations.Empty();
	GetWorldTimerManager().ClearTimer(StepTimer);

	DestroyColumns(Columns);
	//Columns.Empty();
	Columns = SpawnColumns(ColumnClass, FirstColumnTransform, NumberOfColumns, MinColumnValue, MaxColumnValue);
}

void ASortVisualizerGameMode::Sort()
{
	UE_LOG(SortVisualizer, Log, TEXT("%s"), *FString(__FUNCTION__));

	// Check if we have columns to sort.
	if (Columns.Num() <= 0)
	{
		UE_LOG(SortVisualizer, Log, TEXT("There are no values to sort."));
		return;
	}

	// Check if the values are already sorted.
	if (SortingOperations.Num() > 0)
	{
		UE_LOG(SortVisualizer, Log, TEXT("The values are already sorted."));
		return;
	}

	// Create an instance of the sort algorithm.
	checkf(SortAlgorithmClass, TEXT("Sort Algorithm Class is null!"));
	SortAlgorithm = NewObject<USortAlgorithmBase>(this, SortAlgorithmClass);

	// Prepare values to sort.
	TArray<int32> ValuesToSort;
	ValuesToSort.Reserve(Columns.Num());

	for (AColumn* Column : Columns)
	{
		ValuesToSort.Add(Column->GetColumnValue());
	}

	// Sort the values and store the operations.
	SortingOperations = SortAlgorithm->Sort(ValuesToSort);

	// Reset current operation index.
	CurrentSortingOperationIndex = -1;

	ShowNextOperation();
}

#pragma region Columns
TArray<AColumn*> ASortVisualizerGameMode::SpawnColumns(TSubclassOf<AColumn> ColumnClassToSpawn, FTransform FirstColumnSpawnTransform, int32 NumberOfColumnsToSpawn, int32 MinRandColumnValue, int32 MaxRandColumnValue) const
{
	//UE_LOG(SortVisualizer, Log, TEXT("%s"), *FString(__FUNCTION__));

	constexpr float MultiplierOfColumnRightBound = 4.0;

	checkf(ColumnClassToSpawn, TEXT("Column Class To Spawn is null!"));
	checkfSlow(NumberOfColumnsToSpawn >= 1, TEXT("Number Of Columns To Spawn is less than 1"));
	checkf(MaxRandColumnValue > MinRandColumnValue, TEXT("Max Rand Column Value is less than Min Rand Column Value"));

	TArray<AColumn*> SpawnedColumns;
	SpawnedColumns.Reserve(NumberOfColumnsToSpawn);

	FTransform& ColumnSpawnTransform = FirstColumnSpawnTransform;

	for (int32 i = 0; i < NumberOfColumnsToSpawn; i++)
	{
		AColumn* SpawnedColumn = GetWorld()->SpawnActor<AColumn>(ColumnClassToSpawn, ColumnSpawnTransform);

		int32 RandomColumnValue = FMath::RandRange(MinRandColumnValue, MaxRandColumnValue);
		SpawnedColumn->SetColumnValue(RandomColumnValue, MinRandColumnValue, MaxRandColumnValue);

		SpawnedColumns.Add(SpawnedColumn);

		float ColumnRightBound = SpawnedColumn->GetRightColumnBound();
		FVector NextColumnLocation = ColumnSpawnTransform.TransformPosition(FVector(0.0, ColumnRightBound * MultiplierOfColumnRightBound * -1.0, 0.0));
		ColumnSpawnTransform.SetLocation(NextColumnLocation);
	}

	UE_LOG(SortVisualizer, Log, TEXT("Distance between first and last column = %f"), FVector::Distance(SpawnedColumns[0]->GetActorLocation(), SpawnedColumns[SpawnedColumns.Num() - 1]->GetActorLocation()));

	return SpawnedColumns;
}

void ASortVisualizerGameMode::DestroyColumns(const TArray<AColumn*>& ColumnsToDestroy) const
{
	for (AColumn* Column : ColumnsToDestroy)
	{
		Column->Destroy();
	}
}
#pragma endregion

#pragma region Visualization of sorting operations
void ASortVisualizerGameMode::ShowNextOperation()
{
	UE_LOG(SortVisualizer, Log, TEXT("%s"), *FString(__FUNCTION__));

	checkSlow(SortingOperations.Num() <= 0);

	// Check if there are more operations to show.
	if (CurrentSortingOperationIndex + 1 >= SortingOperations.Num())
	{
		return;
	}

	CurrentSortingOperationIndex++;

	// Set Step Timer.
	GetWorldTimerManager().SetTimer(StepTimer, this, &ASortVisualizerGameMode::FinishShowingCurrentOperation, StepTime);

	const FOperationInfo& CurrentOperation = SortingOperations[CurrentSortingOperationIndex];

	// Print next operation.
	SortingControlWidget->SetDescriptionText(CurrentOperation.Description);
	UE_LOG(SortVisualizer, Log, TEXT("%s"), *CurrentOperation.Description.ToString());

	// Show next operation.
	switch (CurrentOperation.OperationType)
	{
	case Info:
	{
		// Do nothing.
		break;
	}

	case Compare:
	{
		// Set Columns Colors.
		Columns[CurrentOperation.FirstIndex]->SetColumnColor(EColumnStatusColor::CompareColor);
		Columns[CurrentOperation.SecondIndex]->SetColumnColor(EColumnStatusColor::CompareColor);
		break;
	}

	case SwapOperation:
	{
		// Set Columns Colors.
		Columns[CurrentOperation.FirstIndex]->SetColumnColor(EColumnStatusColor::SwapColor);
		Columns[CurrentOperation.SecondIndex]->SetColumnColor(EColumnStatusColor::SwapColor);

		// Set New Columns locations.
		FVector FirstColumnLocation = Columns[CurrentOperation.FirstIndex]->GetActorLocation();
		FVector SecondColumnLocation = Columns[CurrentOperation.SecondIndex]->GetActorLocation();

		Columns[CurrentOperation.FirstIndex]->SetNewColumnLocation(SecondColumnLocation, StepTime);
		Columns[CurrentOperation.SecondIndex]->SetNewColumnLocation(FirstColumnLocation, StepTime);
		break;
	}

	case MarkAsSorted:
	{
		//Set Column Color.
		Columns[CurrentOperation.FirstIndex]->SetColumnColor(EColumnStatusColor::SortedColor);
		break;
	}

	default:
		break;
	}
}

void ASortVisualizerGameMode::FinishShowingCurrentOperation()
{
	checkSlow(CurrentSortingOperationIndex > -1);

	const FOperationInfo& CurrentOperation = SortingOperations[CurrentSortingOperationIndex];

	switch (CurrentOperation.OperationType)
	{
	case Info:
	{
		// Do nothing.
		break;
	}

	case SwapOperation:
	{
		//Change locations in the array.
		AColumn* Temp = Columns[CurrentOperation.FirstIndex];
		Columns[CurrentOperation.FirstIndex] = Columns[CurrentOperation.SecondIndex];
		Columns[CurrentOperation.SecondIndex] = Temp;

		// The rest is the same as Compare.
	}

	case Compare:
	{
		// Set Columns Colors.
		Columns[CurrentOperation.FirstIndex]->SetColumnColor(EColumnStatusColor::UnsortedColor);
		Columns[CurrentOperation.SecondIndex]->SetColumnColor(EColumnStatusColor::UnsortedColor);

		break;
	}

	case MarkAsSorted:
	{
		// Do nothing.
		break;
	}

	default:
		break;
	}

	ShowNextOperation();
}
#pragma endregion

#pragma region UI
void ASortVisualizerGameMode::SetupUI()
{
	check(SortingControlWidgetClass);

	SortingControlWidget = CreateWidget<USortingControlWidget>(GetWorld(), SortingControlWidgetClass);
	SortingControlWidget->AddToViewport();
	SortingControlWidget->SetSortGameModePtr(this);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->SetShowMouseCursor(true);
}
#pragma endregion