// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyController.h"
#include "villain_char.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"


AAIEnemyController::AAIEnemyController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	check(BlackboardComponent);

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	check(BehaviorTreeComponent);


}

void AAIEnemyController::BeginPlay()
{
	Super::BeginPlay();

	
}

void AAIEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (InPawn == nullptr) return;

	Enemy = Cast<Avillain_char>(InPawn);
	if (Enemy)
	{
		if (Enemy->GetBehaviorTree())
		{											//Dereference operator due to InitializeBlackboard function needs an Input in the form of the UBlackBoardData but BlackBoardAsset from the GetBehaviorTree() ("BehaviorTree") is a pointer
			BlackboardComponent->InitializeBlackboard(*(Enemy->GetBehaviorTree()->BlackboardAsset));
		}
	}
	
}



void AAIEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}
