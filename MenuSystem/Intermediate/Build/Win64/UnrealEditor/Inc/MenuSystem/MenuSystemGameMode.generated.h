// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef MENUSYSTEM_MenuSystemGameMode_generated_h
#error "MenuSystemGameMode.generated.h already included, missing '#pragma once' in MenuSystemGameMode.h"
#endif
#define MENUSYSTEM_MenuSystemGameMode_generated_h

#define FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_12_SPARSE_DATA
#define FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_12_RPC_WRAPPERS
#define FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_12_RPC_WRAPPERS_NO_PURE_DECLS
#define FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAMenuSystemGameMode(); \
	friend struct Z_Construct_UClass_AMenuSystemGameMode_Statics; \
public: \
	DECLARE_CLASS(AMenuSystemGameMode, AGameModeBase, COMPILED_IN_FLAGS(0 | CLASS_Transient | CLASS_Config), CASTCLASS_None, TEXT("/Script/MenuSystem"), MENUSYSTEM_API) \
	DECLARE_SERIALIZER(AMenuSystemGameMode)


#define FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_12_INCLASS \
private: \
	static void StaticRegisterNativesAMenuSystemGameMode(); \
	friend struct Z_Construct_UClass_AMenuSystemGameMode_Statics; \
public: \
	DECLARE_CLASS(AMenuSystemGameMode, AGameModeBase, COMPILED_IN_FLAGS(0 | CLASS_Transient | CLASS_Config), CASTCLASS_None, TEXT("/Script/MenuSystem"), MENUSYSTEM_API) \
	DECLARE_SERIALIZER(AMenuSystemGameMode)


#define FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	MENUSYSTEM_API AMenuSystemGameMode(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AMenuSystemGameMode) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(MENUSYSTEM_API, AMenuSystemGameMode); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AMenuSystemGameMode); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	MENUSYSTEM_API AMenuSystemGameMode(AMenuSystemGameMode&&); \
	MENUSYSTEM_API AMenuSystemGameMode(const AMenuSystemGameMode&); \
public:


#define FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_12_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	MENUSYSTEM_API AMenuSystemGameMode(AMenuSystemGameMode&&); \
	MENUSYSTEM_API AMenuSystemGameMode(const AMenuSystemGameMode&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(MENUSYSTEM_API, AMenuSystemGameMode); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AMenuSystemGameMode); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AMenuSystemGameMode)


#define FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_9_PROLOG
#define FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_12_SPARSE_DATA \
	FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_12_RPC_WRAPPERS \
	FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_12_INCLASS \
	FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_12_SPARSE_DATA \
	FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_12_INCLASS_NO_PURE_DECLS \
	FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> MENUSYSTEM_API UClass* StaticClass<class AMenuSystemGameMode>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_MenuSystem_Source_MenuSystem_MenuSystemGameMode_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
