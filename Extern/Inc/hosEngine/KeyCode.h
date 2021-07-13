#ifndef _KEY_CODE_H
#define _KEY_CODE_H

// [2022/02/25 노종원]
// 입력을 위한 키코드

// LOG

enum class KeyCode
		{
			None                = 0,	//DirectX::Keyboard::Keys::None            

			Back                = 0x8,	//DirectX::Keyboard::Keys::Back                , VK_BACK
			Tab                 = 0x9,	//DirectX::Keyboard::Keys::Tab                 , VK_TAB
																					   
			Enter               = 0xd,	//DirectX::Keyboard::Keys::Enter               , VK_RETURN
																					   
			Pause               = 0x13,	//DirectX::Keyboard::Keys::Pause               , VK_PAUSE
			CapsLock            = 0x14,	//DirectX::Keyboard::Keys::CapsLock            , VK_CAPITAL
			Kana                = 0x15,	//DirectX::Keyboard::Keys::Kana                , VK_KANA (한영키)
			Kanji               = 0x19,	//DirectX::Keyboard::Keys::Kanji               , VK_KANJI (한자키)

			Escape              = 0x1b,	//DirectX::Keyboard::Keys::Escape              , VK_ESCAPE
			ImeConvert          = 0x1c,	//DirectX::Keyboard::Keys::ImeConvert          , VK_CONVERT
			ImeNoConvert        = 0x1d,	//DirectX::Keyboard::Keys::ImeNoConvert        , VK_NONCONVERT

			Space               = 0x20,	//DirectX::Keyboard::Keys::Space               , VK_SPACE
			PageUp              = 0x21,	//DirectX::Keyboard::Keys::PageUp              , VK_PRIOR
			PageDown            = 0x22,	//DirectX::Keyboard::Keys::PageDown            , VK_NEXT
			End                 = 0x23,	//DirectX::Keyboard::Keys::End                 , VK_END
			Home                = 0x24,	//DirectX::Keyboard::Keys::Home                , VK_HOME
			Left                = 0x25,	//DirectX::Keyboard::Keys::Left                , VK_LEFT
			Up                  = 0x26,	//DirectX::Keyboard::Keys::Up                  , VK_UP
			Right               = 0x27,	//DirectX::Keyboard::Keys::Right               , VK_RIGHT
			Down                = 0x28,	//DirectX::Keyboard::Keys::Down                , VK_DOWN
			Select              = 0x29,	//DirectX::Keyboard::Keys::Select              , VK_SELECT
			Print               = 0x2a,	//DirectX::Keyboard::Keys::Print               , VK_PRINT
			Execute             = 0x2b,	//DirectX::Keyboard::Keys::Execute             , VK_EXECUTE
			PrintScreen         = 0x2c,	//DirectX::Keyboard::Keys::PrintScreen         , VK_SNAPSHOT
			Insert              = 0x2d,	//DirectX::Keyboard::Keys::Insert              , VK_INSERT
			Delete              = 0x2e,	//DirectX::Keyboard::Keys::Delete              , VK_DELETE
			Help                = 0x2f,	//DirectX::Keyboard::Keys::Help                , VK_HELP
			D0                  = 0x30,	//DirectX::Keyboard::Keys::D0                  , 0x30
			D1                  = 0x31,	//DirectX::Keyboard::Keys::D1                  , 0x31
			D2                  = 0x32,	//DirectX::Keyboard::Keys::D2                  , 0x32
			D3                  = 0x33,	//DirectX::Keyboard::Keys::D3                  , 0x33
			D4                  = 0x34,	//DirectX::Keyboard::Keys::D4                  , 0x34
			D5                  = 0x35,	//DirectX::Keyboard::Keys::D5                  , 0x35
			D6                  = 0x36,	//DirectX::Keyboard::Keys::D6                  , 0x36
			D7                  = 0x37,	//DirectX::Keyboard::Keys::D7                  , 0x37
			D8                  = 0x38,	//DirectX::Keyboard::Keys::D8                  , 0x38
			D9                  = 0x39,	//DirectX::Keyboard::Keys::D9                  , 0x39

			A                   = 0x41,	//DirectX::Keyboard::Keys::A                   , 0x41
			B                   = 0x42,	//DirectX::Keyboard::Keys::B                   , 0x42
			C                   = 0x43,	//DirectX::Keyboard::Keys::C                   , 0x43
			D                   = 0x44,	//DirectX::Keyboard::Keys::D                   , 0x44
			E                   = 0x45,	//DirectX::Keyboard::Keys::E                   , 0x45
			F                   = 0x46,	//DirectX::Keyboard::Keys::F                   , 0x46
			G                   = 0x47,	//DirectX::Keyboard::Keys::G                   , 0x47
			H                   = 0x48,	//DirectX::Keyboard::Keys::H                   , 0x48
			I                   = 0x49,	//DirectX::Keyboard::Keys::I                   , 0x49
			J                   = 0x4a,	//DirectX::Keyboard::Keys::J                   , 0x4A
			K                   = 0x4b,	//DirectX::Keyboard::Keys::K                   , 0x4B
			L                   = 0x4c,	//DirectX::Keyboard::Keys::L                   , 0x4C
			M                   = 0x4d,	//DirectX::Keyboard::Keys::M                   , 0x4D
			N                   = 0x4e,	//DirectX::Keyboard::Keys::N                   , 0x4E
			O                   = 0x4f,	//DirectX::Keyboard::Keys::O                   , 0x4F
			P                   = 0x50,	//DirectX::Keyboard::Keys::P                   , 0x50
			Q                   = 0x51,	//DirectX::Keyboard::Keys::Q                   , 0x51
			R                   = 0x52,	//DirectX::Keyboard::Keys::R                   , 0x52
			S                   = 0x53,	//DirectX::Keyboard::Keys::S                   , 0x53
			T                   = 0x54,	//DirectX::Keyboard::Keys::T                   , 0x54
			U                   = 0x55,	//DirectX::Keyboard::Keys::U                   , 0x55
			V                   = 0x56,	//DirectX::Keyboard::Keys::V                   , 0x56
			W                   = 0x57,	//DirectX::Keyboard::Keys::W                   , 0x57
			X                   = 0x58,	//DirectX::Keyboard::Keys::X                   , 0x58
			Y                   = 0x59,	//DirectX::Keyboard::Keys::Y                   , 0x59
			Z                   = 0x5a,	//DirectX::Keyboard::Keys::Z                   , 0x5A
			LeftWindows         = 0x5b,	//DirectX::Keyboard::Keys::LeftWindows         , VK_LWIN
			RightWindows        = 0x5c,	//DirectX::Keyboard::Keys::RightWindows        , VK_RWIN
			Apps                = 0x5d,	//DirectX::Keyboard::Keys::Apps                , VK_APPS

			Sleep               = 0x5f,	//DirectX::Keyboard::Keys::Sleep               , VK_SLEEP
			NumPad0             = 0x60,	//DirectX::Keyboard::Keys::NumPad0             , VK_NUMPAD0
			NumPad1             = 0x61,	//DirectX::Keyboard::Keys::NumPad1             , VK_NUMPAD1
			NumPad2             = 0x62,	//DirectX::Keyboard::Keys::NumPad2             , VK_NUMPAD2
			NumPad3             = 0x63,	//DirectX::Keyboard::Keys::NumPad3             , VK_NUMPAD3
			NumPad4             = 0x64,	//DirectX::Keyboard::Keys::NumPad4             , VK_NUMPAD4
			NumPad5             = 0x65,	//DirectX::Keyboard::Keys::NumPad5             , VK_NUMPAD5
			NumPad6             = 0x66,	//DirectX::Keyboard::Keys::NumPad6             , VK_NUMPAD6
			NumPad7             = 0x67,	//DirectX::Keyboard::Keys::NumPad7             , VK_NUMPAD7
			NumPad8             = 0x68,	//DirectX::Keyboard::Keys::NumPad8             , VK_NUMPAD8
			NumPad9             = 0x69,	//DirectX::Keyboard::Keys::NumPad9             , VK_NUMPAD9
			Multiply            = 0x6a,	//DirectX::Keyboard::Keys::Multiply            , VK_MULTIPLY
			Add                 = 0x6b,	//DirectX::Keyboard::Keys::Add                 , VK_ADD
			Separator           = 0x6c,	//DirectX::Keyboard::Keys::Separator           , VK_SEPARATOR
			Subtract            = 0x6d,	//DirectX::Keyboard::Keys::Subtract            , VK_SUBTRACT

			Decimal             = 0x6e,	//DirectX::Keyboard::Keys::Decimal             , VK_DECIMANL
			Divide              = 0x6f,	//DirectX::Keyboard::Keys::Divide              , VK_DIVIDE
			F1                  = 0x70,	//DirectX::Keyboard::Keys::F1                  , VK_F1
			F2                  = 0x71,	//DirectX::Keyboard::Keys::F2                  , VK_F2
			F3                  = 0x72,	//DirectX::Keyboard::Keys::F3                  , VK_F3
			F4                  = 0x73,	//DirectX::Keyboard::Keys::F4                  , VK_F4
			F5                  = 0x74,	//DirectX::Keyboard::Keys::F5                  , VK_F5
			F6                  = 0x75,	//DirectX::Keyboard::Keys::F6                  , VK_F6
			F7                  = 0x76,	//DirectX::Keyboard::Keys::F7                  , VK_F7
			F8                  = 0x77,	//DirectX::Keyboard::Keys::F8                  , VK_F8
			F9                  = 0x78,	//DirectX::Keyboard::Keys::F9                  , VK_F9
			F10                 = 0x79,	//DirectX::Keyboard::Keys::F10                 , VK_F10
			F11                 = 0x7a,	//DirectX::Keyboard::Keys::F11                 , VK_F11
			F12                 = 0x7b,	//DirectX::Keyboard::Keys::F12                 , VK_F12
			F13                 = 0x7c,	//DirectX::Keyboard::Keys::F13                 , VK_F13
			F14                 = 0x7d,	//DirectX::Keyboard::Keys::F14                 , VK_F14
			F15                 = 0x7e,	//DirectX::Keyboard::Keys::F15                 , VK_F15
			F16                 = 0x7f,	//DirectX::Keyboard::Keys::F16                 , VK_F16
			F17                 = 0x80,	//DirectX::Keyboard::Keys::F17                 , VK_F17
			F18                 = 0x81,	//DirectX::Keyboard::Keys::F18                 , VK_F18
			F19                 = 0x82,	//DirectX::Keyboard::Keys::F19                 , VK_F19
			F20                 = 0x83,	//DirectX::Keyboard::Keys::F20                 , VK_F20
			F21                 = 0x84,	//DirectX::Keyboard::Keys::F21                 , VK_F21
			F22                 = 0x85,	//DirectX::Keyboard::Keys::F22                 , VK_F22
			F23                 = 0x86,	//DirectX::Keyboard::Keys::F23                 , VK_F23
			F24                 = 0x87,	//DirectX::Keyboard::Keys::F24                 , VK_F24

			NumLock             = 0x90,	//DirectX::Keyboard::Keys::NumLock             , VK_NUMLOCK
			Scroll              = 0x91,	//DirectX::Keyboard::Keys::Scroll              , VK_SCROLL

			LeftShift           = 0xa0,	//DirectX::Keyboard::Keys::LeftShift           , VK_LSHIFT
			RightShift          = 0xa1,	//DirectX::Keyboard::Keys::RightShift          , VK_RSHIFT
			LeftControl         = 0xa2,	//DirectX::Keyboard::Keys::LeftControl         , VK_LCONTROL
			RightControl        = 0xa3,	//DirectX::Keyboard::Keys::RightControl        , VK_RCONTROL
			LeftAlt             = 0xa4,	//DirectX::Keyboard::Keys::LeftAlt             , VK_LMENU
			RightAlt            = 0xa5,	//DirectX::Keyboard::Keys::RightAlt            , VK_RMENU
			BrowserBack         = 0xa6,	//DirectX::Keyboard::Keys::BrowserBack         , VK_BROWSER_BACK
			BrowserForward      = 0xa7,	//DirectX::Keyboard::Keys::BrowserForward      , VK_BROWSER_FORWARD
			BrowserRefresh      = 0xa8,	//DirectX::Keyboard::Keys::BrowserRefresh      , VK_BROWSER_REFRESH
			BrowserStop         = 0xa9,	//DirectX::Keyboard::Keys::BrowserStop         , VK_BROWSER_STOP
			BrowserSearch       = 0xaa,	//DirectX::Keyboard::Keys::BrowserSearch       , VK_BROWSER_SEARCH
			BrowserFavorites    = 0xab,	//DirectX::Keyboard::Keys::BrowserFavorites    , VK_BROWSER_FAVORITES
			BrowserHome         = 0xac,	//DirectX::Keyboard::Keys::BrowserHome         , VK_BROWSER_HOME
			VolumeMute          = 0xad,	//DirectX::Keyboard::Keys::VolumeMute          , VK_VOLUME_MUTE
			VolumeDown          = 0xae,	//DirectX::Keyboard::Keys::VolumeDown          , VK_VOLUME_DOWN
			VolumeUp            = 0xaf,	//DirectX::Keyboard::Keys::VolumeUp            , VK_VOLUME_UP
			MediaNextTrack      = 0xb0,	//DirectX::Keyboard::Keys::MediaNextTrack      , VK_MEDIA_NEXT_TRACK
			MediaPreviousTrack  = 0xb1,	//DirectX::Keyboard::Keys::MediaPreviousTrack  , VK_MEDIA_PREV_TRACK
			MediaStop           = 0xb2,	//DirectX::Keyboard::Keys::MediaStop           , VK_MEDIA_STOP
			MediaPlayPause      = 0xb3,	//DirectX::Keyboard::Keys::MediaPlayPause      , VK_MEDIA_PLAY_PAUSE
			LaunchMail          = 0xb4,	//DirectX::Keyboard::Keys::LaunchMail          , VK_LAUNCH_MAIL
			SelectMedia         = 0xb5,	//DirectX::Keyboard::Keys::SelectMedia         , VK_LAUNCH_MEDIA_SELECT
			LaunchApplication1  = 0xb6,	//DirectX::Keyboard::Keys::LaunchApplication1  , VK_LAUNCH_APP1
			LaunchApplication2  = 0xb7,	//DirectX::Keyboard::Keys::LaunchApplication2  , VK_LAUNCH_APP2

			OemSemicolon        = 0xba,	//DirectX::Keyboard::Keys::OemSemicolon        , VK_OEM_1
			OemPlus             = 0xbb,	//DirectX::Keyboard::Keys::OemPlus             , VK_OEM_PLUS
			OemComma            = 0xbc,	//DirectX::Keyboard::Keys::OemComma            , VK_OEM_COMMA
			OemMinus            = 0xbd,	//DirectX::Keyboard::Keys::OemMinus            , VK_OEM_MINUS
			OemPeriod           = 0xbe,	//DirectX::Keyboard::Keys::OemPeriod           , VK_OEM_PERIOD
			OemQuestion         = 0xbf,	//DirectX::Keyboard::Keys::OemQuestion         , VK_OEM_2
			OemTilde            = 0xc0,	//DirectX::Keyboard::Keys::OemTilde            , VK_OEM_3
			OemOpenBrackets     = 0xdb,	//DirectX::Keyboard::Keys::OemOpenBrackets     , VK_OEM_4
			OemPipe             = 0xdc,	//DirectX::Keyboard::Keys::OemPipe             , VK_OEM_5
			OemCloseBrackets    = 0xdd,	//DirectX::Keyboard::Keys::OemCloseBrackets    , VK_OEM_6
			OemQuotes           = 0xde,	//DirectX::Keyboard::Keys::OemQuotes           , VK_OEM_7
			Oem8                = 0xdf,	//DirectX::Keyboard::Keys::Oem8                , VK_OEM_8
			OemBackslash        = 0xe2,	//DirectX::Keyboard::Keys::OemBackslash        , VK_OEM_102

			ProcessKey          = 0xe5,	//DirectX::Keyboard::Keys::ProcessKey          , VK_PROCESSKEY

			OemCopy             = 0xf2,	//DirectX::Keyboard::Keys::OemCopy             , 0XF2
			OemAuto             = 0xf3,	//DirectX::Keyboard::Keys::OemAuto             , 0xF3
			OemEnlW             = 0xf4,	//DirectX::Keyboard::Keys::OemEnlW             , 0xF4

			Attn                = 0xf6,	//DirectX::Keyboard::Keys::Attn                , VK_ATTN
			Crsel               = 0xf7,	//DirectX::Keyboard::Keys::Crsel               , VK_CRSEL
			Exsel               = 0xf8,	//DirectX::Keyboard::Keys::Exsel               , VK_EXSEL
			EraseEof            = 0xf9,	//DirectX::Keyboard::Keys::EraseEof            , VK_EREOF
			Play                = 0xfa,	//DirectX::Keyboard::Keys::Play                , VK_PLAY
			Zoom                = 0xfb,	//DirectX::Keyboard::Keys::Zoom                , VK_ZOOM
			Pa1                 = 0xfd,	//DirectX::Keyboard::Keys::Pa1                 , VK_PA1
			OemClear            = 0xfe,	//DirectX::Keyboard::Keys::OemClear            , VK_OEM_CLEAR
																						 
			KeyBoardKeys = 0xff,
																						 
			MouseLeft,																	 
			MouseMiddle,																 
			MouseRight,
			MouseBtn1,																	 
			MouseBtn2,

			GamePadA,
			GamePadB,
			GamePadX,
			GamePadY,
			GamePadLeftButton,
			GamePadRightButton,
			GamePadLeftStick,
			GamePadRightStick,
			GamePadBack,
			GamePadView = GamePadBack,
			GamePadStart,
			GamePadMenu = GamePadStart,
			GamePadUp,
			GamePadDown,
			GamePadRight,
			GamePadLeft,
		};

		enum Axis
		{
			MouseX,
			MouseY,
			MouseScroll,
			GamePadLeftX,
			GamePadLeftY,
			GamePadRightX,
			GamePadRightY,
			GamePadLeftTrigger,
			GamePadRightTrigger,
		};

#endif // !_KEY_CODE_H