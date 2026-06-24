// Fill out your copyright notice in the Description page of Project Settings.


#include "UE_LP_Character.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UnrealType.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "BaseWeapon.h"
#include "SummonCharacterBase.h"
#include "Blueprint/UserWidget.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AUE_LP_Character::AUE_LP_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);

	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	CurrentSprintTime = MaxSprintTime;
	bIsSprinting = false;

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	TargetCameraDistance = CameraBoom->TargetArmLength;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 60.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(GetRootComponent());
	ProjectileSpawnPoint->SetRelativeLocation(FVector(120.f, 0.f, 50.f));
}

// Called when the game starts or when spawned
void AUE_LP_Character::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(GetController());

	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("No PlayerController"));
		return;
	}

	if (!DefaultMappingContext)
	{
		UE_LOG(LogTemp, Error, TEXT("DefaultMappingContext is NOT assigned"));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

	if (!Subsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("EnhancedInput Subsystem not found"));
		return;
	}

	Subsystem->AddMappingContext(DefaultMappingContext, 0);

	UE_LOG(LogTemp, Warning, TEXT("Mapping Context added"));

	Health = MaxHealth;

	if (HealthWidgetClass)
	{
		HealthWidget = CreateWidget<UUserWidget>(GetWorld(), HealthWidgetClass);

		if (HealthWidget)
		{
			HealthWidget->AddToViewport();
		}
	}

	if (SummonTeamWidgetClass)
	{
		SummonTeamWidget = CreateWidget<UUserWidget>(GetWorld(), SummonTeamWidgetClass);

		if (SummonTeamWidget)
		{
			SummonTeamWidget->AddToViewport();
		}
	}
}


// Called every frame
void AUE_LP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSprinting)
	{
		CurrentSprintTime -= DeltaTime;

		if (CurrentSprintTime <= 0.f)
		{
			CurrentSprintTime = 0.f;

			bIsSprinting = false;

			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
	}
	else
	{
		CurrentSprintTime += DeltaTime * 0.5f;

		CurrentSprintTime = FMath::Clamp(
			CurrentSprintTime,
			0.f,
			MaxSprintTime
		);
	}

	CameraBoom->TargetArmLength = FMath::FInterpTo(
		CameraBoom->TargetArmLength,
		TargetCameraDistance,
		DeltaTime,
		ZoomInterpSpeed
	);
}

// Called to bind functionality to input
void AUE_LP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUE_LP_Character::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUE_LP_Character::Look);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AUE_LP_Character::StartJump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &AUE_LP_Character::StopJump);

		EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &AUE_LP_Character::StartCrouch);
		EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AUE_LP_Character::StopCrouch);
		EnhancedInput->BindAction(SprintAction,	ETriggerEvent::Started,	this, &AUE_LP_Character::StartSprint);
		EnhancedInput->BindAction(SprintAction,	ETriggerEvent::Completed, this,	&AUE_LP_Character::StopSprint);

		EnhancedInput->BindAction(ChangeWeaponAction, ETriggerEvent::Started, this, &AUE_LP_Character::ChangeWeapon);
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &AUE_LP_Character::Interact);
		EnhancedInput->BindAction(ShootAction, ETriggerEvent::Started, this, &AUE_LP_Character::Shoot);
		EnhancedInput->BindAction(ReloadAction,ETriggerEvent::Started, this, &AUE_LP_Character::Reload);

		EnhancedInput->BindAction(ZoomCameraAction, ETriggerEvent::Triggered, this,&AUE_LP_Character::ZoomCamera);
		EnhancedInput->BindAction(CreatureCommandAction, ETriggerEvent::Started, this, &AUE_LP_Character::CommandSummon);

		EnhancedInput->BindAction(PauseAction, ETriggerEvent::Started, this, &AUE_LP_Character::TogglePause);
	}
}

void AUE_LP_Character::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (!Controller) return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AUE_LP_Character::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
	}
}

void AUE_LP_Character::ChangeWeapon(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Change Weapon"));
}

void AUE_LP_Character::Interact(const FInputActionValue& Value)
{
	BP_TryInteract();
}

void AUE_LP_Character::StartJump(const FInputActionValue & Value)
{
		Jump();
}

void AUE_LP_Character::StopJump(const FInputActionValue & Value)
{
		StopJumping();
}

void AUE_LP_Character::StartCrouch(const FInputActionValue& Value)
{
	Crouch();
	GetCharacterMovement()->MaxWalkSpeed = 250.f;
}

void AUE_LP_Character::StopCrouch(const FInputActionValue& Value)
{
	UnCrouch();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AUE_LP_Character::StartSprint(const FInputActionValue& Value)
{
	if (CurrentSprintTime > 0.f)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AUE_LP_Character::StopSprint(const FInputActionValue& Value)
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

float AUE_LP_Character::GetSprintPercent() const
{
	if (MaxSprintTime <= 0.f)
	{
		return 0.f;
	}

	return CurrentSprintTime / MaxSprintTime;
}

void AUE_LP_Character::Shoot(const FInputActionValue& Value)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}

void AUE_LP_Character::Reload(const FInputActionValue& Value)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();
	}
}

