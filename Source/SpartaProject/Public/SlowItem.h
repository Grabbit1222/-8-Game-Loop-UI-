#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "SlowItem.generated.h"

UCLASS()
class SPARTAPROJECT_API ASlowItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ASlowItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float SlowPercentage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float SlowTime;

	virtual void ActivateItem(AActor* Activator) override;

	FTimerHandle SlowTimerHandle;
};
