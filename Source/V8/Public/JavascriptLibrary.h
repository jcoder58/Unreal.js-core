#pragma once

#include "JavascriptProfile.h"
#include "JavascriptIsolate.h"
#include "AI/Navigation/RecastNavMesh.h"
#include "JavascriptLibrary.generated.h"

USTRUCT(BlueprintType)
struct V8_API FDirectoryItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Scripting | Javascript")
	FString Name;

	UPROPERTY(BlueprintReadOnly, Category = "Scripting | Javascript")
	bool bIsDirectory;
};

UENUM()
enum class ELogVerbosity_JS : uint8
{
	/** Not used */
	NoLogging,

	/** Always prints s fatal error to console (and log file) and crashes (even if logging is disabled) */
	Fatal,

	/**
	* Prints an error to console (and log file).
	* Commandlets and the editor collect and report errors. Error messages result in commandlet failure.
	*/
	Error,

	/**
	* Prints a warning to console (and log file).
	* Commandlets and the editor collect and report warnings. Warnings can be treated as an error.
	*/
	Warning,

	/** Prints a message to console (and log file) */
	Display,

	/** Prints a message to a log file (does not print to console) */
	Log,

	/**
	* Prints a verbose message to a log file (if Verbose logging is enabled for the given category,
	* usually used for detailed logging)
	*/
	Verbose,

	/**
	* Prints a verbose message to a log file (if VeryVerbose logging is enabled,
	* usually used for detailed logging that would otherwise spam output)
	*/
	VeryVerbose
};

USTRUCT()
struct FJavascriptLogCategory
{
	GENERATED_USTRUCT_BODY()

#if !NO_LOGGING
	TSharedPtr<FLogCategoryBase> Handle;
#endif
};

USTRUCT()
struct FJavascriptStreamableManager
{
	GENERATED_USTRUCT_BODY()

	FStreamableManager* operator -> () const
	{
		return Handle.Get();
	}

	TSharedPtr<FStreamableManager> Handle;
};

