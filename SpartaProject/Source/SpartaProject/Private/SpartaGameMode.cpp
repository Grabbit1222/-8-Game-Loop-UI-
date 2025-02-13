#include "SpartaGameMode.h"
#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "SpartaGameState.h"
#include "SpartaPawn.h"
#include "SpartaSpiderController.h"

ASpartaGameMode::ASpartaGameMode()
{
	//DefaultPawnClass = ASpartaCharacter::StaticClass();
	//PlayerControllerClass = ASpartaPlayerController::StaticClass();

	//스파이더 모드//
	DefaultPawnClass = ASpartaCharacter::StaticClass();
	PlayerControllerClass = ASpartaPlayerController::StaticClass();
	GameStateClass = ASpartaGameState::StaticClass();
}