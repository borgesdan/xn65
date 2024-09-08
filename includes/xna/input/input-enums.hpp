#ifndef XNA_INPUT_ENUMS_HPP
#define XNA_INPUT_ENUMS_HPP

namespace xna {
    //Identifies the state of a keyboard key. 
    enum class KeyState {
        Up,
        Down,
    };

    //Enumerates input device buttons. 
    enum class Buttons {
        A = 4096, // 0x00001000
        B = 8192, // 0x00002000
        X = 16384, // 0x00004000
        Y = 32768, // 0x00008000
        Back = 32, // 0x00000020
        Start = 16, // 0x00000010
        DPadUp = 1,
        DPadDown = 2,
        DPadLeft = 4,
        DPadRight = 8,
        LeftShoulder = 256, // 0x00000100
        RightShoulder = 512, // 0x00000200
        LeftStick = 64, // 0x00000040
        RightStick = 128, // 0x00000080
        BigButton = 2048, // 0x00000800
        LeftThumbstickLeft = 2097152, // 0x00200000
        LeftThumbstickRight = 1073741824, // 0x40000000
        LeftThumbstickDown = 536870912, // 0x20000000
        LeftThumbstickUp = 268435456, // 0x10000000
        RightThumbstickLeft = 134217728, // 0x08000000
        RightThumbstickRight = 67108864, // 0x04000000
        RightThumbstickDown = 33554432, // 0x02000000
        RightThumbstickUp = 16777216, // 0x01000000
        LeftTrigger = 8388608, // 0x00800000
        RightTrigger = 4194304, // 0x00400000
    };

    //Identifies the state of a controller button. 
    enum class ButtonState
    {
        Released,
        Pressed,
    };

    //Describes the type of a specified Xbox Controller. 
    enum class GamePadCapabilitiesType
    {
        Unknown = 0,
        Gamepad,
        Wheel,
        ArdaceStick,
        FlightStick,
        DancePAd,
        Guitar,
        GuitarAlternate,
        DrumKit,
        GuitarBass = 11,
        ArcadePad = 19,
    };

    //Specifies a type of dead zone processing to apply to Xbox Controller analog sticks when calling GetState. 
    enum class GamePadDeadZone {
        //The X and Y positions of each stick are compared against the dead zone independently. This setting is the default when calling GetState.
        IndependentAxes,
        //The combined X and Y position of each stick is compared to the dead zone. 
        //This provides better control than IndependentAxes when the stick is used as a two-dimensional control surface, such as when controlling a character's view in a first-person game.
        Circular,
        //The values of each stick are not processed and are returned by GetState as "raw" values. This is best if you intend to implement your own dead zone processing.
        None,
    };

    enum class Keys : unsigned char {
        None = 0,

        Back = 0x8,
        Tab = 0x9,

        Enter = 0xd,

        Pause = 0x13,
        CapsLock = 0x14,
        Kana = 0x15,
        ImeOn = 0x16,

        Kanji = 0x19,

        ImeOff = 0x1a,
        Escape = 0x1b,
        ImeConvert = 0x1c,
        ImeNoConvert = 0x1d,

        Space = 0x20,
        PageUp = 0x21,
        PageDown = 0x22,
        End = 0x23,
        Home = 0x24,
        Left = 0x25,
        Up = 0x26,
        Right = 0x27,
        Down = 0x28,
        Select = 0x29,
        Print = 0x2a,
        Execute = 0x2b,
        PrintScreen = 0x2c,
        Insert = 0x2d,
        Delete = 0x2e,
        Help = 0x2f,
        D0 = 0x30,
        D1 = 0x31,
        D2 = 0x32,
        D3 = 0x33,
        D4 = 0x34,
        D5 = 0x35,
        D6 = 0x36,
        D7 = 0x37,
        D8 = 0x38,
        D9 = 0x39,

        A = 0x41,
        B = 0x42,
        C = 0x43,
        D = 0x44,
        E = 0x45,
        F = 0x46,
        G = 0x47,
        H = 0x48,
        I = 0x49,
        J = 0x4a,
        K = 0x4b,
        L = 0x4c,
        M = 0x4d,
        N = 0x4e,
        O = 0x4f,
        P = 0x50,
        Q = 0x51,
        R = 0x52,
        S = 0x53,
        T = 0x54,
        U = 0x55,
        V = 0x56,
        W = 0x57,
        X = 0x58,
        Y = 0x59,
        Z = 0x5a,
        LeftWindows = 0x5b,
        RightWindows = 0x5c,
        Apps = 0x5d,

        Sleep = 0x5f,
        NumPad0 = 0x60,
        NumPad1 = 0x61,
        NumPad2 = 0x62,
        NumPad3 = 0x63,
        NumPad4 = 0x64,
        NumPad5 = 0x65,
        NumPad6 = 0x66,
        NumPad7 = 0x67,
        NumPad8 = 0x68,
        NumPad9 = 0x69,
        Multiply = 0x6a,
        Add = 0x6b,
        Separator = 0x6c,
        Subtract = 0x6d,

        Decimal = 0x6e,
        Divide = 0x6f,
        F1 = 0x70,
        F2 = 0x71,
        F3 = 0x72,
        F4 = 0x73,
        F5 = 0x74,
        F6 = 0x75,
        F7 = 0x76,
        F8 = 0x77,
        F9 = 0x78,
        F10 = 0x79,
        F11 = 0x7a,
        F12 = 0x7b,
        F13 = 0x7c,
        F14 = 0x7d,
        F15 = 0x7e,
        F16 = 0x7f,
        F17 = 0x80,
        F18 = 0x81,
        F19 = 0x82,
        F20 = 0x83,
        F21 = 0x84,
        F22 = 0x85,
        F23 = 0x86,
        F24 = 0x87,

        NumLock = 0x90,
        Scroll = 0x91,

        LeftShift = 0xa0,
        RightShift = 0xa1,
        LeftControl = 0xa2,
        RightControl = 0xa3,
        LeftAlt = 0xa4,
        RightAlt = 0xa5,
        BrowserBack = 0xa6,
        BrowserForward = 0xa7,
        BrowserRefresh = 0xa8,
        BrowserStop = 0xa9,
        BrowserSearch = 0xaa,
        BrowserFavorites = 0xab,
        BrowserHome = 0xac,
        VolumeMute = 0xad,
        VolumeDown = 0xae,
        VolumeUp = 0xaf,
        MediaNextTrack = 0xb0,
        MediaPreviousTrack = 0xb1,
        MediaStop = 0xb2,
        MediaPlayPause = 0xb3,
        LaunchMail = 0xb4,
        SelectMedia = 0xb5,
        LaunchApplication1 = 0xb6,
        LaunchApplication2 = 0xb7,

        OemSemicolon = 0xba,
        OemPlus = 0xbb,
        OemComma = 0xbc,
        OemMinus = 0xbd,
        OemPeriod = 0xbe,
        OemQuestion = 0xbf,
        OemTilde = 0xc0,

        OemOpenBrackets = 0xdb,
        OemPipe = 0xdc,
        OemCloseBrackets = 0xdd,
        OemQuotes = 0xde,
        Oem8 = 0xdf,

        OemBackslash = 0xe2,

        ProcessKey = 0xe5,

        OemCopy = 0xf2,
        OemAuto = 0xf3,
        OemEnlW = 0xf4,

        Attn = 0xf6,
        Crsel = 0xf7,
        Exsel = 0xf8,
        EraseEof = 0xf9,
        Play = 0xfa,
        Zoom = 0xfb,

        Pa1 = 0xfd,
        OemClear = 0xfe,
    };
}

#endif