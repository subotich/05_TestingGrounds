// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseNextWayPoint.h"
#include "AIController.h"
#include "PatrollingGuard.h"	// TODO de-couple
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChooseNextWayPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// TODO protect against empty patrol routes

	// Get the patrol points (from ChooseNextWayPoint_BP function)
	auto AIController = OwnerComp.GetAIOwner();
	auto ControlledPawn = AIController->GetPawn();
	auto PatrollingGuard = Cast<APatrollingGuard>(ControlledPawn);
	auto PatrolPoints = PatrollingGuard->PatrolPointsCPP;

	// Set next waypoint (from ChooseNextWayPoint_BP function)
	auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComponent->GetValueAsInt(IndexKey.SelectedKeyName);
	UE_LOG(LogTemp, Warning, TEXT("Waypoint index: %i"), Index);
	UE_LOG(LogTemp, Warning, TEXT("Waypoint name: %s"), *WaypointKey.SelectedKeyName.ToString());
	BlackboardComponent->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

	// Cycle index (from ChooseNextWayPoint_BP function)
	UE_LOG(LogTemp, Warning, TEXT("Patrol points size: %i"), PatrolPoints.Num());
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComponent->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}
