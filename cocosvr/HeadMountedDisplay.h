//
//  HeadMountedDisplay.hpp
//  cocosVR
//
//  Created by Ricardo Quesada on 4/14/16.
//
//

#ifndef HeadMountedDisplay_hpp
#define HeadMountedDisplay_hpp

class CardboardDeviceParams;
class ScreenParams;
class UIScreenIgnore;

class HeadMountedDisplay
{
public:
    HeadMountedDisplay(UIScreenIgnore *screen);
    HeadMountedDisplay(HeadMountedDisplay *hmd);
    ~HeadMountedDisplay();

    void setScreen(ScreenParams* screen);
    ScreenParams *getScreen();

    void setCardboard(CardboardDeviceParams *cardboard);
    CardboardDeviceParams *getCardboard();

    bool equals(HeadMountedDisplay *other);

private:
    ScreenParams *_screen;
    CardboardDeviceParams *_cardboard;
};


#endif /* HeadMountedDisplay_hpp */
