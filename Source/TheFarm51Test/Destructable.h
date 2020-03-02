// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Destructable.generated.h"

USTRUCT(BlueprintType)
struct FDestStage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
		FString Name;

	UPROPERTY(EditAnywhere)
		UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
		UParticleSystem* Particle;

	UPROPERTY(EditAnywhere)
		class USoundBase* Sound;

	UPROPERTY(EditAnywhere)
		int32 HP;

	FDestStage();
};

UCLASS()
class THEFARM51TEST_API ADestructable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* CollisionComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		TArray<FDestStage> Stages;

	UPROPERTY(EditAnywhere)
		int32 InitialStage;

	UPROPERTY(EditAnywhere)
		int32 InitialHP;

	UPROPERTY(Replicated)
		int32 CurrentStage;

	UPROPERTY(Replicated)
		int32 CurrentHP;

	UPROPERTY(EditAnywhere)
		UParticleSystem* DestroyedParticle;

	UPROPERTY(EditAnywhere)
		USoundBase* DestroyedSound;

public:
	UFUNCTION(Reliable, NetMulticast, WithValidation)
		void NetMultiSetHP(int32 HP);
	void NetMultiSetHP_Implementation(int32 HP);
	bool NetMultiSetHP_Validate(int32 HP);

	int32 GetHP();

	UParticleSystem* GetDestroyedParticle();
	USoundBase* GetDestroyedSound();

	UFUNCTION(Reliable, NetMulticast, WithValidation)
		void NetMultiSetMesh(UStaticMesh* NewMesh);
	void NetMultiSetMesh_Implementation(UStaticMesh* NewMesh);
	bool NetMultiSetMesh_Validate(UStaticMesh* NewMesh);

	UFUNCTION(Reliable, NetMulticast, WithValidation)
		void NetMultiSetStage(int32 Stage);
	void NetMultiSetStage_Implementation(int32 Stage);
	bool NetMultiSetStage_Validate(int32 Stage);

	UFUNCTION(Reliable, Client, WithValidation)
		void ClientChangeStageUp();
	void ClientChangeStageUp_Implementation();
	bool ClientChangeStageUp_Validate();

	UFUNCTION(Reliable, Client, WithValidation)
		void ClientChangeStageDown();
	void ClientChangeStageDown_Implementation();
	bool ClientChangeStageDown_Validate();

protected:
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};