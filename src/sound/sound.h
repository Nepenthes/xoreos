/* eos - A reimplementation of BioWare's Aurora engine
 * Copyright (c) 2010 Sven Hesse (DrMcCoy), Matthew Hoops (clone2727)
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 *
 * This file is part of eos and is distributed under the terms of
 * the GNU General Public Licence. See COPYING for more informations.
 */

#ifndef SOUND_H
#define SOUND_H

namespace Common {
	class SeekableReadStream;
} // End of namespace Common

namespace Sound {

// Yeah, put a class here eventually or something :P

// Initialize/deinitialize the mixer
bool initMixer();
void deinitMixer();
const char *getMixerError();

// Play a wave file (and eventually MP3)
void playSoundFile(Common::SeekableReadStream *wavStream);

} // End of namespace Sound

#endif