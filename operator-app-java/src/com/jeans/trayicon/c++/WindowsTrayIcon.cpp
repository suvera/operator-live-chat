/**
 * WindowsTrayIcon.cpp - a tray icon for Java
 * Best viewed with tab size 8
 *
 * Written by Jan Struyf
 *
 *  jan.struyf@cs.kuleuven.ac.be
 *  http://jeans.studentenweb.org/java/trayicon/trayicon.html
 *
 * Changelog
 *
 * Version 1.7.9b (02/26/04)
 *  * Added method to flash window in task bar (thanks to Joe Page)
 *
 * Version 1.7.9 (02/03/04)
 *  * Callback for clicks on balloon message
 *  * Fix for working with Java Web Start
 *  * Swing menu is now set Always-On-Top
 *  * Fix in balloon message that caused VM to crash
 *  * Fixed bug in balloon message specific to Windows 2000
 *
 * Version 1.7.8 (11/2/03)
 *  * Unicode support also for 95/98/Me
 *  * Exception added for balloon message
 *
 * Version 1.7.7 (10/23/03)
 *  * Unicode support for native components
 *
 * Version 1.7.6 (08/17/03)
 *  * Support for Balloon Messages
 *  * Fix for Swing menu (menu is removed if user clicks on desktop area)
 *
 * Version 1.7.5 (02/20/03)
 *  * Support for MouseListeners (icon can respond to double clicks)
 *  * TrayIcons are not lost anymore when Explorer crashes ;-) (thanks to Laurent Hauben)
 *  * JAWT is only used if Swing Menu is used (call initJAWT()!)
 *
 * Version 1.7.4 (01/08/03)
 *  * setAlwaysOnTop implemented for Swing menu (thanks to Mike Hicks)
 *  * SwingTrayIcon demo updated with nice Swing menu
 * 
 * Version 1.7.3 (01/03/03)
 *  * Bold and disabled AWT menu item support 
 *  * Fixed bug in keepAlive()  
 *
 * Version 1.7.2 (12/17/02)
 *  * Transparency bug fix for Windows ME 
 *  * Fixed naming problem with setWindowsMessageCallback (introduced in 1.7.1) 
 *
 * Version 1.7.1 (11/01/02)
 *  * Works with Java 1.4 
 *  * Bug fix for setCheck(boolean selected) 
 *  * Dependency on jvm.lib removed (thanks to Justin Chapweske) 
 *  *Includes some support for using a Swing popup menu 
 * 
 * Version pre1.6c (07/16/01)
 *	* Fixed minor compilation warnings reported by the more strict VC++ 6.0
 *
 * Version pre1.6b (12/16/00)
 *	* Fixed memory leak for 'animating icons'
 *      * ReleaseDC -> DeleteDC
 *
 * Version pre1.6 (09/02/00)
 *	* Support for old JDK/JRE 1.1.x
 *	* TrayIcon 1.6 will support Microsoft Visual J++
 *
 * Version 1.5 (07/03/00)
 *	* Tray icon popup menu support
 *	* Added code for sendWindowsMessage()
 *
 * Version 1.4 (06/29/00)
 *	* Added DllMain function to clean init code
 *	* Removed redundant LoadLibrary/FreeLibrary calls
 *	* Added code for isRunning()
 *
 * Version 1.3 (06/09/00)
 *	* Trail bug fix for NT (no public release)
 *		(Patch from 'Daniel Hope <danielh@inform.co.nz>')
 *
 * Version 1.2 (05/03/00)
 *	* Message handler for first icon fixed
 *	* WM_RBUTTONDOWN message handler fixed
 *	* Classes are now unregistered on exit
 *		(Patch from 'Daniel Rejment <daniel@rejment.com>')
 *
 * Version 1.0 (06/29/99)
 *	* Initial release
 *
 * Please mail me if you
 *	- 've found bugs
 *	- like this program
 *	- don't like a particular feature
 *	- would like something to be modified
 *
 * To compile:
 *	- Use the MDP project file in the VC++ IDE
 *	- Use Makefile.vc as in:
 *		VCVARS32
 *		nmake /f makefile.vc
 *
 * I always give it my best shot to make a program useful and solid, but
 * remeber that there is absolutely no warranty for using this program as
 * stated in the following terms:
 *
 * THERE IS NO WARRANTY FOR THIS PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE
 * LAW. THE COPYRIGHT HOLDER AND/OR OTHER PARTIES WHO MAY HAVE MODIFIED THE
 * PROGRAM, PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS
 * TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
 * PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
 * REPAIR OR CORRECTION.
 *
 * IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW WILL ANY COPYRIGHT HOLDER,
 * OR ANY OTHER PARTY WHO MAY MODIFY AND/OR REDISTRIBUTE THE PROGRAM,
 * BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR INABILITY TO USE THE
 * PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA BEING RENDERED
 * INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A FAILURE OF THE
 * PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS), EVEN IF SUCH HOLDER OR OTHER
 * PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * May the Force be with you... Just compile it & use it!
 */

#define _WIN32_IE 0x0500
#define USE_GLOBAL_REF

#include <windows.h>
#include <windowsx.h>
#include <process.h>
#include <shlwapi.h>
#include <jni.h>

#include <jawt.h>
#include <win32/jawt_md.h>

#include "com_jeans_trayicon_WindowsTrayIcon.h"

// Message ID for sendWindowsMessage() function
#define MYWM_APPTALK		(WM_APP+10)
#define MYWM_APPMOUSE		(WM_APP+11)
// Message ID for Tray icons
#define MYWM_NOTIFYICON		(WM_APP+100)
// Max number of icons supported by this DLL for each application
#define MY_MAX_ICONS		100
// Max length for the name of the hidden window (used for sendWindowsMessage/isRunning)
#define WNDNAME_MAX             20

// Class for the icon bitmap
class IconData {
private:
	HBITMAP hBitmapXOR;	// Colors
	HBITMAP	hBitmapAND;	// Transparancy info
	int wd, hi;
public:
	IconData();
	~IconData();
	int setData(unsigned long *data, int wd, int hi);
	HICON makeIcon(HINSTANCE hInst);
};

// Class for a popup submenu
class PopupSubMenu {
private:
	HMENU hMenu;
	BOOL bSub;
public:
	PopupSubMenu();
	~PopupSubMenu();
	BOOL isSub();
	void makeSub();
	void reNewMenu();
	HMENU getMenu();
	void TrackPopupMenu(UINT flags, POINT* pos, HWND hWnd);
};

typedef PopupSubMenu* PtrPopupSubMenu;

// Class for a popup menu (the main popup)
class PopupMenu {
private:
	PopupSubMenu **popup;
	int nbLevels;
public:
	PopupMenu(int levels);
	~PopupMenu();
	void TrackPopupMenu(UINT flags, POINT* pos, HWND hWnd);
	HMENU getMenu(int level);
	void initMenu(int level);
	PopupSubMenu *getSubMenu(int level);
};

// Growable array for integers
// Used to store the allocated menu id's
class QSIntArray {
	protected:
		int *m_Array;
		long m_Size, m_Grow, m_ArrSize;
	public:
		QSIntArray();
		QSIntArray(long size, long grow);
		~QSIntArray();
		long getSize();
		int getElementAt(long idx);
		void addElement(int element);
		void setElementAt(long idx, int element);
		void removeElementAt(long idx);
		void removeAll();
	private:
		void shrink();
		void grow();
};

#define JNIProcPtr(name) int (FAR *name) (JNIEnv*, int, int)

// Information for each tray icon
typedef struct {
	BOOL used;		// Is this record in use?
	BOOL visible;		// Icon visible on screen or hidden?
	IconData *icon;		// Icon bitmap data
	char *tooltip;		// Icon tooltip
	PopupMenu *popup;	// Main popup for icon
	jobject globalClass;	// Pointer to Java class for callbacks
} TrayIconData;

#define MOUSE_BTN_UP        1
#define MOUSE_BTN_DOUBLE    2

// Some error codes
#define TRAY_NOERR		  0
#define TRAY_NOTIFYPROCERR	 -1
#define TRAY_TOOMANYICONS	 -2
#define TRAY_NOTENOUGHMEM	 -3
#define TRAY_WRONGICONID	 -4
#define TRAY_DLLNOTFOUND	 -5
#define TRAY_NOVMS		 -6
#define TRAY_ERRTHREAD		 -7
#define TRAY_METHODID		 -8
#define TRAY_NOLISTENER		 -9
#define TRAY_JNIERR		 -10
#define TRAY_CALLBACKDLLERR	 -11
#define TRAY_NO_JAWT             -12
#define TRAY_NO_GET_JAWT         -13
#define TRAY_GET_JAWT_FAILS      -14
#define TRAY_JAWT_DS_FAILS       -15
#define TRAY_JAWT_LOCK_FAILS     -16
#define TRAY_ERRHOOK             -17
#define TRAY_ERRORBALLOON        -18

#define TRAY_WIN_VER_UNKNOWN	0
#define TRAY_WIN_VER_WIN32	1
#define TRAY_WIN_VER_95		2
#define TRAY_WIN_VER_98		3
#define TRAY_WIN_VER_ME		4
#define TRAY_WIN_VER_NT		5
#define TRAY_WIN_VER_2K		6
#define TRAY_WIN_VER_XP		7
#define TRAY_WIN_VER_NET	8

#define TRAY_BALLOON_NONE          0
#define TRAY_BALLOON_INFO          1
#define TRAY_BALLOON_WARNING       2
#define TRAY_BALLOON_ERROR         3
#define TRAY_BALLOON_NOSOUND       0x10

// Constants for the popup menu system
#define POPUP_TYPE_ITEM       0
#define POPUP_TYPE_SEPARATOR  1
#define POPUP_TYPE_CHECKBOX   2
#define POPUP_TYPE_INIT_LEVEL 3
#define POPUP_TYPE_DONE_LEVEL 4
#define POPUP_MODE_ENABLE     1
#define POPUP_MODE_CHECK      2
#define POPUP_MODE_DEFAULT    4

#define NIN_BALLOONSHOW       (WM_USER + 2)
#define NIN_BALLOONHIDE       (WM_USER + 3)
#define NIN_BALLOONTIMEOUT    (WM_USER + 4)
#define NIN_BALLOONUSERCLICK  (WM_USER + 5) 

#define NIN_SELECT            (WM_USER + 0)
#define NINF_KEY              1
#define NIN_KEYSELECT         (NIN_SELECT | NINF_KEY) 

#define UNICODE_CONVERSION_NB       10
#define UNICODE_CONVERSION_SUPPORT  1		// OS has support for Unicode
#define UNICODE_CONVERSION_BALLOON  2

#define BALLOON_MSG_SHOW    1
#define BALLOON_MSG_HIDE    2
#define BALLOON_MSG_CLICK   4
#define BALLOON_MSG_TIMEOUT 8

// Current instance of the TrayIcon DLL
HINSTANCE g_hinst = NULL;
// Handle to the hidden window used to receive the tray icon messages
HWND my_hDlg = NULL;
// App name for the hidden window's registered class
CHAR szAppName[] = "QSJavaTray";
// Title for the hidden window (used for sendWindowsMessage/isRunning)
CHAR szWndName[WNDNAME_MAX+1];
// Number of icons allocated
int nb_tray_icons = 0;
// Record for each icon
TrayIconData tray_icons[MY_MAX_ICONS];
// Contains last error message (see error codes above)
jint last_error = TRAY_NOERR;
// Wait while creating the hidden window
HANDLE wait_event = NULL;
// Wait while destroying hidden window
HANDLE exit_event = NULL;
// Maps menu id's to icon id's
QSIntArray *arrUsedMenuIds = NULL;
// Handle to JAWT
HMODULE hJAWT = NULL;
int hJAWTTried = 0;
// Handle to Java VM
JavaVM *hJavaVM = NULL;
// Taskbar restart id
UINT g_TaskbarRestart = 0;
// Use UNICODE
int bUseUnicode = 0;
// Global ref to WindowsTrayIcon class
jobject hGlobalWinTrayClass = 0;
// Unicode conversion
jboolean g_UnicodeConversion[UNICODE_CONVERSION_NB];

