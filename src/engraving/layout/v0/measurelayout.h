/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef MU_ENGRAVING_MEASURELAYOUT_H
#define MU_ENGRAVING_MEASURELAYOUT_H

#include "../layoutoptions.h"
#include "layoutcontext.h"

namespace mu::engraving {
class Measure;
class MeasureBase;
class Score;
class Segment;
}

namespace mu::engraving::layout::v0 {
class MeasureLayout
{
public:
    MeasureLayout() = default;

    static void layout2(Measure* item, LayoutContext& ctx);

    static void getNextMeasure(const LayoutOptions& options, LayoutContext& ctx);
    static void computePreSpacingItems(Measure* m, LayoutContext& ctx);

    static void layoutStaffLines(Measure* m, LayoutContext& ctx);
    static void layoutMeasureNumber(Measure* m, LayoutContext& ctx);
    static void layoutMMRestRange(Measure* m, LayoutContext& ctx);
    static void layoutMeasureElements(Measure* m, LayoutContext& ctx);

    static double createEndBarLines(Measure* m, bool isLastMeasureInSystem, LayoutContext& ctx);
    static void addSystemHeader(Measure* m, bool isFirstSystem, LayoutContext& ctx);
    static void addSystemTrailer(Measure* m, Measure* nm, LayoutContext& ctx);

    static void createSystemBeginBarLine(Measure* m, LayoutContext& ctx);

    static void stretchMeasureInPracticeMode(Measure* m, double targetWidth, LayoutContext& ctx);

private:

    static void createMMRest(const LayoutOptions& options, Score* score, Measure* firstMeasure, Measure* lastMeasure, const Fraction& len);

    static int adjustMeasureNo(LayoutContext& lc, MeasureBase* m);

    static void barLinesSetSpan(Measure* m, Segment* seg, LayoutContext& ctx);
};
}

#endif // MU_ENGRAVING_MEASURELAYOUT_H