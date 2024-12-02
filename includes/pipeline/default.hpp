#ifndef XNA_PIPELINE_DEFAULT_HPP
#define XNA_PIPELINE_DEFAULT_HPP

#include "xna/default.hpp"

namespace xna {
	class ContentTypeWriter;
	class ContentTypeWriterFactory;
	class ContentCompiler;
	class ContentWriter;

	using P_ContentTypeWriter = sptr<ContentTypeWriter>;
	using P_ContentTypeWriterFactory = sptr<ContentTypeWriterFactory>;
	using P_ContentCompiler = sptr<ContentCompiler>;
	using P_ContentWriter = sptr<ContentWriter>;
}

#endif