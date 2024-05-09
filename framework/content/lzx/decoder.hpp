#ifndef XNA_CONTENT_LZX_LZXDECODE_HPP
#define XNA_CONTENT_LZX_LZXDECODE_HPP

#include "../../default.hpp"
#include "../../csharp/stream.hpp"
#include <algorithm>

namespace xna {
	struct LzxConstants {
		static constexpr Ushort MIN_MATCH = 2;
		static constexpr Ushort MAX_MATCH = 257;
		static constexpr Ushort NUM_CHARS = 256;

		enum class BLOCKTYPE {
			INVALID = 0,
			VERBATIM = 1,
			ALIGNED = 2,
			UNCOMPRESSED = 3
		};

		static constexpr Ushort PRETREE_NUM_ELEMENTS = 20;
		static constexpr Ushort ALIGNED_NUM_ELEMENTS = 8;
		static constexpr Ushort NUM_PRIMARY_LENGTHS = 7;
		static constexpr Ushort NUM_SECONDARY_LENGTHS = 249;

		static constexpr Ushort PRETREE_MAXSYMBOLS = PRETREE_NUM_ELEMENTS;
		static constexpr Ushort PRETREE_TABLEBITS = 6;
		static constexpr Ushort MAINTREE_MAXSYMBOLS = NUM_CHARS + 50 * 8;
		static constexpr Ushort MAINTREE_TABLEBITS = 12;
		static constexpr Ushort LENGTH_MAXSYMBOLS = NUM_SECONDARY_LENGTHS + 1;
		static constexpr Ushort LENGTH_TABLEBITS = 12;
		static constexpr Ushort ALIGNED_MAXSYMBOLS = ALIGNED_NUM_ELEMENTS;
		static constexpr Ushort ALIGNED_TABLEBITS = 7;

		static constexpr Ushort LENTABLE_SAFETY = 64;
	};

	struct LzxState {
		Uint						R0{ 1 }, R1{ 1 }, R2{ 1 };			/* for the LRU offset system				*/
		Ushort						main_elements{ 0 };		/* number of main tree elements				*/
		Int							header_read{ 0 };		/* have we started decoding at all yet? 	*/
		LzxConstants::BLOCKTYPE		block_type{ LzxConstants::BLOCKTYPE::INVALID };			/* type of this block						*/
		Uint						block_length{ 0 };		/* uncompressed length of this block 		*/
		Uint						block_remaining{ 0 };	/* uncompressed bytes still left to decode	*/
		Uint						frames_read{ 0 };		/* the number of CFDATA blocks processed	*/
		Int							intel_filesize{ 0 };		/* magic header value used for transform	*/
		Int							intel_curpos{ 0 };		/* current offset in transform space		*/
		Int							intel_started{ 0 };		/* have we seen any translateable data yet?	*/

		std::vector<Ushort>			PRETREE_table;
		std::vector<Byte>			PRETREE_len;
		std::vector<Ushort>			MAINTREE_table;
		std::vector<Byte>			MAINTREE_len;
		std::vector<Ushort>			LENGTH_table;
		std::vector<Byte>			LENGTH_len;
		std::vector<Ushort>			ALIGNED_table;
		std::vector<Byte>			ALIGNED_len;

		// NEEDED MEMBERS
		// CAB actualsize
		// CAB window
		// CAB window_size
		// CAB window_posn
		Uint						actual_size{ 0 };
		std::vector<Byte>			window;
		Uint						window_size{ 0 };
		Uint						window_posn{ 0 };
	};

	class BitBuffer {
	public:
		BitBuffer(sptr<Stream> const& stream) : byteStream(stream) {
			InitBitStream();
		}

		constexpr void InitBitStream() {
			buffer = 0;
			bitsleft = 0;
		}

		void EnsureBits(Byte bits) {
			while (bitsleft < bits) {
				const auto lo = static_cast<Byte>(byteStream->ReadByte());
				const auto hi = static_cast<Byte>(byteStream->ReadByte());
				//int amount2shift = sizeofstatic_cast<Uint>(*8 - 16 - bitsleft;
				buffer |= static_cast<Uint>(((hi << 8) | lo) << (sizeof(Uint) * 8 - 16 - bitsleft));
				bitsleft += 16;
			}
		}

