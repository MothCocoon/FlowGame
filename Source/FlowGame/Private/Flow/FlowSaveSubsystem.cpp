// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#include "Flow/FlowSaveSubsystem.h"

#include "FlowWorldSettings.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowSaveSubsystem)

FString UFlowSaveSubsystem::CheckpointSlotName = TEXT("Checkpoint");

bool UFlowSaveSubsystem::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	return HasAnyFlags(RF_ClassDefaultObject) ? false : ProcessConsoleExec(Cmd, Ar, nullptr);
}

void UFlowSaveSubsystem::SaveGame()
{
	USaveGame* NewSaveGame = UGameplayStatics::CreateSaveGameObject(UFlowSaveGame::StaticClass());
	OnGameSaved(Cast<UFlowSaveGame>(NewSaveGame));

	UGameplayStatics::SaveGameToSlot(NewSaveGame, CheckpointSlotName, 0);
}

void UFlowSaveSubsystem::LoadGame()
{
	USaveGame* LoadedSave = UGameplayStatics::LoadGameFromSlot(CheckpointSlotName, 0);
	if (LoadedSave)
	{
		AbortActiveFlows();
		OnGameLoaded(Cast<UFlowSaveGame>(LoadedSave));

		const AFlowWorldSettings* WorldSettings = Cast<AFlowWorldSettings>(GetWorld()->GetWorldSettings());
		if (WorldSettings && WorldSettings->GetFlowComponent()->LoadInstance(this))
		{
			WorldSettings->GetFlowComponent()->LoadRootFlow();
		}
	}
}
