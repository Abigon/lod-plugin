// Copyright Dmitrii Shulaev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EditorStaticMeshLibrary.h"
#include "GenerateLODsLibrary.generated.h"


UCLASS()
class GENERATELODS_API UGenerateLODsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "LODs")
	static TArray<FEditorScriptingMeshReductionSettings> GetLODsSetting();



	/*
	Generate LODs from Project Settings (Game / Generate LODs Plugin Settings)
	If Project Settings empty - Generate 3-levels LOD for StaticMesh asset in array Meshes
	LOD0:
	 - PercentTriangles = 1.f;
	 - ScreenSize = 0.75f;
	LOD1:
	 - PercentTriangles = 0.5f;
	 - ScreenSize = 0.5f;
	LOD2:
	 - PercentTriangles = 0.3f;
	 - ScreenSize = 0.3f;
	*/
	UFUNCTION(BlueprintCallable, Category = "LODs")
	static void ModifyLODsForAssets(TArray<UStaticMesh*> Meshes);

	// Remove all LODs from StaticMesh asset in array Meshes
	UFUNCTION(BlueprintCallable, Category = "LODs")
	static void RemoveLODsFromAssets(TArray<UStaticMesh*> Meshes);
	
private:
	static void ModifyAssetLODs(UStaticMesh* AssetInstance, TArray<FEditorScriptingMeshReductionSettings> LODSettings);
	static void RemoveAssetLODs(UStaticMesh* AssetInstance);
	static void SaveAsset(UObject* AssetInstance);

	static TArray<FEditorScriptingMeshReductionSettings> GetDefaultLODsSetting();
	static int32 GetMinNumVerticesSettings();

};
