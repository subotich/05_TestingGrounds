// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "NavigationSystem.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Min = FVector(0, -2000, 0);
	Max = FVector(4000, 2000, 0);
	NavigationBoundsOffset = FVector(2000, 0, 0);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UE_LOG(LogTemp, Warning, TEXT("[%s] EndPlay"), *GetName());

	Pool->Return(NavMeshBoundsVolume);

}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Configured in the blueprint's event graph
void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	TArray<FSpawnPoint> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);

	for (FSpawnPoint SpawnPosition : SpawnPositions)
	{
		PlaceActor(ToSpawn, SpawnPosition);
	}
}

TArray<FSpawnPoint> ATile::RandomSpawnPositions(int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	TArray<FSpawnPoint> SpawnPositions;
	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPoint SpawnPosition;
		FVector SpawnPoint;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);

		bool bFoundEmptyLocation = FindEmptyLocation(SpawnPosition.Location, (Radius * SpawnPosition.Scale));

		if (bFoundEmptyLocation)
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			SpawnPositions.Add(SpawnPosition);
		}
	}
	return SpawnPositions;
}


void ATile::SetPool(UActorPool* InPool)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Setting Pool %s"), *(this->GetName()), *(InPool->GetName()));

	Pool = InPool;

	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();

	if (NavMeshBoundsVolume == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] No actors in pool."), *GetName());
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("[%s] Checked out: {%s}"), *GetName(), *NavMeshBoundsVolume->GetName());
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	
	// After moving NavMeshBoundsVolume it has to be rebuild
	// Updated navigation system
	// TODO implement this, it doesn't work
	/*
	FNavigationSystem::Build(*GetWorld());

	auto WorldSettings = GetWorldSettings();

	auto isNavigationBuild = GetWorld()->GetNavigationSystem()->IsNavigationBuilt(WorldSettings);
	if (isNavigationBuild)
	{
		UE_LOG(LogTemp, Warning, TEXT("Navi build."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Navi NOT build."));
	}
	*/
}



// TODO improve this function so that it includes checking for already spawned actors & walls
bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	FBox Bounds(Min, Max);

	const int MAX_ATTEMPTS = 100;
	for (size_t i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidatePoint, Radius)) {
			OutLocation = CandidatePoint;
			return true;
		}
	}

	return false;
}


void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPoint SpawnPosition)
{
	AActor* Spawned = GetWorld()->SpawnActor(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPosition.Location);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
	Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
}


// TODO check overlapping functionality, still issues with spawns 
bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);

	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation + FVector(0, 0, 1),		// Fix for UE 4.22.2
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);

	//FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
	
	// Draw spheres/capsules

	//DrawDebugSphere(GetWorld(), Location, Radius, 100, ResultColor, true, 100);
	//DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100);
	
	// Check hits

	//UE_LOG(LogTemp, Warning, TEXT("Is hit: %s"), (HasHit ? TEXT("True") : TEXT("False")));

	return HasHit;
}

