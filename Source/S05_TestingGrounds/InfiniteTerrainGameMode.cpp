// Fill out your copyright notice in the Description page of Project Settings.


#include "InfiniteTerrainGameMode.h"
//#include "NavigationSystem/Public/NavMesh/NavMeshBoundsVolume.h"
//#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "Navmesh/NavMeshBoundsVolume.h"
#include "EngineUtils.h"

void AInfiniteTerrainGameMode::PopulateBoundsVolumePool()
{
	auto VolumeIterrator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());

	while (VolumeIterrator)
	{
		AddToPool(*VolumeIterrator);
		++VolumeIterrator;
	}
}

void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume* VolumeToAdd)
{
	UE_LOG(LogTemp, Warning, TEXT("Found Actor: %s"), *VolumeToAdd->GetName());


}
