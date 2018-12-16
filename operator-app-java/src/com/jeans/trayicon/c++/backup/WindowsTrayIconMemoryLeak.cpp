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

#include <windows.h>
#include <windowsx.h>
#include <process.h>
#include <jni.h>
#include "jeans_trayicon_WindowsTrayIcon.h"

// Undefine this to disable Java VM callback
#define HAS_JVM_LIB
// #define USE_JVM_HINSTANCE

// Message ID for sendWindowsMessage() function
#define MYWM_APPTALK		(WM_APP+10)
// Message ID for Tray icons
#define MYWM_NOTIFYICON		(WM_APP+100)
// Max number of icons supported by this DLL for each application
#define MY_MAX_ICONS		100
// Max length for the name of the hidden window (used for sendWindowsMessage/isRunning)
#define WNDNAME_MAX		20

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

// Information for Java callback
typedef struct {
	JNIProcPtr(jni_proc);
	int arg1, arg2;
} ThreadJavaCallback;

// Some error codes
#define TRAY_NOERR		0
#define TRAY_NOTIFYPROCERR	-1
#define TRAY_TOOMANYICONS	-2
#define TRAY_NOTENOUGHMEM	-3
#define TRAY_WRONGICONID	-4
#define TRAY_DLLNOTFOUND	-5
#define TRAY_NOVMS		-6
#define TRAY_ERRTHREAD		-7
#define TRAY_METHODID		-8
#define TRAY_NOLISTENER		-9
#define TRAY_JNIERR		-10
#define TRAY_CALLBACKDLLERR	-11

// Constants for the popup menu system
#define POPUP_TYPE_ITEM       0
#define POPUP_TYPE_SEPARATOR  1
#define POPUP_TYPE_CHECKBOX   2
#define POPUP_TYPE_INIT_LEVEL 3
#define POPUP_TYPE_DONE_LEVEL 4

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

#ifdef USE_JVM_HINSTANCE
	// Instance of native callback DLL
	HINSTANCE hInstNative = NULL;
#endif

// Get free menu id (for icon id - see arrUsedMenuIds)
long getFreeMenuId(int id_num);
// Free menu id (when removing menu from icon)
void setFreeMenuId(int id_num);
// Map menu id to icon id
int getMenuItemIdNum(int id);
// Add/Remove/Modify tray icon to system tray
BOOL TrayMessage(HWND hDlg, DWORD dwMessage, UINT uID, HICON hIcon, PSTR pszTip);
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
// Call a Java method in all available VM's (used for mouse/menu callbacks)
int CallJavaVMS(JNIProcPtr(JNIProc), int arg1, int arg2);
// Call a Java method in a given virtual machine
int CallJavaVM(JavaVM* vm, JNIProcPtr(JNIProc), int arg1, int arg2);

/*
 * Class:     jeans_trayicon_WindowsTrayIcon
 * Method:    getLastError
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jeans_trayicon_WindowsTrayIcon_getLastError(JNIEnv *, jclass) {
	int err = last_error;
	last_error = TRAY_NOERR;
	return last_error;		// Return the last error
}

/*
 * Class:     WindowsTrayIcon
 * Method:    getFreeId
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jeans_trayicon_WindowsTrayIcon_getFreeId(JNIEnv *, jclass) {
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
	last_error = TRAY_NOTIFYPROCERR;
	return TRAY_TOOMANYICONS;
}

/*
 * Class:     WindowsTrayIcon
 * Method:    setIconData
 * Signature: (III[I)V
 */
JNIEXPORT void JNICALL Java_jeans_trayicon_WindowsTrayIcon_setIconData(JNIEnv *env, jclass, jint id_num, jint wd, jint hi, jintArray array) {
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
		if (tray_icons[id_num].icon != NULL) delete tray_icons[id_num].icon;
		tray_icons[id_num].icon = data;
		updateIcon(id_num);
	} else {
		delete data;
		last_error = TRAY_NOTENOUGHMEM;
	}
	// Release java array
	env->ReleaseIntArrayElements(array, body, 0);
}

