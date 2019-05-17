// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseNextWayPoint.h"


EBTNodeResult::Type UChooseNextWayPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("AI in C++ - yay!"));
	return EBTNodeResult::Succeeded;
}