// Shared memory variables
#ifdef CYGWIN
	HHOOK hMouseClickHook  __attribute__((section (".shared"), shared)) = NULL;
	HWND  hMouseClickWin   __attribute__((section (".shared"), shared)) = NULL;
	int   hMouseClickEna   __attribute__((section (".shared"), shared)) = 0;
#else
        #pragma data_seg("Shared")
        HHOOK hMouseClickHook = NULL;
        HWND  hMouseClickWin  = NULL;
        int   hMouseClickEna  = 0;
        #pragma data_seg()
        #pragma comment(linker,"/SECTION:Shared,RWS")
#endif

// Get free menu id (for icon id - see arrUsedMenuIds)
long getFreeMenuId(int id_num);
// Free menu id (when removing menu from icon)
void setFreeMenuId(int id_num);
// Map menu id to icon id
int getMenuItemIdNum(int id);
// Add/Remove/Modify tray icon to system tray
BOOL TrayMessage(HWND hDlg, DWORD dwMessage, UINT uID, HICON hIcon, PSTR pszTip);
BOOL TrayMessageW(HWND hDlg, DWORD dwMessage, UINT uID, HICON hIcon, const jchar* pszTip);
bool TrayBalloon(HWND hDlg, UINT uID, const char* msg, const char* title, UINT time, DWORD flags);
bool TrayBalloonW(HWND hDlg, UINT uID, const jchar* msg, const jchar* title, UINT time, DWORD flags);
bool TraySetVersion(HWND hDlg, UINT uID);
// Create the hidden window to receive the icon's mouse messages
HWND MakeHWND(HINSTANCE hInst);
// Remove the hidden window (on app closing)
void RemoveHWND();
// The thread proc that creates the hidden window an listens for messages
void DialogThread( void *dummy );
// Free an icon's menu resources
void freeMenu(int id_num);
// Free an icon's other resources
void freeIcon(JNIEnv *env, int id_num);
// Clean up @ exit of app
void cleanUpExit(JNIEnv *env);
// Call TrayMessage for the specified icon id
void updateIcon(jint id_num);
// Make an icon dissapear from the system tray
void makeInvisible(jint id_num);
// Delete global reference to icon's Java class
int DeleteGlobalCallback(JNIEnv *env, int id_num, int dummy);
// Call to get JAWT handle
typedef jboolean (JNICALL *PJAWT_GETAWT)(JNIEnv*, JAWT*);
// Version of Shell32 DLL
DWORD hShell32Version = 4;
// Windows Version
DWORD hWindowsVersion = TRAY_WIN_VER_95;
// Initialize Unicode Functions
int initUnicodeFunctions();
// Max chars per multi-byte char
int nbWideMaxChars = 2;


class DoWithHWND {
public:
    virtual int execute(HWND hwnd) = 0;
};

class DoAlwaysOnTop : public DoWithHWND {
    virtual int execute(HWND hwnd);
};

class ThreadJavaCallback {
public:
    virtual int execute(JNIEnv* env) = 0;
};

// Call a Java method in a given virtual machine
int CallJavaVM(JavaVM* vm, ThreadJavaCallback* call);

class SimpleJavaCallback : public ThreadJavaCallback {
protected:
	JNIProcPtr(m_JNIProc);
	int m_Arg1, m_Arg2;
public:
    SimpleJavaCallback(JNIProcPtr(jniproc), int arg1, int arg2);
    virtual int execute(JNIEnv* env);    
};

class MouseJavaCallback : public ThreadJavaCallback {
protected:
	int m_IDNum, m_Button, m_Mask;
	POINT m_Pos;
public:
    MouseJavaCallback(int idnum, int button, int mask);    
    MouseJavaCallback(int idnum, int button, int mask, POINT* pos);
    POINT* getPos();
    virtual int execute(JNIEnv* env);    
};

class BalloonJavaCallback : public ThreadJavaCallback {
protected:
	int m_IDNum, m_Mask;
public:
    BalloonJavaCallback(int idnum, int mask);    
    virtual int execute(JNIEnv* env);    
};

void doWithHWND(DoWithHWND* hwndProc, JNIEnv* env, jobject window);
void GetJAWTHandle();
void RemoveJAWT();
void RemoveHook();
char* find_in_path(const char* append);
BOOL GetOSName(char* osname, int* version);
int TrayGetWindowsVersion();
int GetShell32VerionHelp(DLLVERSIONINFO* version);
DWORD GetShell32Version();

// Struct size for Win95 compatibility
#define MY_NOTIFYICONDATA_V2_SIZE 88

// New struct (WinXP style)
typedef struct {
	DWORD cbSize;			// 4
	HWND  hWnd;			// 8
	DWORD uID;			// 12
	DWORD uFlags;			// 16
	DWORD uCallbackMessage;		// 20
	HICON hIcon;			// 24
	char szTip[128];		// 152
	DWORD dwState;			// 156
	DWORD dwStateMask;		// 160
	char szInfo[256];		// 416
	DWORD uTimeOrVersion;		// 420
	char szInfoTitle[64];		// 484
	DWORD dwInfoFlags;		// 488
	GUID guidItem;			// 492
} MY_NOTIFYICONDATA;
	
// New struct (WinXP style - Unicode Version)
typedef struct {
	DWORD cbSize;			// 4
	HWND  hWnd;			// 8
	DWORD uID;			// 12
	DWORD uFlags;			// 16
	DWORD uCallbackMessage;		// 20
	HICON hIcon;			// 24
	jchar szTip[128];		// 152
	DWORD dwState;			// 156
	DWORD dwStateMask;		// 160
	jchar szInfo[256];		// 416
	DWORD uTimeOrVersion;		// 420
	jchar szInfoTitle[64];		// 484
	DWORD dwInfoFlags;		// 488
	GUID guidItem;			// 492
} MY_NOTIFYICONDATAW;
	
// Should be declared by header files
#define NIF_INFO           0x10
#define NIM_SETVERSION     0x04 
#define NOTIFYICON_VERSION 3

#define NIIF_NONE          0
#define NIIF_INFO          1
#define NIIF_WARNING       2
#define NIIF_ERROR         3
#define NIIF_NOSOUND       0x10

typedef struct {
  UINT cbSize;
  HWND hwnd;
  DWORD dwFlags;
  UINT uCount;
  DWORD dwTimeout;
} MY_FLASHWINFO;

#define MY_FLASHW_STOP		0
#define MY_FLASHW_CAPTION	1
#define MY_FLASHW_TRAY		2
#define MY_FLASHW_TIMER		4
#define MY_FLASHW_TIMERNOFG	12

typedef BOOL (*P_FLASHWINDOWEX)(MY_FLASHWINFO*);

void copyJCharMax(const jchar* src, jchar* tar, int size) {
	for (int i = 0; i < size-1; i++) {
		tar[i] = src[i];
		if (src[i] == 0) return;
	}
	tar[size-1] = 0;
}

int lenJChar(const jchar* src) {
	if (src == NULL) return 0;
	int len = 0;
	while (src[len] != 0) len++;
	return len;
}

#define UNICODE_NAME_MAX 8

typedef struct {
	char name[UNICODE_NAME_MAX];
	jchar value; 	
} UnicodeMapping;

UnicodeMapping UnicodeMap[] =
	{{"lsquo", 8216}, {"rsquo", 8217}, {"ldquo", 8220}, {"rdquo", 8221}, {"dagger", 8224},
	 {"Dagger", 8225}, {"permil", 8240}, {"larr", 8592}, {"uarr", 8593}, {"rarr", 8594},
	 {"darr", 8595}, {"trade", 8482}, {"iexcl", 161}, {"cent", 162}, {"pound", 163},
	 {"curren", 164}, {"yen", 165}, {"sect", 167}, {"copy", 169}, {"not", 172}, {"reg", 174},
	 {"deg", 176}, {"plusmn", 177}, {"sup2", 178}, {"sup3", 179}, {"micro", 181}, {"para", 182},
	 {"middot", 183}, {"sup1", 185}, {"frac14", 188}, {"frac12", 189}, {"frac34", 190}, 
	 {"iquest", 191}, {"Agrave", 192}, {"Aacute", 193}, {"Acirc", 194}, {"Atilde", 195},
	 {"Auml", 196}, {"Aring", 197}, {"AElig", 198}, {"Ccedil", 199}, {"Egrave", 200},
	 {"Eacute", 201}, {"Ecirc", 202}, {"Euml", 203}, {"Igrave", 204}, {"Iacute", 205},
	 {"Icirc", 206}, {"Iuml", 207}, {"ETH", 208}, {"Ntilde", 209}, {"Ograve", 210},
	 {"Oacute", 211}, {"Ocirc", 212}, {"Otilde", 213}, {"Ouml", 214}, {"times", 215},
	 {"Oslash", 216}, {"Ugrave", 217}, {"Uacute", 218}, {"Ucirc", 219}, {"Uuml", 220},
	 {"Yacute", 221}, {"THORN", 222}, {"szlig", 223}, {"agrave", 224}, {"aacute", 225},
	 {"acirc", 226}, {"atilde", 227}, {"auml", 228}, {"aring", 229}, {"aelig", 230},
	 {"ccedil", 231}, {"egrave", 232}, {"eacute", 233}, {"ecirc", 234}, {"euml", 235},
	 {"igrave", 236}, {"iacute", 237}, {"icirc", 238}, {"iuml", 239}, {"eth", 240},
	 {"ntilde", 241}, {"ograve", 242}, {"oacute", 243}, {"ocirc", 244}, {"otilde", 245},
	 {"ouml", 246}, {"divide", 247}, {"oslash", 248}, {"ugrave", 249}, {"uacute", 250},
	 {"ucirc", 251}, {"uuml", 252}, {"yacute", 253}, {"thorn", 254}, {"yuml", 255},
	 {"", 0}};

void convertUnicodeString(const jchar* strg, jchar* strgo, int nbMax) {
	char buffer[UNICODE_NAME_MAX+1];
	int in_buffer = 0, bufpos = 0, pos = 0, opos = 0;
	while (strg[pos] != 0) {
		if (pos >= nbMax) break;
		if (opos >= nbMax) break;
		if (in_buffer == 1) {
			if (strg[pos] == (jchar)';') {
				int mappos = 0, found = 0;
				buffer[bufpos] = 0;
				while (UnicodeMap[mappos].value != 0) {
					if (strcmp(UnicodeMap[mappos].name, buffer) == 0) {
						int len = strlen(UnicodeMap[mappos].name);
						opos -= len+1;
						strgo[opos++] = UnicodeMap[mappos].value;
						found = 1;
					}
					mappos++;
				}
				if (found == 0) {
					strgo[opos++] = (jchar)';';
				}
				in_buffer = 0;
			} else if (strg[pos] == (jchar)'&') {
				strgo[opos++] = (jchar)'&';
				bufpos = 0;
			} else {
				if (bufpos < UNICODE_NAME_MAX) {
					if (strg[pos] < 255) {
						buffer[bufpos++] = (char)strg[pos];
					} else {
						in_buffer = 0;
					}
				} else {
					in_buffer = 0;
				}
				strgo[opos++] = strg[pos];
			}
		} else {
			if (strg[pos] == (jchar)'&') {
				in_buffer = 1;
				bufpos = 0;	
			}
			strgo[opos++] = strg[pos];
		}
		pos++;
	}
	strgo[opos] = 0;
}

