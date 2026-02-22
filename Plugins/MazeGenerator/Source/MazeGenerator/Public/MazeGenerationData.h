#pragma once

#include "CoreMinimal.h"
#include "MazeGenerationData.generated.h"

enum class EMazeType : uint8;

/**
 * This struct is used to hold the data required for maze generation.
 * It includes the number of rows and columns, a selection percentage for long paths,
 * and a list of tile coordinates to be excluded from the maze.
 */
USTRUCT(BlueprintType, meta=(DisplayName="MazeGenerationData"))
struct MAZEGENERATOR_API FMazeGenerationData
{
	GENERATED_USTRUCT_BODY()

	// The number of rows of the maze
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=1, UIMax=33, ClampMin=1, ClampMax=33))
	int32 NumRows;

	// The number of columns of the maze
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=1, UIMax=33, ClampMin=1, ClampMax=33))
	int32 NumColumns;

	// A percentage of selection used to influence the maze generation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=0.f, UIMax=1.f, ClampMin=0.f, ClampMax=1.f))
	float LongPathsSelectionPercentage;

	// A list of tile coordinates to be excluded from the maze
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIntVector> ExclusionList;
};
