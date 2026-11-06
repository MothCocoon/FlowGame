// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#pragma once

#include "Components/ArrowComponent.h"
#include "Engine/StreamableManager.h"

#include "SpawnComponent.generated.h"

class UAnimationAsset;
class UAnimInstance;
class UBehaviorTree;

USTRUCT()
struct FQuestSpawnParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float ActorScale = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Spawn AI")
	TSoftObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "Spawn AI")
	TSubclassOf<UAnimInstance> AnimInstance;

	UPROPERTY(EditAnywhere, Category = "Spawn AI")
	TObjectPtr<UAnimationAsset> AnimationAsset;

	UPROPERTY(EditAnywhere, Category = "Spawn AI")
	bool bNoCollisionFail = true;
};

/**
* Spawn Component
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class  USpawnComponent : public UArrowComponent
{
	GENERATED_UCLASS_BODY()

private:
	TWeakObjectPtr<AActor> SpawnedActor;
	FStreamableManager StreamableManager;
	
public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool Spawn(const FQuestSpawnParams& SpawnParams);
	void Despawn();

	bool HasSpawnedActor() const { return SpawnedActor.IsValid(); }

private:
	static void SetAnimationOnSpawnedMesh(USkeletalMeshComponent* SkeletalMeshComponent, const FQuestSpawnParams& SpawnParams);

	template<typename T>
	T* LoadAsset(TSoftObjectPtr<UObject> AssetPtr)
	{
		if (AssetPtr.IsNull())
		{
			return nullptr;
		}

		if (AssetPtr.IsPending())
		{
			const FSoftObjectPath& AssetRef = AssetPtr.ToSoftObjectPath();
			AssetPtr = Cast<T>(StreamableManager.LoadSynchronous(AssetRef, false));
		}

		return Cast<T>(AssetPtr.Get());
	}
};
