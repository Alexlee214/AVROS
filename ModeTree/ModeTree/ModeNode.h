//
//  ModeNode.h
//  ModeNode
//
//  Created by Alex Lee on 2018-05-31.
//  Copyright © 2018 Alex Lee. All rights reserved.
//

#ifndef ModeNode_h
#define ModeNode_h


#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#endif /* ModeNode_h */


class ModeNode{
    private:
        modeNode* prevNode;
    
    public:
        ModeNode();
        //returns the id numberof the current mode
        short curModeID();
    
        //returns the id number of the previous mode
        short prevModeID();
    
        //set the cur mode to the previous mode
        void prevMode();
        short modeID;
}
