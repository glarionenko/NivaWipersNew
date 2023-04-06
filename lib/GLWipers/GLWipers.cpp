#include "GLWipers.h"

void GLWipers::init(int wipersInPin, int washerInPin, int wipersRelayOutPin, int washerRelayOutPin, int maxWashingTime, int secondModePause, int thirdModePause)
{
    _secondModePause = secondModePause;
    _thirdModePause = thirdModePause;
    _wipersInPin = wipersInPin;
    _washerInPin = washerInPin;
    _wipersRelayOutPin = wipersRelayOutPin;
    _washerRelayOutPin = washerRelayOutPin;
    _maxWashingTime = maxWashingTime;
    pinMode(_washerInPin, INPUT_PULLUP);
    pinMode(_wipersInPin, INPUT_PULLUP);
    pinMode(_wipersRelayOutPin, OUTPUT);
    pinMode(_washerRelayOutPin, OUTPUT);
    _lastMode = getSelectedMode();
    _wipersPulseSizeMillis = 1000;
}

void GLWipers::loop()
{
    if (isStateChangedAndSetWipersMode())
    {
#ifdef DEBUG
        Serial.print("_currentMode - ");
        Serial.println(_currentMode);
#endif
        if (_isWipersEnabled)
        {

            if (_currentMode != WIPERS_CONTINUOUS_MODE)
            {

                setTaskForOneWipe();
                setTimerForNextWipes();
            }
            else
            {
                stopTimerForNextWipes();
                continuousWipingStart();
            }
        }
        else
        {
            stopTimerForNextWipes();
            stopWiping();
        }
    }

    // continuous wiper task check
    multipleWipesLoop();
    oneWipeLoop();
}

void GLWipers::setTimerForNextWipes()
{
    _nextWipesStartTimerTask = 1;
}
void GLWipers::stopTimerForNextWipes()
{
    _nextWipesStartTimerTask = 0;
    _executeNextWipe = 0;
}

void GLWipers::setTaskForOneWipe()
{
    _oneWipeStartTask = true;
}
void GLWipers::continuousWipingStart()
{
    wipersEnablerDisabler(true);
}
void GLWipers::stopWiping()
{
    wipersEnablerDisabler(false);
}

boolean GLWipers::isTimePassed(unsigned long startedAt, int timeInterval)
{
    boolean passed = 0;
    if ((millis() - startedAt) >= timeInterval)
    {
        passed = 1;
    }
    return passed;
}
void GLWipers::multipleWipesLoop()
{
    if (_nextWipesStartTimerTask)
    { // запустить таймер
        _timeOfMultipleWipesTimerStarted = millis();
        _nextWipesStartTimerTask = 0;
        _executeNextWipe = 1;
    }
    if (_executeNextWipe)
    { // запустить 1 взмах по истечении таймера
        if (isTimePassed(_timeOfMultipleWipesTimerStarted, _currentPause))
        {
            setTaskForOneWipe();
            _nextWipesStartTimerTask = 1;
        }
    }
}

void GLWipers::oneWipeLoop()
{
    if (_oneWipeStartTask)
    {
        wipersEnablerDisabler(true);
        _timeOfOneWipeStarted = millis();
        _oneWipeStartTask = 0;
        _oneWipeStopTask = 1;
    }
    if (_oneWipeStopTask)
    {
        if (!_oneWipeStartTask && isTimePassed(_timeOfOneWipeStarted, _wipersPulseSizeMillis))
        {
            wipersEnablerDisabler(false);
            _oneWipeStopTask = 0;
        }
    }
}
void GLWipers::setMode(int remoteMode){
    _currentRemoteMode = remoteMode;

}
boolean GLWipers::isStateChangedAndSetWipersMode()
{
    boolean changed = 0;
    int mode = getSelectedMode();
    int settedRemotelyMode = _currentRemoteMode;

    if (_lastMode != mode)
    {
        wipersSetMode(mode);
        _lastMode = mode;
        changed = 1;
    }
    else if(_lastRemoteMode != settedRemotelyMode)
    {
        wipersSetMode(settedRemotelyMode);
        _lastRemoteMode = settedRemotelyMode;
        changed = 1;
    }
    else
    {
        changed = 0;
    }
    return changed;
}

int GLWipers::getSelectedMode()
{
    int selectorMode = analogRead(_wipersInPin);
    int mode = WIPERS_STOP_MODE;
    if (selectorMode > 1000)
    {
        mode = WIPERS_STOP_MODE;
        return mode;
    }
    if (selectorMode > 25 && selectorMode <= 40)
    {
        mode = WIPERS_FIRST_MODE;
        return mode;
    }
    if (selectorMode > 20 && selectorMode <= 25)
    {
        mode = WIPERS_SECOND_MODE;
        return mode;
    }
    if (selectorMode > 10 && selectorMode <= 20)
    {
        mode = WIPERS_CONTINUOUS_MODE;
        return mode;
    }
    return mode;
}

void GLWipers::wipersEnablerDisabler(boolean enable)
{
    if (!enable)
    {
        digitalWrite(_wipersRelayOutPin, LOW);
    }
    if (enable)
    {
        digitalWrite(_wipersRelayOutPin, HIGH);
    }
}
int GLWipers::getCurrentMode(){
    return _currentMode;
}
void GLWipers::wipersSetMode(int mode)
{
    _currentMode = mode;
    switch (mode)
    {
    case WIPERS_STOP_MODE:
        _isWipersEnabled = 0;
        break;
    case WIPERS_FIRST_MODE:
        _isWipersEnabled = 1;
        _currentPause = _secondModePause;
        break;
    case WIPERS_SECOND_MODE:
        _isWipersEnabled = 1;
        _currentPause = _thirdModePause;
        break;
    case WIPERS_CONTINUOUS_MODE:
        _isWipersEnabled = 1;
        _currentPause = 0;
        break;
    }
}