void AUE_LP_Character::EquipWeapon(ABaseWeapon* NewWeapon)
{
	if (!IsValid(NewWeapon))
	{
		return;
	}

	// If another weapon is already equipped, drop it first.
	if (IsValid(CurrentWeapon) && CurrentWeapon != NewWeapon)
	{
		BP_DropCurrentWeapon();

		// Safety fallback: if Blueprint did not destroy the old weapon,
		// destroy it here so weapons do not overlap in the hand.
		if (IsValid(CurrentWeapon) && CurrentWeapon != NewWeapon)
		{
			CurrentWeapon->Destroy();
			CurrentWeapon = nullptr;
		}
	}

	CurrentWeapon = NewWeapon;
	CurrentWeapon->SetOwner(this);
	CurrentWeapon->SetInstigator(this);

	CurrentWeapon->AttachToComponent(
		GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("WeaponSocket")
	);
}

ABaseWeapon* AUE_LP_Character::GetCurrentWeapon() const
{
	return CurrentWeapon;
}

FTransform AUE_LP_Character::GetProjectileSpawnTransform() const
{
	if (!ProjectileSpawnPoint)
	{
		return GetActorTransform();
	}

	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();

	FRotator SpawnRotation;

	if (FollowCamera)
	{
		SpawnRotation = FollowCamera->GetComponentRotation();
	}
	else
	{
		SpawnRotation = GetActorRotation();
	}

	return FTransform(SpawnRotation, SpawnLocation, FVector(1.f, 1.f, 1.f));
}

void AUE_LP_Character::ChangeHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.f, MaxHealth);

	if (Health <= 0.f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());

		if (!PC)
		{
			return;
		}

		if (UFunction* DefeatFunc = PC->FindFunction(TEXT("OpenDefeatMenu")))
		{
			PC->ProcessEvent(DefeatFunc, nullptr);
		}

		DisableInput(PC);
	}
}

float AUE_LP_Character::GetHealthPercent() const
{
	if (MaxHealth <= 0.f)
	{
		return 0.f;
	}

	return Health / MaxHealth;
}

float AUE_LP_Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageApplied = Super::TakeDamage(
		DamageAmount,
		DamageEvent,
		EventInstigator,
		DamageCauser
	);

	ChangeHealth(-DamageAmount);

	UE_LOG(LogTemp, Warning, TEXT("Player took damage: %f"), DamageAmount);

	return DamageApplied;
}

void AUE_LP_Character::ZoomCamera(const FInputActionValue& Value)
{
	const float ZoomValue = Value.Get<float>();

	TargetCameraDistance = FMath::Clamp(
		TargetCameraDistance - ZoomValue * ZoomStep,
		MinCameraDistance,
		MaxCameraDistance
	);
}

void AUE_LP_Character::CommandSummon(const FInputActionValue& Value)
{
	CleanupSummons();

	if (ActiveSummons.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No summons to command"));
		return;
	}

	// If at least one summon is in Follow mode, switch the whole team to Guard.
	// If all summons are already in Guard mode, switch the whole team back to Follow.
	bool bHasFollowSummon = false;

	for (ASummonCharacterBase* Summon : ActiveSummons)
	{
		if (IsValid(Summon) && Summon->Mode == ESummonMode::Follow)
		{
			bHasFollowSummon = true;
			break;
		}
	}

	const ESummonMode NewMode =
		bHasFollowSummon ? ESummonMode::Guard : ESummonMode::Follow;

	GroupSummonMode = NewMode;

	for (int32 i = 0; i < ActiveSummons.Num(); ++i)
	{
		ASummonCharacterBase* Summon = ActiveSummons[i];

		if (!IsValid(Summon))
		{
			continue;
		}

		if (NewMode == ESummonMode::Guard)
		{
			const float Spacing = 150.f;

			const FVector RightOffset =
				GetActorRightVector() * ((i % 2 == 0) ? Spacing : -Spacing);

			const FVector ForwardOffset =
				GetActorForwardVector() * (200.f + (i / 2) * Spacing);

			const FVector NewGuardLocation =
				GetActorLocation() + ForwardOffset + RightOffset;

			Summon->SetSummonMode(ESummonMode::Guard, NewGuardLocation);
		}
		else
		{
			Summon->SetSummonMode(
				ESummonMode::Follow,
				Summon->GetActorLocation()
			);
		}
	}

	BP_OnSummonTeamChanged();

	if (NewMode == ESummonMode::Guard)
	{
		UE_LOG(LogTemp, Warning, TEXT("All summons mode: Guard"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("All summons mode: Follow"));
	}
}

int32 AUE_LP_Character::GetActiveSummonCount() const
{
	return ActiveSummons.Num();
}

TArray<ASummonCharacterBase*> AUE_LP_Character::GetActiveSummons() const
{
	return ActiveSummons;
}

ESummonMode AUE_LP_Character::GetGroupSummonMode() const
{
	return GroupSummonMode;
}

void AUE_LP_Character::TogglePause(const FInputActionValue& Value)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return;
	}

	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		if (UFunction* ResumeFunc = PC->FindFunction(TEXT("ResumeGame")))
		{
			PC->ProcessEvent(ResumeFunc, nullptr);
		}
	}
	else
	{
		if (UFunction* OpenPauseFunc = PC->FindFunction(TEXT("OpenPauseMenu")))
		{
			PC->ProcessEvent(OpenPauseFunc, nullptr);
		}
	}
}