/*
 * Class:     WindowsTrayIcon
 * Method:    showIcon
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_jeans_trayicon_WindowsTrayIcon_showIcon(JNIEnv *, jclass, jint id_num, jboolean show) {
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
		// Show icon in windows system tray
		if (tray_icons[id_num].visible == FALSE && tray_icons[id_num].icon != NULL) {
			HICON icon = tray_icons[id_num].icon->makeIcon(g_hinst);
			if (icon != NULL) {
				TrayMessage(my_hDlg, NIM_ADD, id_num, icon, tray_icons[id_num].tooltip);
				tray_icons[id_num].visible = TRUE;
			} else {
				last_error = TRAY_NOTENOUGHMEM;
			}
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
JNIEXPORT jint JNICALL Java_jeans_trayicon_WindowsTrayIcon_testVisible(JNIEnv *, jclass, jint id_num) {
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
JNIEXPORT void JNICALL Java_jeans_trayicon_WindowsTrayIcon_clickEnable(JNIEnv *env, jclass, jobject ic_cls, jint id_num, jboolean enable) {
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
JNIEXPORT void JNICALL Java_jeans_trayicon_WindowsTrayIcon_setToolTip(JNIEnv *env, jclass, jint id_num, jstring tip) {
	// Icon id valid?
	if (tray_icons[id_num].used == FALSE) {
		last_error = TRAY_WRONGICONID;
		return;
	}
	// Get java string for tooltip
	const char *tooltip = env->GetStringUTFChars(tip, 0);
	int len = strlen(tooltip);
	// If already exists delete first..
	if (tray_icons[id_num].tooltip != NULL)	delete tray_icons[id_num].tooltip;
	tray_icons[id_num].tooltip = new char[len+1];
	strcpy(tray_icons[id_num].tooltip, tooltip);
	// Release java string
	env->ReleaseStringUTFChars(tip, tooltip);
	updateIcon(id_num);
}

/*
 * Class:     WindowsTrayIcon
 * Method:    freeIcon
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_jeans_trayicon_WindowsTrayIcon_freeIcon(JNIEnv *env, jclass, jint id_num) {
	freeIcon(env, id_num);
}

/*
 * Class:     jeans_trayicon_WindowsTrayIcon
 * Method:    initTrayIcon
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_jeans_trayicon_WindowsTrayIcon_initTrayIcon(JNIEnv *env, jclass, jstring wndName) {
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
#ifdef USE_JVM_HINSTANCE
		//Try to load Native part
		hInstNative = LoadLibrary("npjava32");
#endif
	}
}

/*
 * Class:     jeans_trayicon_WindowsTrayIcon
 * Method:    isRunning
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT jboolean JNICALL Java_jeans_trayicon_WindowsTrayIcon_isRunning(JNIEnv *env, jclass, jstring wndName) {
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
JNIEXPORT jint JNICALL Java_jeans_trayicon_WindowsTrayIcon_sendWindowsMessage
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
 * Method:    cleanUp
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jeans_trayicon_WindowsTrayIcon_cleanUp(JNIEnv *env, jclass) {
	cleanUpExit(env);
}

/*
 * Class:     jeans_trayicon_WindowsTrayIcon
 * Method:    initPopup
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_jeans_trayicon_WindowsTrayIcon_initPopup
  (JNIEnv *env, jclass, jint id_num, jint nbLevels) {
	// Icon id valid?
	if (tray_icons[id_num].used == FALSE) {
		last_error = TRAY_WRONGICONID;
		return;
	}
	// Free previous allocated menu
	freeMenu(id_num);
	// Create new popup menu with given depth
	tray_icons[id_num].popup = new PopupMenu(nbLevels);
}

/*
 * Class:     jeans_trayicon_WindowsTrayIcon
 * Method:    checkPopup
 * Signature: (IIZ)V
 */
