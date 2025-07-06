// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EQUIP_Interface.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CONQUEROR_API UMyAnimInstance : public UAnimInstance, public IEQUIP_Interface
{
	GENERATED_BODY()
	

public:

    UPROPERTY()
    class AconquerorCharacter* playerChar;

protected:
    virtual void NativeInitializeAnimation() override;

    //BulletHit_Implementation Function(Interface)
    virtual void Equip_Implementation(bool bEquip) override;

    UFUNCTION()
    void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& Payload);



private:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "animequip", meta = (AllowPrivateAccess = "true"))
    bool isequip;
};
