/* xoreos - A reimplementation of BioWare's Aurora engine
 *
 * xoreos is the legal property of its developers, whose names can be
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

/** @file sound/decoders/asf.h
 *  Decoding Microsoft's Advanced Streaming Format.
 */

#ifndef SOUND_DECODERS_ASF_H
#define SOUND_DECODERS_ASF_H

namespace Sound {

/**
 * Try to load a ASF from the given seekable stream and create a RewindableAudioStream
 * from that data.
 *
 * @param stream          The SeekableReadStream from which to read the ASF data.
 * @param disposeAfterUse Whether to delete the stream after use.
 *
 * @return A new RewindableAudioStream, or 0, if an error occured.
 */

RewindableAudioStream *makeASFStream(
	Common::SeekableReadStream *stream,
	bool disposeAfterUse = true);

} // End of namespace Sound

#endif // SOUND_DECODERS_ASF_H
