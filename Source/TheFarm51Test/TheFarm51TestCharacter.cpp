// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TheFarm51TestCharacter.h"
#include "TheFarm51TestProjectile.h"
#include "TheFarm51TestHUD.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UnrealNetwork.h"
#include "Engine/Blueprint.h"
#include "TextBlock.h"

ATheFarm51TestCharacter::ATheFarm51TestCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	bReplicates = true;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->SetIsReplicated(true);

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
	Mesh1P->SetHiddenInGame(false, true);
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetIsReplicated(true);

	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(RootComponent);
	FP_Gun->SetIsReplicated(true);

	TP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TP_Gun"));
	TP_Gun->SetOnlyOwnerSee(false);
	TP_Gun->SetOwnerNoSee(true);
	TP_Gun->bCastDynamicShadow = false;
	TP_Gun->CastShadow = false;
	TP_Gun->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	TP_Gun->SetIsReplicated(true);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));
	FP_MuzzleLocation->SetIsReplicated(true);

	TP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("TP_MuzzleLocation"));
	TP_MuzzleLocation->SetupAttachment(TP_Gun);
	TP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));
	TP_MuzzleLocation->SetIsReplicated(true);

	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	GetMesh()->SetOnlyOwnerSee(false);
	GetMesh()->SetOwnerNoSee(true);
}

void ATheFarm51TestCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

		if (GetController()->CastToPlayerController())
		{
			Cast<ATheFarm51TestHUD>(GetController()->CastToPlayerController()->GetHUD())->UpdateImageVisibility(false);
		}
	}
}

void ATheFarm51TestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority())
		ClientInteractCheck();
}

void ATheFarm51TestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATheFarm51TestCharacter::ClientFire);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ATheFarm51TestCharacter::ClientInteract);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATheFarm51TestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATheFarm51TestCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("TurnRate", this, &ATheFarm51TestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATheFarm51TestCharacter::LookUpAtRate);
}

void ATheFarm51TestCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ATheFarm51TestCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ATheFarm51TestCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATheFarm51TestCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATheFarm51TestCharacter::ClientFire_Implementation()
{
	if (!HasAuthority())
	{
		if (IsLocallyControlled())
		{
			ServerOnFire(this);
		}

		if (FireAnimation != NULL)
		{
			UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
}

void ATheFarm51TestCharacter::ServerOnFire_Implementation(AActor* OtherActor)
{
	if (HasAuthority())
	{
		if (OtherActor)
		{
			auto Character = Cast<ATheFarm51TestCharacter>(OtherActor);

			if (Character)
			{
				if (Character->ProjectileClass != NULL)
				{
					UWorld* const World = GetWorld();

					if (World != NULL)
					{
						const FRotator SpawnRotation = Character->GetControlRotation();
						const FVector SpawnLocation = ((Character->FP_MuzzleLocation != nullptr) ? Character->FP_MuzzleLocation->GetComponentLocation() : Character->GetActorLocation()) + SpawnRotation.RotateVector(Character->GunOffset);

						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

						World->SpawnActor<ATheFarm51TestProjectile>(Character->ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
					}
				}

				if (Character->FireSound != NULL)
				{
					UGameplayStatics::PlaySoundAtLocation(this, Character->FireSound, Character->GetActorLocation());
					NetMultiPlaySnd(Character->FireSound, Character->GetActorLocation());
				}

				if (Character->FireAnimation != NULL)
				{
					UAnimInstance* AnimInstance = Character->Mesh1P->GetAnimInstance();
					if (AnimInstance != NULL)
					{
						AnimInstance->Montage_Play(FireAnimation, 1.f);
					}
				}
			}
		}
	}
}

bool ATheFarm51TestCharacter::ServerOnFire_Validate(AActor* OtherActor)
{
	if (OtherActor)
		return true;
	return false;
}

void ATheFarm51TestCharacter::NetMultiPlaySnd_Implementation(USoundBase* Sound, FVector Loc)
{
	if (Sound->GetSoundClass())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Loc);
	}
}

bool ATheFarm51TestCharacter::NetMultiPlaySnd_Validate(USoundBase* Sound, FVector Loc)
{
	return true;
}

void ATheFarm51TestCharacter::ClientInteractCheck_Implementation()
{
	if (IsLocallyControlled())
	{
		FHitResult Hit;
		FVector Start = FirstPersonCameraComponent->GetComponentLocation();

		FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
		FVector End = (ForwardVector * 200.f) + Start;
		FCollisionQueryParams CollisionParams;

		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams))
		{
			if (Hit.bBlockingHit)
			{
				if (Hit.GetActor()->GetClass()->IsChildOf(AInteractable::StaticClass()))
				{
					auto Other = Cast<AInteractable>(Hit.GetActor());
					if (Other)
					{
						Cast<ATheFarm51TestHUD>(GetController()->CastToPlayerController()->GetHUD())->UpdateTextVisibility(true);

						FString Text;
						Text.Append("Pick up "); Text.Append(Other->GetObjectName());

						Cast<ATheFarm51TestHUD>(GetController()->CastToPlayerController()->GetHUD())->UpdateText(Text);
					}

					CurrentItem = Cast<AInteractable>(Hit.GetActor());
				}
			}
		}
		else
		{
			Cast<ATheFarm51TestHUD>(GetController()->CastToPlayerController()->GetHUD())->UpdateTextVisibility(false);
			CurrentItem = NULL;
		}
	}
}

