#include "xnaerror.hpp"
#include "types.hpp"
#include "helpers.hpp"
#include "forward.hpp"
#include "enums.hpp"
#include "default.hpp"
#include "audio/audioengine.hpp"
#include "audio/soundeffect.hpp"
#include "common/color.hpp"
#include "common/math.hpp"
#include "common/matrix.hpp"
#include "common/packedvalue.hpp"
#include "common/point.hpp"
#include "common/quaternion.hpp"
#include "common/rectangle.hpp"
#include "common/vectors.hpp"
#include "content/defaultreaders.hpp"
#include "content/manager.hpp"
#include "content/reader.hpp"
#include "content/typereadermanager.hpp"
#include "content/lzx/decoder.hpp"
#include "content/lzx/decoderstream.hpp"
#include "csharp/binary.hpp"
#include "csharp/buffer.hpp"
#include "csharp/object.hpp"
#include "csharp/service.hpp"
#include "csharp/stream.hpp"
#include "csharp/timespan.hpp"
#include "csharp/type.hpp"
#include "game/clock.hpp"
#include "game/component.hpp"
#include "game/game.hpp"
#include "game/gdeviceinfo.hpp"
#include "game/gdevicemanager.hpp"
#include "game/servicecontainer.hpp"
#include "game/time.hpp"
#include "game/window.hpp"
#include "graphics/adapter.hpp"
#include "graphics/blendstate.hpp"
#include "graphics/constbuffer.hpp"
#include "graphics/databuffer.hpp"
#include "graphics/depthstencilstate.hpp"
#include "graphics/device.hpp"
#include "graphics/displaymode.hpp"
#include "graphics/gresource.hpp"
#include "graphics/indexbuffer.hpp"
#include "graphics/presentparams.hpp"
#include "graphics/rasterizerstate.hpp"
#include "graphics/rendertarget.hpp"
#include "graphics/samplerstate.hpp"
#include "graphics/shader.hpp"
#include "graphics/spritebatch.hpp"
#include "graphics/spritefont.hpp"
#include "graphics/swapchain.hpp"
#include "graphics/texture.hpp"
#include "graphics/vertexbuffer.hpp"
#include "graphics/vertexinput.hpp"
#include "graphics/vertexposition.hpp"
#include "graphics/viewport.hpp"
#include "input/gamepad.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"

#include "platform-dx/xna-dx.hpp"