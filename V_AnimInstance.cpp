// Fill out your copyright notice in the Description page of Project Settings.


#include "V_AnimInstance.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"
#include "Animation/AnimMontage.h"
#include "conquerorCharacter.h"
#include "villain_char.h"



void UV_AnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    // Bind Notify Begin delegate
    OnPlayMontageNotifyBegin.AddDynamic(this, &UV_AnimInstance::OnNotifyBeginReceived);

    // Initialize playerChar from owning actor
    AActor* Owner = GetOwningActor();
    if (Owner)
    {
        Villain = Cast<Avillain_char>(Owner);
    }
}

void UV_AnimInstance::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
    UE_LOG(LogTemp, Warning, TEXT("Notify Begin (Delegate): %s"), *NotifyName.ToString());

    if (!Villain)
    {
        UE_LOG(LogTemp, Warning, TEXT("Villain is null"));
        return;
    }

    if (NotifyName == FName("AttackStart"))
    {
        Villain->isV_Attacking = true;

        Villain->MeleeAttackHittrance();


    }
    else if (NotifyName == FName("AttackEnd"))
    {
        Villain->isV_Attacking = false;

    }

   
}
