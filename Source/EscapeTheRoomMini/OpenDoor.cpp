// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	if (!Owner) { UE_LOG(LogTemp, Warning, TEXT("Owner not found.")); return; }
	//Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
	OnOpen.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	if (!Owner) { UE_LOG(LogTemp, Warning, TEXT("Owner not found.")); return; }
	OnClose.Broadcast();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPlate() > MassRequire)
	{
		OpenDoor();
	}
	else {
		CloseDoor();
	}
}


float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	if (!PressurePlate) {
		UE_LOG(LogTemp, Warning, TEXT("PressurePlate not found."));
		return 0;
	}
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;

	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}