char* convertUnicodeANSI(jchar* unicode) {
	int len = lenJChar(unicode);
	int nbmax = nbWideMaxChars*len;
	char* result = new char[nbmax+2];
	// Convert Unicode string to ANSI string
	if (!WideCharToMultiByte(CP_ACP, 0, (const WCHAR*)unicode, len+1, result, nbmax+1, NULL, NULL)) {
		// Fall back :-(
		for (int i = 0; i < len; i++) {
			result[i] = (char)(unicode[i] & 0xFF);
		}
	}
	return result;	
}

/*
 * Class:     com_jeans_trayicon_WindowsTrayIcon
 * Method:    enableUnicodeConversion
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_enableUnicodeConversion(JNIEnv *, jclass, jint which, jboolean enable) {
	g_UnicodeConversion[which] = enable;
}

/*
 * Class:     com_jeans_trayicon_WindowsTrayIcon
 * Method:    hasUnicodeConversion
 * Signature: (I)I
 */
JNIEXPORT jboolean JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_hasUnicodeConversion(JNIEnv *, jclass, jint which) {
	return g_UnicodeConversion[which];
}

/*
 * Class:     com_jeans_trayicon_WindowsTrayIcon
 * Method:    supportsBalloonMessages
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_supportsBalloonMessages(JNIEnv *, jclass) {
	if (hShell32Version < 5) {
		// No balloon messages supported :-(
		return FALSE;	
	} else {
		return TRUE;
	}	
}

/*
 * Class:     jeans_trayicon_WindowsTrayIcon
 * Method:    getLastError
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_getLastError(JNIEnv *, jclass) {
	int err = last_error;
	last_error = TRAY_NOERR;
	return last_error;		// Return the last error
}

/*
 * Class:     WindowsTrayIcon
 * Method:    getFreeId
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_getFreeId(JNIEnv *env, jclass) {
	// Wait for the hidden window to be created (see initTrayIcon())
	if (wait_event != NULL) {
		WaitForSingleObject(wait_event,10000);
		CloseHandle(wait_event);
		wait_event = NULL;
	}
	// Hidden window handle valid?
	if (my_hDlg == NULL) {
		last_error = TRAY_NOTIFYPROCERR;
		return TRAY_NOTIFYPROCERR;
	}
	// Instance handle valid?
	if (g_hinst == NULL) {
		last_error = TRAY_DLLNOTFOUND;
		return TRAY_DLLNOTFOUND;
	}
	// Find unused icon record and return icon id
	for (int ctr = 0; ctr < MY_MAX_ICONS; ctr++) {
		if (tray_icons[ctr].used == FALSE) {
			// Unused record, initialize fields and return id
			tray_icons[ctr].used = TRUE;
			tray_icons[ctr].visible = FALSE;
			tray_icons[ctr].icon = NULL;
			tray_icons[ctr].tooltip = NULL;
			tray_icons[ctr].popup = NULL;
			return ctr;
		}
	}
	// No empty record left, return error
	last_error = TRAY_TOOMANYICONS;
	return TRAY_TOOMANYICONS;
}

/*
 * Class:     WindowsTrayIcon
 * Method:    setIconData
 * Signature: (III[I)V
 */
JNIEXPORT void JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_setIconData(JNIEnv *env, jclass, jint id_num, jint wd, jint hi, jintArray array) {
	// Instance handle valid?
	if (g_hinst == NULL) {
		last_error = TRAY_DLLNOTFOUND;
		return;
	}
	// Icon id valid?
	if (tray_icons[id_num].used == FALSE) {
		last_error = TRAY_WRONGICONID;
		return;
	}
	// Get array length and elements
	jsize len = env->GetArrayLength(array);
	jint *body = env->GetIntArrayElements(array, 0);
	// Allocate data for icon bitmap and store the given pixel array
	IconData *data = new IconData();
	if (data != NULL && data->setData((unsigned long *)body, wd, hi) != -1) {
		IconData *olddata = tray_icons[id_num].icon;
		tray_icons[id_num].icon = data;
		updateIcon(id_num);
		if (olddata != NULL) delete olddata;
	} else {
		delete data;
		last_error = TRAY_NOTENOUGHMEM;
	}
	// Release java array
	env->ReleaseIntArrayElements(array, body, 0);
}

void showIcon(int id_num) {
	// Show icon in windows system tray
	if (tray_icons[id_num].icon != NULL) {
		HICON icon = tray_icons[id_num].icon->makeIcon(g_hinst);
		if (icon != NULL) {
			if (bUseUnicode == 0) {			
				TrayMessage(my_hDlg, NIM_ADD, id_num, icon, tray_icons[id_num].tooltip);
			} else {
				TrayMessageW(my_hDlg, NIM_ADD, id_num, icon, (jchar*)tray_icons[id_num].tooltip);
			}
			tray_icons[id_num].visible = TRUE;
		} else {
			last_error = TRAY_NOTENOUGHMEM;
		}
	}
}

/*
 * Class:     WindowsTrayIcon
 * Method:    showIcon
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_showIcon(JNIEnv *, jclass, jint id_num, jboolean show) {
	// Hidden window handle valid?
	if (my_hDlg == NULL) {
		last_error = TRAY_NOTIFYPROCERR;
		return;
	}
	// Instance handle valid?
	if (g_hinst == NULL) {
		last_error = TRAY_DLLNOTFOUND;
		return;
	}
	// Icon id valid?
	if (tray_icons[id_num].used == FALSE) {
		last_error = TRAY_WRONGICONID;
		return;
	}
	if (show) {
		if (tray_icons[id_num].visible == FALSE) {
			showIcon(id_num);
		}
	} else {
		// Hide icon (make invisible)
		if (tray_icons[id_num].visible == TRUE) {
			TrayMessage(my_hDlg, NIM_DELETE, id_num, NULL, NULL);
			tray_icons[id_num].visible = FALSE;
		}
	}
}

/*
 * Class:     jeans_trayicon_WindowsTrayIcon
 * Method:    testVisible
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_testVisible(JNIEnv *, jclass, jint id_num) {
	// Icon id valid?
	if (tray_icons[id_num].used == FALSE) {
		last_error = TRAY_WRONGICONID;
		return TRAY_WRONGICONID;
	}
	// Is the icon visible now?
	if (tray_icons[id_num].visible == TRUE) {
		return 1;
	} else {
		return 0;
	}
}

/*
 * Class:     jeans_trayicon_WindowsTrayIcon
 * Method:    clickEnable
 * Signature: (Ljeans/graph/trayicon/WindowsTrayIcon;IZ)V
 */
JNIEXPORT void JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_clickEnable(JNIEnv *env, jclass, jobject ic_cls, jint id_num, jboolean enable) {
	// Icon id valid?
	if (tray_icons[id_num].used == FALSE) {
		last_error = TRAY_WRONGICONID;
		return;
	}
	if (enable == TRUE) {
		// Make icon clickenable by storing a global reference to it's class
		jobject globalWinTrayClass = env->NewGlobalRef(ic_cls);
		if (globalWinTrayClass != 0) {
			if (tray_icons[id_num].globalClass != 0)
				env->DeleteGlobalRef(tray_icons[id_num].globalClass);
			tray_icons[id_num].globalClass = globalWinTrayClass;
		}
	} else {
		// Remove the icon's global class reference
		if (tray_icons[id_num].globalClass != 0)
			env->DeleteGlobalRef(tray_icons[id_num].globalClass);
		tray_icons[id_num].globalClass = 0;
	}
}

/*
 * Class:     jeans_trayicon_WindowsTrayIcon
 * Method:    setToolTip
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_setToolTip(JNIEnv *env, jclass, jint id_num, jstring tip) {
	// Icon id valid?
	if (tray_icons[id_num].used == FALSE) {
		last_error = TRAY_WRONGICONID;
		return;
	}
	// If already exists delete first..
	if (tray_icons[id_num].tooltip != NULL)	delete tray_icons[id_num].tooltip;
	// Get java string for tooltip
	int len = env->GetStringLength(tip);
	const jchar *j_tip = env->GetStringChars(tip, 0);
	if (bUseUnicode == 0) {
		jchar* m_tip = new jchar[len+1];
		convertUnicodeString(j_tip, m_tip, len);
		tray_icons[id_num].tooltip = convertUnicodeANSI(m_tip);
		delete m_tip;
	} else {
		tray_icons[id_num].tooltip = (char*)new jchar[len+1];
		convertUnicodeString(j_tip, (jchar*)tray_icons[id_num].tooltip, len);
	}
	env->ReleaseStringChars(tip, j_tip);
	updateIcon(id_num);
}

/*
 * Class:     com_jeans_trayicon_WindowsTrayIcon
 * Method:    showBalloon
 * Signature: (ILjava/lang/String;Ljava/lang/String;II)V
 */
JNIEXPORT jint JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_showBalloon(JNIEnv* env, jclass, jint id_num, jstring msg, jstring title, jint time, jint flags) {
	bool result = FALSE;
	// Icon id valid?
	if (tray_icons[id_num].used == FALSE) {
		last_error = TRAY_WRONGICONID;
		return 0;
	}
	if (my_hDlg != NULL && tray_icons[id_num].visible == TRUE && g_hinst != NULL) {
		DWORD myflags = flags;
		// Allocate buffers for Unicode version
		int mlen = env->GetStringLength(msg);
		int tlen = env->GetStringLength(title);
		jchar* m_msg = new jchar[mlen+1];
		jchar* m_title = new jchar[tlen+1];
		// Get strings from Java side
		const jchar *j_msg = env->GetStringChars(msg, 0);
		const jchar *j_title = env->GetStringChars(title, 0);
		// Convert strings (replace HTML codes)
		convertUnicodeString(j_msg, m_msg, mlen);
		convertUnicodeString(j_title, m_title, tlen);
		// Release Java Strings
		env->ReleaseStringChars(msg, j_msg);
		env->ReleaseStringChars(title, j_title);
		// Call method
		if (bUseUnicode == 0 || g_UnicodeConversion[UNICODE_CONVERSION_BALLOON]) {
			char *a_msg = convertUnicodeANSI(m_msg);
			char *a_title = convertUnicodeANSI(m_title);
			result = TrayBalloon(my_hDlg, id_num, a_msg, a_title, (UINT)time, myflags);
			delete a_msg;
			delete a_title;
		} else {
			result = TrayBalloonW(my_hDlg, id_num, m_msg, m_title, (UINT)time, myflags);
		}
		delete m_title;
		delete m_msg;
	}
	if (!result) {
		last_error = TRAY_ERRORBALLOON;
		return 0;
	}
	return 1;
}

/*
 * Class:     WindowsTrayIcon
 * Method:    freeIcon
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_freeIcon(JNIEnv *env, jclass, jint id_num) {
	freeIcon(env, id_num);
}

/*
 * Class:     com_jeans_trayicon_WindowsTrayIcon
 * Method:    initTrayIcon
 * Signature: (Ljava/lang/String;Lcom/jeans/trayicon/WindowsTrayIcon;)V
 */
JNIEXPORT void JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_initTrayIcon(JNIEnv *env, jclass, jstring wndName, jobject obj) {
	// Store handle to Java VM
	env->GetJavaVM(&hJavaVM);
#ifdef USE_GLOBAL_REF
	hGlobalWinTrayClass = env->NewGlobalRef(obj);
#endif
	// Store version of Shell32 DLL
	hShell32Version = GetShell32Version();
	// Store version of Windows
	hWindowsVersion = TrayGetWindowsVersion();
	// Initialize Unicode Functions
	initUnicodeFunctions();
	// Make dialog
	if (my_hDlg == NULL) {
		// Copy window name
		const char *cWndName = env->GetStringUTFChars(wndName, 0);
		strncpy(szWndName ,cWndName, WNDNAME_MAX);
		szWndName[WNDNAME_MAX] = 0;
		env->ReleaseStringUTFChars(wndName, cWndName);
		// Initialize icon data struct
		for (int ctr = 0; ctr < MY_MAX_ICONS; ctr++) tray_icons[ctr].used = FALSE;
		// Popup invisible dummy window
		if (g_hinst != NULL) {
			wait_event = CreateEvent(NULL,FALSE,FALSE,NULL);
			_beginthread(DialogThread, 0, NULL );
		}
	}
}

