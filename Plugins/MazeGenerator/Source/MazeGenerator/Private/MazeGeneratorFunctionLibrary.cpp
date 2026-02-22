#include "MazeGeneratorFunctionLibrary.h"

#include "MazeDirection.h"
#include "MazeGenerationData.h"
#include "MazeGeneratorLog.h"

TMap<FIntVector, int32> UMazeGeneratorFunctionLibrary::GenerateEmptyMaze(const int32 NumRows, const int32 NumColumns)
{
	TMap<FIntVector, int32> Map;
	for (int32 X = 0; X < NumRows; X++)
	{
		for (int32 Y = 0; Y < NumColumns; Y++)
		{
			Map.Add(FIntVector(X, Y, 0), 0);
		}
	}
	UE_LOG(LogMazeGenerator, Display, TEXT("Creating empty map with %d rows and %d columns."), NumRows, NumColumns);
	return Map;
}

TMap<FIntVector, int32> UMazeGeneratorFunctionLibrary::GenerateMaze(const FMazeGenerationData Data)
{
	// Ensure data is valid
	ensureMsgf((Data.NumRows > 0), TEXT("Maze Generator NumRows must be greater than zero."));
	ensureMsgf((Data.NumColumns > 0), TEXT("Maze Generator NumColumns must be greater than zero."));

	UE_LOG(LogMazeGenerator, Display, TEXT("-------------- GENERATING MAP (%d, %d) --------------"), Data.NumRows, Data.NumColumns);

	// Create the empty maze
	TMap<FIntVector, int32> Map = GenerateEmptyMaze(Data.NumRows, Data.NumColumns);

	// Remove excluded tiles from the map 
	for (auto Element: Data.ExclusionList)
	{
		if (Map.Find(Element) != nullptr)
		{
			Map.Remove(Element);
		}
	}

	// Create a list of all tiles coordinates
	TArray<FIntVector> KeyList;
	Map.GetKeys(KeyList);

	// Create a list of the visited tiles and add the first, randomly selected, tile
	TArray<FIntVector> VisitedList = {};
	VisitedList.Add(KeyList[FMath::RandRange(0, KeyList.Num() - 1)]);
	
	const auto ClampedSelectionPercentage = FMath::Clamp(Data.LongPathsSelectionPercentage, 0.f, 1.f);

	// Starts the main generation loop
	UE_LOG(LogMazeGenerator, Display, TEXT("-------------- TILE GENERATION LOOP --------------"));
	while (VisitedList.Num() > 0)
    {
		// Select a tile from the visited list: depending on the selection position,
		// the maze will have longer or shorter paths
		auto SelectedIndex = FMath::RandRange(0, VisitedList.Num() - 1);
    	if (const auto RandomPercentage = FMath::RandRange(0.f, 1.f) <= ClampedSelectionPercentage)
    	{
    		SelectedIndex = VisitedList.Num() - 1;
    	}
    	
        FIntVector CurrentTile = VisitedList[SelectedIndex];
    	UE_LOG(LogMazeGenerator, Display, TEXT(">>> Current tile: (%d, %d)."), CurrentTile.X, CurrentTile.Y);

	  	UE_LOG(LogMazeGenerator, Display, TEXT("Checking unvisited neighbors for tile (%d, %d)."), CurrentTile.X, CurrentTile.Y);
	 	TArray<EMazeDirection> AvailableDirections;

     	auto CurrentTileId = Map.FindRef(CurrentTile);

    	// Check if there are any unvisited neighbors
    	auto CheckTile = FIntVector(CurrentTile.X - 1, CurrentTile.Y, 0);
		if (Map.Contains(CheckTile) && Map.FindRef(CheckTile) == 0)
		{
	    	UE_LOG(LogMazeGenerator, Display, TEXT(">>> Contains North"));
			AvailableDirections.Add(EMazeDirection::North);
		}
    	CheckTile = FIntVector(CurrentTile.X, CurrentTile.Y + 1, 0);
		if (Map.Contains(CheckTile) && Map.FindRef(CheckTile) == 0)
		{
	    	UE_LOG(LogMazeGenerator, Display, TEXT(">>> Contains East"));
			AvailableDirections.Add(EMazeDirection::East);
		}
    	CheckTile = FIntVector(CurrentTile.X + 1, CurrentTile.Y, 0);
    	if (Map.Contains(CheckTile) && Map.FindRef(CheckTile) == 0)
    	{
	    	UE_LOG(LogMazeGenerator, Display, TEXT(">>> Contains South"));
    		AvailableDirections.Add(EMazeDirection::South);
    	}
    	CheckTile = FIntVector(CurrentTile.X, CurrentTile.Y - 1, 0);
    	if (Map.Contains(CheckTile) && Map.FindRef(CheckTile) == 0)
    	{
	    	UE_LOG(LogMazeGenerator, Display, TEXT(">>> Contains West"));
    		AvailableDirections.Add(EMazeDirection::West);
    	}

    	// If no opening is possible, remove tile from the visited list
    	if (AvailableDirections.Num() == 0)
    	{
    		// Log visited list length
			UE_LOG(LogMazeGenerator, Display, TEXT("--- Visited list length: %d"), VisitedList.Num());
    		UE_LOG(LogMazeGenerator, Display, TEXT("No available directions for tile (%d, %d). Removing tile from list."), CurrentTile.X, CurrentTile.Y);

			VisitedList.Remove(CurrentTile);
			UE_LOG(LogMazeGenerator, Display, TEXT("--- Visited list length: %d"), VisitedList.Num());
    		continue;;
    	}

		// Adds the new opening to the selected tile
		UE_LOG(LogMazeGenerator, Display, TEXT("Randomize opening direction for tile (%d, %d)."), CurrentTile.X, CurrentTile.Y);
    	const EMazeDirection SelectedDirection = AvailableDirections[FMath::RandRange(0, AvailableDirections.Num() - 1)];
    	CurrentTileId = AddDirection(CurrentTileId, SelectedDirection);
    	Map.Add(CurrentTile, CurrentTileId);
    	
		// Adds the selected direction to the new tile and add the tile to the visited list
    	auto Tile = FIntVector(0, 0, 0);
    	auto TileId = 0;
    	switch (SelectedDirection)
    	{
    	case EMazeDirection::North:
    		Tile = FIntVector(CurrentTile.X - 1, CurrentTile.Y, 0);
    		TileId = Map.FindRef(Tile);
    		TileId = AddDirection(TileId, EMazeDirection::South);
    		break;
    	case EMazeDirection::East:
    		Tile = FIntVector(CurrentTile.X, CurrentTile.Y + 1, 0);
    		TileId = Map.FindRef(Tile);
    		TileId = AddDirection(TileId, EMazeDirection::West);
    		break;
    	case EMazeDirection::South:
    		Tile = FIntVector(CurrentTile.X + 1, CurrentTile.Y, 0);
    		TileId = Map.FindRef(Tile);
    		TileId = AddDirection(TileId, EMazeDirection::North);
    		break;
    	case EMazeDirection::West:
    		Tile = FIntVector(CurrentTile.X, CurrentTile.Y - 1, 0);
    		TileId = Map.FindRef(Tile);
    		TileId = AddDirection(TileId, EMazeDirection::East);
    		break;
    	default:
    		break;
    	}
		UE_LOG(LogMazeGenerator, Display, TEXT("Adding neighbor tile (%d, %d) with corresponding opening."), Tile.X, Tile.Y);
   		Map.Add(Tile, TileId);
    	VisitedList.Add(Tile);
    }
	return Map;
}

