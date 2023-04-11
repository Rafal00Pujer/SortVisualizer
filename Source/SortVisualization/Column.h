// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Column.generated.h"

class UTextRenderComponent;

UENUM()
enum EColumnStatusColor
{
	UnsortedColor,
	SortedColor,
	CompareColor,
	SwapColor
};

UCLASS()
class SORTVISUALIZATION_API AColumn : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AColumn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		float GetRightColumnBound() const;

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Sets the column value and height based on a range of possible values.
		/// Additionally, it changes the height of the text.
		/// </summary>
		/// <param name="Value">The randomly selected value of the column.</param>
		/// <param name="MinValueRange">The lower range of values.</param>
		/// <param name="MaxValueRange">The upper range of values.</param>
		void SetColumnValue(int32 Value, int32 MinValueRange, int32 MaxValueRange);

	UFUNCTION(BlueprintCallable)
		int32 GetColumnValue() const;

	/*UFUNCTION(BlueprintCallable)*/
		void SetColumnColor(const FLinearColor& NewColor);

	UFUNCTION(BlueprintCallable)
		void SetColumnColor(EColumnStatusColor NewColumnStatusColor);

private:
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UStaticMeshComponent> ColumnMesh;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UTextRenderComponent> ValueTextRender;

	UPROPERTY(VisibleInstanceOnly)
		int32 ColumnValue;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.01"))
		/// <summary>
		/// Max Allowed Z Scale of Column Mesh.
		/// </summary>
		float MaxColumnZScale;

	UPROPERTY(EditDefaultsOnly)
		TMap<TEnumAsByte<EColumnStatusColor>, FLinearColor> ColumnStatusColors;

	UPROPERTY(VisibleInstanceOnly)
		TObjectPtr<UMaterialInstanceDynamic> DynamicColumnMaterial;

#pragma region Move To New Location
public:
	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Sets a new position for the column and starts the move.
		/// The movement will be in a circle whose diameter is the distance between the current position and the new position.
		/// </summary>
		/// <param name="Time">Time of movement.</param>
		void SetNewColumnLocation(const FVector& NewLocation, float Time);

private:
	static constexpr float AngleOfMotionOnCircle = 180.0;

	UPROPERTY(VisibleInstanceOnly)
		FVector TargetLocation;

	UPROPERTY(VisibleInstanceOnly)
		FVector CircleLocation;

	UPROPERTY(VisibleInstanceOnly)
		FQuat CurrentRotation;

	UPROPERTY(VisibleInstanceOnly)
		/// <summary>
		/// The middle rotation is needed to force the direction of motion on the circle.
		/// </summary>
		FQuat MiddleRotation;

	UPROPERTY(VisibleInstanceOnly)
		FQuat FinalRotation;

	UPROPERTY(VisibleInstanceOnly)
		float CircleRadius;

	UPROPERTY(VisibleInstanceOnly)
		float MoveTime;

	UPROPERTY(VisibleInstanceOnly)
		bool bMiddleReached;

	FTimerHandle MoveToLocationTimer;

	void FinishMove();

	void TickMove(float DeltaTime);
#pragma endregion
};
