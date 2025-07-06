// Fill out your copyright notice in the Description page of Project Settings.


#include "villain_char.h"
#include "AIEnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/StaticMeshComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "conquerorCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
Avillain_char::Avillain_char():
	Health(100.f),
	MaxHealth(100.f),
	BaseDamage(20.f),
	AttackAnim1("Attack1"),
	AttackAnim2("Attack2"),
	AttackAnim3("Attack3"),
	HitReactVAnim1("HitHandR"),
	HitReactVAnim2("HitLegL"),
	HitReactVAnim3("HitLegR")
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Melee_V = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Melee_V"));
	Melee_V->SetupAttachment(GetMesh());

	AttackE = CreateDefaultSubobject<USphereComponent>(TEXT("AttackS"));
	AttackE->SetupAttachment(GetMesh());
	
}



// Called when the game starts or when spawned
void Avillain_char::BeginPlay()
{
	Super::BeginPlay();

	EnemyController = Cast<AAIEnemyController>(GetController());

	EnemyController->RunBehaviorTree(BehaviorTree);


	AttackE->OnComponentBeginOverlap.AddDynamic(this, &Avillain_char::AttackShereE);

	AttackE->OnComponentEndOverlap.AddDynamic(this, &Avillain_char::AttackShereEEnd);
}


void Avillain_char::ShowHealthBar_Implementation()
{
	GetWorldTimerManager().ClearTimer(HealthBarTime);
	GetWorldTimerManager().SetTimer(HealthBarTime, this, &Avillain_char::HideHealthBar, HealthBarTimeDisplay); //display to hide widget
}																					//call back fumction





// Called every frame
void Avillain_char::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	player = Cast<AconquerorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (player)
	{

		if (EnemyController->LineOfSightTo(player))
		{

			if (EnemyController)
			{
				EnemyController->MoveToActor(player, 100);//move to player

				EnemyController->SetFocus(player);
			}
			

		}
		else
		{
			EnemyController->ClearFocus(EAIFocusPriority::Gameplay);

		}

	}



	/*if (player && player->isoverlap == true)
	{
		if (EnemyController)
		{
			EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttacking"), true);
		}
	}
	else if (player && player->isoverlap == false)
	{
		if (EnemyController)
		{
			EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttacking"), false);

		}
	}*/


}



// Called to bind functionality to input
void Avillain_char::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void Avillain_char::Die()
{
	HideHealthBar();//Hide Widget 

	UE_LOG(LogTemp, Warning, TEXT("%s has died."), *GetName());

	// Optional: disable collision and movement
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
	EnemyController->ClearFocus(EAIFocusPriority::Gameplay);
	// Optional: auto-destroy after delay
	SetLifeSpan(2.0f);
}


float Avillain_char::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	// Clamp damage to valid range
	float ActualDamage = FMath::Clamp(DamageAmount, 0.0f, Health);

	Health -= ActualDamage;

	UE_LOG(LogTemp, Warning, TEXT("%s took damage: %f | Remaining Health: %f"), *GetName(), ActualDamage, Health);

	if (Health <= 0.0f)
	{
		Health = 0.0f;
		Die();

		// Play death animation if valid
		if (Death && GetMesh())
		{
			GetMesh()->PlayAnimation(Death, false);
		}
	}

	return ActualDamage;
}


void Avillain_char::MeleeAttackHittrance()
{
	FVector Start = Melee_V->GetSocketLocation(FName("hurt_v"));

	FVector End = Start + Melee_V->GetComponentLocation() * 40.0f;
	float Radius = 50.0f;


	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Ignore self

	if (isV_Attacking)
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
		DrawDebugSphere(GetWorld(), End, Radius, 12, TraceColor, false, 2.0f);

		// Log result
		if (bHit && HitResult.GetActor())
		{
			class AconquerorCharacter* player_H = Cast<AconquerorCharacter>(HitResult.GetActor());

			if (player_H)
			{
				player_H->H_HitReactAnim(player_H->HitreactSectionName(), 1.0f);

				UGameplayStatics::ApplyDamage(player_H, H_Damage, GetController(), this, UDamageType::StaticClass());

				UE_LOG(LogTemp, Warning, TEXT("Sphere Trace Hit: %s"), *HitResult.GetActor()->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Sphere Trace: No Hit"));
		}
	}

}
void Avillain_char::AttackMontage(FName Section, float Rate)
{

	UAnimInstance* AttackAnimInstance = GetMesh()->GetAnimInstance();
	if (AttackAnimInstance && Attack_v_Montage)
	{
		AttackAnimInstance->Montage_Play(Attack_v_Montage);
		AttackAnimInstance->Montage_JumpToSection(Section, Attack_v_Montage);//Each section has its individual animation
	}


}




FName Avillain_char::villainAttackSectionName()
{
	int Attacksection{ FMath::RandRange(1,3) };

	switch (Attacksection)
	{
	case 1:
		AttackSectionNameAnim = AttackAnim1;
		break;

	case 2:
		AttackSectionNameAnim = AttackAnim2;
		break;

	case 3:
		AttackSectionNameAnim = AttackAnim3;
		break;


	}
	return AttackSectionNameAnim;

}


void Avillain_char::Hit_ReactAnim(FName Section, float Rate)
{
	UAnimInstance* AttackAnimInstance = GetMesh()->GetAnimInstance();
	if (AttackAnimInstance && HitMontage)
	{
		AttackAnimInstance->Montage_Play(HitMontage);
		AttackAnimInstance->Montage_JumpToSection(Section, HitMontage);//Each section has its individual animation

		//float MontageLength = PlayAnimMontage(HitMontage);
		//UE_LOG(LogTemp, Error, TEXT("Montage Play Result: %f"), MontageLength);
	}
}

FName Avillain_char::HitReact_v_SectionName()
{
	int HitReactsection{ FMath::RandRange(1,3) };

	switch (HitReactsection)
	{
	case 1:
		Hit_v_SectionNameAnim = HitReactVAnim1;
		break;

	case 2:
		Hit_v_SectionNameAnim = HitReactVAnim2;
		break;

	case 3:
		Hit_v_SectionNameAnim = HitReactVAnim3;
		break;


	}
	return Hit_v_SectionNameAnim;

}

void Avillain_char::AttackShereE(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	
	if (OtherActor && OtherActor != this)
	{
		AconquerorCharacter* Player_H = Cast<AconquerorCharacter>(OtherActor);
			if (Player_H && EnemyController)
			{
					EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttacking"), true);
					//AttackE->SetGenerateOverlapEvents(false); // Prevent repeated triggers
					UE_LOG(LogTemp, Warning, TEXT("Sphere Trace Hit: %s"), *OtherActor->GetName());
			}
	}
	


}

void Avillain_char::AttackShereEEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		AconquerorCharacter* Player_H = Cast<AconquerorCharacter>(OtherActor);
		if (Player_H && EnemyController)
		{
			EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttacking"), false);
			//AttackE->SetGenerateOverlapEvents(true); // Prevent repeated triggers
			UE_LOG(LogTemp, Warning, TEXT("Sphere Trace Hit: %s"), *OtherActor->GetName());
		}
	}

}
