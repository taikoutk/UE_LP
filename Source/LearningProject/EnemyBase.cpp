// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

float AEnemyBase::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	const float DamageApplied =
		Super::TakeDamage(
			DamageAmount,
			DamageEvent,
			EventInstigator,
			DamageCauser
		);

	CurrentHealth -= DamageAmount;

	UE_LOG(LogTemp, Warning, TEXT("Enemy HP: %f"), CurrentHealth);

	if (CurrentHealth <= 0.f)
	{
		Die();
	}

	return DamageApplied;
}

void AEnemyBase::Die()
{
	Destroy();
}