		constexpr Uint PeekBits(Byte bits) const
		{
			return (buffer >> ((sizeof(Uint) * 8) - bits));
		}

		constexpr void RemoveBits(Byte bits) {
			buffer <<= bits;
			bitsleft -= bits;
		}

		Uint ReadBits(Byte bits)
		{
			Uint ret = 0;

			if (bits > 0)
			{
				EnsureBits(bits);
				ret = PeekBits(bits);
				RemoveBits(bits);
			}

			return ret;
		}

		constexpr Uint GetBuffer() const {
			return buffer;
		}

		constexpr Byte GetBitsLeft() const {
			return bitsleft;
		}

	private:
		Uint buffer{ 0 };
		Byte bitsleft{ 0 };
		sptr<Stream> byteStream = nullptr;
	};

	class LzxDecoder {
	public:
		LzxDecoder(Int window) {
			Uint wndsize = (Uint)(1 << window);
			Int posn_slots = 0;

			// setup proper exception
			if (window < 15 || window > 21)
				return;

			// let's initialise our state
			m_state.window = std::vector<Byte>(wndsize, 0xDC);

			m_state.actual_size = wndsize;
			m_state.window_size = wndsize;

			/* initialize static tables */

			if (extra_bits.empty()) {
				extra_bits.resize(52);

				for (size_t i = 0, j = 0; i <= 50; i += 2) {
					extra_bits[i] = extra_bits[i + 1] = static_cast<Byte>(j);

					if ((i != 0) && (j < 17))
						j++;
				}
			}

			if (position_base.empty()) {
				position_base.resize(51);

				for (size_t i = 0, j = 0; i <= 50; i++) {
					position_base[i] = static_cast<Uint>(j);
					j += static_cast<size_t>(1) << extra_bits[i];
				}
			}

			/* calculate required position slots */
			if (window == 20)
				posn_slots = 42;
			else if (window == 21)
				posn_slots = 50;
			else
				posn_slots = window << 1;

			m_state.main_elements = static_cast<Ushort>(LzxConstants::NUM_CHARS + (posn_slots << 3));

			// yo dawg i herd u liek arrays so we put arrays in ur arrays so u can array while u array
			m_state.PRETREE_table = std::vector<Ushort>((1 << LzxConstants::PRETREE_TABLEBITS) + (LzxConstants::PRETREE_MAXSYMBOLS << 1));
			m_state.PRETREE_len = std::vector<Byte>(LzxConstants::PRETREE_MAXSYMBOLS + LzxConstants::LENTABLE_SAFETY);
			m_state.MAINTREE_table = std::vector<Ushort>((1 << LzxConstants::MAINTREE_TABLEBITS) + (LzxConstants::MAINTREE_MAXSYMBOLS << 1));
			m_state.MAINTREE_len = std::vector<Byte>(LzxConstants::MAINTREE_MAXSYMBOLS + LzxConstants::LENTABLE_SAFETY);
			m_state.LENGTH_table = std::vector<Ushort>((1 << LzxConstants::LENGTH_TABLEBITS) + (LzxConstants::LENGTH_MAXSYMBOLS << 1));
			m_state.LENGTH_len = std::vector<Byte>(LzxConstants::LENGTH_MAXSYMBOLS + LzxConstants::LENTABLE_SAFETY);
			m_state.ALIGNED_table = std::vector<Ushort>((1 << LzxConstants::ALIGNED_TABLEBITS) + (LzxConstants::ALIGNED_MAXSYMBOLS << 1));
			m_state.ALIGNED_len = std::vector<Byte>(LzxConstants::ALIGNED_MAXSYMBOLS + LzxConstants::LENTABLE_SAFETY);

			/* initialise tables to 0 (because deltas will be applied to them) */
			for (size_t i = 0; i < LzxConstants::MAINTREE_MAXSYMBOLS; i++)
				m_state.MAINTREE_len[i] = 0;

			for (size_t i = 0; i < LzxConstants::LENGTH_MAXSYMBOLS; i++)
				m_state.LENGTH_len[i] = 0;
		}

