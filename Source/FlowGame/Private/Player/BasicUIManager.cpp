// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors
#include "Player/BasicUIManager.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BasicUIManager)

void UBasicUIManager::OpenWidget(const TSoftClassPtr<UUserWidget> SoftClass)
{
	if (SoftClass == nullptr || OpenedWidgets.Contains(SoftClass))
	{
		return;
	}

	UClass* WidgetClass = SoftClass.LoadSynchronous();
	if (WidgetClass->HasAnyClassFlags(CLASS_Abstract))
	{
		return;
	}

	UUserWidget* Widget = Cast<UUserWidget>(UWidgetBlueprintLibrary::Create(GetWorld(), WidgetClass, GetWorld()->GetFirstPlayerController()));
	OpenedWidgets.Add(SoftClass, Widget);
	Widget->AddToViewport();
}

void UBasicUIManager::CloseWidget(const TSoftClassPtr<UUserWidget> SoftClass)
{
	if (UUserWidget* Widget = OpenedWidgets.FindRef(SoftClass))
	{
		Widget->RemoveFromParent();
		OpenedWidgets.Remove(SoftClass);
	}
}

void UBasicUIManager::ToggleWidget(const TSoftClassPtr<UUserWidget> SoftClass)
{
	if (OpenedWidgets.Contains(SoftClass))
	{
		CloseWidget(SoftClass);
	}
	else
	{
		OpenWidget(SoftClass);
	}
}

void UBasicUIManager::HideWidgets()
{
	for (auto WidgetIt = OpenedWidgets.CreateIterator(); WidgetIt; ++WidgetIt)
	{
		const TPair<TSoftClassPtr<UUserWidget>, UUserWidget*> Widget = *WidgetIt;

		HiddenWidgets.Add(Widget.Key);
		CloseWidget(Widget.Key);
	}
}

void UBasicUIManager::RestoreWidgets()
{
	for (const TSoftClassPtr<UUserWidget>& WidgetClass : HiddenWidgets)
	{
		OpenWidget(WidgetClass);
	}
	HiddenWidgets.Empty();
}
