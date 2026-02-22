#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MazeGeneratorFunctionLibrary.generated.h"

struct FMazeGenerationData;
enum class EMazeDirection : uint8;
enum class EMazeType : uint8;


/**
 * A Blueprint Function Library for generating mazes using Wang Tiles.
 * This library provides functions to create empty mazes, generate mazes,
 * and manipulate maze tiles by adding directions and checking for doors.
 */
UCLASS()
class MAZEGENERATOR_API UMazeGeneratorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Generate an empty grid, used for creating a new maze
	 * */
	UFUNCTION(BlueprintCallable, Category="Maze Generation")
	static TMap<FIntVector, int32> GenerateEmptyMaze(int32 NumRows, int32 NumColumns);

	/**
	 * Generate a maze by using Wang Tiles
	 */
	UFUNCTION(BlueprintCallable, Category="Maze Generation")
	static TMap<FIntVector, int32> GenerateMaze(const FMazeGenerationData Data);

	/**
	 * Returns the size of the maze (number of rows and columns)
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Maze Generation")
	static FIntVector GetMazeSize(const TMap<FIntVector, int32> Map);

	/**
	 * Adds a direction to a tile
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Maze Generation")
	static int32 AddDirection(int32 TileId, EMazeDirection Direction);

	/**
	 * Checks if a tile has a door by its id
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Maze Generation")
	static bool HasDirection(int32 TileId, EMazeDirection Direction);
};
