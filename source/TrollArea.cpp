#include "TrollArea.h"

extern "C" {
    void OSShutdownSystem();
    void OSReturnToMenu();
    GameScene *getGameScene();
    double __kernel_sin(double x, double y, int iy);
    double __kernel_cos(double x, double y);
    int __ieee754_rem_pio2(double x, double *y);
}

#define bus_read(addr)           (*(volatile u32 *)(addr))
#define bus_write(addr, val) (*(volatile u32 *)(addr)) = (val)

s32 gScreenWidth = -1;
s32 gAmountOfDamage = -1;
bool gIsSwapSinAndCos = false;

// ------------------------------

TrollArea::TrollArea (const char *pName) : AreaObj(pName) {
    mHasRun = false;
}

TrollArea::~TrollArea () {
    
}

void TrollArea::init (const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
    MR::getJMapInfoArg0WithInit(rIter, &mTrollMode);
    MR::getJMapInfoArg7NoInit(rIter, &mRunOnce);
}

void TrollArea::movement () {
    if (isInVolume(*MR::getPlayerPos())) {
        if ((!mRunOnce || mRunOnce && !mHasRun)) {
            switch (mTrollMode) {
                case 0: // Crash the game
                    crashGame();
                    break;
                case 1: // Shut down the Wii
                    OSShutdownSystem();
                    break;
                case 2: // Return to the Home Menu
                    OSReturnToMenu();
                    break;
                case 3: // Rumble Wiimote very strong
                    MR::tryRumblePadVeryStrong(MarioAccess::getPlayerActor(), 0);
                    break;
                case 4: // Play random sound
                    MR::startSystemSE(getRandomSoundId(), -1, -1);
                    break;
                case 5: // Stop rendering the game
                    GameScene *pGameScene = getGameScene();
                    pGameScene->mDraw3D = false;
                    break;
                case 6: // Teleport player to a random position
                    TVec3f randomVec;
                    if (mObjArg1 == -1) 
                        mObjArg1 = 10000.0f;
                    MR::getRandomVector(&randomVec, mObjArg1);
                    MR::getPlayerPos()->set(randomVec);
                    break;
                case 7: // Teleport a random object to the player's position
                    AllLiveActorGroup *pAllGroup = MR::getAllLiveActorGroup();
                    int index = MR::getRandom(0, pAllGroup->getLivingActorNum());
                    LiveActor *pRandomActor = pAllGroup->getActor(index);
                    pRandomActor->mTranslation = *MR::getPlayerPos();
                    break;
                case 8: // Change screen width
                    gScreenWidth = mObjArg1;
                    break;
                case 9: // Change framerate [by: VTXG]
                    f32 framerate = (mObjArg1 < 0 ? MR::getRandom(0.01f, 3.0f) : 60.0f / mObjArg1);
                    MainLoopFramework *pMainLoop = MainLoopFramework::sManager;
                    pMainLoop->mTickDuration = pMainLoop->frameToTick(framerate);
                    break;
                case 10: // Bitcoin ransom message
                    GXColor bg;
                    bg.r = (mObjArg1 < 0 ? 0 : mObjArg1);
                    bg.g = (mObjArg2 < 0 ? 0 : mObjArg2);
                    bg.b = (mObjArg3 < 0 ? 0 : mObjArg3);
                    bg.a = 0;
		            GXColor fg;
                    fg.r = (mObjArg4 < 0 ? 255 : mObjArg4);
                    fg.g = (mObjArg5 < 0 ? 0 : mObjArg5);
                    fg.b = (mObjArg6 < 0 ? 0 : mObjArg6);
                    fg.a = 0;
		            OSFatal(fg, bg, "Nintendo? has found suspicious modding activity happening on this Nintendo? Wii? with Super Mario? Galaxy 2?. In order to prevent modding activity, this Wii? will be shut down and bricked forever unless you pay\n\n0.1564845484 BTC\n\nto this wallet address:\n\n0xDEADBEEF\n\nSincerely,\nYour mother- I mean Nintendo?.");
                    break;
                case 11: // Fake Game Over
                    MR::startGameOverWipe();
                    break;
                case 12: // Battery warning
                    GameSystem *pGameSystem = SingletonHolder<GameSystem>::sInstance;
                    pGameSystem->mErrorWatcher->mBatteryLayout->mWinBatteryInfo1->exeCriticalAppear();
                    pGameSystem->mErrorWatcher->mBatteryLayout->mWinBatteryInfo2->exeCriticalAppear();
                    break;
                case 13: // sin and cos swap
                    gIsSwapSinAndCos = !gIsSwapSinAndCos;
                    break;
                case 14: // Delete Save file(s)
                    SaveDataHandleSequence *pSequence = GameSequenceFunction::getSaveDataHandleSequence();
                    if (mObjArg1 == -1) {
                        pSequence->startDeleteUserFile(1);
                        pSequence->startDeleteUserFile(2);
                        pSequence->startDeleteUserFile(3);
                    } else {
                        pSequence->startDeleteUserFile(mObjArg1);
                    }
                    break;
                case 15: // Set amount of Damage
                    gAmountOfDamage = mObjArg1;
                    break;
                case 16: // Eject game disc [by: shibboleet]
                    u32 reg;
                    reg = bus_read(0xCD8000C0);
                    reg = reg | 0x00000200;
                    bus_write(0xCD8000C0, reg);
                    break;
                case 17: // Set player scale
                    TVec3f playerScale;
                    if (mObjArg1 == -1 && mObjArg2 == -1 && mObjArg3 == -1) {
                        MR::getRandomVector(&playerScale, 10.0f);
                    } else {
                        playerScale.set(mObjArg1, mObjArg2, mObjArg3);
                    }
                    MarioAccess::getPlayerActor()->mScale.set(playerScale);
                    break;
            }
            mHasRun = true;
        }
    } else {
        mHasRun = false;
    }
}

