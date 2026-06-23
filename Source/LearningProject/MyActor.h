// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class LEARNINGPROJECT_API AMyActor : public AActor
{
    GENERATED_BODY()

public:
    AMyActor();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats|Vitals", meta = (ClampMin = "0", ClampMax = "100"))
    float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Vitals", 
        meta = (ClampMin = "0", ClampMax = "100", UIMin = "0.0", UIMax = "100.0"))
    float Mana;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Attributes")
    int32 Intellect;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Stats|Attributes")
    int32 Stamina;

    UPROPERTY(VisibleAnywhere, Category = "Stats|Combat")
    float CriticalStrikeChance;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Stats|Combat")
    float Mastery;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats|Combat")
    bool Weapon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Movement")
    float MoveSpeed;


protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};
