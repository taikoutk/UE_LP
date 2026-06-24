// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SummonCharacterBase.generated.h"

class AUE_LP_Character;
class UTexture2D;

UENUM(BlueprintType)
enum class ESummonMode : uint8
{
	Follow UMETA(DisplayName = "Follow"),
	Guard UMETA(DisplayName = "Guard")
};

UCLASS()
class LEARNINGPROJECT_API ASummonCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASummonCharacterBase();

	UPROPERTY(BlueprintReadWrite, Category = "Summon")
	AUE_LP_Character* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Summon")
	ESummonMode Mode = ESummonMode::Follow;

	UPROPERTY(BlueprintReadOnly, Category = "Summon")
	int32 TeamIndex = -1;

	UPROPERTY(BlueprintReadWrite, Category = "Summon")
	FVector GuardLocation;

	// Name shown in UI.
	// Example: Bird, Wolf, etc.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Summon|UI")
	FText SummonDisplayName;

	// Optional icon for the summon UI.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Summon|UI")
	UTexture2D* SummonIcon;

	UFUNCTION(BlueprintCallable, Category = "Summon")
	void InitSummon(AUE_LP_Character* NewOwner, int32 NewTeamIndex);

	UFUNCTION(BlueprintCallable, Category = "Summon")
	void SetSummonMode(ESummonMode NewMode, FVector NewGuardLocation);

	UFUNCTION(BlueprintPure, Category = "Summon")
	ESummonMode GetSummonMode() const;

	UFUNCTION(BlueprintPure, Category = "Summon|UI")
	FText GetSummonModeText() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};