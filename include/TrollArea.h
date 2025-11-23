#pragma once

#include "syati.h"
#include "Game/Scene/GameScene.h"

class TrollArea : public AreaObj {
public:
    TrollArea(const char *);
    ~TrollArea();
    virtual void init(const JMapInfoIter &);
    virtual void movement();
    
    s32 mTrollMode;
    bool mRunOnce;
    bool mHasRun;
};

void crashGame();
JAISoundID getRandomSoundId();