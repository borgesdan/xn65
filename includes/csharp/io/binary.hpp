#ifndef CSHARP_IO_BINARY_CPP
#define CSHARP_IO_BINARY_CPP

#include "stream.hpp"
#include "exception.hpp"
#include <optional>

namespace csharp {
	class BinaryReader {
	public:
		BinaryReader(std::shared_ptr<Stream> input, bool leaveOpen = false)
			: _stream(input), _leaveOpen(leaveOpen) 
		{
			if (input == nullptr)
				throw csharp::ArgumentNullException(std::nullopt, "input");
		}

		virtual void Close() {
			if (!_leaveOpen)
				_stream->Close();
		}

	private:
		std::shared_ptr<Stream> _stream;
		bool _leaveOpen;
	};
}

#endif