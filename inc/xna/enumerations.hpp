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

    enum class Blend {
        Zero,
        One,
        SourceColor,
        InverseSourceColor,
        SourceAlpha,
        InverseSourceAlpha,
        DestinationAlpha,
        InverseDestinationAlpha,
        DestinationColor,
        InverseDestinationColor,
        SourceAlphaSaturation,
        BlendFactor,
        InverseBlendFactor,
        Source1Color,
        InverseSource1Color,
        Source1Alpha,
        InverseSource1Alpha
    };

    enum class BlendFunction {
        Add = 0,
        Subtract = 1,
        ReverseSubtract = 2,
        Min = 3,
        Max = 4,
    };

    using BlendOperation = BlendFunction;

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

    enum class ColorWriteChannels {
        Red,
        Green,
        Blue,
        Alpha,
        All,
        None
    };

    enum class ContainmentType {
        Disjoint,
        Contains,
        Intersects,
    };

    enum class ComparisonFunction {
        Never,
        Less,
        Equal,
        LessEquals,
        Greater,
        NotEqual,
        GreaterEqual,
        Always
    };

    using CompareFunction = ComparisonFunction;

    enum class CurveContinuity {
        Smooth,
        Step,
    };

    enum class CurveTangent
    {
        Flat,
        Linear,
        Smooth,
    };

    enum class CurveLoopType {
        Constant,
        Cycle,
        CycleOffset,
        Oscillate,
        Linear,
    };

    enum class CullMode {
        None,
        CullClockwiseFace,
        CullCounterClockwiseFace,
    };

    enum class DepthFormat {
        None,
        Depth16,
        Depth24,
        Depth24Stencil8
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

    enum class FileMode {
        CreateNew,
        Create,
        Append,
        Open,
        OpenOrCreate,
        Truncate
    };

    enum class FillMode
    {
        WireFrame,
        Solid,
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

    enum class PlaneIntersectionType {
        Front,
        Back,
        Intersecting,
    };

    enum class PlayerIndex
    {
        One,
        Two,
        Three,
        Four,
    };

    //Defines flags that describe the relationship between the adapter refresh rate and the rate at which Present operations are completed. 
    enum class PresentInterval {
        //Equivalent to setting One.
        Default,
        //The driver waits for the vertical retrace period (the runtime will beam trace to prevent tearing). Present operations are not affected more frequently than the screen refresh rate; the runtime completes one Present operation per adapter refresh period, at most. This option is always available for both windowed and full-screen swap chains.
        One,
        //The driver waits for the vertical retrace period. Present operations are not affected more frequently than every second screen refresh. 
        Two,
        //The runtime updates the window client area immediately, and might do so more than once during the adapter refresh period. Present operations might be affected immediately. This option is always available for both windowed and full-screen swap chains.
        Immediate
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

    enum class SpriteSortMode
    {
        Deferred,
        Immediate,
        Texture,
        BackToFront,
        FrontToBack,
    };

    enum class StencilOperation
    {
        Keep,
        Zero,
        Replace,
        IncrementSaturation,
        DecrementSaturation,
        Invert,
        Increment,
        Decrement,
    };

    enum class SurfaceFormat {
        Color = 0,
        Bgr565 = 1,
        Bgra5551 = 2,
        Bgra4444 = 3,
        Dxt1 = 4,
        Dxt3 = 5,
        Dxt5 = 6,
        NormalizedByte2 = 7,
        NormalizedByte4 = 8,
        Rgba1010102 = 9,
        Rg32 = 10,
        Rgba64 = 11,
        Alpha8 = 12,
        Single = 13,
        Vector2 = 14,
        Vector4 = 15,
        HalfSingle = 16,
        HalfVector2 = 17,
        HalfVector4 = 18,
        HdrBlendable = 19,
        Unknown,
    };

    enum class SwapEffect {
        Discard,
        Sequential,
        FlipSequential,
        FlipDiscard
    };

    enum class TextureAddressMode {
        Wrap,
        Mirror,
        Clamp,
        Border,
        MirrorOnce
    };

    enum class TextureFilter {
        Linear,
        Point,
        Anisotropic,
        LinearMipPoint,
        PointMipLinear,
        MinLinearMagPointMipLinear,
        MinLinearMagPointMipPoint,
        MinPointMagLinearMipLinear,
        MinPointMagLinearMipPoint,
    };

    constexpr int SURFACE_FORMAT_COUNT = 19;
}

#endif