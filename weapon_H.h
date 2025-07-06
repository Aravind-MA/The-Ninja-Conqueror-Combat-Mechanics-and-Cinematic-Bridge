// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "weapon_H.generated.h"

UCLASS()
class CONQUEROR_API Aweapon_H : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aweapon_H();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
