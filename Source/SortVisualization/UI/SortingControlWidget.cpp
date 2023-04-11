// Fill out your copyright notice in the Description page of Project Settings.


#include "SortingControlWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../SortVisualizerGameMode.h"

USortingControlWidget::USortingControlWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void USortingControlWidget::SetSortGameModePtr(ASortVisualizerGameMode* SortGameModePtr)
{
	check(SortGameModePtr);

	// Remove the current bindings for buttons.
	if (SortGameMode)
	{
		GenerateButton->OnClicked.RemoveDynamic(SortGameMode, &ASortVisualizerGameMode::GenerateColumns);
		SortButton->OnClicked.RemoveDynamic(SortGameMode, &ASortVisualizerGameMode::Sort);
	}

	SortGameMode = SortGameModePtr;

	// Add bindings for buttons.
	GenerateButton->OnClicked.AddDynamic(SortGameMode, &ASortVisualizerGameMode::GenerateColumns);
	SortButton->OnClicked.AddDynamic(SortGameMode, &ASortVisualizerGameMode::Sort);
}

void USortingControlWidget::SetDescriptionText(const FText& Description)
{
	DescriptionText->SetText(Description);
}