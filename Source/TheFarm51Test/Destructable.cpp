// Fill out your copyright notice in the Description page of Project Settings.

#include "Destructable.h"
#include "Engine.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "TheFarm51TestCharacter.h"

FDestStage::FDestStage()
{
	HP = 0;
	Name = "";
}

// Sets default values
ADestructable::ADestructable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->bCastDynamicShadow = false;
	Mesh->CastShadow = false;
	Mesh->SetHiddenInGame(false, true);
	Mesh->SetIsReplicated(true);

	RootComponent = Mesh;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->InitBoxExtent(FVector(200.f, 200.f, 200.f));
	CollisionComp->SetupAttachment(RootComponent);
	CollisionComp->SetIsReplicated(true);

	InitialStage = 0;
	InitialHP = 0;
}

// Called when the game starts or when spawned
void ADestructable::BeginPlay()
{
	Super::BeginPlay();
	
	NetMultiSetStage(InitialStage);
	NetMultiSetHP(InitialHP);
	NetMultiSetMesh(Stages[CurrentStage].Mesh);
}

// Called every frame
void ADestructable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestructable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADestructable, CurrentHP);
	DOREPLIFETIME(ADestructable, CurrentStage);
}

void ADestructable::NetMultiSetHP_Implementation(int32 HP)
{
	CurrentHP = HP;
}

bool ADestructable::NetMultiSetHP_Validate(int32 HP)
{
	return true;
}

int32 ADestructable::GetHP()
{
	return CurrentHP;
}

UParticleSystem* ADestructable::GetDestroyedParticle()
{
	return DestroyedParticle;
}

USoundBase* ADestructable::GetDestroyedSound()
{
	return DestroyedSound;
}

void ADestructable::NetMultiSetMesh_Implementation(UStaticMesh* NewMesh)
{
	if (NewMesh)
		Mesh->SetStaticMesh(NewMesh);
}

bool ADestructable::NetMultiSetMesh_Validate(UStaticMesh* NewMesh)
{
	if (NewMesh)
		return true;
	return false;
}

void ADestructable::NetMultiSetStage_Implementation(int32 Stage)
{
	CurrentStage = Stage;
}

bool ADestructable::NetMultiSetStage_Validate(int32 Stage)
{
	if (Stage >= 0 && Stage < Stages.Num())
		return true;
	return false;
}

void ADestructable::ClientChangeStageUp_Implementation()
{
	if (Stages.IsValidIndex(CurrentStage) && CurrentStage < Stages.Num())
	{
		CurrentStage++;
		Mesh->SetStaticMesh(Stages[CurrentStage].Mesh);
	}
}

bool ADestructable::ClientChangeStageUp_Validate()
{
	if (Stages.Num() > 0)
		return true;
	return false;
}

void ADestructable::ClientChangeStageDown_Implementation()
{
	if (Stages.IsValidIndex(CurrentStage) && CurrentStage >= 0)
	{
		CurrentStage--;
		Mesh->SetStaticMesh(Stages[CurrentStage].Mesh);
	}
}

bool ADestructable::ClientChangeStageDown_Validate()
{
	if (Stages.Num() > 0)
		return true;
	return false;
}

float ADestructable::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Call the base class - this will tell us how much damage to apply  
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		NetMultiSetHP(GetHP() - 1);

		if (CurrentHP <= 0)
		{
			ClearComponentOverlaps();

			if (DestroyedParticle)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyedParticle, GetActorLocation());

			if (DestroyedSound)
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestroyedSound, GetActorLocation());

			SetLifeSpan(0.001f);
		}
		else
		{
			if (InitialStage != 0)
			{
				if (CurrentStage > 0 && CurrentHP <= Stages[CurrentStage].HP)
				{
					ClientChangeStageDown();

					if (Stages[CurrentStage].Particle)
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Stages[CurrentStage].Particle, GetActorLocation());
					}

					if (Stages[CurrentStage].Sound)
					{
						if (Stages[CurrentStage].Sound->GetSoundClass())
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), Stages[CurrentStage].Sound, GetActorLocation());
					}
				}
			}
			else
			{
				if (CurrentStage < Stages.Num() - 1 && CurrentHP <= Stages[CurrentStage].HP)
				{
					ClientChangeStageUp();

					if (Stages[CurrentStage].Particle)
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Stages[CurrentStage].Particle, GetActorLocation());
					}

					if (Stages[CurrentStage].Sound)
					{
						if (Stages[CurrentStage].Sound->GetSoundClass())
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), Stages[CurrentStage].Sound, GetActorLocation());
					}
				}
			}
		}
	}

	return ActualDamage;
}