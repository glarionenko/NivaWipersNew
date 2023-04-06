#include <Arduino.h>
class GLWipers
{
public:
    void init(int wipersInPin, int washerInPin, int wipersRelayOutPin, int washerRelayOutPin, int maxWashingTime, int secondModePause, int thirdModePause);
    void loop();
    int getCurrentMode();
    void setMode(int remoteMode);
    

private:
    enum Wiper_modes
    {
        WIPERS_STOP_MODE = 1,
        WIPERS_FIRST_MODE = 2,
        WIPERS_SECOND_MODE = 3,
        WIPERS_CONTINUOUS_MODE = 4 //without pauses
    };
    unsigned long _timeOfOneWipeStarted = 0;
    int _maxWashingTime = 0;
    int _washerRelayOutPin = 0;
    int _wipersRelayOutPin = 0;
    int _wipersInPin = 0;
    int _washerInPin = 0;
    int _currentMode = 0;
    int _lastMode = 0;
    int _currentRemoteMode = 0;
    int _lastRemoteMode = 0;
    int _wipersPulseSizeMillis = 0;
    boolean _isWipersEnabled = 0;
    boolean _wipersTask = 0;
    boolean _washerTask = 0;
    boolean _oneWipeStartTask = 0;
    boolean _oneWipeStopTask = 0;
    boolean _nextWipesStartTimerTask = 0;
    boolean _executeNextWipe = 0;
    unsigned long _timeOfMultipleWipesTimerStarted = 0;
    boolean _commandToEnableWasher = 0;
    ;
    int _currentPause = 0;
    int _secondModePause = 0;
    int _thirdModePause = 0;

    boolean isTimePassed(unsigned long startedAt, int timeInterval);
    void multipleWipesLoop();
    void oneWipeLoop();
    boolean isStateChangedAndSetWipersMode();
    int getSelectedMode();
    void wipersEnablerDisabler(boolean enable);
    void wipersSetMode(int mode);
    void setTimerForNextWipes();
    void stopTimerForNextWipes();
    void setTaskForOneWipe();
    void continuousWipingStart();
    void stopWiping();
};