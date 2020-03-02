// Fill out your copyright notice in the Description page of Project Settings.


#include "TheFarm51TestGameSession.h"
#include "Engine.h"
#include "UnrealNetwork.h"
#include "CoreOnline.h"
#include "OnlineSubsystem.h"

void ATheFarm51TestGameSession::InitOptions(const FString& Options)
{
	Super::InitOptions(Options);

	GameMode = GetWorld()->GetAuthGameMode<ATheFarm51TestGameMode>();
	
	if (GameMode)
	{
		MaxPlayers = 10;
		SessionName = GameSessionName;
	}
}

void ATheFarm51TestGameSession::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void ATheFarm51TestGameSession::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}

void ATheFarm51TestGameSession::PostLogin(APlayerController* Player)
{
	Super::PostLogin(Player);
}

void ATheFarm51TestGameSession::NotifyLogout(const APlayerController* Player)
{
	Super::NotifyLogout(Player);
}

void ATheFarm51TestGameSession::RegisterServer()
{
	Super::RegisterServer();


}