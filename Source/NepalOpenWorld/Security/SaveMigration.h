#pragma once
#include "CoreMinimal.h"
#define NOW_SAVE_VERSION 1
class UNOWSaveGame;

namespace NOWSaveMigration
{
    /** Versioned migration chain. Unknown/newer versions are rejected, never guessed. */
    inline bool MigrateIfNeeded(UNOWSaveGame *S);
}