/*
 * Class:     com_jeans_trayicon_WindowsTrayIcon
 * Method:    initJAWT
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_initJAWT(JNIEnv *, jclass) {
    GetJAWTHandle();
}

/*
 * The Swing menu needs a message hook that tracks all mouse clicks
 * if a user clicks outside the menu, it should dissapear
 */

extern "C" {

LRESULT CALLBACK WinTrayMouseClickHook(int nCode, WPARAM wParam, LPARAM lParam)  {
	if (hMouseClickEna == 1 && nCode == HC_ACTION) {
		MOUSEHOOKSTRUCT* pStruct = (MOUSEHOOKSTRUCT*)lParam;
		unsigned int isLButton = (unsigned int)GetAsyncKeyState(VK_LBUTTON);
		if (isLButton > 1) {
			SendMessage(hMouseClickWin, MYWM_APPMOUSE, pStruct->pt.x, pStruct->pt.y);
		}
	}
	return CallNextHookEx(hMouseClickHook, nCode, wParam, lParam);
}

}

/*
 * Class:     com_jeans_trayicon_WindowsTrayIcon
 * Method:    initHook
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_initHook(JNIEnv *, jclass) {
	hMouseClickEna  = 0;
	hMouseClickWin  = my_hDlg;
	if (hMouseClickHook == NULL) {
		RemoveHook();
		hMouseClickHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)WinTrayMouseClickHook, g_hinst, 0);
		if (hMouseClickHook == NULL) {
			last_error = TRAY_ERRHOOK;
		}
	}
}

JNIEXPORT void JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_setMouseHookEnabled(JNIEnv *, jclass, jint enable) {
	hMouseClickEna = enable;
}

/*
 * Class:     com_jeans_trayicon_WindowsTrayIcon
 * Method:    getWindowsVersionString
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_getWindowsVersionString(JNIEnv *env, jclass) {
	int version_int; 
	jstring result = NULL;
	char version_string[300];
	GetOSName(version_string, &version_int);
	int len = strlen(version_string);
	jchar* data = new jchar[len+1];
	for (int i = 0; i < len; i++) {
		data[i] = (jchar)version_string[i];
	}
	data[len] = 0;
	if (data != NULL) {
		result = env->NewString((const jchar*)data, (long int)len);
		delete data;
	}
	return result;
}

/*
 * Class:     com_jeans_trayicon_WindowsTrayIcon
 * Method:    getWindowsVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_getWindowsVersion(JNIEnv *, jclass) {
	int version_int; 
	char version_string[300];
	GetOSName(version_string, &version_int);
	return (jint)version_int;
}

/*
 * Class:     jeans_trayicon_WindowsTrayIcon
 * Method:    isRunning
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT jboolean JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_isRunning(JNIEnv *env, jclass, jstring wndName) {
	const char *cWndName = env->GetStringUTFChars(wndName, 0);
	// Find out if there's a hidden window with the given title
	HWND mHwnd = FindWindow(szAppName, cWndName);
	env->ReleaseStringUTFChars(wndName, cWndName);
	// If there is, another instance of our app is already running
	return mHwnd != NULL;
}

/*
 * Class:     jeans_trayicon_WindowsTrayIcon
 * Method:    sendWindowsMessage
 * Signature: (Ljava/lang/String;J)J
 */
JNIEXPORT jint JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_sendWindowsMessage
  (JNIEnv *env, jclass, jstring wndName, jint lParam) {
	const char *cWndName = env->GetStringUTFChars(wndName, 0);
	// Find hidden window handle by name
	HWND mHwnd = FindWindow(szAppName, cWndName);
	env->ReleaseStringUTFChars(wndName, cWndName);
	// If the window exists, send out our message and wait for return value
	if (mHwnd == NULL) return -1;
	else return SendMessage(mHwnd, MYWM_APPTALK, 0, lParam);
}

/*
 * Class:     jeans_trayicon_WindowsTrayIcon
 * Method:    termTrayIcon
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_termTrayIcon(JNIEnv *env, jclass) {
	cleanUpExit(env);
}

/*
 * Class:     jeans_trayicon_WindowsTrayIcon
 * Method:    initPopup
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_initPopup
  (JNIEnv *env, jclass, jint id_num, jint nbLevels) {
	// Icon id valid?
	if (tray_icons[id_num].used == FALSE) {
		last_error = TRAY_WRONGICONID;
		return;
	}
	// Free previous allocated menu
	freeMenu(id_num);
	if (nbLevels != -1) {
	    // Create new popup menu with given depth
	    tray_icons[id_num].popup = new PopupMenu(nbLevels);
    }
}

/*
 * Class:     jeans_trayicon_WindowsTrayIcon
 * Method:    checkPopup
 * Signature: (IIZ)V
 */
JNIEXPORT void JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_checkPopup
  (JNIEnv *env, jclass, jint id_num, jint menuId, jboolean selected) {
	// Get main popup menu handle of icon
	PopupMenu *popup = tray_icons[id_num].popup;
	if (popup != NULL) {
		HMENU menu = popup->getMenu(0);
		// Add check mark or remove check mark
		UINT how = selected ?
			MF_BYCOMMAND | MF_CHECKED :
			MF_BYCOMMAND | MF_UNCHECKED;
		// Check the menu item by command id (menuId)
		CheckMenuItem(menu, menuId, how);
	}
}

/*
 * Class:     jeans_trayicon_WindowsTrayIcon
 * Method:    enablePopup
 * Signature: (IIZ)V
 */
JNIEXPORT void JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_enablePopup
  (JNIEnv *env, jclass, jint id_num, jint menuId, jboolean enable) {
	// Get main popup menu handle of icon
	PopupMenu *popup = tray_icons[id_num].popup;
	if (popup != NULL) {
		HMENU menu = popup->getMenu(0);
		// Enable or disable item
		UINT how = enable ?
			MF_BYCOMMAND | MF_ENABLED :
			MF_BYCOMMAND | MF_GRAYED;
		// Check the menu item by command id (menuId)
		EnableMenuItem(menu, menuId, how);
	}
}

/*
 * Class:     jeans_trayicon_WindowsTrayIcon
 * Method:    modifyPopup
 * Signature: (IIIZ)V
 */
JNIEXPORT void JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_modifyPopup
  (JNIEnv *env, jclass, jint id_num, jint menuId, jint what, jboolean state) {
	// Get main popup menu handle of icon
	PopupMenu *popup = tray_icons[id_num].popup;
	if (popup != NULL) {
	    MENUITEMINFO info;
        HMENU menu = popup->getMenu(0);
	    info.cbSize = sizeof(MENUITEMINFO);
	    info.fMask = MIIM_STATE;
        GetMenuItemInfo(menu, menuId, FALSE, &info);	    	    
	    switch (what) {
	        case POPUP_MODE_ENABLE:
	            info.fState &= (MFS_DISABLED | MFS_ENABLED | MFS_GRAYED) ^ ((UINT)-1);
	            if (state) info.fState |= MFS_ENABLED; else info.fState |= MFS_GRAYED;
	            break;
	        case POPUP_MODE_CHECK:
	            info.fState &= (MFS_CHECKED | MFS_UNCHECKED) ^ ((UINT)-1);
	            if (state) info.fState |= MFS_CHECKED; else info.fState |= MFS_UNCHECKED;
	            break;
            case POPUP_MODE_DEFAULT:
	            info.fState &= (MFS_DEFAULT) ^ ((UINT)-1);
	            if (state) info.fState |= MFS_DEFAULT;
                break;
        }		        
        info.fMask = MIIM_STATE;
        SetMenuItemInfo(menu, menuId, FALSE, &info);
	}
}

/*
 * Class:     com_jeans_trayicon_WindowsTrayIcon
 * Method:    flashWindowImpl
 * Signature: (Ljava/lang/String;III)Z
 */
JNIEXPORT jboolean JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_flashWindowImpl(JNIEnv *env, jclass, jstring windowTitle, jint flags, jint count, jint timeout) {
	HMODULE hShell = LoadLibrary("User32.dll");
	if (hShell == NULL) {
		return FALSE;
	}
	P_FLASHWINDOWEX proc = (P_FLASHWINDOWEX)GetProcAddress(hShell, "FlashWindowEx");
	if (proc == NULL) {
		FreeLibrary(hShell);
		return FALSE;
	}
	const char* cWindowTitle = env->GetStringUTFChars(windowTitle, 0);
    	HWND hwnd = FindWindow(NULL, cWindowTitle);
    	env->ReleaseStringUTFChars(windowTitle, cWindowTitle);
	MY_FLASHWINFO fi;
	fi.cbSize = sizeof(fi);
	fi.hwnd = hwnd;
	fi.dwFlags = flags;
	fi.uCount = count;
	fi.dwTimeout = timeout;
	proc(&fi);
	FreeLibrary(hShell);
	return TRUE;
}

void TrayAppendMenu(JNIEnv* env, HMENU hMenu, unsigned long type, UINT id, jstring mname) {
	int len = env->GetStringLength(mname);
	jchar* m_mname = new jchar[len+1];
	const jchar* j_mname = env->GetStringChars(mname, 0);
	convertUnicodeString(j_mname, m_mname, len);
	env->ReleaseStringChars(mname, j_mname);
	if (bUseUnicode == 0) {
		char* a_mname = convertUnicodeANSI(m_mname);
		AppendMenu(hMenu, type, id, a_mname);
		delete a_mname;
	} else {
		AppendMenuW(hMenu, type, id, (const WCHAR*)m_mname);
	}
	delete m_mname;
}

