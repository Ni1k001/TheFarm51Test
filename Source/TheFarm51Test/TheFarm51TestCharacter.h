// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Interactable.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "TheFarm51TestCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class ATheFarm51TestCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* TP_Gun;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* TP_MuzzleLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

public:
	ATheFarm51TestCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay();

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ATheFarm51TestProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	void MoveForward(float Val);
	void MoveRight(float Val);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	UFUNCTION(Reliable, Client)
		void ClientFire();
	void ClientFire_Implementation();

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerOnFire(AActor* OtherActor);
	void ServerOnFire_Implementation(AActor* OtherActor);
	bool ServerOnFire_Validate(AActor* OtherActor);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void NetMultiPlaySnd(USoundBase* Sound, FVector Loc);
	void NetMultiPlaySnd_Implementation(USoundBase* Sound, FVector Loc);
	bool NetMultiPlaySnd_Validate(USoundBase* Sound, FVector Loc);

	UFUNCTION(Reliable, Client)
		void ClientInteractCheck();
	void ClientInteractCheck_Implementation();

	UFUNCTION(Reliable, Client)
		void ClientInteract();
	void ClientInteract_Implementation();

	UFUNCTION(Reliable, Client)
		void ClientSendMessage(const FString& msg);
	void ClientSendMessage_Implementation(const FString& msg);

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerEquipItem(AActor* Other, AInteractable* Item);
	void ServerEquipItem_Implementation(AActor* Other, AInteractable* Item);
	bool ServerEquipItem_Validate(AActor* Other, AInteractable* Item);

	UFUNCTION(Reliable, Client)
		void ClientEquipItem(AInteractable* Item);
	void ClientEquipItem_Implementation(AInteractable* Item);

	UPROPERTY()
		AInteractable* CurrentItem;

	UPROPERTY(VisibleAnywhere, Category = Config)
		AInteractable* EquippedItem;

	UFUNCTION()
		AInteractable* GetCurrentItem();

	UFUNCTION()
		AInteractable* GetEquippedItem();

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerSetEquippedItem(AActor* Other, AInteractable* NewItem);
	void ServerSetEquippedItem_Implementation(AActor* Other, AInteractable* NewItem);
	bool ServerSetEquippedItem_Validate(AActor* Other, AInteractable* NewItem);

	UFUNCTION(Reliable, Client)
		void ClientSetEquippedItem(AInteractable* NewItem);
	void ClientSetEquippedItem_Implementation(AInteractable* NewItem);
	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

