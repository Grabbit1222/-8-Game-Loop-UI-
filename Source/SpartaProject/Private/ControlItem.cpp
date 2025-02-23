#include "ControlItem.h"
#include "SpartaCharacter.h"

AControlItem::AControlItem()
{
	ReverseTime = 5.0f;
	ItemType = "Debuff";
}

void AControlItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			// ReverseTime 동안 플레이어 컨트롤 반전
			PlayerCharacter->ReverseControls(ReverseTime);
		}

		DestroyItem();
	}
}