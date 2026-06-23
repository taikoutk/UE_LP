// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BonfireActor.generated.h"

class UNiagaraComponent;

UCLASS()
class LEARNINGPROJECT_API ABonfireActor : public AActor
{
	GENERATED_BODY()
	
public:	

	ABonfireActor();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Niagara")
	UNiagaraComponent* NiagaraComp;

public:	

	// 1) BlueprintImplementableEvent
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Niagara")
	void NiagaraComponentName(UNiagaraComponent* Niagara);


	// 2) BlueprintNativeEvent
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Niagara")
	void SetNiagaraComponentScale(UNiagaraComponent* Niagara, float NewScale, float& AppliedScale);
	

	// 3) BlueprintCallable

	UFUNCTION(BlueprintCallable, Category = "Niagara")
	void GetNiagaraComponent(UNiagaraComponent*& Niagara);
};
