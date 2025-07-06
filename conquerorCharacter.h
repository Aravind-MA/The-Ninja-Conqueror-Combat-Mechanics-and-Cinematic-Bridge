// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EQUIP_Interface.h"
#include "Logging/LogMacros.h"
#include "conquerorCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AconquerorCharacter : public ACharacter, public IEQUIP_Interface
{
	GENERATED_BODY()


	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "attack", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* Hand1SphereComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "attack", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* Hand2SphereComponent;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "attack", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AttackS;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "attack", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Melee_H;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "attack", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* leg1CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "attack", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* leg2CapsuleComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	AconquerorCharacter();
	
	//virtual void Equip_Implementation(bool bEquip) override;

private:
	FVector2D LastMoveInput;


public:

	UFUNCTION()
	void Combathand2SphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

	UFUNCTION()
	void CombatLeg1SphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CombatLeg2SphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AttackShere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void AttackShereEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Look(const FInputActionValue& Value);
			
	UFUNCTION(BlueprintPure, Category = "Input")
	FVector2D GetLastMoveInput() const;

	UFUNCTION(BlueprintPure)
	FName SectionName();

	

	UFUNCTION(BlueprintCallable)
	void EquipAnim(float Rate);

	UFUNCTION(BlueprintCallable)
	void UnEquipAnim(float Rate);

	UFUNCTION(BlueprintCallable)
	void BoxingAnim(FName Section,float Rate);

	UFUNCTION(BlueprintCallable)
	void MeleeAnim(FName Section,float Rate);


	UFUNCTION(BlueprintCallable)
	void H_HitReactAnim(FName Section, float Rate);

	

	
	UFUNCTION(BlueprintPure)
	FName MelleSectionName();

	UFUNCTION(BlueprintPure)
	FName HitreactSectionName();

	

	UFUNCTION(BlueprintCallable)
	void Attack_Mode();

	UFUNCTION(BlueprintCallable)
	void AttackCombat(FName BoxingSection, FName MeleeSection);

	UFUNCTION(BlueprintCallable)
	void AttachToPutin();

	UFUNCTION(BlueprintCallable)
	void AttachToMelee();

	UFUNCTION(BlueprintCallable)
	void AttackHittrance();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	void H_Die();

	class Avillain_char* EnemyVillain;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent)
	void H_ShowHealthBar();
	void H_ShowHealthBar_Implementation();

	//blueprint impletable event can be overridden in Blueprint and Do not provide a body for this function  
	UFUNCTION(BlueprintImplementableEvent)
	void H_HideHealthBar();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Returns Hand2SphereComponent subobject **/
	FORCEINLINE class USphereComponent* GetHand2SphereComponent() const { return Hand2SphereComponent; }

	/** Returns Hand2SphereComponent subobject **/
	FORCEINLINE class UCapsuleComponent* Getleg1CapsuleComponent() const { return leg1CapsuleComponent; }

	/** Returns Hand2SphereComponent subobject **/
	FORCEINLINE class UCapsuleComponent* Getleg2CapsuleComponent() const { return leg2CapsuleComponent; }



	FName Anim1;
	FName Anim2;
	FName Anim3;
	FName Anim4;
	FName Anim5;

	FName MelleAnim1;
	FName MelleAnim2;
	FName MelleAnim3;

	FName HitAnim1;
	FName HitAnim2;
	

	bool isAttacking = false;

	bool isCombat = false;

	bool isoverlap = false;


	

private:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* EquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* UnEquipMontage;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* BoxingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MelleAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactMontage;
	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"))
	bool equipcpp = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animgethit", meta = (AllowPrivateAccess = "true"))
	class UAnimationAsset* Hand_L_get_hit;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animgethit", meta = (AllowPrivateAccess = "true"))
	class UAnimationAsset* Leg_L_get_hit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animgethit", meta = (AllowPrivateAccess = "true"))
	class UAnimationAsset* Leg_R_get_hit;

	float PlayRate = 1.0f;

	FName SectionNameAnim;

	FName MelleSectionNameAnim;

	FName HitReactSectionNameAnim;

	

	FHitResult HitResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animgethit", meta = (AllowPrivateAccess = "true"))
	bool isClimbing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animgethit", meta = (AllowPrivateAccess = "true"))
	bool isledegehit = false;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animgethit", meta = (AllowPrivateAccess = "true"))
	float ClimbInputY;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animgethit", meta = (AllowPrivateAccess = "true"))
	float ClimbInputX;

	
	


	//Enemy's health aspects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyHealth", meta = (AllowPrivateAccess = "true"))
	float E_Damage = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyHealth", meta = (AllowPrivateAccess = "true"))
	float E_MeleeDamage = 25.f;

	

	//Health

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float H_Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float H_MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	FTimerHandle H_HealthBarTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float H_HealthBarTimeDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float H_BaseDamage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage", meta = (AllowPrivateAccess = "true"))
	class UAnimationAsset* H_Death;

};