/* **
 * Returns the size of the maze (number of rows and columns)
 */
FIntVector UMazeGeneratorFunctionLibrary::GetMazeSize(const TMap<FIntVector, int32> Map)
{
	TArray<FIntVector> KeyList;
	Map.GetKeys(KeyList);
	auto Result = FIntVector(0, 0, 0);
	for (const auto List : KeyList)
	{
		if (Result.X < List.X) { Result.X = List.X; };
		if (Result.Y < List.Y) { Result.Y = List.Y; };
	}
	return Result;
}

/* **
 * Adds a direction to a tile
 */
int32 UMazeGeneratorFunctionLibrary::AddDirection(const int32 TileId, const EMazeDirection Direction)
{
	switch (Direction)
	{
		case EMazeDirection::North:
			return TileId | 1;
		case EMazeDirection::East:
			return TileId | 2;
		case EMazeDirection::South:
			return TileId | 4;
		case EMazeDirection::West:
			return TileId | 8;
		default:
			return TileId;
	}
}

/* **
 * Checks if a tile has a direction opened
 */
bool UMazeGeneratorFunctionLibrary::HasDirection(const int32 TileId, EMazeDirection const Direction)
{
	switch (Direction)
	{
		case EMazeDirection::North:
			return (TileId & 1) == 1;
		case EMazeDirection::East:
			return (TileId & 2) == 2;
		case EMazeDirection::South:
			return (TileId & 4) == 4;
		case EMazeDirection::West:
			return (TileId & 8) == 8;
		default:
			return false;
	}
}