		Int Decompress(sptr<Stream>& inData, Int inLen, sptr<Stream>& outData, Int outLen) {
			BitBuffer bitbuf(inData);

			auto startpos = inData->Position();
			auto endpos = inData->Position() + inLen;

			auto& window = m_state.window;

			Uint window_posn = m_state.window_posn;
			Uint window_size = m_state.window_size;
			Uint R0 = m_state.R0;
			Uint R1 = m_state.R1;
			Uint R2 = m_state.R2;
			Uint i = 0;
			Uint j = 0;

			Int togo = outLen;
			Int this_run = 0;
			Int main_element = 0;
			Int match_length = 0;
			Int match_offset = 0;
			Int length_footer = 0;
			Int extra = 0;
			Int verbatim_bits = 0;
			Int rundest = 0;
			Int runsrc = 0;
			Int copy_length = 0;
			Int aligned_bits = 0;

			bitbuf.InitBitStream();

			/* read header if necessary */
			if (m_state.header_read == 0)
			{
				const auto intel = bitbuf.ReadBits(1);
				if (intel != 0)
				{
					// read the filesize
					i = bitbuf.ReadBits(16);
					j = bitbuf.ReadBits(16);
					m_state.intel_filesize = static_cast<Int>((i << 16) | j);
				}
				m_state.header_read = 1;
			}

			while (togo > 0)
			{
				/* last block finished, new block expected */
				if (m_state.block_remaining == 0)
				{
					// TODO may screw something up here
					if (m_state.block_type == LzxConstants::BLOCKTYPE::UNCOMPRESSED) {
						if ((m_state.block_length & 1) == 1)
							inData->ReadByte(); /* realign bitstream to word */

						bitbuf.InitBitStream();
					}

					m_state.block_type = static_cast<LzxConstants::BLOCKTYPE>(bitbuf.ReadBits(3));
					i = bitbuf.ReadBits(16);
					j = bitbuf.ReadBits(8);
					m_state.block_remaining = m_state.block_length = static_cast<Uint>((i << 8) | j);

					switch (m_state.block_type) {
					case LzxConstants::BLOCKTYPE::ALIGNED: {
						for (i = 0, j = 0; i < 8; i++) {
							j = bitbuf.ReadBits(3);
							m_state.ALIGNED_len[i] = static_cast<Byte>(j);
						}

						MakeDecodeTable(LzxConstants::ALIGNED_MAXSYMBOLS, LzxConstants::ALIGNED_TABLEBITS,
							m_state.ALIGNED_len, m_state.ALIGNED_table);

						//O mesmo que verbatim
						ReadLengths(m_state.MAINTREE_len, 0, 256, bitbuf);
						ReadLengths(m_state.MAINTREE_len, 256, m_state.main_elements, bitbuf);
						MakeDecodeTable(LzxConstants::MAINTREE_MAXSYMBOLS, LzxConstants::MAINTREE_TABLEBITS,
							m_state.MAINTREE_len, m_state.MAINTREE_table);

						if (m_state.MAINTREE_len[0xE8] != 0)
							m_state.intel_started = 1;

						ReadLengths(m_state.LENGTH_len, 0, LzxConstants::NUM_SECONDARY_LENGTHS, bitbuf);
						MakeDecodeTable(LzxConstants::LENGTH_MAXSYMBOLS, LzxConstants::LENGTH_TABLEBITS,
							m_state.LENGTH_len, m_state.LENGTH_table);
						break;
					}
					case LzxConstants::BLOCKTYPE::VERBATIM: {
						ReadLengths(m_state.MAINTREE_len, 0, 256, bitbuf);
						ReadLengths(m_state.MAINTREE_len, 256, m_state.main_elements, bitbuf);
						MakeDecodeTable(LzxConstants::MAINTREE_MAXSYMBOLS, LzxConstants::MAINTREE_TABLEBITS,
							m_state.MAINTREE_len, m_state.MAINTREE_table);

						if (m_state.MAINTREE_len[0xE8] != 0)
							m_state.intel_started = 1;

						ReadLengths(m_state.LENGTH_len, 0, LzxConstants::NUM_SECONDARY_LENGTHS, bitbuf);
						MakeDecodeTable(LzxConstants::LENGTH_MAXSYMBOLS, LzxConstants::LENGTH_TABLEBITS,
							m_state.LENGTH_len, m_state.LENGTH_table);
						break;
					}
					case LzxConstants::BLOCKTYPE::UNCOMPRESSED: {
						m_state.intel_started = 1; /* because we can't assume otherwise */
						bitbuf.EnsureBits(16); /* get up to 16 pad bits into the buffer */

						if (bitbuf.GetBitsLeft() > 16)
							inData->Seek(-2, SeekOrigin::Current); /* and align the bitstream! */

						Byte hi = 0;
						Byte mh = 0;
						Byte ml = 0;
						Byte lo = 0;

						lo = static_cast<Byte>(inData->ReadByte());
						ml = static_cast<Byte>(inData->ReadByte());
						mh = static_cast<Byte>(inData->ReadByte());
						hi = static_cast<Byte>(inData->ReadByte());
						R0 = static_cast<Uint>(lo | ml << 8 | mh << 16 | hi << 24);
						lo = static_cast<Byte>(inData->ReadByte());
						ml = static_cast<Byte>(inData->ReadByte());
						mh = static_cast<Byte>(inData->ReadByte());
						hi = static_cast<Byte>(inData->ReadByte());
						R1 = static_cast<Uint>(lo | ml << 8 | mh << 16 | hi << 24);
						lo = static_cast<Byte>(inData->ReadByte());
						ml = static_cast<Byte>(inData->ReadByte());
						mh = static_cast<Byte>(inData->ReadByte());
						hi = static_cast<Byte>(inData->ReadByte());
						R2 = static_cast<Uint>(lo | ml << 8 | mh << 16 | hi << 24);
						break;
					}
					default:
						return -1; // TODO throw proper exception
					}
				}

				return 0;

				/* buffer exhaustion check */
				if (inData->Position() > (startpos + inLen))
				{
					/* it's possible to have a file where the next run is less than
					 * 16 bits in size. In this case, the READ_HUFFSYM() macro used
					 * in building the tables will exhaust the buffer, so we should
					 * allow for this, but not allow those accidentally read bits to
					 * be used (so we check that there are at least 16 bits
					 * remaining - in this boundary case they aren't really part of
					 * the compressed data)
					 */
					 //Debug.WriteLine("WTF");

					if (inData->Position() > (startpos + inLen + 2) || bitbuf.GetBitsLeft() < 16)
						return -1; //TODO throw proper exception
				}

				while ((this_run = static_cast<Int>(m_state.block_remaining)) > 0 && togo > 0)
				{
					if (this_run > togo)
						this_run = togo;

					togo -= this_run;
					m_state.block_remaining -= static_cast<Uint>(this_run);

					/* apply 2^x-1 mask */
					window_posn &= window_size - 1;
					/* runs can't straddle the window wraparound */
					if ((window_posn + this_run) > window_size)
						return -1; //TODO throw proper exception

					switch (m_state.block_type)
					{
					case LzxConstants::BLOCKTYPE::VERBATIM: {
						while (this_run > 0)
						{
							main_element = static_cast<Int>(ReadHuffSym(m_state.MAINTREE_table, m_state.MAINTREE_len,
								LzxConstants::MAINTREE_MAXSYMBOLS, LzxConstants::MAINTREE_TABLEBITS,
								bitbuf));

							if (main_element < LzxConstants::NUM_CHARS)
							{
								/* literal: 0 to NUM_CHARS-1 */
								window[window_posn++] = static_cast<Byte>(main_element);
								this_run--;
							}
							else
							{
								/* match: NUM_CHARS + ((slot<<3) | length_header (3 bits)) */
								main_element -= LzxConstants::NUM_CHARS;

								match_length = main_element & LzxConstants::NUM_PRIMARY_LENGTHS;
								if (match_length == LzxConstants::NUM_PRIMARY_LENGTHS)
								{
									length_footer = static_cast<Int>(ReadHuffSym(m_state.LENGTH_table, m_state.LENGTH_len,
										LzxConstants::LENGTH_MAXSYMBOLS, LzxConstants::LENGTH_TABLEBITS,
										bitbuf));
									match_length += length_footer;
								}
								match_length += LzxConstants::MIN_MATCH;

								match_offset = main_element >> 3;

								if (match_offset > 2)
								{
									/* not repeated offset */
									if (match_offset != 3)
									{
										extra = extra_bits[match_offset];
										verbatim_bits = static_cast<Int>(bitbuf.ReadBits(static_cast<Int>(extra)));
										match_offset = static_cast<Int>(position_base[match_offset]) - 2 + verbatim_bits;
									}
									else
									{
										match_offset = 1;
									}

									/* update repeated offset LRU queue */
									R2 = R1; R1 = R0; R0 = static_cast<Uint>(match_offset);
								}
								else if (match_offset == 0)
								{
									match_offset = (int)R0;
								}
								else if (match_offset == 1)
								{
									match_offset = (int)R1;
									R1 = R0; R0 = static_cast<Uint>(match_offset);
								}
								else /* match_offset == 2 */
								{
									match_offset = (int)R2;
									R2 = R0; R0 = static_cast<Uint>(match_offset);
								}

								rundest = (int)window_posn;
								this_run -= match_length;

								/* copy any wrapped around source data */
								if (static_cast<Int>(window_posn) >= match_offset)
								{
									/* no wrap */
									runsrc = rundest - match_offset;
								}
								else
								{
									runsrc = rundest + ((int)window_size - match_offset);
									copy_length = match_offset - (int)window_posn;
									if (copy_length < match_length)
									{
										match_length -= copy_length;
										window_posn += static_cast<Uint>(copy_length);
										while (copy_length-- > 0) window[rundest++] = window[runsrc++];
										runsrc = 0;
									}
								}
								window_posn += static_cast<Uint>(match_length);

								/* copy match data - no worries about destination wraps */
								while (match_length-- > 0) window[rundest++] = window[runsrc++];
							}
						}
						break;
					}
					case LzxConstants::BLOCKTYPE::ALIGNED: {
						while (this_run > 0)
						{
							main_element = static_cast<Int>(ReadHuffSym(m_state.MAINTREE_table, m_state.MAINTREE_len,
								LzxConstants::MAINTREE_MAXSYMBOLS, LzxConstants::MAINTREE_TABLEBITS,
								bitbuf));

							if (main_element < LzxConstants::NUM_CHARS)
							{
								/* literal 0 to NUM_CHARS-1 */
								window[window_posn++] = static_cast<Byte>(main_element);
								this_run--;
							}
							else
							{
								/* match: NUM_CHARS + ((slot<<3) | length_header (3 bits)) */
								main_element -= LzxConstants::NUM_CHARS;

								match_length = main_element & LzxConstants::NUM_PRIMARY_LENGTHS;
								if (match_length == LzxConstants::NUM_PRIMARY_LENGTHS)
								{
									length_footer = static_cast<Int>(ReadHuffSym(m_state.LENGTH_table, m_state.LENGTH_len,
										LzxConstants::LENGTH_MAXSYMBOLS, LzxConstants::LENGTH_TABLEBITS,
										bitbuf));
									match_length += length_footer;
								}
								match_length += LzxConstants::MIN_MATCH;

								match_offset = main_element >> 3;

								if (match_offset > 2)
								{
									/* not repeated offset */
									extra = extra_bits[match_offset];
									match_offset = static_cast<Int>(position_base[match_offset]) - 2;
									if (extra > 3)
									{
										/* verbatim and aligned bits */
										extra -= 3;
										verbatim_bits = static_cast<Int>(bitbuf.ReadBits(static_cast<Byte>(extra)));
										match_offset += (verbatim_bits << 3);
										aligned_bits = static_cast<Int>(ReadHuffSym(m_state.ALIGNED_table, m_state.ALIGNED_len,
											LzxConstants::ALIGNED_MAXSYMBOLS, LzxConstants::ALIGNED_TABLEBITS,
											bitbuf));
										match_offset += aligned_bits;
									}
									else if (extra == 3)
									{
										/* aligned bits only */
										aligned_bits = static_cast<Int>(ReadHuffSym(m_state.ALIGNED_table, m_state.ALIGNED_len,
											LzxConstants::ALIGNED_MAXSYMBOLS, LzxConstants::ALIGNED_TABLEBITS,
											bitbuf));
										match_offset += aligned_bits;
									}
									else if (extra > 0) /* extra==1, extra==2 */
									{
										/* verbatim bits only */
										verbatim_bits = static_cast<Int>(bitbuf.ReadBits(static_cast<Byte>(extra)));
										match_offset += verbatim_bits;
									}
									else /* extra == 0 */
									{
										/* ??? */
										match_offset = 1;
									}

									/* update repeated offset LRU queue */
									R2 = R1; R1 = R0; R0 = static_cast<Uint>(match_offset);
								}
								else if (match_offset == 0)
								{
									match_offset = (int)R0;
								}
								else if (match_offset == 1)
								{
									match_offset = (int)R1;
									R1 = R0; R0 = static_cast<Uint>(match_offset);
								}
								else /* match_offset == 2 */
								{
									match_offset = (int)R2;
									R2 = R0; R0 = static_cast<Uint>(match_offset);
								}

								rundest = (int)window_posn;
								this_run -= match_length;

								/* copy any wrapped around source data */
								if (static_cast<Int>(window_posn) >= match_offset)
								{
									/* no wrap */
									runsrc = rundest - match_offset;
								}
								else
								{
									runsrc = rundest + ((int)window_size - match_offset);
									copy_length = match_offset - (int)window_posn;
									if (copy_length < match_length)
									{
										match_length -= copy_length;
										window_posn += static_cast<Uint>(copy_length);
										while (copy_length-- > 0) window[rundest++] = window[runsrc++];
										runsrc = 0;
									}
								}
								window_posn += static_cast<Uint>(match_length);

								/* copy match data - no worries about destination wraps */
								while (match_length-- > 0) window[rundest++] = window[runsrc++];
							}
						}
						break;
					}
					case LzxConstants::BLOCKTYPE::UNCOMPRESSED: {
						if ((inData->Position() + this_run) > endpos) return -1; //TODO throw proper exception
						std::vector<Byte> temp_buffer(this_run);
						inData->Read(temp_buffer, 0, this_run);

						for (size_t offset = 0; offset < temp_buffer.size(); ++offset)
							window[window_posn + offset] = temp_buffer[offset];

						window_posn += static_cast<Uint>(this_run);
						break;
					}
					default:
						return -1; //TODO throw proper exception
					}
				}
			}

			if (togo != 0) 
				return -1; //TODO throw proper exception

			Int start_window_pos = static_cast<Int>(window_posn);
			
			if (start_window_pos == 0) 
				start_window_pos = static_cast<Int>(window_size);

			start_window_pos -= outLen;
			outData->Write(window, start_window_pos, outLen);

			m_state.window_posn = window_posn;
			m_state.R0 = R0;
			m_state.R1 = R1;
			m_state.R2 = R2;

			// TODO finish intel E8 decoding
			/* intel E8 decoding */
			if ((m_state.frames_read++ < 32768) && m_state.intel_filesize != 0)
			{
				if (outLen <= 6 || m_state.intel_started == 0)
				{
					m_state.intel_curpos += outLen;
				}
				else
				{
					Int dataend = outLen - 10;
					auto curpos = static_cast<Uint>(m_state.intel_curpos);

					m_state.intel_curpos = static_cast<Int>(curpos) + outLen;

					while (outData->Position() < dataend)
					{
						if (outData->ReadByte() != 0xE8) {
							curpos++; 
							continue; 
						}
					}
				}

				return -1;
			}

			return 0;
		}

