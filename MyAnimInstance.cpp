// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"
#include "Animation/AnimMontage.h"
#include "EQUIP_Interface.h"
#include "conquerorCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

void UMyAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    // Bind Notify Begin delegate
    OnPlayMontageNotifyBegin.AddDynamic(this, &UMyAnimInstance::OnNotifyBeginReceived);

    // Initialize playerChar from owning actor
    AActor* Owner = GetOwningActor();
    if (Owner)
    {
        playerChar = Cast<AconquerorCharacter>(Owner);
    }
}

void UMyAnimInstance::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
    UE_LOG(LogTemp, Warning, TEXT("Notify Begin (Delegate): %s"), *NotifyName.ToString());

    if (!playerChar)
    {
        UE_LOG(LogTemp, Warning, TEXT("playerChar is null"));
        return;
    }

    if (NotifyName == FName("putit"))
    {
        playerChar->AttachToPutin();
    }
    else if (NotifyName == FName("melle"))
    {
        playerChar->AttachToMelee();
    }
    else if (NotifyName == FName("Start"))
    {
        playerChar->isAttacking = true;
        playerChar->AttackHittrance();
    }
    else if (NotifyName == FName("End"))
    {
        playerChar->isAttacking = false;
    }
    else if (NotifyName == FName("StartCombat"))
    {
        playerChar->isCombat = true;

       
    }
    else if (NotifyName == FName("EndCombat"))
    {
        playerChar->isCombat = false;

      
    }
}



void UMyAnimInstance::Equip_Implementation(bool bEquip)
{
    isequip = bEquip;
}