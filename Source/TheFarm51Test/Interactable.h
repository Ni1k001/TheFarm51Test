// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Interactable.generated.h"

UCLASS()
class THEFARM51TEST_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
		UBoxComponent* CollisionComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Replicated, Category = Config)
		FString Name;

	UPROPERTY(EditAnywhere, Replicated, Category = Config)
		class UTexture2D* Image;

	UFUNCTION()
		FString GetObjectName();

	UFUNCTION(Reliable, Server)
		void ServerSetObjectName(const FString& NewName);
	void ServerSetObjectName_Implementation(const FString& NewName);

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerSetObjectTexture(class UTexture2D* NewTexture);
	void ServerSetObjectTexture_Implementation(class UTexture2D* NewTexture);
	bool ServerSetObjectTexture_Validate(class UTexture2D* NewTexture);

	UFUNCTION()
		UStaticMeshComponent* GetMesh();

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerSetMesh(UStaticMeshComponent* OtherMesh);
	void ServerSetMesh_Implementation(UStaticMeshComponent* OtherMesh);
	bool ServerSetMesh_Validate(UStaticMeshComponent* OtherMesh);

	UFUNCTION()
		class UTexture2D* GetImage();

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerSetImage(class UTexture2D* NewTexture);
	void ServerSetImage_Implementation(class UTexture2D* NewTexture);
	bool ServerSetImage_Validate(class UTexture2D* NewTexture);
};
