// Fill out your copyright notice in the Description page of Project Settings.


#include "Column.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AColumn::AColumn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ColumnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ColumnMesh"));
	ColumnMesh->SetupAttachment(Root);

	ValueTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ValueTextRender"));
	ValueTextRender->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AColumn::BeginPlay()
{
	Super::BeginPlay();

	DynamicColumnMaterial = ColumnMesh->CreateAndSetMaterialInstanceDynamic(0);

	//DynamicColumnMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor(1.0, 0.0, 0.0));
}

// Called every frame
void AColumn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickMove(DeltaTime);
}

float AColumn::GetRightColumnBound() const
{
	FVector LocalMinBounds;
	FVector LocalMaxBounds;

	ColumnMesh->GetLocalBounds(LocalMinBounds, LocalMaxBounds);

	return LocalMaxBounds.Y;
}

void AColumn::SetColumnValue(int32 Value, int32 MinValueRange, int32 MaxValueRange)
{
	checkfSlow(MaxValueRange > MinValueRange, TEXT("Max Value is less than Min Value"));
	checkfSlow(Value >= MinValueRange && Value <= MaxValueRange, TEXT("The Value is outside the Min and Max values."));

	ColumnValue = Value;

	FVector LocalMinBounds;
	FVector LocalMaxBounds;
	ColumnMesh->GetLocalBounds(LocalMinBounds, LocalMaxBounds);
	const float ColumnMeshDefaultUpExtend = LocalMaxBounds.Z;

	const FVector2D InRange(MinValueRange, MaxValueRange);
	const FVector2D OutRange(1, MaxColumnZScale);
	const float NewZScale = FMath::GetMappedRangeValueClamped(InRange, OutRange, ColumnValue);
	const float ComponentsZDeltaLocation = ColumnMeshDefaultUpExtend * NewZScale;

	FVector CurrentColumnMeshScale = ColumnMesh->GetRelativeScale3D();
	CurrentColumnMeshScale.Z = NewZScale;

	ColumnMesh->SetRelativeScale3D(CurrentColumnMeshScale);
	ColumnMesh->AddRelativeLocation(FVector(0.0, 0.0, ComponentsZDeltaLocation));

	ValueTextRender->AddRelativeLocation(FVector(0.0, 0.0, ComponentsZDeltaLocation));
	ValueTextRender->SetText(FText::AsNumber(ColumnValue));
}

int32 AColumn::GetColumnValue() const
{
	return ColumnValue;
}

void AColumn::SetColumnColor(const FLinearColor& NewColor)
{
	DynamicColumnMaterial->SetVectorParameterValue(TEXT("Color"), NewColor);
}

void AColumn::SetColumnColor(EColumnStatusColor NewColumnStatusColor)
{
	const FLinearColor& NewColumnColor = ColumnStatusColors[NewColumnStatusColor];

	SetColumnColor(NewColumnColor);
}

#pragma region Move To New Location
void AColumn::SetNewColumnLocation(const FVector& NewLocation, float Time)
{
	check(Time > 0.0);

	MoveTime = Time;
	TargetLocation = NewLocation;
	bMiddleReached = false;

	const FVector UpVector = GetActorUpVector();
	const FVector RightVector = GetActorRightVector();
	const FVector ForwardVector = GetActorForwardVector();
	const FVector CurrentColumnLocation = GetActorLocation();
	const FVector LocationDifferenceToTarget = TargetLocation - CurrentColumnLocation;

	float DistanceToTargetLoaction;// In other words, the diameter of the circle on which we will move.
	FVector DirectionToTargetLocation;
	LocationDifferenceToTarget.ToDirectionAndLength(DirectionToTargetLocation, DistanceToTargetLoaction);

	CircleRadius = DistanceToTargetLoaction / 2;
	CircleLocation = CurrentColumnLocation + (DirectionToTargetLocation * CircleRadius);
	// Draw the center of the circle.
	DrawDebugPoint(GetWorld(), CircleLocation, 10.0, FColor::Red, false, 2.0);

	FinalRotation = FRotationMatrix::MakeFromXZ(DirectionToTargetLocation, UpVector).ToQuat();
	CurrentRotation = FRotationMatrix::MakeFromXZ(DirectionToTargetLocation * -1.0, UpVector).ToQuat();

	// Dot Product will define the direction of movement on the circle.
	const float DotProduct = FVector::DotProduct(RightVector, DirectionToTargetLocation);
	UE_LOG(LogTemp, Log, TEXT("Actor: %s, Dot Product: %f"), *GetName(), DotProduct);
	checkSlow(FMath::IsNearlyEqual(DotProduct, 1.0) || FMath::IsNearlyEqual(DotProduct, -1.0));

	MiddleRotation = FRotationMatrix::MakeFromXZ(ForwardVector * DotProduct, UpVector).ToQuat();

	GetWorldTimerManager().SetTimer(MoveToLocationTimer, this, &AColumn::FinishMove, MoveTime / 2, true);
}

void AColumn::FinishMove()
{
	//UE_LOG(LogTemp, Log, TEXT("%s"), *FString(__FUNCTION__));

	if (!bMiddleReached)
	{
		bMiddleReached = true;
		return;
	}

	SetActorLocation(TargetLocation);
	GetWorldTimerManager().ClearTimer(MoveToLocationTimer);
}

void AColumn::TickMove(float DeltaTime)
{
	if (!GetWorldTimerManager().IsTimerActive(MoveToLocationTimer))
	{
		return;
	}

	FQuat CurrentTargetRotation;

	// Decide which rotation is our current goal.
	if (bMiddleReached)
	{
		CurrentTargetRotation = FinalRotation;
	}
	else
	{
		CurrentTargetRotation = MiddleRotation;
	}

	// Advance rotation
	CurrentRotation = FMath::QInterpConstantTo(CurrentRotation, CurrentTargetRotation, DeltaTime, FMath::DegreesToRadians(AngleOfMotionOnCircle / MoveTime));

	FVector DirectionToNewLocation = CurrentRotation.Vector();
	FVector NewLocation = CircleLocation + (DirectionToNewLocation * CircleRadius);

	SetActorLocation(NewLocation);
}
#pragma endregion