/*
 * Class:     jeans_trayicon_WindowsTrayIcon
 * Method:    subPopup
 * Signature: (IILjava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_subPopup
  (JNIEnv *env, jclass, jint id_num, jint level, jstring menuName, jint type, jint extra) {
	// Return a menu id for the new submenu item
	jint id = -1;
	// Icon id valid?
	if (tray_icons[id_num].used == FALSE) {
		last_error = TRAY_WRONGICONID;
		return -1;
	}
	// Popup valid? (use initPopup())
	if (tray_icons[id_num].popup != NULL) {
		PopupMenu *popup = tray_icons[id_num].popup;
		if (type == POPUP_TYPE_INIT_LEVEL || type == POPUP_TYPE_DONE_LEVEL) {
			// Add new level to the popup menu (= new submenu)
			switch (type) {
				case POPUP_TYPE_INIT_LEVEL:
					// Marks the first item of the new level
					popup->initMenu(level);
					break;
				case POPUP_TYPE_DONE_LEVEL:
					// Marks the last item of the current level
					if (level > 0) {
						PopupSubMenu *menu = popup->getSubMenu(level);
						HMENU hMenu = popup->getMenu(level-1);
						HMENU sMenu = menu->getMenu();
						TrayAppendMenu(env, hMenu, MF_POPUP | MF_ENABLED | MF_UNCHECKED, (UINT)sMenu, menuName);
						// Sub menus must not be destroyed, only parents must: mark as sub
						menu->makeSub();
					}
					break;
			}
		} else {
			MENUITEMINFO info;
			info.cbSize = sizeof(MENUITEMINFO);
			info.fMask = MIIM_STATE;
			info.fState = 0;
			// Add a regular item to a (sub)menu
			HMENU hMenu = popup->getMenu(level);
			if (hMenu == NULL) return -1;
			switch (type) {
				case POPUP_TYPE_CHECKBOX:			    
				case POPUP_TYPE_ITEM:
					if ((extra & POPUP_MODE_CHECK) != 0) info.fState |= MFS_CHECKED; 
					else info.fState |= MFS_UNCHECKED;
					if ((extra & POPUP_MODE_ENABLE) != 0) info.fState |= MFS_ENABLED;
					else info.fState |= MFS_GRAYED;
					if ((extra & POPUP_MODE_DEFAULT) != 0) info.fState |= MFS_DEFAULT;
					// Get free id for the new menu item
					id = getFreeMenuId(id_num);
					// Append the new item to the existing menu
					TrayAppendMenu(env, hMenu, MF_STRING, (UINT)id, menuName);
					SetMenuItemInfo(hMenu, id, FALSE, &info);
					break;
				case POPUP_TYPE_SEPARATOR:
					// Append a separator to the menu
					AppendMenu(hMenu,MF_SEPARATOR, 0, NULL);
					break;
			}
		}
	}
	// Return the id of the new menu item (used for callback messages)
	return id;
}

extern "C" {

// Main proc of DLL, called on initialisation, termination
BOOL WINAPI DllMain(HANDLE hInst, ULONG fdwReason, LPVOID lpReserved) {
    switch(fdwReason) {
        case DLL_PROCESS_ATTACH:
		// Store the instance handle
		g_hinst = (HINSTANCE)hInst;
		// Make new map for menu ids
		if (arrUsedMenuIds == NULL) arrUsedMenuIds = new QSIntArray();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		// Delete the map for menu ids
		if (arrUsedMenuIds != NULL) {
			delete arrUsedMenuIds;
			arrUsedMenuIds = NULL;
		}
		break;
    }
    // Initialisation OK
    return TRUE;
}

}

// This proc is called on exit (before Java's System.exit())
// Free all icon resources and remove the hidden window
void cleanUpExit(JNIEnv *env) {
	for (int id_num = 0; id_num < MY_MAX_ICONS; id_num++) {
		freeIcon(env, id_num);
	}
	RemoveHWND();
	RemoveJAWT();
	RemoveHook();
	if (hGlobalWinTrayClass != 0) {
		env->DeleteGlobalRef(hGlobalWinTrayClass);
		hGlobalWinTrayClass = 0;
	}
}

// Free menu resources for icon with given id
void freeMenu(int id_num) {
	// Free allocated menu id's
	setFreeMenuId(id_num);
	// Free popup class
	if (tray_icons[id_num].popup != NULL) {
		delete tray_icons[id_num].popup;
		tray_icons[id_num].popup = NULL;
	}
}

// Free all icon resources for given id
// Make invisible, destroy icon, destroy tooltip, destroy global reference, free menu,..
void freeIcon(JNIEnv *env, int id_num) {
	// Icon handle valid?
	if (tray_icons[id_num].used == TRUE) {
		// Make invisible
		if (tray_icons[id_num].visible == TRUE) {
			makeInvisible(id_num);
		}
		// Make invalid
		tray_icons[id_num].used = FALSE;
		// Free icon
		if (tray_icons[id_num].icon != NULL) {
			delete tray_icons[id_num].icon;
			tray_icons[id_num].icon = NULL;
		}
		// Free tooltip
		if (tray_icons[id_num].tooltip != NULL) {
			delete tray_icons[id_num].tooltip;
			tray_icons[id_num].tooltip = NULL;
		}
		// Free global ref to callback class for mouse/menu events
		if (tray_icons[id_num].globalClass != 0) {
			if (env != NULL) {
				env->DeleteGlobalRef(tray_icons[id_num].globalClass);
			} else {
				SimpleJavaCallback call(DeleteGlobalCallback, id_num, 0);
				CallJavaVM(hJavaVM, &call);
			}
			tray_icons[id_num].globalClass = 0;
		}
		// Free menu ids and resources
		freeMenu(id_num);
	}
}

// Update icon with given id in system tray
// Show or Hide and add tooltip,..
void updateIcon(jint id_num) {
	// Valid hidden window handle, icon id an visible?
	if (my_hDlg != NULL && tray_icons[id_num].used == TRUE && tray_icons[id_num].visible == TRUE) {
		// Valid instance handle and icon resources?
		if (g_hinst != NULL && tray_icons[id_num].icon != NULL) {
			// Get icon handle
			HICON icon = tray_icons[id_num].icon->makeIcon(g_hinst);
			if (icon != NULL) {
				// Modify icon status
				if (bUseUnicode == 0) {
					TrayMessage(my_hDlg, NIM_MODIFY, id_num, icon, tray_icons[id_num].tooltip);
				} else {
					TrayMessageW(my_hDlg, NIM_MODIFY, id_num, icon, (jchar*)tray_icons[id_num].tooltip);
				}
			} else {
				last_error = TRAY_NOTENOUGHMEM;
			}
		} else {
			// Make icon invisible if no valid resources
			makeInvisible(id_num);
		}
	}
}

// Hide icon
void makeInvisible(jint id_num) {
	// Valid icon id and currently visible?
	if (tray_icons[id_num].used == TRUE && tray_icons[id_num].visible == TRUE) {
		// Make invisible
		if (my_hDlg != NULL) TrayMessage(my_hDlg, NIM_DELETE, id_num, NULL, NULL);
		tray_icons[id_num].visible = FALSE;
	}
}

// Add/Remove/Modify tray icon to system tray
BOOL TrayMessage(HWND hDlg, DWORD dwMessage, UINT uID, HICON hIcon, PSTR pszTip) {
	BOOL res;
	// Fill data struct for tray icon
	MY_NOTIFYICONDATA tnd;
	if (hShell32Version >= 5) {
		tnd.cbSize = sizeof(MY_NOTIFYICONDATA);
	} else {
		tnd.cbSize = sizeof(NOTIFYICONDATA);
		// NOTIFYICONDATA_V2_SIZE;
	}
	tnd.hWnd		= hDlg;
	tnd.uID			= uID;
	tnd.uFlags		= NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnd.uCallbackMessage	= MYWM_NOTIFYICON;
	tnd.hIcon		= hIcon;
	// Include tooltip?
	if (pszTip) {
		lstrcpyn(tnd.szTip, pszTip, sizeof(tnd.szTip));
		tnd.szTip[sizeof(tnd.szTip)-1] = 0;
	} else {
		tnd.szTip[0] = '\0';
	}
	tnd.dwState = 0;
	tnd.dwStateMask = 0;
	tnd.szInfo[0] = '\0';
	tnd.uTimeOrVersion = 10000;
	tnd.szInfoTitle[0] = '\0';
	tnd.dwInfoFlags = 0;
	// tnd.guidItem = 0;
	// Call tray icon windows API function
	res = Shell_NotifyIcon(dwMessage, (NOTIFYICONDATA*)&tnd);
	// Destroy the icon's handle (icon data is copied by Windows function)
	if (hIcon) DestroyIcon(hIcon);
	return res;
}

// Add/Remove/Modify tray icon to system tray
BOOL TrayMessageW(HWND hDlg, DWORD dwMessage, UINT uID, HICON hIcon, const jchar* pszTip) {
	BOOL res;
	// Fill data struct for tray icon
	MY_NOTIFYICONDATAW tnd;
	if (hShell32Version >= 5) {
		tnd.cbSize = sizeof(MY_NOTIFYICONDATAW);
	} else {
		tnd.cbSize = sizeof(NOTIFYICONDATAW);
		// NOTIFYICONDATA_V2_SIZEW;
	}
	tnd.hWnd		= hDlg;
	tnd.uID			= uID;
	tnd.uFlags		= NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnd.uCallbackMessage	= MYWM_NOTIFYICON;
	tnd.hIcon		= hIcon;
	// Include tooltip?
	if (pszTip) {
		copyJCharMax(pszTip, tnd.szTip, sizeof(tnd.szTip)/sizeof(jchar));
	} else {
		tnd.szTip[0] = 0;
	}
	tnd.dwState = 0;
	tnd.dwStateMask = 0;
	tnd.szInfo[0] = 0;
	tnd.uTimeOrVersion = 10000;
	tnd.szInfoTitle[0] = 0;
	tnd.dwInfoFlags = 0;
	// tnd.guidItem = 0;
	// Call tray icon windows API function
	res = Shell_NotifyIconW(dwMessage, (NOTIFYICONDATAW*)&tnd);
	// Destroy the icon's handle (icon data is copied by Windows function)
	if (hIcon) DestroyIcon(hIcon);
	return res;
}

bool TraySetVersion(HWND hDlg, UINT uID) {
	if (hShell32Version < 5) {
		// No balloon messages supported :-(
		return FALSE;	
	}
	// Fill data struct for tray icon
	MY_NOTIFYICONDATA tnd;
	tnd.cbSize              = sizeof(MY_NOTIFYICONDATA);
	tnd.hWnd		= hDlg;
	tnd.uID			= uID;
	tnd.uFlags		= 0;
	tnd.uCallbackMessage	= 0;
	tnd.hIcon		= NULL;
	tnd.szTip[0]            = '\0';
	tnd.dwState             = 0;
	tnd.dwStateMask         = 0;		
	tnd.szInfo[0]           = '\0';
	tnd.uTimeOrVersion      = NOTIFYICON_VERSION;
	tnd.szInfoTitle[0]      = '\0';
	tnd.dwInfoFlags         = 0;
	// tnd.guidItem = NULL;		
	// Call tray icon windows API function
	return Shell_NotifyIcon(NIM_SETVERSION, (NOTIFYICONDATA*)&tnd);
}

bool TrayBalloon(HWND hDlg, UINT uID, const char* msg, const char* title, UINT time, DWORD flags) {
	if (hShell32Version < 5) {
		// No balloon messages supported :-(
		return FALSE;	
	}
	// Fill data struct for tray icon
	MY_NOTIFYICONDATA tnd;
	tnd.cbSize              = 488; // sizeof(MY_NOTIFYICONDATA);
	tnd.hWnd		= hDlg;
	tnd.uID			= uID;
	tnd.uFlags		= NIF_INFO;
	tnd.uCallbackMessage	= 0;
	tnd.hIcon		= NULL;
	tnd.szTip[0]            = '\0';	
	tnd.dwState             = 0;
	tnd.dwStateMask         = 0;		
	if (msg) {
		lstrcpyn(tnd.szInfo, msg, sizeof(tnd.szInfo));
		tnd.szInfo[sizeof(tnd.szInfo)-1] = 0;
	} else {
		tnd.szInfo[0] = '\0';
	}
	tnd.uTimeOrVersion = time;
	if (title) {
		lstrcpyn(tnd.szInfoTitle, title, sizeof(tnd.szInfoTitle));
		tnd.szInfoTitle[sizeof(tnd.szInfoTitle)-1] = 0;
	} else {
		tnd.szInfoTitle[0] = '\0';
	}
	tnd.dwInfoFlags = flags;
	// tnd.guidItem = NULL;
	// Call tray icon windows API function
	return Shell_NotifyIcon(NIM_MODIFY, (NOTIFYICONDATA*)&tnd);
}

// Unicode version of TrayBalloon
bool TrayBalloonW(HWND hDlg, UINT uID, const jchar* msg, const jchar* title, UINT time, DWORD flags) {
	if (hShell32Version < 5) {
		// No balloon messages supported :-(
		return FALSE;	
	}
	// Fill data struct for tray icon
	MY_NOTIFYICONDATAW tnd;
	tnd.cbSize              = sizeof(MY_NOTIFYICONDATAW);
	tnd.hWnd		= hDlg;
	tnd.uID			= uID;
	tnd.uFlags		= NIF_INFO;
	tnd.uCallbackMessage	= 0;
	tnd.hIcon		= NULL;
	tnd.szTip[0]            = '\0';	
	tnd.dwState             = 0;
	tnd.dwStateMask         = 0;		
	if (msg) {
		copyJCharMax(msg, tnd.szInfo, sizeof(tnd.szInfo)/sizeof(jchar));
	} else {
		tnd.szInfo[0] = 0;
	}
	tnd.uTimeOrVersion = time;
	if (title) {
		copyJCharMax(title, tnd.szInfoTitle, sizeof(tnd.szInfoTitle)/sizeof(jchar));		
	} else {
		tnd.szInfoTitle[0] = 0;
	}
	tnd.dwInfoFlags = flags;
	// tnd.guidItem = NULL;
	// Call tray icon windows API function
	return Shell_NotifyIconW(NIM_MODIFY, (NOTIFYICONDATAW*)&tnd);
}

// Java VM callback function to delete a global reference to a given Java class
// Used to delete the global reference to the icon's class to receive mouse/menu events
int DeleteGlobalCallback(JNIEnv *env, int id_num, int dummy) {
	env->DeleteGlobalRef(tray_icons[id_num].globalClass);
	tray_icons[id_num].globalClass = 0;
	return 0;
}

jclass GetTrayIconClass(JNIEnv* env) {
	if (hGlobalWinTrayClass != 0) {
		jclass winTrayClass = env->GetObjectClass(hGlobalWinTrayClass);
		if (winTrayClass != 0) return winTrayClass;
	}
	MessageBox(NULL, "Error", "using FindClass", MB_OK);
	return env->FindClass("com/jeans/trayicon/WindowsTrayIcon");
}

// Java VM callback function used to notify icon class after incomming sendWindowsMessage()
int WindowsMessageCallback(JNIEnv *env, int dummy, int wParam) {
	// Get reference to WindowsTrayIcon Java class
	jclass cls = GetTrayIconClass(env);
	if (cls == 0) return -1;
	// Get static callback method id
	jmethodID mid = env->GetStaticMethodID(cls, "callWindowsMessage", "(I)I");
	if (mid == 0) return -1;
	// Make call to "callWindowsMessage" with parameter wParam
	return env->CallStaticIntMethod(cls, mid, (jint)wParam);
}

int MouseHookCallback(JNIEnv *env, int xp, int yp) {
	// Get reference to WindowsTrayIcon Java class
	jclass cls = GetTrayIconClass(env);
	if (cls == 0) return -1;
	// Get static callback method id
	jmethodID mid = env->GetStaticMethodID(cls, "callMouseHook", "(II)V");
	if (mid == 0) return -1;
	// Make call to "callMouseHook" with given coordinates
	env->CallStaticVoidMethod(cls, mid, (jint)xp, (jint)yp);
	return 0;
}

// Java VM callback function used for menu item callbacks
int MenuItemCallback(JNIEnv *env, int id_num, int menu_id) {
	// Valid icon id and valid global reference to icon's Java class?
	if (tray_icons[id_num].used == FALSE) return TRAY_WRONGICONID;
	jobject obj = tray_icons[id_num].globalClass;
	if (obj == 0) return TRAY_NOLISTENER;
	jclass winTrayClass = env->GetObjectClass(obj);
	if (winTrayClass == 0) return TRAY_NOTENOUGHMEM;
	// Get callback method id
	jmethodID mid = env->GetMethodID(winTrayClass, "notifyMenuListeners", "(I)V");
	if (mid == 0) return TRAY_METHODID;
	// Call method "notifyMenuListeners"
	env->CallVoidMethod(obj, mid, menu_id);
	return TRAY_NOERR;
}

MouseJavaCallback::MouseJavaCallback(int idnum, int button, int mask, POINT* pos) {
    m_Button = button; 
    m_Mask = mask;
    m_IDNum = idnum; 
    m_Pos.x = pos->x;
    m_Pos.y = pos->y;
}

MouseJavaCallback::MouseJavaCallback(int idnum, int button, int mask) {
    m_Button = button; 
    m_Mask = mask;
    m_IDNum = idnum;     
}

POINT* MouseJavaCallback::getPos() {
    return &m_Pos;
}

int MouseJavaCallback::execute(JNIEnv* env) {
	// Valid icon id and valid global reference to icon's Java class?
	if (tray_icons[m_IDNum].used == FALSE) return TRAY_WRONGICONID;
	jobject obj = tray_icons[m_IDNum].globalClass;
	if (obj == 0) return TRAY_NOLISTENER;
	jclass winTrayClass = env->GetObjectClass(obj);
	if (winTrayClass == 0) return TRAY_NOTENOUGHMEM;
	// Get callback method id
	jmethodID mid = env->GetMethodID(winTrayClass, "notifyMouseListeners", "(IIII)V");
	if (mid == 0) return TRAY_METHODID;
	// Call method "notifyMouseListeners"
	env->CallVoidMethod(obj, mid, m_Button, m_Mask, m_Pos.x, m_Pos.y);
	return TRAY_NOERR;
}

BalloonJavaCallback::BalloonJavaCallback(int idnum, int mask) {
    m_Mask = mask;
    m_IDNum = idnum; 
}

int BalloonJavaCallback::execute(JNIEnv* env) {
	// Valid icon id and valid global reference to icon's Java class?
	if (tray_icons[m_IDNum].used == FALSE) return TRAY_WRONGICONID;
	jobject obj = tray_icons[m_IDNum].globalClass;
	if (obj == 0) return TRAY_NOLISTENER;
	jclass winTrayClass = env->GetObjectClass(obj);
	if (winTrayClass == 0) return TRAY_NOTENOUGHMEM;
	// Get callback method id
	jmethodID mid = env->GetMethodID(winTrayClass, "notifyBalloonListeners", "(I)V");
	if (mid == 0) return TRAY_METHODID;
	// Call method "notifyMouseListeners"
	env->CallVoidMethod(obj, mid, m_Mask);
	return TRAY_NOERR;
}

// Call a Java method in a given virtual machine
int CallJavaVM(JavaVM* vm, ThreadJavaCallback* call) {
	int result = TRAY_NOERR;
	JNIEnv* env;
	// Attach current thread to given Java VM
	if (vm->AttachCurrentThread((void**) &env, NULL) < 0) return TRAY_ERRTHREAD;
	// Call method (MousePressedCallback/MenuItemCallback/WindowsMessageCallback/..)
	result = call->execute(env);
	// Check for exception detach thread and exit
	if (env->ExceptionOccurred()) env->ExceptionDescribe();
	vm->DetachCurrentThread();
	return result;
}

// Thread proc to call Java method (calls CallJavaVMS but with wrapped params)
void CallJavaThread(void *arg) {
	ThreadJavaCallback *tjc = (ThreadJavaCallback*)arg;
	int result = CallJavaVM(hJavaVM, tjc);
	if (result != TRAY_NOERR) last_error = result;
	delete tjc;
}

// Call a Java method in a new thread
void CallJavaVMSThread(ThreadJavaCallback* tjc) {
	if (_beginthread(CallJavaThread, 0, tjc) == -1) delete tjc;
}

SimpleJavaCallback::SimpleJavaCallback(JNIProcPtr(jniproc), int arg1, int arg2) {
    m_JNIProc = jniproc;
    m_Arg1 = arg1;
    m_Arg2 = arg2;
}

int SimpleJavaCallback::execute(JNIEnv* env) {
    return m_JNIProc(env, m_Arg1, m_Arg2);
}

// Handle popup menu command
void HandleMenuCommand(WPARAM menuId) {
	// Get icon id given menu id
	int id_num = getMenuItemIdNum(menuId);
	// Callback to Java class in new thread, using method "MenuItemCallback()"
	SimpleJavaCallback* call = new SimpleJavaCallback(MenuItemCallback, id_num, menuId);
	CallJavaVMSThread(call);
}

// Handle icon mouse event (left/right button)
void HandleNotifyIcon(WPARAM id_num, LPARAM lParam) {	
    if (lParam == WM_RBUTTONDOWN || lParam == NIN_KEYSELECT || lParam == NIN_SELECT || lParam == WM_CONTEXTMENU) {
		if (id_num >= 0) {
		    POINT mouse_pos;
			GetCursorPos(&mouse_pos);				
			// If menu defined, then show it
			if (tray_icons[id_num].popup != NULL) {
				// Display and track the popup menu
				PopupMenu *menu = tray_icons[id_num].popup;
				menu->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON,&mouse_pos,my_hDlg);
			} else {
				// Callback Java class in new thread using "MousePressedCallback()"
		                MouseJavaCallback* call = new MouseJavaCallback(id_num, 1, 0, &mouse_pos);
				CallJavaVMSThread(call);					
			}
		}
    } else {
		int button = -1;
        	int mask = 0;
	        switch (lParam)	{
		    case WM_LBUTTONDOWN:
		        button = 0; break;
		    case WM_MBUTTONDOWN:
		        button = 2; break;
		    case WM_LBUTTONUP:
		        button = 0; mask |= MOUSE_BTN_UP; break;
		    case WM_MBUTTONUP:
		        button = 2; mask |= MOUSE_BTN_UP; break;
		    case NIN_KEYSELECT:
		    case NIN_SELECT:
		    case WM_CONTEXTMENU:
	            case WM_RBUTTONUP:
		        button = 1; mask |= MOUSE_BTN_UP; break;
	            case WM_LBUTTONDBLCLK:
        	        button = 0; mask |= MOUSE_BTN_DOUBLE; break;
	            case WM_RBUTTONDBLCLK:
	                button = 1; mask |= MOUSE_BTN_DOUBLE; break;
        	    case WM_MBUTTONDBLCLK:
	                button = 2; mask |= MOUSE_BTN_DOUBLE; break;
	            case NIN_BALLOONSHOW:
	            	button = 3; mask |= BALLOON_MSG_SHOW; break;
	            case NIN_BALLOONHIDE:
			button = 3; mask |= BALLOON_MSG_HIDE; break;
	            case NIN_BALLOONTIMEOUT:
			button = 3; mask |= BALLOON_MSG_TIMEOUT; break;	            
	            case NIN_BALLOONUSERCLICK:
			button = 3; mask |= BALLOON_MSG_CLICK; break;	                
        	}
		if (id_num >= 0 && button != -1) {
			if (button == 3) {
				BalloonJavaCallback* call = new BalloonJavaCallback(id_num, mask);
				CallJavaVMSThread(call);				
			} else {
				MouseJavaCallback* call = new MouseJavaCallback(id_num, button, mask);
				GetCursorPos(call->getPos());
				CallJavaVMSThread(call);
			}
		}        
    }	
}

void ReAddAllIcons() {
	for (int id_num = 0; id_num < MY_MAX_ICONS; id_num++) {
		if (tray_icons[id_num].visible == TRUE) {
			showIcon(id_num);
		}
	}
}

int HandleAppTalk(int value) {
	SimpleJavaCallback call(WindowsMessageCallback, 0, value);
	return CallJavaVM(hJavaVM, &call);
}

int HandleAppMouse(int xp, int yp) {
	SimpleJavaCallback call(MouseHookCallback, xp, yp);
	return CallJavaVM(hJavaVM, &call);
}

// Windows message proc for hidden window
// Receives mouse/menu events for the apps tray icons
LONG APIENTRY WndProc(HWND hWnd, UINT iMessage, UINT wParam, LONG lParam) {
	switch (iMessage) {
		case WM_DESTROY:
			// Exit message thread
			PostQuitMessage(0) ;
			break;
		case MYWM_NOTIFYICON:
			// Mouse event for icon (left/right button)
			HandleNotifyIcon(wParam, lParam);
			break;
		case MYWM_APPTALK:
			// ID used for sendWindowsMessage()
			// Callback Java class using "WindowsMessageCallback()"
			return HandleAppTalk((int)lParam);
		case MYWM_APPMOUSE:
			// Handle mouse message (from global hook)
			return HandleAppMouse((int)wParam, (int)lParam);
		case WM_COMMAND:
			// Menu command for icon
			HandleMenuCommand(LOWORD(wParam));
			break;		
			// Get notified if task bar restarts
		case WM_CREATE:
            		g_TaskbarRestart = RegisterWindowMessage(TEXT("TaskbarCreated"));
			break;
		// Do nothing for other messages		    
		case WM_GETMINMAXINFO:
		case WM_PAINT:
		case WM_MOUSEMOVE:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MOVE:
		case WM_SIZE:
		case WM_LBUTTONDOWN:
		case WM_KEYDOWN:
			break;
		    // Or use default handler
		default:
			if (iMessage == g_TaskbarRestart) ReAddAllIcons();
			else return DefWindowProc (hWnd, iMessage, wParam, lParam) ;
	}
	return 0L ;
}

// Create the hidden window to receive the icon's mouse messages
HWND MakeHWND(HINSTANCE hInst) {
	HWND hWnd;
	// Create window class
	WNDCLASS wndclass;
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = (WNDPROC) WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = hInst;
	wndclass.hIcon         = LoadIcon(hInst, IDI_APPLICATION);
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = szAppName;
	if (!RegisterClass(&wndclass)) return NULL;
	// Create window
	hWnd = CreateWindow(szAppName, szWndName, WS_OVERLAPPEDWINDOW,
                          0,0,100,100,
                          NULL, NULL, hInst, NULL);
	return hWnd;
}

// Remove the hidden window (on app closing)
void RemoveHWND() {
	// Close dial
	if (my_hDlg != NULL) {
		// Create wait event
		exit_event = CreateEvent(NULL,FALSE,FALSE,NULL);
		// Send destroy messages
		PostMessage(my_hDlg, WM_NCDESTROY, 0, 0);
		PostMessage(my_hDlg, WM_DESTROY, 0, 0);
		// Wait for window to destroy
		WaitForSingleObject(exit_event,10000);
		CloseHandle(exit_event);
		exit_event = NULL;
	}
	// Free handle
	if (wait_event != NULL) {
		CloseHandle(wait_event);
		wait_event = NULL;
	}
}

// The thread proc that creates the hidden window an listens for messages
void DialogThread(void *dummy) {
	MSG msg;
	// Create window
	my_hDlg = MakeHWND(g_hinst);
	// Signal wait event
	if (wait_event != NULL) SetEvent(wait_event);
	// Hide window
	ShowWindow(my_hDlg, SW_HIDE);
	UpdateWindow(my_hDlg);
	// Process messages
	while (GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg) ;
		DispatchMessage(&msg) ;
	}
	// Unregister window class
	UnregisterClass(szAppName, g_hinst);
	// Signal exit event (so app knows hidden window is destroyed)
	if (exit_event != NULL) SetEvent(exit_event);
}

JNIEXPORT void JNICALL Java_com_jeans_trayicon_WindowsTrayIcon_setAlwaysOnTop(JNIEnv *env, jclass cls, jobject win, jboolean onTop) {
    DoAlwaysOnTop top;
    doWithHWND(&top, env, win);
}

int DoAlwaysOnTop::execute(HWND hwnd) {
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
    return 0;
}

void doWithHWND(DoWithHWND* hwndProc, JNIEnv* env, jobject window) {    
    if (hJAWT == NULL) {
        last_error = TRAY_NO_JAWT;
        return;
    }
    PJAWT_GETAWT JAWT_GetAWT = (PJAWT_GETAWT)GetProcAddress(hJAWT, "_JAWT_GetAWT@8");
    if (JAWT_GetAWT == NULL) {
        last_error = TRAY_NO_GET_JAWT;
        return;
    }
    JAWT awt;
    awt.version = JAWT_VERSION_1_3;
    jboolean result = JAWT_GetAWT(env, &awt);
    if (result == JNI_FALSE) {
        last_error = TRAY_GET_JAWT_FAILS;
        return;
    }    
    JAWT_DrawingSurface* ds = awt.GetDrawingSurface(env, window);
    if (ds == NULL) {
        last_error = TRAY_JAWT_DS_FAILS;
        return;
    }    
    jint lock = ds->Lock(ds);
    if ((lock & JAWT_LOCK_ERROR) == 0) {                                                                                     
        JAWT_DrawingSurfaceInfo* dsi = ds->GetDrawingSurfaceInfo(ds);
        JAWT_Win32DrawingSurfaceInfo* dsi_win = (JAWT_Win32DrawingSurfaceInfo*)dsi->platformInfo;
        hwndProc->execute(dsi_win->hwnd);
        ds->FreeDrawingSurfaceInfo(dsi);
        ds->Unlock(ds);                                                            
    } else {
        last_error = TRAY_JAWT_LOCK_FAILS;
    }
    awt.FreeDrawingSurface(ds);                                                      
}  

void RemoveJAWT() {
    if (hJAWT != NULL) {
        FreeLibrary(hJAWT);
        hJAWT = NULL;
    }
}

void RemoveHook() {
	if (hMouseClickHook != NULL) {
		UnhookWindowsHookEx(hMouseClickHook);
		hMouseClickHook = NULL;
	}
}

void GetJAWTHandle() {
	if (hJAWTTried == 0 && hJAWT == NULL) {
		hJAWTTried = 1;
		hJAWT = LoadLibrary("jawt.dll");
		if (hJAWT == NULL) {
			char* jawt_path = find_in_path("../jre/bin/jawt.dll");
			if (jawt_path != NULL) hJAWT = LoadLibrary(jawt_path);
		}
		if (hJAWT == NULL) {
			last_error = TRAY_NO_JAWT;
		}
	}
}

// New icon resource with no bitmap and zero size
IconData::IconData() {
	wd = hi = 0;
	hBitmapAND = hBitmapXOR = NULL;
}

// Destroy icon resources (and and xor bitmap)
IconData::~IconData() {
	if (hBitmapAND != NULL) DeleteObject(hBitmapAND);
	if (hBitmapXOR != NULL) DeleteObject(hBitmapXOR);
}

// Make icon using and and xor bitmaps (returns handle to new icon)
HICON IconData::makeIcon(HINSTANCE hInst) {
        ICONINFO ii;
        ii.fIcon    = TRUE;
        ii.xHotspot = 0;
        ii.yHotspot = 0;
        ii.hbmMask  = hBitmapAND;
        ii.hbmColor = hBitmapXOR;
        return CreateIconIndirect(&ii);
}

// Set icon's image data (pixel array, width, height)
int IconData::setData(unsigned long *data, int wd, int hi) {
	int res = -1;
	// Set size
	this->wd = wd;
	this->hi = hi;
	// Clean up if setData was called before
	if (hBitmapAND != NULL) DeleteObject(hBitmapAND);
	if (hBitmapXOR != NULL) DeleteObject(hBitmapXOR);
	// To protect against java sending a dud image
	if (wd > 0 && hi > 0) {
		    // Set up the header for creating our 24 bit colour bitmap
	    	BITMAPINFOHEADER bih;
		    bih.biSize          = sizeof(BITMAPINFOHEADER);
	    	bih.biWidth         = wd;
		    bih.biHeight        = hi;
		    bih.biPlanes        = 1;
	    	bih.biBitCount      = 24;
	    	bih.biCompression   = BI_RGB;
	    	bih.biSizeImage     = 0;
	    	bih.biXPelsPerMeter = 0;
	    	bih.biYPelsPerMeter = 0;
	    	bih.biClrUsed       = 0;
	    	bih.biClrImportant  = 0;
	    	// Create memory DC
	    	HDC hdc = CreateCompatibleDC(NULL);
	    	// Make the 24-bit DIB
	    	hBitmapXOR = CreateDIBSection(hdc, (LPBITMAPINFO)&bih, DIB_RGB_COLORS, (LPVOID *)NULL, NULL, 0);
	    	// Select it into the DC so we can draw onto it
	    	SelectObject(hdc, hBitmapXOR);
	    	// Calloc memory to be used to create a monochrome bitmask
		long size = (wd*hi/8)+1;
	    	unsigned char *andMask = new unsigned char[size];
		if (andMask != NULL) {
			    for (int i = 0; i < size; i++) andMask[i] = 0;
				// Loop through the given pixels and draw onto the colour and mono bitmaps
				unsigned long pixel;
		        	unsigned char red, green, blue, alpha;
		        	for (int row = 0; row < hi; row++) {
    			    	for (int col = 0; col < wd; col++) {
	            			pixel = data[(row*wd)+col];
		            		alpha = (unsigned char)((pixel >> 24) & 0x000000ff);
		            		red   = (unsigned char)((pixel >> 16) & 0x000000ff);
	                    	green = (unsigned char)((pixel >>  8) & 0x000000ff);
	                    	blue  = (unsigned char)( pixel        & 0x000000ff);
	        		    	if (alpha == 0xFF) {
						        // Pixel is not transparent - update xor bitmap
						        SetPixel(hdc, col, row, RGB(red, green, blue));
					        } else {
						        // Pixel is transparent - update and mask
		        			    int p = (row*wd) + col;
				        	    andMask[p/8] |= 1 << (7-(p%8));
						        SetPixel(hdc, col, row, RGB(0, 0, 0));
					    }
				    }
			    }
			    // Create the monochrome bitmask with transparency info
			    hBitmapAND = CreateBitmap(wd, hi, 1, 1, andMask);
			    // Free memory
			    delete andMask;
			    // Return code
			    res = 0;
		}
		// Release the memory DC
		DeleteDC(hdc);
	}
	// Error on zero size icons
	return res;
}

// Find free menu id for submenu item (given icon id = owner)
// Each tray icon with a submenu has a number of menu ids, one for each menu item
long getFreeMenuId(int id_num) {
	long size = arrUsedMenuIds->getSize();
	long id = 0;
	BOOL done = FALSE;
	// Find for free id in array that maps menu ids to icon ids
	for (id = 0; id < size && !done; id++)
		if (arrUsedMenuIds->getElementAt(id) == -1) done = TRUE;
	// Found none, create new id
	if (!done) {
		id = size;
		arrUsedMenuIds->addElement(-1);
	}
	// Mark id in array as belonging to icon with id id_num
	arrUsedMenuIds->setElementAt(id, id_num);
	// Return id for new submenu item
	return id;
}

// Free all menu ids for a given icon
void setFreeMenuId(int id_num) {
	long size = arrUsedMenuIds->getSize();
	// Find ids for given icon and free them
	for (long ps = 0; ps < size; ps++) {
		if (arrUsedMenuIds->getElementAt(ps) == id_num)
			arrUsedMenuIds->setElementAt(ps, -1);
	}
	// Shrink array if there are trailing empty places
	long delofs = arrUsedMenuIds->getSize();
	while (delofs > 0 && arrUsedMenuIds->getElementAt(delofs-1) == -1) {
		delofs--;
		arrUsedMenuIds->removeElementAt(delofs);
	}
}

// Map menu id to icon id
int getMenuItemIdNum(int id) {
	long size = arrUsedMenuIds->getSize();
	if (id < size) return arrUsedMenuIds->getElementAt(id);
	else return -1;
}

// Make new popup menu
PopupSubMenu::PopupSubMenu() {
	hMenu = CreatePopupMenu();
	bSub = FALSE;
}

// Make new popup menu (reset all class members)
void PopupSubMenu::reNewMenu() {
	if (!isSub()) DestroyMenu(hMenu);
	hMenu = CreatePopupMenu();
	bSub = FALSE;
}

// Destroy popupmenu
PopupSubMenu::~PopupSubMenu() {
	if (!isSub()) DestroyMenu(hMenu); // Only needed when parent menu, not for submenus
}

// Is this a submenu or a parent popup menu
BOOL PopupSubMenu::isSub() {
	return bSub;
}

// Mark this popup menu as being a submenu
void PopupSubMenu::makeSub() {
	bSub = TRUE;
}

// Get the windows menu handle, used for AppendMenu,..
HMENU PopupSubMenu::getMenu() {
	return hMenu;
}

// Display the popupmenu at given position (in neighbourhood of tray icon)
void PopupSubMenu::TrackPopupMenu(UINT flags, POINT* pos, HWND hWnd) {
	// SetForegroundWindow/PostMessage are hacks because TrackPopupMenu contains bug/feature
	// Otherwise it's not hidden properly when the user clicks on desktop
	SetForegroundWindow(hWnd);
	::TrackPopupMenu(hMenu, flags, (*pos).x, (*pos).y, 0, hWnd, NULL);
	PostMessage(hWnd, WM_NULL, 0, 0);
}

// Make new popup main menu with maximum submenu depth
PopupMenu::PopupMenu(int levels) {
	nbLevels = levels;
	popup = new PtrPopupSubMenu[levels];
	for (int ctr = 0; ctr < levels; ctr++) popup[ctr] = NULL;
}

// Destroy main popup menu
PopupMenu::~PopupMenu() {
	// Destroy all submenu levels
	for (int ctr = 0; ctr < nbLevels; ctr++)
		if (popup[ctr] != NULL) delete popup[ctr];
	delete popup;
}

// Init menu at given level (make new or call reNewMenu())
void PopupMenu::initMenu(int level) {
	if (level >= nbLevels) return;
	if (popup[level] != NULL) popup[level]->reNewMenu();
	else popup[level] = new PopupSubMenu();
}

// Return windows menu handle for given level (should be 0 for main menu)
HMENU PopupMenu::getMenu(int level) {
	PopupSubMenu *menu = getSubMenu(level);
	if (menu == NULL) return NULL;
	else return menu->getMenu();
}

// Return sub menu @ given level (used in getMenu()/TrackPopupMenu())
PopupSubMenu* PopupMenu::getSubMenu(int level) {
	if (level >= nbLevels) return NULL;
	return popup[level];
}

// Display the popupmenu at given position (in neighbourhood of tray icon)
void PopupMenu::TrackPopupMenu(UINT flags, POINT* pos, HWND hWnd) {
	PopupSubMenu *menu = getSubMenu(0);
	if (menu != NULL) menu->TrackPopupMenu(flags, pos, hWnd);
}

// Create new growable integer array
QSIntArray::QSIntArray() {
	m_Array = new int[5];
	m_ArrSize = 5;
	m_Size = 0;
	m_Grow = 10;
}

// Create new growable integer array with initial size and grow term
QSIntArray::QSIntArray(long size, long grow) {
	m_Array = new int[size];
	m_ArrSize = size;
	m_Size = 0;
	m_Grow = grow;
}

// Delete growable integer array
QSIntArray::~QSIntArray() {
	delete m_Array;
}

// Get size
long QSIntArray::getSize() {
	return m_Size;
}

// Get integer @ given position
int QSIntArray::getElementAt(long idx) {
	return m_Array[idx];
}

// Add integer at end of array
void QSIntArray::addElement(int element) {
	m_Size++;
	grow();
	m_Array[m_Size-1] = element;
}

// Set integer @ given position (may be >= getSize() - array grows)
void QSIntArray::setElementAt(long idx, int element) {
	m_Size = max(m_Size, idx+1);
	grow();
	m_Array[idx] = element;
}

// Remove integer at given position
void QSIntArray::removeElementAt(long idx) {
	m_Size--;
	for (long pos = idx; pos < m_Size; pos++)
		m_Array[pos] = m_Array[pos+1];
	shrink();
}

// Empty array
void QSIntArray::removeAll() {
	m_Size = 0;
	shrink();
}

// Shrink array to convenient size
void QSIntArray::shrink() {
	if (m_Size < m_ArrSize - m_Grow) {
		long nSize = m_Size + m_Grow;
		int* nArray = new int[nSize];
		for (long idx = 0; idx < m_Size; idx++)
			nArray[idx] = m_Array[idx];
		delete m_Array;
		m_Array = nArray;
		m_ArrSize = nSize;
	}
}

// Grow array to convenient size
void QSIntArray::grow() {
	if (m_Size > m_ArrSize) {
		long nSize = m_Size + m_Grow;
		int* nArray = new int[nSize];
		for (long idx = 0; idx < m_ArrSize; idx++)
			nArray[idx] = m_Array[idx];
		delete m_Array;
		m_Array = nArray;
		m_ArrSize = nSize;
	}
}

// Check if path ends with '/' or '\\'
int has_final_slash(const char *path, int pos) {
	if (pos == -1) pos = strlen(path);
	if (pos > 0) return path[pos-1] == '/' || path[pos-1] == '\\';
	else return 0;
}

// Append path separator (if not yet there)
void append_final_slash(char *path) {
	int len = strlen(path);
	if (!has_final_slash(path, len)) {
		path[len] = '\\';
		path[len+1] = 0;
	}
}

// Convert all slashes to OS dependent slashes
void convert_slashes(char *path) {
	int index = 0;
	while (path[index] != 0) {
		if (path[index] == '/' || path[index] == '\\') 
			path[index] = '\\';
		index++;
	}
}

// Find string between path separators
const char* strstr_path_delim(const char* str1, const char* str2) {
	int idx1 = 0, idx2 = 0, len = 0;
	while (str1[idx1] != 0) {
		if (str1[idx1] == str2[idx2]) {
			idx2++;
			if (str2[idx2] == 0) {
				if (len == 0) len = strlen(str2);                
				if ((str1[idx1-len] == 0 || str1[idx1-len] == '\\') &&
				    (str1[idx1+1] == 0 || str1[idx1+1] == '\\')) {
					return str1 + idx1 - len + 1;
				} else {
					idx2 = 0;
				}
			}
		} else {
			idx2 = 0;
		}
		idx1++;
	}
	return NULL;
} 

// Remove all '..' and '.' from path, convert all slashes to OS dependent slashes
void make_canonical_path(char *path) {
	const char *find;
	int pos, from, ctr, ch;
	convert_slashes(path);
	while ((find = strstr_path_delim(path, "..")) != NULL) {
		from = pos = find - path - 2;
		if (pos > 0) {
			ctr = 0;
			while (from > 0 && path[from] != '\\') from --;			
			while ((ch = path[pos+ctr+5]) != 0) {
				path[from+ctr+1] = ch;
				ctr++;
			}
			path[from+ctr+1] = 0;
		} else {
			break;
		}		
	}	
	while ((find = strstr_path_delim(path, ".")) != NULL) {
		pos = find - path;
		if (pos >= 0) {
		    	ctr = path[pos+1] == '\\' ? 2 : 1;
		    	while (path[pos] != 0) {
		            	path[pos] = path[pos+ctr];
		            	pos++;
	            	}
		} else {
		    	break;
		}		
	}
}

int aid_file_exists(char* fname) {
	WIN32_FIND_DATA find_data;
	HANDLE hFind = FindFirstFile(fname, &find_data);
	if (hFind == INVALID_HANDLE_VALUE) return 0;
	FindClose(hFind);
	return 1;
}

const char* search_path_next_entry(const char* spath, int* idx, int* len) {
	int midx = *idx, oidx = *idx;
    	if (spath[midx] == 0) return NULL;
    	while (spath[midx] != 0 && spath[midx] != ';') midx++;
    	*len = midx - oidx + 1;
    	if (spath[midx] == ';') {
        	*len = midx - oidx;
        	midx++;
    	} else {
        	*len = midx - oidx + 1;
    	}
    	*idx = midx;
	return spath + oidx;
}

char* find_in_path(const char* append) {
	char *origpath;
	int offset = 0, length;	
	int max_size = 0, spathmax = 0, spathidx = 0;		
	const char *find, *spath = NULL;
	if ((spath = getenv("PATH")) != NULL) {
	        while ((find = search_path_next_entry(spath, &spathidx, &length)) != NULL) {
			    if (spathmax < length) spathmax = length;
	        }
		max_size = strlen(append) + spathmax + 10;
		origpath = (char*)malloc(max_size+1);		
		if (origpath == NULL) return NULL;
        	spathidx = 0;
		while ((find = search_path_next_entry(spath, &spathidx, &length)) != NULL) {
			strncpy(origpath, find, length);
			origpath[length] = 0;
			append_final_slash(origpath);			
			strcat(origpath, append);
			make_canonical_path(origpath);
			if (aid_file_exists(origpath)) return origpath;
		}	        
	}
	return NULL;
}

BOOL GetOSName(char* osname, int* version) {
   OSVERSIONINFOEX osvi;
   BOOL bOsVersionInfoEx;
   *version = TRAY_WIN_VER_UNKNOWN;
   osname[0] = 0;
   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
   if(!(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi))) {
      osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
      if (!GetVersionEx ( (OSVERSIONINFO *) &osvi)) return FALSE;
   }
   switch (osvi.dwPlatformId) {
      case VER_PLATFORM_WIN32_NT:
         if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 ) {
		strcpy(osname, "Windows 2000");
		*version = TRAY_WIN_VER_2K;
         } else if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 ) {
		strcpy(osname, "Windows XP");
		*version = TRAY_WIN_VER_XP;
         } else if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 ) {
		strcpy(osname, "Windows .NET");
		*version = TRAY_WIN_VER_NET;
         } else {
		sprintf(osname, "Windows NT %ld.%ld", osvi.dwMajorVersion, osvi.dwMinorVersion);
		*version = TRAY_WIN_VER_NT;
	 }
         break;
      case VER_PLATFORM_WIN32_WINDOWS:
         if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0) {
		strcpy(osname, "Windows 95");
		*version = TRAY_WIN_VER_95;
		if (osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B') strcat(osname, " OSR2");
         } else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10) {
		strcpy(osname, "Windows 98");
		*version = TRAY_WIN_VER_98;
		if ( osvi.szCSDVersion[1] == 'A' ) strcat(osname, " SE");
         } else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90) {
		strcpy(osname, "Windows ME");
		*version = TRAY_WIN_VER_ME;
         } else {
		sprintf(osname, "Windows Win32 %ld.%ld", osvi.dwMajorVersion, osvi.dwMinorVersion);
		*version = TRAY_WIN_VER_WIN32;
	 }
         break;
   }
   return TRUE;
}

int TrayGetWindowsVersion() {
	int osversion;
	char osname[300];
	GetOSName(osname, &osversion);
	return osversion;
}

int GetShell32VerionHelp(DLLVERSIONINFO* version) {
	HMODULE hShell = LoadLibrary("Shell32.dll");
	if (hShell == NULL) {
		return 0;
	}
	DLLGETVERSIONPROC proc = (DLLGETVERSIONPROC)GetProcAddress(hShell, "DllGetVersion");
	if (proc == NULL) {
		FreeLibrary(hShell);
		return 0;
	}
	int result = proc(version) == NOERROR ? 1 : 0;
	FreeLibrary(hShell);
	return result;
}

DWORD GetShell32Version() {
	DLLVERSIONINFO version;
	version.cbSize = sizeof(DLLVERSIONINFO);
	if (GetShell32VerionHelp(&version) == 1) {
		return version.dwMajorVersion;
	} else {
		int osversion;
		char osname[300];
		GetOSName(osname, &osversion);
		if (osversion >= TRAY_WIN_VER_2K) {
			return 5;
		} else {
			return 4;
		}
	}
}

int initUnicodeFunctions() {
	// Disable all Unicode conversion
	for (int i = 0; i < UNICODE_CONVERSION_NB; i++) {
		g_UnicodeConversion[i] = 0;
	}	
	// Get maximum number of bytes for each wide char
	CPINFO cp_info;
	UINT acp = GetACP();
	if (GetCPInfo(acp, &cp_info)) {
		nbWideMaxChars = cp_info.MaxCharSize;
	}
	// What to do with Unicode functions?
	if (hWindowsVersion < TRAY_WIN_VER_2K) {
		for (int i = 0; i < UNICODE_CONVERSION_NB; i++) {
			g_UnicodeConversion[i] = 1;
		}
		g_UnicodeConversion[UNICODE_CONVERSION_SUPPORT] = 0;
		bUseUnicode = 0;
	} else {
		g_UnicodeConversion[UNICODE_CONVERSION_SUPPORT] = 1;
		bUseUnicode = 1;
	}
	// Use conversion for balloon in NT
	if (hWindowsVersion == TRAY_WIN_VER_2K) {
		g_UnicodeConversion[UNICODE_CONVERSION_BALLOON] = 1;
	}
	return 0;
}