	public:
		inline static std::vector<Uint> position_base;
		inline static std::vector<Byte> extra_bits;

	private:
		LzxState m_state;

		Int MakeDecodeTable(Uint nsyms, Uint nbits, std::vector<Byte>& length, std::vector<Ushort>& table) {
			Ushort sym = 0;
			Uint leaf = 0;
			Byte bit_num = 1;
			Uint fill;
			Uint pos = 0; /* the current position in the decode table */
			Uint table_mask = static_cast<Uint>(1 << static_cast<Int>(nbits));
			Uint bit_mask = table_mask >> 1; /* don't do 0 length codes */
			Uint next_symbol = bit_mask;	/* base of allocation for long codes */

			/* fill entries for codes short enough for a direct mapping */
			while (bit_num <= nbits)
			{
				for (sym = 0; sym < nsyms; sym++)
				{
					if (length[sym] == bit_num)
					{
						leaf = pos;

						if ((pos += bit_mask) > table_mask) return 1; /* table overrun */

						/* fill all possible lookups of this symbol with the symbol itself */
						fill = bit_mask;
						while (fill-- > 0) table[leaf++] = sym;
					}
				}
				bit_mask >>= 1;
				bit_num++;
			}

			/* if there are any codes longer than nbits */
			if (pos != table_mask)
			{
				/* clear the remainder of the table */
				for (sym = static_cast<Ushort>(pos);
					sym < table_mask; sym++) table[sym] = 0;

				/* give ourselves room for codes to grow by up to 16 more bits */
				pos <<= 16;
				table_mask <<= 16;
				bit_mask = 1 << 15;

				while (bit_num <= 16)
				{
					for (sym = 0; sym < nsyms; sym++)
					{
						if (length[sym] == bit_num)
						{
							leaf = pos >> 16;
							for (fill = 0; fill < bit_num - nbits; fill++)
							{
								/* if this path hasn't been taken yet, 'allocate' two entries */
								if (table[leaf] == 0)
								{
									table[(next_symbol << 1)] = 0;
									table[(next_symbol << 1) + 1] = 0;
									table[leaf] = static_cast<Ushort>(next_symbol++);
								}
								/* follow the path and select either left or right for next bit */
								leaf = static_cast<Uint>(table[leaf] << 1);
								if (((pos >> static_cast<Int>(15 - fill)) & 1) == 1) leaf++;
							}
							table[leaf] = sym;

							if ((pos += bit_mask) > table_mask) return 1;
						}
					}
					bit_mask >>= 1;
					bit_num++;
				}
			}

			/* full talbe? */
			if (pos == table_mask) return 0;

			/* either erroneous table, or all elements are 0 - let's find out. */
			for (sym = 0; sym < nsyms; sym++) if (length[sym] != 0) return 1;
			return 0;
		}
		void ReadLengths(std::vector<Byte>& lens, Uint first, Uint last, BitBuffer& bitbuf) {
			Uint x = 0;
			Uint y = 0;
			Int z = 0;

			// hufftbl pointer here?

			for (x = 0; x < 20; x++)
			{
				y = bitbuf.ReadBits(4);
				m_state.PRETREE_len[x] = static_cast<Byte>(y);
			}

			MakeDecodeTable(LzxConstants::PRETREE_MAXSYMBOLS, LzxConstants::PRETREE_TABLEBITS,
				m_state.PRETREE_len, m_state.PRETREE_table);

			for (x = first; x < last;)
			{
				z = (int)ReadHuffSym(m_state.PRETREE_table, m_state.PRETREE_len,
					LzxConstants::PRETREE_MAXSYMBOLS, LzxConstants::PRETREE_TABLEBITS, bitbuf);
				if (z == 17)
				{
					y = bitbuf.ReadBits(4); y += 4;
					while (y-- != 0) lens[x++] = 0;
				}
				else if (z == 18)
				{
					y = bitbuf.ReadBits(5); y += 20;
					while (y-- != 0) lens[x++] = 0;
				}
				else if (z == 19)
				{
					y = bitbuf.ReadBits(1); y += 4;
					z = static_cast<Int>(ReadHuffSym(m_state.PRETREE_table, m_state.PRETREE_len,
						LzxConstants::PRETREE_MAXSYMBOLS, LzxConstants::PRETREE_TABLEBITS, bitbuf));
					z = lens[x] - z; if (z < 0) z += 17;
					while (y-- != 0) lens[x++] = static_cast<Byte>(z);
				}
				else
				{
					z = lens[x] - z; if (z < 0) z += 17;
					lens[x++] = static_cast<Byte>(z);
				}
			}
		}

		Uint ReadHuffSym(std::vector<Ushort>& table, std::vector<Byte>& lengths, Uint nsyms, Uint nbits, BitBuffer& bitbuf) {
			Uint i = 0;
			Uint j = 0;

			bitbuf.EnsureBits(16);

			if ((i = table[bitbuf.PeekBits(static_cast<Byte>(nbits))]) >= nsyms)
			{
				j = static_cast<Uint>(1 << static_cast<Int>((sizeof(Uint) * 8) - nbits));
				do
				{
					j >>= 1; i <<= 1; i |= (bitbuf.GetBuffer() & j) != 0 ? static_cast<Uint>(1) : 0;
					if (j == 0) return 0; // TODO throw proper exception
				} while ((i = table[i]) >= nsyms);
			}
			j = lengths[i];
			bitbuf.RemoveBits(static_cast<Byte>(j));

			return i;
		}
	};
}

#endif