JNIEXPORT void JNICALL Java_jeans_trayicon_WindowsTrayIcon_checkPopup
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
 * Method:    subPopup
 * Signature: (IILjava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_jeans_trayicon_WindowsTrayIcon_subPopup
  (JNIEnv *env, jclass, jint id_num, jint level, jstring menuName, jint type) {
	// Return a menu id for the new submenu item
	jint id = -1;
	// Icon id valid?
	if (tray_icons[id_num].used == FALSE) {
		last_error = TRAY_WRONGICONID;
		return -1;
	}
	// Popup valid? (use initPopup())
	if (tray_icons[id_num].popup != NULL) {
		const char *cMenuName;
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
						// Get the name for the submenu
						cMenuName = env->GetStringUTFChars(menuName, 0);
						// Append the submenu to the menu one level back (thus the parent menu)
						AppendMenu(hMenu,MF_POPUP | MF_ENABLED | MF_UNCHECKED, (UINT)sMenu, cMenuName);
						env->ReleaseStringUTFChars(menuName, cMenuName);
						// Sub menus must not be destroyed, only parents must: mark as sub
						menu->makeSub();
					}
					break;
			}
		} else {
			// Add a regular item to a (sub)menu
			HMENU hMenu = popup->getMenu(level);
			if (hMenu == NULL) return -1;
			switch (type) {
				case POPUP_TYPE_ITEM:
				case POPUP_TYPE_CHECKBOX:
					// Get menu item name (checkbox or simple menu item)
					cMenuName = env->GetStringUTFChars(menuName, 0);
					// Get free id for the new menu item
					id = getFreeMenuId(id_num);
					// Append the new item to the existing menu
					AppendMenu(hMenu, MF_STRING | MF_ENABLED | MF_UNCHECKED, id, cMenuName);
					env->ReleaseStringUTFChars(menuName, cMenuName);
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

// Main proc of DLL, called on initialisation, termination
BOOL WINAPI DllMain(HANDLE hInst, ULONG fdwReason, LPVOID lpReserved) {
    switch(fdwReason) {
    case DLL_PROCESS_ATTACH:
			// Store the instance handle
			g_hinst = hInst;
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

// This proc is called on exit (before Java's System.exit())
// Free all icon resources and remove the hidden window
void cleanUpExit(JNIEnv *env) {
	for (int id_num = 0; id_num < MY_MAX_ICONS; id_num++) freeIcon(env, id_num);
	RemoveHWND();
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
		tray_icons[id_num].used == FALSE;
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
			if (env != NULL) env->DeleteGlobalRef(tray_icons[id_num].globalClass);
			else CallJavaVMS(DeleteGlobalCallback, id_num, 0);
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
				TrayMessage(my_hDlg, NIM_MODIFY, id_num, icon, tray_icons[id_num].tooltip);
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
	NOTIFYICONDATA tnd;
	tnd.cbSize		= sizeof(NOTIFYICONDATA);
	tnd.hWnd		= hDlg;
	tnd.uID			= uID;
	tnd.uFlags		= NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnd.uCallbackMessage	= MYWM_NOTIFYICON;
	tnd.hIcon		= hIcon;
	// Include tooltip?
	if (pszTip) {
		lstrcpyn(tnd.szTip, pszTip, sizeof(tnd.szTip));
	} else {
		tnd.szTip[0] = '\0';
	}
	// Call tray icon windows API function
	res = Shell_NotifyIcon(dwMessage, &tnd);
	// Destroy the icon's handle (icon data is copied by Windows function)
	if (hIcon) DestroyIcon(hIcon);
	return res;
}

// Java VM callback function to delete a global reference to a given Java class
// Used to delete the global reference to the icon's class to receive mouse/menu events
int DeleteGlobalCallback(JNIEnv *env, int id_num, int dummy) {
	env->DeleteGlobalRef(tray_icons[id_num].globalClass);
	tray_icons[id_num].globalClass = 0;
	return 0;
}

// Java VM callback function used to notify icon class after incomming sendWindowsMessage()
int WindowsMessageCallback(JNIEnv *env, int dummy, int wParam) {
	// Get reference to WindowsTrayIcon Java class
	jclass cls = env->FindClass("jeans/trayicon/WindowsTrayIcon");
	if (cls == 0) return -1;
	// Get static callback method id
	jmethodID mid = env->GetStaticMethodID(cls, "callWindowsMessage", "(I)I");
	if (mid == 0) return -1;
	// Make call to "callWindowsMessage" with parameter wParam
	return env->CallStaticIntMethod(cls, mid, (jint)wParam);
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

// Java VM callback used for mouse pressed callbacks
int MousePressedCallback(JNIEnv *env, int id_num, int button) {
	// Valid icon id and valid global reference to icon's Java class?
	if (tray_icons[id_num].used == FALSE) return TRAY_WRONGICONID;
	jobject obj = tray_icons[id_num].globalClass;
	if (obj == 0) return TRAY_NOLISTENER;
	jclass winTrayClass = env->GetObjectClass(obj);
	if (winTrayClass == 0) return TRAY_NOTENOUGHMEM;
	// Get callback method id
	jmethodID mid = env->GetMethodID(winTrayClass, "notifyMouseListeners", "(I)V");
	if (mid == 0) return TRAY_METHODID;
	// Call method "notifyMouseListeners"
	env->CallVoidMethod(obj, mid, button);
	return TRAY_NOERR;
}

// Call a Java method in a given virtual machine
int CallJavaVM(JavaVM* vm, JNIProcPtr(JNIProc), int arg1, int arg2) {
	int result = TRAY_NOERR;
#ifdef HAS_JVM_LIB
	JNIEnv* env;
	// Attach current thread to given Java VM
	if (vm->AttachCurrentThread(&env, NULL) < 0) return TRAY_ERRTHREAD;
	// Call method (MousePressedCallback/MenuItemCallback/WindowsMessageCallback/..)
	result = (*JNIProc)(env, arg1, arg2);
	// Check for exception detach thread and exit
	if (env->ExceptionOccurred()) env->ExceptionDescribe();
	vm->DetachCurrentThread();
#endif
	return result;
}

// Call a Java method in all available VM's (used for mouse/menu callbacks)
int CallJavaVMS(JNIProcPtr(JNIProc), int arg1, int arg2) {
	int value = TRAY_NOERR;
	JavaVM *vm_ptr;
	jsize number;
#ifdef HAS_JVM_LIB
	// Check for existing Java virtual machines
	if (JNI_GetCreatedJavaVMs(NULL, 0, &number) != 0) return TRAY_JNIERR;
	if (number == 0) return TRAY_NOVMS;
	// Call method in each available VM
	for (int ctr = 0; ctr < number; ctr++) {
		if (JNI_GetCreatedJavaVMs(&vm_ptr, ctr+1, NULL) != 0) return TRAY_JNIERR;
		int res = CallJavaVM(vm_ptr, JNIProc, arg1, arg2);
		if (res != TRAY_NOERR) value = res;
	}
#endif
#ifdef USE_JVM_HINSTANCE
	// Get addr of "JNI_GetCreatedJavaVMs"
	jint (FAR* lpfnJNI_GetCreatedJavaVMs)(JavaVM **, jsize, jsize *);
	lpfnJNI_GetCreatedJavaVMs =
		(jint (FAR*) (JavaVM **, jsize, jsize *))GetProcAddress(hInstNative, "JNI_GetCreatedJavaVMs");
	if (lpfnJNI_GetCreatedJavaVMs == NULL) return TRAY_CALLBACKDLLERR;
	// Check for existing Java virtual machines
	if ((*lpfnJNI_GetCreatedJavaVMs)(NULL, 0, &number) != 0) return TRAY_JNIERR;
	if (number == 0) return TRAY_NOVMS;
	// Call method in each available VM
	for (int ctr = 0; ctr < number; ctr++) {
		if ((*lpfnJNI_GetCreatedJavaVMs)(&vm_ptr, ctr+1, NULL) != 0) return TRAY_JNIERR;
		int res = CallJavaVM(vm_ptr, JNIProc, arg1, arg2);
		if (res != TRAY_NOERR) value = res;
	}
#endif
	return value;
}

// Thread proc to call Java method (calls CallJavaVMS but with wrapped params)
void CallJavaThread(void *arg) {
	ThreadJavaCallback *tjc = (ThreadJavaCallback*)arg;
	int result = CallJavaVMS(tjc->jni_proc, tjc->arg1, tjc->arg2);
	if (result != TRAY_NOERR) last_error = result;
	delete tjc;
}

// Call a Java method in a new thread
void CallJavaVMSThread(JNIProcPtr(JNIProc), int arg1, int arg2) {
	// Wrap parameters in struct
	ThreadJavaCallback *tjc = new ThreadJavaCallback;
	tjc->jni_proc = JNIProc;
	tjc->arg1 = arg1;
	tjc->arg2 = arg2;
	// Create new thread and call "CallJavaThread()"
	if (_beginthread(CallJavaThread, 0, tjc) == -1) delete tjc;
}

// Handle popup menu command
void HandleMenuCommand(WPARAM menuId) {
	// Get icon id given menu id
	int id_num = getMenuItemIdNum(menuId);
	// Callback to Java class in new thread, using method "MenuItemCallback()"
	CallJavaVMSThread(MenuItemCallback, id_num, menuId);
}

// Handle icon mouse event (left/right button)
void HandleNotifyIcon(WPARAM id_num, LPARAM lParam) {
	switch (lParam)	{
		case WM_LBUTTONDOWN:
			// Callback Java class in new thread using "MousePressedCallback()"
			if (id_num >= 0) CallJavaVMSThread(MousePressedCallback,id_num,0);
			break;
		case WM_RBUTTONDOWN:
			if (id_num >= 0) {
				// If menu defined, then show it
				if (tray_icons[id_num].popup != NULL) {
					POINT pos;
					GetCursorPos(&pos);
					// Display and track the popup menu
					PopupMenu *menu = tray_icons[id_num].popup;
					menu->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON,&pos,my_hDlg);
				} else {
					// Callback Java class in new thread using "MousePressedCallback()"
					CallJavaVMSThread(MousePressedCallback,id_num,1);
				}
			}
			break;
	}
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
			return CallJavaVMS(WindowsMessageCallback,0,(int)lParam);
		case WM_COMMAND:
			// Menu command for icon
			HandleMenuCommand(LOWORD(wParam));
			break;
		// Do nothing for other messages
		case WM_CREATE:
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
			return DefWindowProc (hWnd, iMessage, wParam, lParam) ;

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
	wndclass.hbrBackground = GetStockObject(BLACK_BRUSH);
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
					}
				}
			}
			// Create the monochrome bitmask with transparency info
			hBitmapAND = CreateBitmap(wd, hi, 1, 1, andMask);
			// Free memory
			delete andMask;
			// Release the memory DC
			ReleaseDC(NULL, hdc);
			return 0;
		}
		// Release the memory DC
		ReleaseDC(NULL, hdc);
	}
	// Error on zero size icons
	return -1;
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







