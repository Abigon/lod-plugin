// Copyright Dmitrii Shulaev. All Rights Reserved.


#include "GenerateLODsLibrary.h"
#include "EditorStaticMeshLibrary.h"
#include "GenerateLODSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogGenerateLODs, All, All);

void UGenerateLODsLibrary::ModifyLODsForAssets(TArray<UStaticMesh*> Meshes)
{
	if (Meshes.Num() <= 0) return;
	const auto LODSettings = GetLODsSetting();
	for (UStaticMesh* Mesh : Meshes)
	{
		if (Mesh->GetNumVertices(0) >= GetMinNumVerticesSettings())
		{
			ModifyAssetLODs(Mesh, LODSettings);
			SaveAsset(Mesh);
		}
	}
}

void UGenerateLODsLibrary::RemoveLODsFromAssets(TArray<UStaticMesh*> Meshes)
{
	if (Meshes.Num() <= 0) return;
	for (UStaticMesh* Mesh : Meshes)
	{
		RemoveAssetLODs(Mesh);
		SaveAsset(Mesh);
	}
}

void UGenerateLODsLibrary::ModifyAssetLODs(UStaticMesh* AssetInstance, TArray<FEditorScriptingMeshReductionSettings> LODSettings)
{
	if (!AssetInstance) return;

	FEditorScriptingMeshReductionOptions Options;
	Options.ReductionSettings = LODSettings;

	UEditorStaticMeshLibrary::SetLods(AssetInstance, Options);
	AssetInstance->MarkPackageDirty();
}

void UGenerateLODsLibrary::RemoveAssetLODs(UStaticMesh* AssetInstance)
{
	if (!AssetInstance) return;
	UEditorStaticMeshLibrary::RemoveLods(AssetInstance);
	AssetInstance->MarkPackageDirty();
}

void UGenerateLODsLibrary::SaveAsset(UObject* AssetInstance)
{
	if (!AssetInstance) return;
	if (UPackage* Package = AssetInstance->GetPackage())
	{
		if (Package->IsDirty())
		{
			const FString PackageName = FPackageName::LongPackageNameToFilename(Package->GetPathName(), FPackageName::GetAssetPackageExtension());

			UE_LOG(LogGenerateLODs, Log, TEXT("Saving asset: %s..."), *PackageName);
			if (Package->SavePackage(Package, AssetInstance, RF_Standalone, *PackageName, GLog))
			{
				UE_LOG(LogGenerateLODs, Log, TEXT("Done"));
			}
		}
	}
}

TArray<FEditorScriptingMeshReductionSettings> UGenerateLODsLibrary::GetLODsSetting()
{
	TArray<FEditorScriptingMeshReductionSettings> LODsFromSettings = GetDefault<UGenerateLODSettings>()->LODs;
	return LODsFromSettings.Num() > 0 ? LODsFromSettings : GetDefaultLODsSetting();
}

TArray<FEditorScriptingMeshReductionSettings> UGenerateLODsLibrary::GetDefaultLODsSetting()
{
	TArray<FEditorScriptingMeshReductionSettings> ReductionSettings;
	FEditorScriptingMeshReductionSettings Settings;

	// LOD0
	Settings.PercentTriangles = 1.f;
	Settings.ScreenSize = 0.75f;
	ReductionSettings.Add(Settings);

	// LOD1
	Settings.PercentTriangles = 0.5f;
	Settings.ScreenSize = 0.5f;
	ReductionSettings.Add(Settings);

	// LOD2
	Settings.PercentTriangles = 0.3f;
	Settings.ScreenSize = 0.3f;
	ReductionSettings.Add(Settings);

	return ReductionSettings;
}

int32 UGenerateLODsLibrary::GetMinNumVerticesSettings()
{
	return GetDefault<UGenerateLODSettings>()->MinNumVertices;
}
