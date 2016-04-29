//--- ProjExp.h - Header file for for the dBASE Plus Project Explorer ----

#define PE_BUILD "1063 (12/16/2015)"  // Start at "1009 (12/09/2004)" - Dave Casler 


#define _DEBUG

#define MAX_LOG_SIZE 75000
#define MAX_PATHLEN 25

// Window menu
#define dBM_WINDOWCASCADE     0xE132
#define dBM_WINDOWTILEHORZ    0xE133
#define dBM_WINDOWTILEVERT    0xE134
#define dBM_WINDOWARRANGEICO  0xE131
#define dBM_WINDOWCLOSEALL    0x08CF
// WinAPI menu command constant
#define WM_COMMAND            0x0111

#define MIN_HEIGHT 536
//#define MIN_WIDTH 779
#define MIN_WIDTH 823	//D.Brooks - 09/27/2005
#define MIN_SPLITLEFT 109
#define TREEVIEW_HEIGHT_ADJ 71
#define BUTTON_SEP 10
#define BUTTON_MARGIN 10
#define MIN_NOTEBOOK_WIDTH 473
#define SPLITTER_MARGIN 14
#define SPLITTER_DBLCLK_POS 37
#define SPLITTER_ORG_POS 282

// macro functions
#define getTempFolder() (fUnique().left(fUnique().lastIndexOf("\")))
#define isDir(d) (new array().dir(d+"\*.*","D") > 0)


