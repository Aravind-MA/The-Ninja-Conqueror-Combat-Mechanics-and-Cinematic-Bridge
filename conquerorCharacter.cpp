// Copyright Epic Games, Inc. All Rights Reserved.

#include "conquerorCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"              
#include "Engine/World.h"                      
#include "DrawDebugHelpers.h"                  
#include "CollisionShape.h"                    
#include "Engine/EngineTypes.h" 
#include "villain_char.h"
#include "Components/SkeletalMeshComponent.h"
#include "conquerorCharacter.h"
DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AconquerorCharacter

AconquerorCharacter::AconquerorCharacter():
		H_Health(100.f),
		H_MaxHealth(100.f),
		H_BaseDamage(20.f),
		Anim1("boxing_1"),
		Anim2("boxing_2"),
		Anim3("boxing_3"),
		Anim4("kick_1"),
		Anim5("kick_2"),
		MelleAnim1("melle_1"),
		MelleAnim2("melle_2"),
		MelleAnim3("melle_3"),
		HitAnim1("Hit_1"),
		HitAnim2("Hit_2")
	   

		
{
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Hand1SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Hand1SphereComponent"));
	Hand1SphereComponent->SetupAttachment(GetMesh());

	Hand2SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Hand2SphereComponent"));
	Hand2SphereComponent->SetupAttachment(GetMesh());

	leg1CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("leg1CapsuleComponent"));
	leg1CapsuleComponent->SetupAttachment(GetMesh());

	leg2CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("leg2CapsuleComponent"));
	leg2CapsuleComponent->SetupAttachment(GetMesh());

	//AttackS = CreateDefaultSubobject<USphereComponent>(TEXT("AttackS"));
	//AttackS->SetupAttachment(GetMesh());

	Melee_H = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Melee_H"));
	Melee_H->SetupAttachment(GetMesh());
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AconquerorCharacter::BeginPlay()
{
	Super::BeginPlay();

	

	Hand2SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AconquerorCharacter::Combathand2SphereOverlapBegin);

	leg1CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AconquerorCharacter::CombatLeg1SphereOverlapBegin);

	leg2CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AconquerorCharacter::CombatLeg2SphereOverlapBegin);

	//AttackS->OnComponentBeginOverlap.AddDynamic(this, &AconquerorCharacter::AttackShere);

	//AttackS->OnComponentEndOverlap.AddDynamic(this, &AconquerorCharacter::AttackShereEnd);
	
}

void AconquerorCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AconquerorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AconquerorCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AconquerorCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AconquerorCharacter::H_ShowHealthBar_Implementation()
{
}




void AconquerorCharacter::Combathand2SphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (isCombat && OtherActor )
	{
		Avillain_char* Enemy = Cast<Avillain_char>(OtherActor);
		if (Enemy && Enemy->GetMesh())
		{
			Enemy->Hit_ReactAnim(FName("HitHandR"), 1.0f);
			
			UGameplayStatics::ApplyDamage(Enemy, E_Damage, GetController(), this, UDamageType::StaticClass());
		

			if (OtherActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("Sphere Trace Hit: %s"), *OtherActor->GetName());
			}
			
		}
	}
}

void AconquerorCharacter::CombatLeg1SphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (isCombat && OtherActor )
	{
		Avillain_char* Enemy = Cast<Avillain_char>(OtherActor);
		if (Enemy && Enemy->GetMesh() )
		{
			Enemy->Hit_ReactAnim(FName("HitLegL"), 1.0f);

			UGameplayStatics::ApplyDamage(Enemy, E_Damage, GetController(), this, UDamageType::StaticClass());

			if (OtherActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("Sphere Trace Hit: %s"), *OtherActor->GetName());
			}
		}
	}
}

void AconquerorCharacter::CombatLeg2SphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (isCombat && OtherActor )
	{
		Avillain_char* Enemy = Cast<Avillain_char>(OtherActor);
		if (Enemy && Enemy->GetMesh())
		{
			Enemy->Hit_ReactAnim(FName("HitLegR"), 1.0f);

			UGameplayStatics::ApplyDamage(Enemy, E_Damage, GetController(), this, UDamageType::StaticClass());

			if (OtherActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("Sphere Trace Hit: %s"), *OtherActor->GetName());
			}
		}
	}
}

void AconquerorCharacter::AttackShere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	Avillain_char* Enemy = Cast<Avillain_char>(OtherActor);
	if (Enemy)
	{
		isoverlap = true;

		if (OtherActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Sphere Trace Hit: %s"), *OtherActor->GetName());
		}
	}
	

}