void ATheFarm51TestCharacter::ClientInteract_Implementation()
{
	if (!HasAuthority() && IsLocallyControlled())
	{
		if (GetCurrentItem())
		{
			ServerEquipItem(this, GetCurrentItem());
		}
	}
}

void ATheFarm51TestCharacter::ClientSendMessage_Implementation(const FString& msg)
{
	if (!HasAuthority() && IsLocallyControlled())
	{
		GetController()->CastToPlayerController()->ClientMessage(msg);
	}
}

void ATheFarm51TestCharacter::ServerEquipItem_Implementation(AActor* OtherActor, AInteractable* Item)
{
	if (HasAuthority())
	{
		if (OtherActor)
		{
			auto Character = Cast<ATheFarm51TestCharacter>(OtherActor);

			if (Character)
			{
				if (Item)
				{
					if (Character->GetEquippedItem())
					{
						FActorSpawnParameters SpawnParams;
						FVector ItemLoc = Character->GetActorLocation() + Character->GetActorForwardVector() * 100.f;
						FRotator ItemRot = Character->GetActorRotation();

						auto Dropped = GetWorld()->SpawnActor<AInteractable>(Character->GetEquippedItem()->GetClass(), ItemLoc, ItemRot, SpawnParams);
						if (Dropped)
						{
							Dropped->ServerSetMesh(Character->GetEquippedItem()->GetMesh());
							Dropped->SetActorScale3D(Character->GetEquippedItem()->GetActorScale3D());
							Dropped->ServerSetObjectName(Character->GetEquippedItem()->GetObjectName());
							Dropped->ServerSetImage(Character->GetEquippedItem()->GetImage());
						}

						Character->GetEquippedItem()->Destroy();
						Character->EquippedItem = NULL;
					}

					Character->ClientEquipItem(Item);

					if (Item)
						Item->Destroy();
				}
			}
		}
	}
}

bool ATheFarm51TestCharacter::ServerEquipItem_Validate(AActor* OtherActor, AInteractable* Item)
{
	if (OtherActor)
		return true;
	return false;
}

void ATheFarm51TestCharacter::ClientEquipItem_Implementation(AInteractable* Item)
{
	if (IsLocallyControlled() && Item)
	{
		ServerSetEquippedItem(this, Item);
		ClientSetEquippedItem(Item);

		auto HUD = Cast<ATheFarm51TestHUD>(GetController()->CastToPlayerController()->GetHUD());
		if (HUD && GetEquippedItem())
		{
			HUD->UpdateImage(Item->GetImage());
			HUD->UpdateImageVisibility(true);
		}
	}
}

AInteractable* ATheFarm51TestCharacter::GetCurrentItem()
{
	return CurrentItem;
}

AInteractable* ATheFarm51TestCharacter::GetEquippedItem()
{
	return EquippedItem;
}

void ATheFarm51TestCharacter::ServerSetEquippedItem_Implementation(AActor* Other, AInteractable* NewItem)
{
	if (HasAuthority() && Other)
	{
		ATheFarm51TestCharacter* Character = Cast<ATheFarm51TestCharacter>(Other);
		if (Character)
		{
			EquippedItem = NewItem;
		}
	}
}

bool ATheFarm51TestCharacter::ServerSetEquippedItem_Validate(AActor* Other, AInteractable* NewItem)
{
	if (Other && NewItem)
		return true;
	return false;
}

void ATheFarm51TestCharacter::ClientSetEquippedItem_Implementation(AInteractable* NewItem)
{
	if (!HasAuthority() && IsLocallyControlled() && NewItem)
	{
		EquippedItem = NewItem;
	}

}