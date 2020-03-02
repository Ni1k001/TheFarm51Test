// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TheFarm51TestHUD.generated.h"

UCLASS()
class ATheFarm51TestHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATheFarm51TestHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	void UpdateText(FString Text);
	void UpdateTextVisibility(bool Visibility);

	void UpdateImage(class UTexture2D* Texture);
	void UpdateImageVisibility(bool Visibility);

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

	TSubclassOf<class UUserWidget> HUDInfoWidgetClass;
	class UUserWidget* CurrentWidget;

	class ATheFarm51TestCharacter* Player;
};