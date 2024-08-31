/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_NEW_AGE                    2       /* control type: numeric, callback function: (none) */
#define  PANEL_GRAPH1                     3       /* control type: command, callback function: graph_but */
#define  PANEL_EXPORT                     4       /* control type: command, callback function: Export */
#define  PANEL_ADDRESS                    5       /* control type: textBox, callback function: (none) */
#define  PANEL_TXT_DISPLAY                6       /* control type: textBox, callback function: (none) */
#define  PANEL_NEW_NAME                   7       /* control type: textBox, callback function: (none) */
#define  PANEL_PANEL_BTN_SEND_NEW         8       /* control type: command, callback function: OnSendNew */
#define  PANEL_PICTURE                    9       /* control type: picture, callback function: (none) */
#define  PANEL_QUITBUTTON                 10      /* control type: command, callback function: QuitCallback */
#define  PANEL_PANEL_TIMER                11      /* control type: timer, callback function: OnTimer */

#define  PANEL_2                          2
#define  PANEL_2_GRAPH                    2       /* control type: graph, callback function: (none) */
#define  PANEL_2_QUITBUTTON               3       /* control type: command, callback function: QuitCallback1 */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Export(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK graph_but(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSendNew(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif