#define NOMINMAX 
#include "xnaerror.hpp"
#include "types.hpp"
#include "helpers.hpp"
#include "enums.hpp"
#include "audio/audioengine.hpp"
#include "audio/soundeffect.hpp"
#include "common/color.hpp"
#include "common/curve.hpp"
#include "common/gjk.hpp"
#include "common/math.hpp"
#include "common/packedvalue.hpp"
#include "common/numerics.hpp"
#include "common/collision.hpp"
#include "content/readers/default.hpp"
#include "content/readers/graphics.hpp"
#include "content/readers/audio.hpp"
#include "content/manager.hpp"
#include "content/reader.hpp"
#include "content/typereadermanager.hpp"
#include "content/lzx/decoder.hpp"
#include "csharp/binary.hpp"
#include "csharp/buffer.hpp"
#include "csharp/object.hpp"
#include "csharp/service.hpp"
#include "csharp/stream.hpp"
#include "csharp/timespan.hpp"
#include "csharp/type.hpp"
#include "game/component.hpp"
#include "game/game.hpp"
#include "game/gdeviceinfo.hpp"
#include "game/gdevicemanager.hpp"
#include "game/servicecontainer.hpp"
#include "game/time.hpp"
#include "game/window.hpp"
#include "graphics/adapter.hpp"
#include "graphics/blendstate.hpp"
#include "graphics/depthstencilstate.hpp"
#include "graphics/device.hpp"
#include "graphics/displaymode.hpp"
#include "graphics/gresource.hpp"
#include "graphics/presentparams.hpp"
#include "graphics/rasterizerstate.hpp"
#include "graphics/rendertarget.hpp"
#include "graphics/samplerstate.hpp"
#include "graphics/shader.hpp"
#include "graphics/sprite.hpp"
#include "graphics/swapchain.hpp"
#include "graphics/texture.hpp"
#include "graphics/vertexposition.hpp"
#include "graphics/viewport.hpp"
#include "input/gamepad.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include "platforminit.hpp"
#include "platform-dx/xna-dx.hpp"