#if !defined(VX_SCRIPT_H)
#define VX_SCRIPT_H

/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc. 
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2005/03/30 08:52:28 $
   
   This software and its accompanying manuals have been developed by CMLabs
   Simulations Inc. ("CMLabs") and MathEngine PLC ("MathEngine").  The
   copyright to the Vortex Toolkits and all associated materials belongs to
   CMLabs.
   
   All intellectual property rights in the software belong to CMLabs,
   except for portions that have been licensed from MathEngine.

   All rights conferred by law (including rights under international
   copyright conventions) are reserved to CMLabs. This software may also
   incorporate information which is confidential to CMLabs.

   Save to the extent permitted by law, or as otherwise expressly permitted
   by CMLabs, this software and the manuals must not be copied (in whole or
   in part), re-arranged, altered or adapted in any way without the prior
   written consent of CMLabs. In addition, the information contained in the
   software may not be disseminated without the prior written consent of
   CMLabs.

 */

// VxLua.h: interface for Lua scripting.
//
//////////////////////////////////////////////////////////////////////


// tolua_begin

struct lua_State;
struct VxScriptTimedCommandList;

namespace Vx
{
    class VxUniverse;
    class VxBase;
}

/** Lua scripting interface.
Allows the user to load scripts and run timed commands.
*/
class  VxScript
{
public:

    typedef int (*registerFunction)(lua_State* );
    VxScript();
    VxScript(int (*initfn)(lua_State* ));
    VxScript(int (**initfn)(lua_State* ));

    virtual ~VxScript();
    
    bool executeFile(const char *filename);
    bool executeString(const char *string);
    bool getVariable(const char* inName, int *outInt);
    bool getVariable(const char* inName, float *outReal);
    bool getVariable(const char* inName, double *outReal);
    bool getVariable(const char* inName, char *outString);
    
    bool setVariable(const char* inName, int inInt);
    bool setVariable(const char* inName, double inReal);
    bool setVariable(const char* inName, const char* inString);

    bool getMemberVariable(const char* inName, const char* inMember, char *outString);

    void* getVxObject(const char* inName, const char* inType = 0);

    bool exportVxObject(Vx::VxBase* obj, const char* inObjectAndVariableName = 0);

    bool exportVxObjects(Vx::VxUniverse* universe); // TODO

    bool loadTimedScriptFile(const char *filename);
    bool executeTimed(double absoluteTimeInSeconds);
    bool executeFrameCB(double absoluteTimeInSeconds, const char* cbName);
    bool addTimed(double time, const char* command);

    bool interactive(const char* progname);

    void setDebugLevel(int verbose) { mDebug = verbose; }

    int addPath(const char* scriptfilepath);
        
    class timeItem
    {
    public:
        timeItem() : time(0), done(0), scriptline(0) {}
        ~timeItem();
        double time;
        char *scriptline;
        int done;
    };

private:

    static void openStdLibs(lua_State*l);
    
    struct lua_State *mLuaState;
    VxScriptTimedCommandList* mTimedCommands;

    // for timed script loading:
    static const char *getTimedFromFile(lua_State *L, void *ud, size_t *size);
    void* mFile;
    bool mIsTimed;
    double mCurrTime;

    int mDebug;

    char *mPath;

}; // tolua_export

#endif //VX_SCRIPT_H 
