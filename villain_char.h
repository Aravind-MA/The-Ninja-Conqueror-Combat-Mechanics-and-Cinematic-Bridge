// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "villain_char.generated.h"

UCLASS()
class CONQUEROR_API Avillain_char : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Avillain_char();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "attack", meta = (AllowPrivateAccess = "true"))
	class AAIEnemyController* EnemyController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "attack", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Melee_V;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "attack", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AttackE;

	void MeleeAttackHittrance();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AttackSphereComponent;
	
	UFUNCTION(BlueprintCallable)
	void AttackMontage(FName Section, float Rate);

	UFUNCTION(BlueprintPure)
	FName villainAttackSectionName();

	UFUNCTION(BlueprintCallable)
	void Hit_ReactAnim(FName Section, float Rate);

	UFUNCTION(BlueprintPure)
	FName HitReact_v_SectionName();

	UFUNCTION()
	void AttackShereE(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AttackShereEEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Widget HealthBar

	//BlueprintNativeEvent is also defined in C++, but that can be overridden in Blueprint
	UFUNCTION(BlueprintNativeEvent)
	void ShowHealthBar();
	void ShowHealthBar_Implementation();

	//blueprint impletable event can be overridden in Blueprint and Do not provide a body for this function  
	UFUNCTION(BlueprintImplementableEvent)
	void HideHealthBar();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "radius", meta = (AllowPrivateAccess = "true"))
	float appectradius = 120;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "radius", meta = (AllowPrivateAccess = "true"))
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimationAsset* AttackAssetAnimation;

	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY()
	class AconquerorCharacter* player;

	bool bHasAttacked = false;

	bool isV_Attacking = false;

	FHitResult HitResult;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	void Die();

private:

	/** Behavior tree for the AI Character */
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Attack_v_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage;


	//Health

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	FTimerHandle HealthBarTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float HealthBarTimeDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyHealth", meta = (AllowPrivateAccess = "true"))
	float H_Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"))
	class UAnimationAsset* Death;

public: 

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }


	FName AttackAnim1;
	FName AttackAnim2;
	FName AttackAnim3;

	FName HitReactVAnim1;
	FName HitReactVAnim2;
	FName HitReactVAnim3;

	FName AttackSectionNameAnim;

	FName Hit_v_SectionNameAnim;
};
