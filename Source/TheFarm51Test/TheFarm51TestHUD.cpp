// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TheFarm51TestHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "TheFarm51TestCharacter.h"
//#include "Interactable.h"
#include "TextBlock.h"
#include "Image.h"

ATheFarm51TestHUD::ATheFarm51TestHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;

	static ConstructorHelpers::FClassFinder<UUserWidget> InfoWidgetObj(TEXT("/Game/FirstPersonCPP/Widgets/InfoWidget"));
	HUDInfoWidgetClass = InfoWidgetObj.Class;
}

void ATheFarm51TestHUD::DrawHUD()
{
	Super::DrawHUD();

	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	const FVector2D CrosshairDrawPosition((Center.X), (Center.Y + 20.0f));

	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}

void ATheFarm51TestHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwningPlayerController()->IsLocalPlayerController())
	{
		if (HUDInfoWidgetClass != nullptr)
		{
			Player = Cast<ATheFarm51TestCharacter>(GetOwningPlayerController()->GetCharacter());
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDInfoWidgetClass);

			if (CurrentWidget)
			{
				CurrentWidget->AddToViewport();
			}
		}
	}
}

void ATheFarm51TestHUD::UpdateText(FString text)
{
	if (GetOwningPlayerController()->IsLocalPlayerController())
	{
		//		ATheFarm51TestCharacter* Player = Cast<ATheFarm51TestCharacter>(GetOwningPlayerController()->GetCharacter());
		if (Player && CurrentWidget)
		{
			if (CurrentWidget->GetWidgetFromName("PickUpText")->IsValidLowLevel())
			{
				Cast<UTextBlock>(CurrentWidget->GetWidgetFromName("PickUpText"))->SetText(FText::FromString(text));
			}
		}
	}
}

void ATheFarm51TestHUD::UpdateTextVisibility(bool Visibility)
{
	if (GetOwningPlayerController()->IsLocalPlayerController())
	{
//		ATheFarm51TestCharacter* Player = Cast<ATheFarm51TestCharacter>(GetOwningPlayerController()->GetCharacter());
		if (Player && CurrentWidget)
		{
			if (CurrentWidget->GetWidgetFromName("PickUpText")->IsValidLowLevel())
			{
				if (Visibility)
					CurrentWidget->GetWidgetFromName("PickUpText")->SetVisibility(ESlateVisibility::Visible);
				else
					CurrentWidget->GetWidgetFromName("PickUpText")->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void ATheFarm51TestHUD::UpdateImage(class UTexture2D* Texture)
{
	if (GetOwningPlayerController()->IsLocalPlayerController())
	{
		if (Player && CurrentWidget)
		{
			if (CurrentWidget->GetWidgetFromName("ItemImage")->IsValidLowLevel())
			{
				Cast<UImage>(CurrentWidget->GetWidgetFromName("ItemImage"))->SetBrushFromTexture(Texture);
			}
		}
	}
}

void ATheFarm51TestHUD::UpdateImageVisibility(bool Visibility)
{
	if (GetOwningPlayerController()->IsLocalPlayerController())
	{
//		ATheFarm51TestCharacter* Player = Cast<ATheFarm51TestCharacter>(GetOwningPlayerController()->GetCharacter());
		if (Player && CurrentWidget)
		{
			if (CurrentWidget->GetWidgetFromName("ItemImage")->IsValidLowLevel())
			{
				if (Visibility)
					CurrentWidget->GetWidgetFromName("ItemImage")->SetVisibility(ESlateVisibility::Visible);
				else
					CurrentWidget->GetWidgetFromName("ItemImage")->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}