void AconquerorCharacter::AttackShereEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	Avillain_char* Enemy = Cast<Avillain_char>(OtherActor);
	if (Enemy)
	{
		isoverlap = false;

		if (OtherActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Sphere Trace Hit: %s"), *OtherActor->GetName());
		}
	}

}


void AconquerorCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	LastMoveInput = Value.Get<FVector2D>();

	if (!Controller) return;

	if (isClimbing)
	{
		ClimbInputY = LastMoveInput.Y;

		ClimbInputX = LastMoveInput.X;

		if (isledegehit)
		{
			// Use the actor's own rotation to determine climb directions
			FRotator ActorRotation = GetActorRotation();
			FVector UpVector = FRotationMatrix(ActorRotation).GetUnitAxis(EAxis::Z);    // Up
			FVector RightVector = FRotationMatrix(ActorRotation).GetUnitAxis(EAxis::Y); // Right

			AddMovementInput(UpVector, ClimbInputY);
			AddMovementInput(RightVector, ClimbInputX);
		}

		// ?? Else do nothing (no movement input applied while climbing but ledge not hit)
		return;
	}

	// Regular movement
	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);

	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // Forward
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);   // Right

	AddMovementInput(ForwardDirection, LastMoveInput.Y);
	AddMovementInput(RightDirection, LastMoveInput.X);
}



void AconquerorCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

FVector2D AconquerorCharacter::GetLastMoveInput() const
{
	return LastMoveInput;
}
//void AconquerorCharacter::Equip_Implementation(bool bEquip)
//{
	//UE_LOG(LogTemp, Warning, TEXT("DoSomething() called from Blueprint!"));
//}

void AconquerorCharacter::EquipAnim(float Rate)
{
	UAnimInstance* AttackAnimInstance = GetMesh()->GetAnimInstance();
	if (AttackAnimInstance && EquipMontage)
	{
		AttackAnimInstance->Montage_Play(EquipMontage);
		//AttackAnimInstance->Montage_JumpToSection(Section, UnEquipMontage);//Each section has its individual animation
		
	}
}
void AconquerorCharacter::UnEquipAnim(float Rate)
{
	UAnimInstance* AttackAnimInstance = GetMesh()->GetAnimInstance();
	if (AttackAnimInstance && UnEquipMontage)
	{
		AttackAnimInstance->Montage_Play(UnEquipMontage);
		//AttackAnimInstance->Montage_JumpToSection(Section, AttackMontage);//Each section has its individual animation

	}
}

void AconquerorCharacter::BoxingAnim(FName Section,float Rate)
{
	UAnimInstance* AttackAnimInstance = GetMesh()->GetAnimInstance();
	if (AttackAnimInstance && BoxingMontage)
	{
		AttackAnimInstance->Montage_Play(BoxingMontage);
		AttackAnimInstance->Montage_JumpToSection(Section, BoxingMontage);//Each section has its individual animation
	}
}

void AconquerorCharacter::MeleeAnim(FName Section,float Rate)
{
	UAnimInstance* AttackAnimInstance = GetMesh()->GetAnimInstance();
	if (AttackAnimInstance && MelleAttackMontage)
	{
		AttackAnimInstance->Montage_Play(MelleAttackMontage);
		AttackAnimInstance->Montage_JumpToSection(Section, MelleAttackMontage);//Each section has its individual animation
	}
}
void AconquerorCharacter::H_HitReactAnim(FName Section, float Rate)
{
	UAnimInstance* AttackAnimInstance = GetMesh()->GetAnimInstance();
	if (AttackAnimInstance && HitReactMontage)
	{
		AttackAnimInstance->Montage_Play(HitReactMontage);
		AttackAnimInstance->Montage_JumpToSection(Section, HitReactMontage);//Each section has its individual animation
	}
}

FName AconquerorCharacter::SectionName()
{
	int section { FMath::RandRange(1,5)};
	
	switch (section)
	{
	case 1: 
		SectionNameAnim = Anim1;
		break;

	case 2:
		SectionNameAnim = Anim2;
		break;

	case 3:
		SectionNameAnim = Anim3;
		break;

	case 4:
		SectionNameAnim = Anim4;
		break;

	case 5:
		SectionNameAnim = Anim5;
		break;

	}
	return SectionNameAnim;

}





