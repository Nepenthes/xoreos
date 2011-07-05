/* eos - A reimplementation of BioWare's Aurora engine
 *
 * eos is the legal property of its developers, whose names can be
 * found in the AUTHORS file distributed with this source
 * distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 */

// Largely based on the ASF/WMA implementation found in FFmpeg.

/** @file sound/decoders/wma.cpp
 *  Decoding Windows Media Audio.
 */

#include "common/error.h"
#include "common/stream.h"
#include "common/util.h"

#include "sound/audiostream.h"
#include "sound/decoders/wma.h"

namespace Sound {

class ASFGUID {
public:
	ASFGUID(Common::SeekableReadStream &stream) {
		stream.read(id, 16);
	}

	ASFGUID(byte a0, byte a1, byte a2, byte a3, byte a4, byte a5, byte a6, byte a7, byte a8, byte a9, byte a10, byte a11, byte a12, byte a13, byte a14, byte a15) {
		id[0]  = a0;   id[1]  = a1;  id[2]  = a2;  id[3]  = a3;
		id[4]  = a4;   id[5]  = a5;  id[6]  = a6;  id[7]  = a7;
		id[8]  = a8;   id[9]  = a9;  id[10] = a10; id[11] = a11;
		id[12] = a12;  id[13] = a13; id[14] = a14; id[15] = a15;
	}

	bool operator==(const ASFGUID &g) const {
		return !memcmp(g.id, id, 16);
	}

	bool operator!=(const ASFGUID &g) const {
		return memcmp(g.id, id, 16);
	}

	Common::UString toString() const {
		return Common::UString::sprintf("%02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x",
				id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7], id[8], id[9], id[10], id[11], id[12], id[13], id[14], id[15]);
	}

private:
	byte id[16];
};

// GUID's that we need
static const ASFGUID s_asfHeader       = ASFGUID(0x30, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11, 0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C);
static const ASFGUID s_asfFileHeader   = ASFGUID(0xA1, 0xDC, 0xAB, 0x8C, 0x47, 0xA9, 0xCF, 0x11, 0x8E, 0xE4, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65);
static const ASFGUID s_asfHead1        = ASFGUID(0xb5, 0x03, 0xbf, 0x5f, 0x2E, 0xA9, 0xCF, 0x11, 0x8e, 0xe3, 0x00, 0xc0, 0x0c, 0x20, 0x53, 0x65);
static const ASFGUID s_asfComment      = ASFGUID(0x33, 0x26, 0xb2, 0x75, 0x8E, 0x66, 0xCF, 0x11, 0xa6, 0xd9, 0x00, 0xaa, 0x00, 0x62, 0xce, 0x6c);
static const ASFGUID s_asfStreamHeader = ASFGUID(0x91, 0x07, 0xDC, 0xB7, 0xB7, 0xA9, 0xCF, 0x11, 0x8E, 0xE6, 0x00, 0xC0, 0x0C, 0x20, 0x53, 0x65);
static const ASFGUID s_asfCodecComment = ASFGUID(0x40, 0x52, 0xD1, 0x86, 0x1D, 0x31, 0xD0, 0x11, 0xA3, 0xA4, 0x00, 0xA0, 0xC9, 0x03, 0x48, 0xF6);
static const ASFGUID s_asfDataHeader   = ASFGUID(0x36, 0x26, 0xb2, 0x75, 0x8E, 0x66, 0xCF, 0x11, 0xa6, 0xd9, 0x00, 0xaa, 0x00, 0x62, 0xce, 0x6c);
static const ASFGUID s_asfAudioStream  = ASFGUID(0x40, 0x9E, 0x69, 0xF8, 0x4D, 0x5B, 0xCF, 0x11, 0xA8, 0xFD, 0x00, 0x80, 0x5F, 0x5C, 0x44, 0x2B);

class WMAStream : public RewindableAudioStream {
public:
	WMAStream(Common::SeekableReadStream *stream, bool dispose);
	~WMAStream();

	int readBuffer(int16 *buffer, const int numSamples) { return 0; }

	bool endOfData() const		{ return true; }
	bool isStereo() const		{ return _channels == 2; }
	int getRate() const			{ return _rate; }
	bool rewind()               { return false; }

private:
	Common::SeekableReadStream *_stream;
	bool _disposeAfterUse;

	void parseStreamHeader();

	uint16 _channels;
	int _rate;
	uint16 _blockAlign;
	uint16 _bitsPerCodedSample;
	Common::SeekableReadStream *_extraData;
};

WMAStream::WMAStream(Common::SeekableReadStream *stream, bool dispose) : _stream(stream), _disposeAfterUse(dispose) {
	_extraData = 0;

	ASFGUID guid = ASFGUID(*_stream);
	if (guid != s_asfHeader)
		throw Common::Exception("WMAStream: Missing asf header");

	_stream->readUint64LE();
	_stream->readUint32LE();
	_stream->readByte();
	_stream->readByte();

	for (;;) {
		uint64 startPos = _stream->pos();
		guid = ASFGUID(*_stream);
		uint64 size = _stream->readUint64LE();

		if (_stream->eos())
			throw Common::Exception("WMAStream: Unexpected eos");

		// TODO: Parse each chunk
		if (guid == s_asfFileHeader) {
			// TODO
		} else if (guid == s_asfHead1) {
			// Should be safe to ignore
		} else if (guid == s_asfComment) {
			// Ignored
		} else if (guid == s_asfStreamHeader) {
			parseStreamHeader();
		} else if (guid == s_asfCodecComment) {
			// TODO
		} else if (guid == s_asfDataHeader) {
			// Done parsing the header
			break;
		} else
			warning("Found unknown ASF GUID: %s", guid.toString().c_str());
		
		_stream->seek(startPos + size);
	}

	throw Common::Exception("STUB: WMAStream");
}

WMAStream::~WMAStream() {
	if (_disposeAfterUse)
		delete _stream;
}

void WMAStream::parseStreamHeader() {
	ASFGUID guid = ASFGUID(*_stream);

	if (guid != s_asfAudioStream)
		throw Common::Exception("WMAStream::parseStreamHeader(): Found non-audio stream");

	_stream->skip(16); // skip a guid
	_stream->readUint64LE(); // total size
	uint32 typeSpecificSize = _stream->readUint32LE();
	_stream->readUint32LE();
	_stream->readUint16LE(); // stream id
	_stream->readUint32LE();

	// Parse the wave header
	uint16 compression = _stream->readUint16LE();
	_channels = _stream->readUint16LE();
	_rate = _stream->readUint32LE();
	_stream->readUint32LE(); // bit rate
	_blockAlign = _stream->readUint16LE();
	_bitsPerCodedSample = (typeSpecificSize == 14) ? 8 : _stream->readUint16LE();

	if (compression != 0x161)
		throw Common::Exception("WMAStream::parseStreamHeader(): Only WMAv2 is supported");

	if (typeSpecificSize >= 18) {
		uint32 cbSize = _stream->readUint16LE();
		cbSize = MIN<int>(cbSize, typeSpecificSize - 18);
		_extraData = _stream->readStream(cbSize);
	}
}

RewindableAudioStream *makeWMAStream(
	Common::SeekableReadStream *stream,
	bool disposeAfterUse) {
	RewindableAudioStream *s = new WMAStream(stream, disposeAfterUse);

	if (s && s->endOfData()) {
		delete s;
		return 0;
	}

	return s;
}

} // End of namespace Sound