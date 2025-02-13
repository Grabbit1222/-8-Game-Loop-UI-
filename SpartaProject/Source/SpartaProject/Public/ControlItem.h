#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "ControlItem.generated.h"

UCLASS()
class SPARTAPROJECT_API AControlItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AControlItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ReverseTime;

	virtual void ActivateItem(AActor* Activator) override;

	FTimerHandle ReverseControlTimerHandle;
};
