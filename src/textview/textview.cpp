#include "textview.h"

#include <iostream>

#include "../physics/particle.h"
#include "../physics/element.h"
#include "../physics/accelerator.h"

void TextView::draw(const Box &to_draw){ to_draw.print(stream); }

void TextView::draw(const Particle &to_draw){ to_draw.print(stream); }

void TextView::draw(const Beam &to_draw){ to_draw.print(stream); }

void TextView::draw(const Element &to_draw){ to_draw.print(stream); }

void TextView::draw(const Accelerator &to_draw){ to_draw.print(stream, false); }
