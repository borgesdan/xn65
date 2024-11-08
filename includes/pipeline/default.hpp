#ifndef XNA_PIPELINE_DEFAULT_HPP
#define XNA_PIPELINE_DEFAULT_HPP

#include "xna/default.hpp"

namespace xna {
	class ContentTypeWriter;
	class ContentTypeWriterFactory;

	using P_ContentTypeWriter = sptr<ContentTypeWriter>;
	using P_ContentTypeWriterFactory = sptr<ContentTypeWriterFactory>;
}

#endif