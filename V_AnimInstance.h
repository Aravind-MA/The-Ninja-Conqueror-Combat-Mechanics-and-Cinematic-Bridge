#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "V_AnimInstance.generated.h"

UCLASS()
class CONQUEROR_API UV_AnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:

   UPROPERTY()
   class Avillain_char* Villain;

protected:
    virtual void NativeInitializeAnimation() override;

   

    UFUNCTION()
    void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& Payload);




};
