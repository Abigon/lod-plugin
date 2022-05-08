// Copyright Dmitrii Shulaev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorStaticMeshLibrary.h"
#include "Engine/DeveloperSettings.h"
#include "GenerateLODSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Generate LODs Plugin Settings"))
class GENERATELODS_API UGenerateLODSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	// LODs settings
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "LODs")
	TArray<FEditorScriptingMeshReductionSettings> LODs = {
			RS(1.f, 0.75f),
			RS(0.5f, 0.5f),
			RS(0.3f, 0.3f),
	};

	// The minimum number of StaticMesh vertices to generate LODs with setting above
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Vertices")
	int32 MinNumVertices = 1000;

private:
	FEditorScriptingMeshReductionSettings RS(float P, float S)
	{
		FEditorScriptingMeshReductionSettings R;
		R.PercentTriangles = P;
		R.ScreenSize = S;
		return R;
	}
};
