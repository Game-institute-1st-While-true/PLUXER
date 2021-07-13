#ifndef _INTERNAL_WINDOW_H
#define _INTERNAL_WINDOW_H

// [2021/03/08 노종원]
// 윈도우 헤더파일에서 필요없는걸 전부 제거해서 사용하기위한 헤더

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef WIN32_EXTRA_LEAN
#define WIN32_EXTRA_LEAN
#endif

#define NOIME
#define NOWINRES
#define NOGDICAPMASKS		//- CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#define NOVIRTUALKEYCODES	//- VK_*
///#define NOWINMESSAGES	//- WM_*, EM_*, LB_*, CB_*
///#define NOWINSTYLES		//- WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#define NOSYSMETRICS		//- SM_*
#define NOMENUS				//- MF_*
#define NOICONS				//- IDI_*
#define NOKEYSTATES			//- MK_*
#define NOSYSCOMMANDS		//- SC_*
#define NORASTEROPS			//- Binary and Tertiary raster ops
///#define NOSHOWWINDOW		//- SW_*
#define OEMRESOURCE			//- OEM Resource values
#define NOATOM				//- Atom Manager routines
#define NOCLIPBOARD			//- Clipboard routines
#define NOCOLOR				//- Screen colors
#define NOCTLMGR			//- Control and Dialog routines
#define NODRAWTEXT			//- DrawText() and DT_*
#define NOGDI				//- All GDI defines and routines
///#define NOKERNEL			//- All KERNEL defines and routines
///#define NOUSER			//- All USER defines and routines
///#define NONLS			//- All NLS defines and routines
///#define NOMB				//- MB_* and MessageBox()
#define NOMEMMGR			//- GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE			//- typedef METAFILEPICT
#define NOMINMAX			//- Macros min(a,b) and max(a,b)
///#define NOMSG			//- typedef MSG and associated routines
#define NOOPENFILE			//- OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL			//- SB_* and scrolling routines
#define NOSERVICE			//- All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND				//- Sound driver routines
#define NOTEXTMETRIC		//- typedef TEXTMETRIC and associated routines
#define NOWH				//- SetWindowsHook and WH_*
#define NOWINOFFSETS		//- GWL_*, GCL_*, associated routines
#define NOCOMM				//- COMM driver routines
#define NOKANJI				//- Kanji support stuff.
#define NOHELP				//- Help engine interface.
#define NOPROFILER			//- Profiler interface.
#define NODEFERWINDOWPOS	//- DeferWindowPos routines
#define NOMCX				//- Modem Configuration Extensions
#define NOPROXYSTUB			///?
#define NOIMAGE				///?
#define NO					///?
#define NOTAPE				///?

//Window Multimedia
#define NOMMIDS				//- Multimedia IDs are not defined
#define NONEWWAVE			//- No new waveform types are defined except WAVEFORMATEX
#define NONEWRIFF			//- No new RIFF forms are defined
#define NOJPEGDIB			//- No JPEG DIB definitions
#define NONEWIC				//- No new Image Compressor types are defined
#define NOBITMAP			//- No extended bitmap info header definition

#if defined(NOGDI) && !defined(NOBITMAP)
#define NOBITMAP
#endif // defined(NOGDI)| !defined(NOBITMAP)

#ifdef UNICODE
#define UNICODE_ONLY
#else
#define ANSI_ONLY
#endif // UNICODE

#include <Windows.h>

#endif // !_INTERNAL_WINDOW_H