void AUE_LP_Character::AddSummonToTeam(TSubclassOf<ASummonCharacterBase> NewSummonClass)
{
	if (!NewSummonClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddSummonToTeam: NewSummonClass is null"));
		return;
	}

	// Save this summon class as collected/unlocked.
	OwnedSummonClasses.Add(NewSummonClass);

	const int32 NewSummonIndex = OwnedSummonClasses.Num() - 1;

	ASummonCharacterBase* NewSummon =
		SpawnSummonFromClass(NewSummonClass, NewSummonIndex);

	if (NewSummon)
	{
		ActiveSummons.Add(NewSummon);

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Summon added to team. Total active summons: %d"),
			ActiveSummons.Num()
		);

		BP_OnSummonTeamChanged();
	}
}

ASummonCharacterBase* AUE_LP_Character::SpawnSummonFromClass(
	TSubclassOf<ASummonCharacterBase> ClassToSpawn,
	int32 SummonIndex
)
{
	if (!ClassToSpawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnSummonFromClass: ClassToSpawn is null"));
		return nullptr;
	}

	UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnSummonFromClass: World is null"));
		return nullptr;
	}

	// Spread summons behind the player.
	// This prevents multiple summons from spawning at the same point.
	const float Spacing = 150.f;

	const FVector RightOffset =
		GetActorRightVector() * ((SummonIndex % 2 == 0) ? Spacing : -Spacing);

	const FVector BackOffset =
		-GetActorForwardVector() * (200.f + (SummonIndex / 2) * Spacing);

	const FVector SpawnLocation =
		GetActorLocation() + RightOffset + BackOffset;

	const FRotator SpawnRotation = GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	ASummonCharacterBase* SpawnedSummon =
		World->SpawnActor<ASummonCharacterBase>(
			ClassToSpawn,
			SpawnLocation,
			SpawnRotation,
			SpawnParams
		);

	if (!SpawnedSummon)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnSummonFromClass: Failed to spawn summon"));
		return nullptr;
	}

	// Initialize summon owner/player reference.
	SpawnedSummon->InitSummon(this, SummonIndex);

	UE_LOG(LogTemp, Warning, TEXT("Summon spawned from pickup"));

	return SpawnedSummon;
}

void AUE_LP_Character::CleanupSummons()
{
	for (int32 i = ActiveSummons.Num() - 1; i >= 0; --i)
	{
		if (!IsValid(ActiveSummons[i]))
		{
			ActiveSummons.RemoveAt(i);
		}
	}
}

void AUE_LP_Character::SetSummonModeByIndex(int32 SummonIndex, ESummonMode NewMode)
{
	CleanupSummons();

	if (!ActiveSummons.IsValidIndex(SummonIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetSummonModeByIndex: invalid index %d"), SummonIndex);
		return;
	}

	ASummonCharacterBase* Summon = ActiveSummons[SummonIndex];

	if (!IsValid(Summon))
	{
		return;
	}

	if (NewMode == ESummonMode::Guard)
	{
		const float Spacing = 150.f;

		const FVector RightOffset =
			GetActorRightVector() * ((SummonIndex % 2 == 0) ? Spacing : -Spacing);

		const FVector ForwardOffset =
			GetActorForwardVector() * (200.f + (SummonIndex / 2) * Spacing);

		const FVector NewGuardLocation =
			GetActorLocation() + ForwardOffset + RightOffset;

		Summon->SetSummonMode(ESummonMode::Guard, NewGuardLocation);
	}
	else
	{
		Summon->SetSummonMode(ESummonMode::Follow, Summon->GetActorLocation());
	}

	BP_OnSummonTeamChanged();
}

void AUE_LP_Character::ToggleSummonModeByIndex(int32 SummonIndex)
{
	CleanupSummons();

	if (!ActiveSummons.IsValidIndex(SummonIndex))
	{
		return;
	}

	ASummonCharacterBase* Summon = ActiveSummons[SummonIndex];

	if (!IsValid(Summon))
	{
		return;
	}

	if (Summon->Mode == ESummonMode::Follow)
	{
		SetSummonModeByIndex(SummonIndex, ESummonMode::Guard);
	}
	else
	{
		SetSummonModeByIndex(SummonIndex, ESummonMode::Follow);
	}
}