// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class LEARNINGPROJECT_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon")
	void Reload();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetCurrentAmmo() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetMaxAmmo() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//magazine
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 CurrentAmmo = 30;

	//clip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MaxAmmo = 30;

	//reserve
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 Ammo = 200;

};
