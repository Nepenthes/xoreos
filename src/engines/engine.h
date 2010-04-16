/* eos - A reimplementation of BioWare's Aurora engine
 * Copyright (c) 2010 Sven Hesse (DrMcCoy), Matthew Hoops (clone2727)
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 *
 * This file is part of eos and is distributed under the terms of
 * the GNU General Public Licence. See COPYING for more informations.
 */

/** @file engines/engine.h
 *  Generic engine interface
 */

#ifndef ENGINES_ENGINE_H
#define ENGINES_ENGINE_H

#include <string>

#include "common/types.h"

namespace Common {
	class SeekableReadStream;
}

namespace Engines {

/** The base class for an engine within BioWare's Aurora family. */
class Engine {
public:
	Engine();
	virtual ~Engine();

	/** Run the game. */
	virtual void run(const std::string &directory) = 0;

protected:
	/** Add a KEY file to the resource manager, erroring out if the file does not exist. */
	void indexMandatoryKEY(const std::string &key, uint32 priority = 10);

	/** Add a KEY file to the resource manager, if it exists. */
	void indexOptionalKEY(const std::string &key, uint32 priority = 10);

	/** Add an ERF file to the resource manager, erroring out if the file does not exist. */
	void indexMandatoryERF(const std::string &erf, uint32 priority = 100);

	/** Add an ERF file to the resource manager, if it exists. */
	void indexOptionalERF(const std::string &erf, uint32 priority = 100);

	/** Add a RIM file to the resource manager, erroring out if the file does not exist. */
	void indexMandatoryRIM(const std::string &rim, uint32 priority = 100);

	/** Add a RIM file to the resource manager, if it exists. */
	void indexOptionalRIM(const std::string &rim, uint32 priority = 100);

	/** Debug method to quickly dump a stream to disk. */
	void dumpStream(Common::SeekableReadStream &stream, const std::string &fileName);
};

} // End of namespace Engines

#endif // ENGINES_ENGINE_H