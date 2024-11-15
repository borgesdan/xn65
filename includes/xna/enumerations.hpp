#ifndef XNA_ENUMERATIONS_HPP
#define XNA_ENUMERATIONS_HPP

namespace xna {
    enum class AudioChannels
    {
        Mono = 1,
        Stereo = 2,
    };

    enum  class AudioReverb {
        Off,
        Default,
        Generic,
        Forest,
        PaddedCell,
        Room,
        Bathroom,
        LivingRoom,
        StoneRoom,
        Auditorium,
        ConcertHall,
        Cave,
        Arena,
        Hangar,
        CarpetedHallway,
        Hallway,
        StoneCorridor,
        Alley,
        City,
        Mountains,
        Quarry,
        Plain,
        ParkingLot,
        SewerPipe,
        Underwater,
        SmallRoom,
        MediumRoom,
        LargeRoom,
        MediumHall,
        LargeHall,
        Plate,
        Max
    };

    enum class BufferUsage {
        Default,
        Immutable,
        Dynamic,
        Static,
    };

    enum class ClearOptions {
        DepthBuffer,
        Stencil,
        Target,
    };                

    enum class DepthWriteMask {
        Zero,
        All
    };

    enum class DisplayOrientation {
        Default = 0,
        LandscapeLeft = 1,
        LandscapeRight = 2,
        Portrait = 4,
    };

    enum class EffectParameterClass {
        Matrix,
        Object,
        Scalar,
        Struct,
        Vector
    };

    enum class EffectParameterType {
        Bool,
        Int32,
        Single,
        String,
        Texture,
        Texture1D,
        Texture2D,
        Texture3D,
        TextureCube,
        Void
    };        

    enum class GameComponentType {
        Updatable,
        Drawable,
    };

    //Identifies the set of supported devices for the game based on device capabilities. 
    enum class GraphicsProfile {
        //Use a limited set of graphic features and capabilities, allowing the game to support the widest variety of devices, including all Windows-based computers.        
        Reach,
        //Use the largest available set of graphic features and capabilities to target devices,
        //such as an Xbox 360 console and a Windows-based computer, that have more enhanced graphic capabilities. 
        HiDef
    };    

    enum class PlayerIndex
    {
        One,
        Two,
        Three,
        Four,
    };    

    enum class PrimitiveType
    {
        TriangleList,
        TriangleStrip,
        LineList,
        LineStrip,
    };

    enum RenderTargetUsage {
        DiscardContents,
        PreserveContents,
        PlatformContents
    };    

    enum class SpriteEffects {
        None = 0,
        FlipHorizontally = 1,
        FlipVertically = 2,
        Both = FlipHorizontally | FlipVertically
    };    

    constexpr int SURFACE_FORMAT_COUNT = 19;
}

#endif