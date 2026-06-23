// Fill out your copyright notice in the Description page of Project Settings.


#include "BonfireActor.h"
#include "Components/SceneComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Logging/LogMacros.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"


ABonfireActor::ABonfireActor()
{
    // Niagara Component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
    NiagaraComp->SetupAttachment(RootComponent);
    NiagaraComp->bAutoActivate = true;

    // Assign Niagara System in C++
    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystem(
        TEXT("/Game/VFX/Niagara/NS_Explotion")
    );

    if (NiagaraSystem.Succeeded())
    {
        NiagaraComp->SetAsset(NiagaraSystem.Object);
    }

}

void ABonfireActor::BeginPlay()
{
	Super::BeginPlay();

}

void ABonfireActor::SetNiagaraComponentScale_Implementation(UNiagaraComponent* Niagara, float NewScale, float& AppliedScale)
{
    
    if (!Niagara)
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] FAILED: NiagaraComponent is NULL!"), *GetName());

        if (GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,TEXT("Warning: Niagara Component is NULL!"));}

        AppliedScale = 0.0f;
        return;
    }
    if (Niagara->GetAsset() == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] FAILED: Niagara Asset is EMPTY!"), *GetName());

        if (GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,TEXT("Warning: Niagara Asset is NULL!"));}

        AppliedScale = 0.0f;
        return;
    }

        NiagaraComp->SetRelativeScale3D(FVector(NewScale));
        AppliedScale = NewScale;
        UE_LOG(LogTemp, Log, TEXT("[%s] Scale successfully changed to %f"), *GetName(), NewScale);

}

void ABonfireActor::GetNiagaraComponent(UNiagaraComponent*& Niagara)
{
    Niagara = NiagaraComp;
}