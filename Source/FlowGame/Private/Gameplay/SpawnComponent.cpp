// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#include "Gameplay/SpawnComponent.h"

#include "Animation/AnimInstance.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnComponent)

USpawnComponent::USpawnComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetUsingAbsoluteScale(true);
	SetUsingAbsoluteRotation(false);
	ArrowColor = FColor::Red;
	ArrowSize = 0.5f;
}

void USpawnComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Despawn();

	Super::EndPlay(EndPlayReason);
}

bool USpawnComponent::Spawn(const FQuestSpawnParams& SpawnParams)
{
	if (SpawnParams.ActorClass->IsChildOf(APawn::StaticClass()))
	{
		UBehaviorTree* LoadedBT = LoadAsset<UBehaviorTree>(SpawnParams.BehaviorTree);
		SpawnedActor = UAIBlueprintHelperLibrary::SpawnAIFromClass(this, SpawnParams.ActorClass.Get(), LoadedBT, GetComponentLocation(), GetComponentRotation(), SpawnParams.bNoCollisionFail);

		if (SpawnedActor.IsValid() && (SpawnParams.AnimInstance || SpawnParams.AnimationAsset))
		{
			if (const ACharacter* Character = Cast<ACharacter>(SpawnedActor))
			{
				SetAnimationOnSpawnedMesh(Character->GetMesh(), SpawnParams);
			}
			else if (const APawn* Pawn = Cast<APawn>(SpawnedActor))
			{
				if (USkeletalMeshComponent* SkeletalMesh = Pawn->FindComponentByClass<USkeletalMeshComponent>())
				{
					SetAnimationOnSpawnedMesh(SkeletalMesh, SpawnParams);
				}
			}
		}
	}
	else
	{
		SpawnedActor = GetWorld()->SpawnActor(SpawnParams.ActorClass, &GetComponentTransform());
	}

	if (SpawnedActor.IsValid() && SpawnParams.ActorScale != 1.0f)
	{
		SpawnedActor->SetActorScale3D(SpawnedActor->GetActorScale3D() * SpawnParams.ActorScale);
	}

	return SpawnedActor.IsValid();
}

void USpawnComponent::Despawn()
{
	if (SpawnedActor.IsValid())
	{
		SpawnedActor->Destroy();
	}

	SpawnedActor = nullptr;
}

void USpawnComponent::SetAnimationOnSpawnedMesh(USkeletalMeshComponent* SkeletalMeshComponent, const FQuestSpawnParams& SpawnParams)
{
	if (SpawnParams.AnimInstance)
	{
		SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		SkeletalMeshComponent->SetAnimInstanceClass(SpawnParams.AnimInstance);
	}
	else if (SpawnParams.AnimationAsset)
	{
		SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		SkeletalMeshComponent->SetAnimation(SpawnParams.AnimationAsset);
	}
}
