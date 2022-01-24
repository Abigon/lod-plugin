// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "GenerateLODsCommandlet.generated.h"


UCLASS()
class GENERATELODS_API UGenerateLODsCommandlet : public UCommandlet
{
	GENERATED_BODY()

	virtual int32 Main(const FString& Params) override;

	void ProcessAssets(TArray<FString> RootDirectories, bool bIsModify);

	TArray<UStaticMesh*> GetMeshesFromDirectories(TArray<FString> RootDirectories);
};