FName AconquerorCharacter::MelleSectionName()
{
	int Mellesection{ FMath::RandRange(1,3) };

	switch (Mellesection)
	{
	case 1:
		MelleSectionNameAnim = MelleAnim1;
		break;

	case 2:
		MelleSectionNameAnim = MelleAnim2;
		break;

	case 3:
		MelleSectionNameAnim = MelleAnim3;
		break;


	}
	return MelleSectionNameAnim;

}


FName AconquerorCharacter::HitreactSectionName()
{
	int Hitreactsection{ FMath::RandRange(1,2) };

	switch (Hitreactsection)
	{
	case 1:
		HitReactSectionNameAnim = HitAnim1;
		break;

	case 2:
		HitReactSectionNameAnim = HitAnim2;
		break;


	}
	return HitReactSectionNameAnim;

}


void AconquerorCharacter::Attack_Mode()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	// Early out if no valid AnimInstance or a montage is already playing
	if (!AnimInstance || AnimInstance->IsAnyMontagePlaying())
	{
		return;
	}

	// Check if AnimInstance implements the interface
	if (AnimInstance->GetClass()->ImplementsInterface(UEQUIP_Interface::StaticClass()))
	{
		if (equipcpp)
		{
			// Call the Equip interface function with false (un-equip)
			IEQUIP_Interface::Execute_Equip(AnimInstance, false);
			UnEquipAnim(PlayRate);
			equipcpp = false;
		}
		else
		{
			// Call the Equip interface function with true (equip)
			IEQUIP_Interface::Execute_Equip(AnimInstance, true);
			EquipAnim(PlayRate);
			equipcpp = true;
		}
	}
}

void AconquerorCharacter::AttackCombat(FName BoxingSection, FName MeleeSection)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	// Early out if no valid AnimInstance or a montage is already playing
	if (!AnimInstance || AnimInstance->IsAnyMontagePlaying())
	{
		return;
	}
	else
	{
		if (equipcpp)
		{
			
			MeleeAnim(MeleeSection,PlayRate);
			
			
		}
		else
		{
			
			BoxingAnim(BoxingSection,PlayRate);
			
			
		}
	}

}

void AconquerorCharacter::AttachToPutin()
{
	if (Melee_H && GetMesh())
	{
		Melee_H->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
			FName("weapon_Socket")
		);
	}
}

void AconquerorCharacter::AttachToMelee()
{
	if (Melee_H && GetMesh())
	{
		Melee_H->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
			FName("RightHandSocket")
		);
	}
}

void AconquerorCharacter::AttackHittrance()  
{
	FVector Start = Melee_H->GetComponentLocation();
	FVector End = Start + Melee_H->GetForwardVector() * 90.0f; 
	float Radius = 50.0f;

	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Ignore self

	if (isAttacking)
	{
		bool bHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			Start,
			End,
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeSphere(Radius),
			QueryParams
		);

		// Debug Draw
		FColor TraceColor = bHit ? FColor::Red : FColor::Green;
		DrawDebugLine(GetWorld(), Start, End, TraceColor, false, 2.0f);
		//DrawDebugSphere(GetWorld(), End, Radius, 12, TraceColor, false, 2.0f);

		// Log result
		if (bHit && HitResult.GetActor())
		{
			EnemyVillain = Cast<Avillain_char>(HitResult.GetActor());

			if (EnemyVillain)
			{
				EnemyVillain->Hit_ReactAnim(EnemyVillain->HitReact_v_SectionName(), 1.0f);

				UGameplayStatics::ApplyDamage(EnemyVillain, E_MeleeDamage,GetController(), this, UDamageType::StaticClass());
				
			}
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Sphere Trace: No Hit"));
		}
	}

}

float AconquerorCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Clamp damage to valid range
	float ActualDamage = FMath::Clamp(DamageAmount, 0.0f, H_Health);

	H_Health -= ActualDamage;

	UE_LOG(LogTemp, Warning, TEXT("%s took damage: %f | Remaining Health: %f"), *GetName(), ActualDamage, H_Health);

	if (H_Health <= 0.0f)
	{
		H_Health = 0.0f;
		H_Die();

		// Play death animation if valid
		if (H_Death && GetMesh())
		{
			GetMesh()->PlayAnimation(H_Death, false);
		}
	}

	return ActualDamage;
}

void AconquerorCharacter::H_Die()
{
	H_HideHealthBar(); 

	UE_LOG(LogTemp, Warning, TEXT("%s has died."), *GetName());

	// Optional: disable collision and movement
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
	// Optional: auto-destroy after delay
	SetLifeSpan(5.0f);

}


