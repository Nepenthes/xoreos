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

/** @file engines/nwn/script/container.h
 *  An object containing scripts.
 */

#ifndef ENGINES_NWN_SCRIPT_CONTAINER_H
#define ENGINES_NWN_SCRIPT_CONTAINER_H

#include "common/types.h"
#include "common/ustring.h"

#include "aurora/types.h"

#include "engines/nwn/types.h"

namespace Aurora {
	namespace NWScript {
		class Object;
		class ScriptState;
	}
}
namespace Engines {

namespace NWN {

class ScriptContainer {
public:
	ScriptContainer();
	~ScriptContainer();

	const Common::UString &getScript(Script script) const;

	bool hasScript(Script script) const;

	bool runScript(Script script, Aurora::NWScript::Object *owner = 0,
	               Aurora::NWScript::Object *triggerer = 0);

	static bool runScript(const Common::UString &script,
	                      Aurora::NWScript::Object *owner = 0,
	                      Aurora::NWScript::Object *triggerer = 0);
	static bool runScript(const Common::UString &script,
	                      const Aurora::NWScript::ScriptState &state,
	                      Aurora::NWScript::Object *owner = 0,
	                      Aurora::NWScript::Object *triggerer = 0);

protected:
	void clearScripts();

	void readScripts(const Aurora::GFFStruct &gff);
	void readScripts(const ScriptContainer &container);

private:
	Common::UString _scripts[kScriptMAX];
};

} // End of namespace NWN

} // End of namespace Engines

#endif // ENGINES_NWN_SCRIPT_CONTAINER_H
