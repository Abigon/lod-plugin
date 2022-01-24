// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateLODsCommandlet.h"
#include "GenerateLODsLibrary.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Modules/ModuleManager.h"

int32 UGenerateLODsCommandlet::Main(const FString& Params)
{
	TArray<FString> Tokens;
	TArray<FString> Switches;

	// -run=GenerateLODs -GenerateLODs /Game/StarterContent/Shapes ...
	// -run=GenerateLODs -RemoveLODs /Game/StarterContent/Shapes ...
	ParseCommandLine(*Params, Tokens, Switches);

	if (Tokens.Num() > 0)
	{
		if (Switches.Contains(TEXT("GenerateLODs")))
		{
			ProcessAssets(Tokens, true);
		}
		else if (Switches.Contains(TEXT("RemoveLODs")))
		{
			ProcessAssets(Tokens, false);
		}
	}
	return Super::Main(Params);
}

void UGenerateLODsCommandlet::ProcessAssets(TArray<FString> RootDirectories, bool bIsModify)
{
	const auto Meshes = GetMeshesFromDirectories(RootDirectories);
	bIsModify ? UGenerateLODsLibrary::ModifyLODsForAssets(Meshes): UGenerateLODsLibrary::RemoveLODsFromAssets(Meshes);	
}

TArray<UStaticMesh*> UGenerateLODsCommandlet::GetMeshesFromDirectories(TArray<FString> RootDirectories)
{
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);
	AssetRegistryModule.Get().SearchAllAssets(true);

	const FString ClassName = TEXT("StaticMesh");
	TArray<FAssetData> AssetList;
	AssetRegistryModule.Get().GetAssetsByClass(*ClassName, AssetList, true);

	TArray<UStaticMesh*> Meshes;

	for (FAssetData AssetData : AssetList)
	{
		for (FString RootDirectory : RootDirectories)
		{
			if (AssetData.ObjectPath.ToString().StartsWith(RootDirectory, ESearchCase::IgnoreCase))
			{
				UStaticMesh* AssetInstance = Cast<UStaticMesh>(AssetData.GetAsset());
				if (AssetInstance)
				{
					Meshes.Add(AssetInstance);
				}
				break;
			}
		}
	}
	return Meshes;
}