UCLASS()
class V8_API UJavascriptLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static FJavascriptStreamableManager CreateStreamableManager();

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static void SimpleAsyncLoad(const FJavascriptStreamableManager& Manager, FStringAssetReference const& Target, int32 Priority);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static void RequestAsyncLoad(const FJavascriptStreamableManager& Manager, const TArray<FStringAssetReference>& TargetsToStream, FJavascriptFunction DelegateToCall, int32 Priority);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static void V8_SetFlagsFromString(const FString& V8Flags);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static void V8_SetIdleTaskBudget(float BudgetInSeconds);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static void Unload(const FJavascriptStreamableManager& Manager, FStringAssetReference const& Target);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static bool IsAsyncLoadComplete(const FJavascriptStreamableManager& Manager, FStringAssetReference const& Target);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static FJavascriptLogCategory CreateLogCategory(const FString& CategoryName, ELogVerbosity_JS InDefaultVerbosity);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static void Log(const FJavascriptLogCategory& Category, ELogVerbosity_JS Verbosity, const FString& Message, const FString& FileName, int32 LineNumber);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	bool IsSuppressed(const FJavascriptLogCategory& Category, ELogVerbosity_JS Verbosity);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	FName GetCategoryName(const FJavascriptLogCategory& Category);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static void SetMobile(USceneComponent* SceneComponent);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static void SetMobility(USceneComponent* SceneComponent, EComponentMobility::Type Type);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static UWorld* Actor_GetWorld(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static UClass* GetBlueprintGeneratedClass(UBlueprint* Blueprint);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static UClass* GetBlueprintGeneratedClassFromPath(FString Path);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static UObject* GetOuter(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static UObject* GetOutermost(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static UObject* Duplicate(UObject* Object, UObject* Outer, FName Name);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static bool HasAnyFlags(UObject* Object, int32 Flags);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static bool HasAnyPackageFlags(UPackage* Package, int32 Flags);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static FString GetName(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static bool IsPlayInEditor(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static bool IsPlayInPreview(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static bool IsGameWorld(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static void SetClientTravel(UEngine* Engine, UWorld *InWorld, FString NextURL, ETravelType InTravelType);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static UPackage* CreatePackage(UObject* Outer, FString PackageName);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static UPackage* FindPackage(UObject* InOuter, FString PackageName);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static UPackage* LoadPackage(UPackage* InOuter, FString PackageName);
	
	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static void AddDynamicBinding(UClass* Outer, UDynamicBlueprintBinding* BindingObject);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static UDynamicBlueprintBinding* GetDynamicBinding(UClass* Outer, TSubclassOf<UDynamicBlueprintBinding> BindingObjectClass);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static void HandleSeamlessTravelPlayer(AGameMode* GameMode, AController*& C);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static void SetRootComponent(AActor* Actor, USceneComponent* Component);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static int32 GetFileSize(UObject* Object, FString Filename);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static bool ReadFile(UObject* Object, FString Filename);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static bool WriteFile(UObject* Object, FString Filename);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static FString ReadStringFromFile(UObject* Object, FString Filename);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static bool WriteStringToFile(UObject* Object, FString Filename, const FString& Data);

	UFUNCTION(BlueprintCallable, Category = "Scripting|Javascript")
	static FString GetDir(UObject* Object, FString WhichDir);
		
	UFUNCTION(BlueprintCallable, Category = "Javascript | Editor")
	static bool HasUndo(UEngine* Engine);

	UFUNCTION(BlueprintCallable, Category = "Javascript | Editor")
	static bool ReadDirectory(UObject* Object, FString Directory, TArray<FDirectoryItem>& OutItems);

	UFUNCTION(BlueprintCallable, Category = "Javascript | Editor")
	static void ReregisterComponent(UActorComponent* ActorComponent);

	UFUNCTION(BlueprintCallable, Category = "Javascript | Editor")
	static void RegisterComponent(UActorComponent* ActorComponent);

	UFUNCTION(BlueprintCallable, Category = "Javascript | Editor")
	static void UnregisterComponent(UActorComponent* ActorComponent);

	UFUNCTION(BlueprintCallable, Category = "Javascript | Editor")
	static bool IsRegistered(UActorComponent* ActorComponent);

	UFUNCTION(BlueprintCallable, Category = "Javascript | Editor")
	static void GetAllActorsOfClassAndTags(UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, const TArray<FName>& Tags_Accept, const TArray<FName>& Tags_Deny, TArray<AActor*>& OutActors);

	UFUNCTION(BlueprintCallable, Category = "Javascript | Editor")
	static int32 GetCurrentProcessId();

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static UModel* GetModel(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static ULevel* GetLevel(AActor* Actor);	

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static TArray<ULevel*> GetLevels(UWorld* World);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static FString GetFunctionName(FJavascriptProfileNode Node);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static int32 GetScriptId(FJavascriptProfileNode Node);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static FString GetScriptResourceName(FJavascriptProfileNode Node);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static int32 GetLineNumber(FJavascriptProfileNode Node);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static int32 GetColumnNumber(FJavascriptProfileNode Node);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static int32 GetHitLineCount(FJavascriptProfileNode Node);


	//static bool GetLineTicks(LineTick* entries, unsigned int length) const;
	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static FString GetBailoutReason(FJavascriptProfileNode Node);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static int32 GetHitCount(FJavascriptProfileNode Node);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static int32 GetCallUid(FJavascriptProfileNode Node);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static int32 GetNodeId(FJavascriptProfileNode Node);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static int32 GetChildrenCount(FJavascriptProfileNode Node);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static FJavascriptProfileNode GetChild(FJavascriptProfileNode Node,int32 index);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static int32 GetDeoptInfosCount(FJavascriptProfileNode Node, int32 index);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static FString GetDeoptInfo_Reason(FJavascriptProfileNode Node, int32 index);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static FString GetDeoptInfo_Stack(FJavascriptProfileNode Node, int32 index);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static FString GetArchetypePathName(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static FString GetClassPathName(UClass* Class);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static void SetObjectFlags(UObject* Obj, int32 Flags);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static float GetLastRenderTime(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static UEnum* CreateEnum(UObject* Outer, FName Name, TArray<FName> DisplayNames);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static void MarkRenderStateDirty(UActorComponent* Component);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static void ReregisterAllComponents(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static bool SegmentIntersection2D(const FVector& SegmentStartA, const FVector& SegmentEndA, const FVector& SegmentStartB, const FVector& SegmentEndB, FVector& IntersectionPoint);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static bool FileExists(const FString& Filename);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static bool DirectoryExists(const FString& InDirectory);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static bool MakeDirectory(const FString& Path, bool Tree);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static bool DeleteDirectory(const FString& Path, bool RequireExists, bool Tree);
	
	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static void GetObjectsWithOuter(const class UObject* Outer, TArray<UObject *>& Results, bool bIncludeNestedObjects, int32 ExclusionFlags, int32 ExclusionInternalFlags);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static class UObject* FindObjectWithOuter(class UObject* Outer, class UClass* ClassToLookFor, FName NameToLookFor);
	
	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static void GetObjectsOfClass(UClass* ClassToLookFor, TArray<UObject *>& Results, bool bIncludeDerivedClasses, int32 ExcludeFlags, int32 ExclusionInternalFlags);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static void GetDerivedClasses(UClass* ClassToLookFor, TArray<UClass *>& Results, bool bRecursive);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static FString GetPlatformName();

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static UObject* TryLoadByPath(FString Path);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static void GenerateNavigation(UWorld* world, ARecastNavMesh* NavData);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static const FString& GetMetaData(UField* Field, const FString Key);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static TArray<UField*> GetFields(const UObject* Object, bool bIncludeSuper);

	UFUNCTION(BlueprintCallable, Category = "Scripting | Javascript")
	static int32 GetFunctionParmsSize(UFunction* Function);
};