JAISoundID getRandomSoundId () {
    s32 randomSoundID;
    switch (MR::getRandom(0L, 0xDL)) {
        case 0:
            randomSoundID = MR::getRandom(0L, 0x191L);
            break;
        case 1:
            randomSoundID = MR::getRandom(0x10000L, 0x10084L);
            break;
        case 2:
            randomSoundID = MR::getRandom(0x20000L, 0x200CEL);
            break;
        case 3:
            randomSoundID = MR::getRandom(0x30000L, 0x30152L);
            break;
        case 4:
            randomSoundID = MR::getRandom(0x40000L, 0x40387L);
            break;
        case 5:
            randomSoundID = MR::getRandom(0x50000L, 0x505BAL);
            break;
        case 6:
            randomSoundID = MR::getRandom(0x60000L, 0x60089L);
            break;
        case 7:
            randomSoundID = MR::getRandom(0x70000L, 0x7007CL);
            break;
        case 8:
            randomSoundID = MR::getRandom(0x80000L, 0x8008DL);
            break;
        case 9:
            randomSoundID = MR::getRandom(0x90000L, 0x90314L);
            break;
        case 0xA:
            randomSoundID = MR::getRandom(0xA0000L, 0xA01B8L);
            break;
        case 0xB:
            randomSoundID = MR::getRandom(0xB0000L, 0xB0188L);
            break;
        case 0xC:
            randomSoundID = MR::getRandom(0xC0000L, 0xC0016L);
            break;
        case 0xD:
            randomSoundID = MR::getRandom(0xD0000L, 0xD0015L);
            break;
    }
    JAISoundID id = JAISoundID(randomSoundID);
    return id;
}

asm void crashGame () {
    lis r12, 0xDEAD
    ori r12, r12, 0xBEEF
    mtctr r12
    bctrl
    blr
}

s32 newScreenWidth () {
    s32 screenWidth = gScreenWidth;
    if (screenWidth == -1)
        screenWidth = (MR::isScreen16Per9() ? 812 : 608);
    return screenWidth;
}
kmBranch(0x80058560, newScreenWidth);

s32 changeDecLife (s32 originalHealth) {
    return originalHealth + gAmountOfDamage;
}
kmCall(0x803CA248, changeDecLife);
kmWrite32(0x803CA24C, 0x907F06B0);

// sin and cos

double sin(double x) {
    double y[2], z = 0.0;
    int n, ix;

    ix = *(int*)&x;
    ix &= 0x7FFFFFFF;

    if (ix <= 0x3FE921FB) {
        return __kernel_sin(x, z, 0);
    }
    else if (ix >= 0x7FF00000) {
        return x - x;
    }
    else {
        n = __ieee754_rem_pio2(x, y);

        switch (n & 3) {
            case 0:
                return __kernel_sin(y[0], y[1], 1);
            case 1:
                return __kernel_cos(y[0], y[1]);
            case 2:
                return -__kernel_sin(y[0], y[1], 1);
            default:
                return -__kernel_cos(y[0], y[1]);
        }
    }
}

double cos(double x) {
    double y[2], z = 0.0;
    int n, ix;

    ix = *(int*)&x;
    ix &= 0x7FFFFFFF;

    if (ix <= 0x3FE921FB) {
        return __kernel_cos(x, z);
    }
    else if (ix >= 0x7FF00000) {
        return x - x;
    }
    else {
        n = __ieee754_rem_pio2(x, y);

        switch (n & 3) {
            case 0:
                return __kernel_cos(y[0], y[1]);
            case 1:
                return -__kernel_sin(y[0], y[1], 1);
            case 2:
                return -__kernel_cos(y[0], y[1]);
            default:
                return __kernel_sin(y[0], y[1], 1);
        }
    }
}

double newSin(double x) {
    if (gIsSwapSinAndCos) {
        return cos(x);
    }
    return sin(x);
}
kmBranch(0x8063DC98, newSin);

double newCos(double x) {
    if (gIsSwapSinAndCos) {
        return sin(x);
    }
    return cos(x);
}
kmBranch(0x8063D88C, newCos);