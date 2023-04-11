// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SortingControlWidget.generated.h"

class UButton;
class UTextBlock;
class ASortVisualizerGameMode;

/**
 *
 */
UCLASS()
class SORTVISUALIZATION_API USortingControlWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USortingControlWidget(const FObjectInitializer& ObjectInitializer);

	/// <summary>
	/// Sets the Game mode and adds bindings for sorting and generating values.
	/// </summary>
	void SetSortGameModePtr(ASortVisualizerGameMode* SortGameModePtr);

	void SetDescriptionText(const FText& Description);

private:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		TObjectPtr<UButton> GenerateButton;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		TObjectPtr<UButton> SortButton;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		TObjectPtr<UTextBlock> DescriptionText;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<ASortVisualizerGameMode> SortGameMode;
};
