#ifndef _MCDUWTKCOMMON_H
#define _MCDUWTKCOMMON_H
/* -*- mode: C; -*- */

/*
   Copyright (c) 2000-2004 CMLabs Simulations Inc.
   
   http://www.cm-labs.com

   $Name:  $
   
   Date: $Date: 2004/03/25 16:50:41 $
   
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
 
#include "wt.h"          /* Standard WTK header files */
#include "MeVersion.h" 
#include "MeApp.h" 
 
extern int ShowTextCount;
extern char APP_NAME[];

#define  TEXT_BUFFER_SIZE 128
static int  ShowFrameRateCount;
static char ShowFrameRateBuf[TEXT_BUFFER_SIZE];

/* */
char *hlpMsg[] = {
	" - " ME_PRODUCT_NAME " (v" ME_VERSION_STRING ")",
	" - " ME_PRODUCT_NAME " (v" ME_VERSION_STRING ") [WTK]",
	ME_WEB_SITE,
	ME_COPYRIGHT_LINE1,
	"Press 'h' to show help text"
};


/* */
#ifdef WTKR9_COMPATIBLE
void drawfg( WTwindow *win, int eye) 
#else
void drawfg( WTwindow *win, FLAG eye) 
#endif
{  
  int x0, y0, w, h; 
  char buf[100];  

  WTwindow_getposition(win, &x0, &y0, &w, &h);   
  WTwindow_set2Dcolor(win, 240,240,240);
  WTwindow_set2Dfont(win, 0);     

  if (ShowTextCount<600) {
    sprintf(buf, "%s%s", APP_NAME, hlpMsg[0]);
    WTwindow_draw2Dtext(win,10.0f/(w+1),(1-20.0f/(h+1)),buf);  
    WTwindow_draw2Dtext(win,10.0f/(w+1),(1-40.0f/(h+1)),hlpMsg[2]);  
    WTwindow_draw2Dtext(win,10.0f/(w+1),(1-60.0f/(h+1)),hlpMsg[3]);  
    WTwindow_draw2Dtext(win,10.0f/(w+1),(1-80.0f/(h+1)),hlpMsg[4]);
    ShowTextCount++; 
  }

  WTwindow_draw2Dtext(win,10.0f/(w+1),10.0f/(h+1),"Powered by Vortex");  

  if (ShowFrameRateCount==0)
    sprintf(ShowFrameRateBuf, "%d fps", (int)(WTuniverse_framerate()+0.5f) );

  ShowFrameRateCount = (ShowFrameRateCount+1)%10;

  WTwindow_draw2Dtext(win,0.8f,10.0f/(h+1), ShowFrameRateBuf);  
}

/* */
void change_texture_searchpath(void) 
{

#define ENV_BUFFER_LENGTH 512

  char imagePathSet = 0;
  if (!imagePathSet) {
    int iter, count = 0;
    char *buff1, *buff2 = NULL, buff3[ENV_BUFFER_LENGTH];

    for (iter = 0; MeGetDefaultFileLocation(iter); iter++)
      count += strlen(MeGetDefaultFileLocation(iter));

#ifdef WIN32
    if (GetEnvironmentVariable("WTIMAGES",buff3,ENV_BUFFER_LENGTH)) {
      buff2 = buff3;
      count += strlen(buff2);
    }
    buff1 = (char *)malloc(count+2);
    if (buff1) {
      *buff1 = 0 ;
      if (buff2 != NULL) {
        sprintf(buff1, "%s", buff2);
        strcat(buff1, ";");
      }
      count = 0;
      for (iter=0; MeGetDefaultFileLocation(iter); iter++) {
        strcat(buff1,MeGetDefaultFileLocation(iter));
        count = strlen(buff1);
        if (count > 1) buff1[count-1] = ';';
      }
      buff1[count] = '\0';

      SetEnvironmentVariable("WTIMAGES", buff1);
      free(buff1);
    }

#else
    count += strlen("WTIMAGES")+1;
    buff2 = getenv("WTIMAGES");
    if (buff2 != NULL)
      count += strlen(buff2);
    buff1 = (char *)malloc(count+2);
    if (buff1) {
      sprintf(buff1, "WTIMAGES=");
      if (buff2 != NULL) {
        strcat(buff1, buff2); strcat(buff1, ":");
      }
      count = 0;
      for (iter=0; MeGetDefaultFileLocation(iter); iter++) {
        strcat(buff1,MeGetDefaultFileLocation(iter));
        count = strlen(buff1);
        if (count > 1) buff1[count-1] = ':';
      }
      buff1[count] = '\0';
      putenv(buff1); // Must not free() buff1!!!
    }
#endif

    imagePathSet = 1;
  }
}

/* */
void setup_graphics_scene( WTnode **root, WTnode **xtTrackBall )
{
  WTnode *ambient, *directed;
  WTp3 dir;   
  WTm4 m;
  WTpq pq;
  WTwindow *win;
  char buf[100];
  WTviewpoint *uview;   
  WTsensor    *mouse; 

  sprintf(buf, "%s%s", APP_NAME, hlpMsg[0]);
  WTmessage(buf); WTmessage("\n");
  WTmessage(hlpMsg[3]); WTmessage("\n\n");

  WTmessage ("Creating new universe\n");
  WTuniverse_new(WTDISPLAY_DEFAULT, WTWINDOW_DEFAULT);
  *root = WTuniverse_getrootnodes();
  
  uview = WTuniverse_getviewpoints();   

  win = WTuniverse_getwindows();
  WTwindow_setfgactions(win, drawfg);

#ifdef WIN32     
  sprintf(buf, "%s%s", APP_NAME, hlpMsg[1]);
  SetWindowText((HWND)WTwindow_getidx(win), buf);   
#endif

  /* add lights */
  ambient = WTlightnode_newambient(*root);
  WTlightnode_setintensity(ambient,0.02f);

  *xtTrackBall = WTxformnode_new(*root);    

  WTpq_init(&pq);   
  pq.q[0] = 1; pq.q[1] = 0;
  pq.q[2] = 0; pq.q[3] = 0;    
  WTpq_2m4(&pq, m);
  WTnode_settransform(*xtTrackBall, m);

  directed = WTlightnode_newdirected(*root);
  dir[0] = -0.68f;
  dir[1] = -0.48f;
  dir[2] = 0.58f;
  WTlightnode_setdirection(directed,dir);

  directed = WTlightnode_newdirected(*root);
  dir[0] = 0.68f;
  dir[1] = -0.48f;
  dir[2] = -0.58f;
  WTlightnode_setdirection(directed,dir);

  WTkeyboard_open();  
  
  /* setup mouse sensor */
  mouse = WTmouse_new();   
  if (!mouse) WTerror ("Couldn't find mouse\n");   
  
  WTsensor_setupdatefn (mouse, WTmouse_trackball);
  WTmouse_settrackballdrift( mouse, 0.80f);
  
  /* Attach the sensor to the object. */
  if (*xtTrackBall) { 
    WTmotionlink *mlink; 
#ifdef WTKR9_COMPATIBLE
    mlink = WTmotionlink_new(mouse, *xtTrackBall); 
#else
    mlink = WTmotionlink_new(mouse, *xtTrackBall, WTSOURCE_SENSOR, WTTARGET_TRANSFORM); 
    WTmotionlink_setreferenceframe(mlink, WTFRAME_VPOINT, WTuniverse_getviewpoints()); 
#endif
   }

  WTsensor_setsensitivity (mouse, 0.05f); 

  change_texture_searchpath(); 
}

 
#endif