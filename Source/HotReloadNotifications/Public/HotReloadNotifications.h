// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Misc/CompilationResult.h"

class FHotReloadNotificationsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void OnCompileFinished(const FString& LogDump, ECompilationResult::Type CompilationResult, bool bShowLog) const;

};
