#include "game/gameanimation/animationskipper.h"
#include "coreengine/settings.h"
#include "coreengine/interpreter.h"

AnimationSkipper::AnimationSkipper()
{
    Interpreter::setCppOwnerShip(this);
}

void AnimationSkipper::startSeeking()
{
    storeAnimationSettings();
    disableAllAnimations();
}

void AnimationSkipper::disableAllAnimations()
{
    Settings* pSettings = Settings::getInstance();
    pSettings->setOverworldAnimations(false);
    pSettings->setBattleAnimationMode(GameEnums::BattleAnimationMode::BattleAnimationMode_None);
    pSettings->setDialogAnimation(false);
    pSettings->setCaptureAnimation(false);
    pSettings->setMovementAnimations(false);
    pSettings->setDay2dayScreen(false);
    pSettings->setAnimationSpeed(100);
    pSettings->setBattleAnimationSpeed(100);
    pSettings->setDialogAnimationSpeed(100);
    pSettings->setCaptureAnimationSpeed(100);
}

void AnimationSkipper::storeAnimationSettings()
{
    Settings* pSettings = Settings::getInstance();
    // store animation modes
    m_storedOverworldAnimations = pSettings->getOverworldAnimations();
    m_storedBattleAnimMode = pSettings->getBattleAnimationMode();
    m_storedBatteAnimType = pSettings->getBattleAnimationType();
    m_storedDialog = pSettings->getDialogAnimation();
    m_storedCaptureAnimation = pSettings->getCaptureAnimation();
    m_storedMovementAnimation = pSettings->getMovementAnimations();
    m_storedDay2DayAnimation = pSettings->getDay2dayScreen();
    m_storedAnimationSpeed = pSettings->getAnimationSpeedValue();
    m_storedBattleAnimationSpeed = pSettings->getBattleAnimationSpeedValue();
    m_storedDialogAnimationSpeed = pSettings->getDialogAnimationSpeedValue();
    m_storedCaptureAnimationSpeed = pSettings->getCaptureAnimationSpeedValue();
}

void AnimationSkipper::restoreAnimationSettings()
{
    Settings* pSettings = Settings::getInstance();
    pSettings->setOverworldAnimations(m_storedOverworldAnimations);
    pSettings->setBattleAnimationMode(m_storedBattleAnimMode);
    pSettings->setBattleAnimationType(m_storedBatteAnimType);
    pSettings->setDialogAnimation(m_storedDialog);
    pSettings->setCaptureAnimation(m_storedCaptureAnimation);
    pSettings->setMovementAnimations(m_storedMovementAnimation);
    pSettings->setDay2dayScreen(m_storedDay2DayAnimation);
    pSettings->setAnimationSpeed(m_storedAnimationSpeed);
    pSettings->setBattleAnimationSpeed(m_storedBattleAnimationSpeed);
    pSettings->setDialogAnimationSpeed(m_storedDialogAnimationSpeed);
    pSettings->setCaptureAnimationSpeed(m_storedCaptureAnimationSpeed);
}
