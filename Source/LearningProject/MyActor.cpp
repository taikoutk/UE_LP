// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


    Health = 100.0f;
    Mana = 50.0f;

    Intellect = 10;
    Stamina = 12;

    MoveSpeed = 600.0f;

    CriticalStrikeChance = 0.05f; // 5% base
    Mastery = 0.0f;

    Weapon = false;

}


// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

