// Fill out your copyright notice in the Description page of Project Settings.

#include "SummonCharacterBase.h"
#include "UE_LP_Character.h"

// Sets default values
ASummonCharacterBase::ASummonCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASummonCharacterBase::InitSummon(AUE_LP_Character* NewOwner, int32 NewTeamIndex)
{
	OwnerCharacter = NewOwner;
	TeamIndex = NewTeamIndex;
	Mode = ESummonMode::Follow;
	GuardLocation = GetActorLocation();
}

void ASummonCharacterBase::SetSummonMode(ESummonMode NewMode, FVector NewGuardLocation)
{
	Mode = NewMode;
	GuardLocation = NewGuardLocation;
}

void ASummonCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASummonCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASummonCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FText ASummonCharacterBase::GetSummonModeText() const
{
	switch (Mode)
	{
	case ESummonMode::Follow:
		return FText::FromString(TEXT("Follow"));

	case ESummonMode::Guard:
		return FText::FromString(TEXT("Guard"));

	default:
		return FText::FromString(TEXT("Unknown"));
	}
}

ESummonMode ASummonCharacterBase::GetSummonMode() const
{
	return Mode;
}