#ifndef VX_BASE_H
#define  VX_BASE_H

/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $

   Date: $Date: 2005/02/03 22:45:46 $

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

#include "Vx/Vx.h"

namespace Vx
{

#define _VXINSTANCE_CHECK

#if defined(_MECHECK) && defined(_VXINSTANCE_CHECK)
#define MAXINSTANCE 1000
#endif

class VxNameMap;

/**
 Base class for most Vx objects.
 Handles user data and object naming.
 */
class VX_DLLSPEC VxBase 
{
public:
    VxBase();
    virtual ~VxBase();

    /** Sets the user data pointer. */
    inline void setUserData(void* userData) {mUserData = userData;}
    /** Retrieves the user data pointer. */
    inline void* getUserData() const {return mUserData;}
    /** Sets the user id. */
    inline void setUserID(int userID) {mUserID = userID;}
    /** Retrieves the user id. */
    inline int getUserID() const {return mUserID;}

    /** @internal Sets the internal data pointer. */
    inline void setInternalData(void* userData) {mInternalData = userData;}
    /** @internal Retrieves the internal data pointer. */
    inline void* getInternalData() const {return mInternalData;}
    /** @internal Sets the internal ID. */
    inline void setInternalID(int InternalID) {mInternalID = InternalID;}
    /** @internal Retrieves the Internal id. */
    inline int getInternalID() const {return mInternalID;}

    /** Returns the class name */
    virtual const char* getClassName() const = 0;

    #if defined(_MECHECK) && defined(_VXINSTANCE_CHECK)
    static int getInstanceCounter();
    #endif

    /** Sets the name. */
    virtual void setName(const char *name) { setNameBase(name); }
    void setCombinedName(const char* base, const char *name, int i);
    void setCombinedName(const char* base, const char *name);
    /** Retrieves the name. */
    inline const char * getName() const{return mName;}

protected:
        void setNameBase(const char *name, VxNameMap** map=0);
        static VxBase* findFromName(const char* name, VxNameMap* map);
private:
        char *mName;
        void *mUserData;
        void *mInternalData;
        int mUserID;
        int mInternalID;

    #if defined(_MECHECK) && defined(_VXINSTANCE_CHECK)
        static int sInstanceCounter;
        static VxBase* sInstanceList[MAXINSTANCE];
    #endif
};


class VX_DLLSPEC VxRefCount
{
public:

    VxRefCount() : mCounter(0) {}

    /** Returns the reference count */
    int getref() const { return mCounter; }
    /** Increment the reference count */
    virtual void ref() { mCounter++; }
    /** Decrement the reference count */
    virtual void unref() 
    { 
        mCounter--; 
        if (mCounter<=0) 
            delete this; 
    }
    void buildInPlace() { mCounter = INT_MAX/2; } 

protected:
    virtual ~VxRefCount() {}
private:
    unsigned int mCounter;
};


/**
 Base class for reference counted Vx objects.
 */
class VX_DLLSPEC VxRefCountBase : public VxBase, public VxRefCount
{
public:

    VxRefCountBase() {}

protected:
    virtual ~VxRefCountBase() {}
};

char* VxStrDup(const char* str);
void  VxStrDel(char* str);

} // namespace Vx

#endif // VX_BASE_H
