// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"
#include "Engine.h"
#include "UnrealNetwork.h"

// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->bCastDynamicShadow = false;
	Mesh->CastShadow = true;
	Mesh->SetHiddenInGame(false, true);
	Mesh->SetIsReplicated(true);

	RootComponent = Mesh;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->InitBoxExtent(FVector(200.f, 200.f, 200.f));
	CollisionComp->SetupAttachment(RootComponent);
	CollisionComp->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInteractable, Name);
	DOREPLIFETIME(AInteractable, Image);
}

FString AInteractable::GetObjectName()
{
	return Name;
}

void AInteractable::ServerSetObjectName_Implementation(const FString& NewName)
{
	if (HasAuthority())
	{
		Name = NewName;
	}
}

void AInteractable::ServerSetObjectTexture_Implementation(class UTexture2D* NewTexture)
{
	if (HasAuthority())
	{
		Image = NewTexture;
	}
}

bool AInteractable::ServerSetObjectTexture_Validate(class UTexture2D* NewTexture)
{
	if (NewTexture)
		return true;
	return false;
}

UStaticMeshComponent* AInteractable::GetMesh()
{
	return Mesh;
}

void AInteractable::ServerSetMesh_Implementation(UStaticMeshComponent* OtherMesh)
{
	if (HasAuthority())
	{
		if (OtherMesh)
		{
			Mesh->SetStaticMesh(OtherMesh->GetStaticMesh());
		}
	}
}

bool AInteractable::ServerSetMesh_Validate(UStaticMeshComponent* OtherMesh)
{
	if (OtherMesh)
		return true;
	return false;
}

class UTexture2D* AInteractable::GetImage()
{
	return Image;
}

void AInteractable::ServerSetImage_Implementation(class UTexture2D* NewTexture)
{
	if (HasAuthority())
	{
		Image = NewTexture;
	}
}

bool AInteractable::ServerSetImage_Validate(class UTexture2D* NewTexture)
{
	if (NewTexture)
		return true;
	return false;
}