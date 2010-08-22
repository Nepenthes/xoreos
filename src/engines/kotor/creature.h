/* eos - A reimplementation of BioWare's Aurora engine
 * Copyright (c) 2010 Sven Hesse (DrMcCoy), Matthew Hoops (clone2727)
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 *
 * This file is part of eos and is distributed under the terms of
 * the GNU General Public Licence. See COPYING for more informations.
 */

/** @file engines/kotor/creature.h
 *  A creature.
 */

#ifndef ENGINES_KOTOR_CREATURE_H
#define ENGINES_KOTOR_CREATURE_H

#include <list>

#include "engines/kotor/modelobject.h"

#include "common/types.h"

#include "graphics/aurora/types.h"

namespace Common {
	class UString;
}

namespace Engines {

class ModelLoader;

namespace KotOR {

class Creature : public ModelObject {
public:
	Creature(const ModelLoader &modelLoader);
	~Creature();

	void load(const Common::UString &name);

	void show();
	void hide();

private:
	struct Part {
		Graphics::Aurora::Model *model;

		Part();
		Part(Graphics::Aurora::Model *m);
		Part(Part &p);
		~Part();
	};

	uint32 _appearance;

	std::list<Part *> _parts;

	void changedPosition();
	void changedBearing();
	void changedOrientation();

	void loadModel(const Common::UString &name);
};

} // End of namespace KotOR

} // End of namespace Engines

#endif // ENGINES_KOTOR_CREATURE_H