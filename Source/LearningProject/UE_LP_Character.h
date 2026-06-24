// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SummonCharacterBase.h"
#include "UE_LP_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USceneComponent;
class UInputMappingContext;
class UInputAction;
class ABaseWeapon;
class UUserWidget;
class ACharacter;

UCLASS()
class LEARNINGPROJECT_API AUE_LP_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUE_LP_Character();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_TryInteract();

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(ABaseWeapon* NewWeapon);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon")
	void BP_DropCurrentWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100.f;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float Health = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthWidgetClass;

	UPROPERTY()
	UUserWidget* HealthWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> DefeatWidgetClass;

	UUserWidget* DefeatWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SummonTeamWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* SummonTeamWidget;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void ChangeHealth(float Amount);

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure, Category = "Movement")
	float GetSprintPercent() const;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	ABaseWeapon* GetCurrentWeapon() const;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	FTransform GetProjectileSpawnTransform() const;

	UFUNCTION(BlueprintPure, Category = "Summon")
	int32 GetActiveSummonCount() const;

	UFUNCTION(BlueprintPure, Category = "Summon")
	TArray<ASummonCharacterBase*> GetActiveSummons() const;

	// Adds a new summon type
	UFUNCTION(BlueprintCallable, Category = "Summon")
	void AddSummonToTeam(TSubclassOf<ASummonCharacterBase> NewSummonClass);

	UFUNCTION(BlueprintImplementableEvent, Category = "Summon")
	void BP_OnSummonTeamChanged();

	UFUNCTION(BlueprintCallable, Category = "Summon")
	void SetSummonModeByIndex(int32 SummonIndex, ESummonMode NewMode);

	UFUNCTION(BlueprintCallable, Category = "Summon")
	void ToggleSummonModeByIndex(int32 SummonIndex);

	UFUNCTION(BlueprintPure, Category = "Summon")
	ESummonMode GetGroupSummonMode() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ChangeWeaponAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ShootAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	ABaseWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ReloadAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* CrouchAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* PauseAction;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY()
	UUserWidget* PauseWidget;

	bool bIsPaused = false;

	void TogglePause(const FInputActionValue& Value);


	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);

	void ChangeWeapon(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void Reload(const FInputActionValue& Value);

	void StartCrouch(const FInputActionValue& Value);
	void StopCrouch(const FInputActionValue& Value);

	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ZoomCameraAction;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MinCameraDistance = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MaxCameraDistance = 650.f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomStep = 50.f;

	void ZoomCamera(const FInputActionValue& Value);
	float TargetCameraDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomInterpSpeed = 15.f;

	// Summon classes
	UPROPERTY(BlueprintReadOnly, Category = "Summon")
	TArray<TSubclassOf<ASummonCharacterBase>> OwnedSummonClasses;

	// Summons that are currently spawned and active in the world
	UPROPERTY(BlueprintReadOnly, Category = "Summon")
	TArray<ASummonCharacterBase*> ActiveSummons;

	UPROPERTY(BlueprintReadOnly, Category = "Summon")
	ESummonMode GroupSummonMode = ESummonMode::Follow;

	// Spawns one summon actor from the given class
	UFUNCTION(BlueprintCallable, Category = "Summon")
	ASummonCharacterBase* SpawnSummonFromClass(
		TSubclassOf<ASummonCharacterBase> ClassToSpawn,int32 SummonIndex);

	// Removes invalid or destroyed summon references from ActiveSummons
	UFUNCTION(BlueprintCallable, Category = "Summon")
	void CleanupSummons();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* CreatureCommandAction;

	void CommandSummon(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage( float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,AActor* DamageCauser) override;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WalkSpeed = 450.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintSpeed = 900.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MaxSprintTime = 5.f;

	float CurrentSprintTime;

	bool bIsSprinting;
};
