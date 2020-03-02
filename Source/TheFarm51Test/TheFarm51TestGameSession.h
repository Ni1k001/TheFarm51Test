// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "TheFarm51TestGameMode.h"
#include "TheFarm51TestGameSession.generated.h"

/**
 * 
 */
UCLASS()
class THEFARM51TEST_API ATheFarm51TestGameSession : public AGameSession
{
	GENERATED_BODY()

	void InitOptions(const FString& Options);
	void HandleMatchHasStarted();
	void HandleMatchHasEnded();
	void PostLogin(APlayerController* Player);
	void NotifyLogout(const APlayerController* Player);
	void RegisterServer();

	ATheFarm51TestGameMode* GameMode;
};
