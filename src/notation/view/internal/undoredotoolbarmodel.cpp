/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * MuseScore Studio
 * Music Composition & Notation
 *
 * Copyright (C) 2024 MuseScore Limited
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

#include "undoredotoolbarmodel.h"

#include "uicomponents/view/menuitem.h"

using namespace mu::notation;
using namespace muse;
using namespace muse::uicomponents;

UndoRedoToolbarModel::UndoRedoToolbarModel(QObject* parent)
    : QObject(parent), Injectable(iocCtxForQmlObject(this))
{
}

void UndoRedoToolbarModel::load()
{
    context()->currentNotationChanged().onNotify(this, [this]() {
        updateItems();

        auto stack = undoStack();
        if (stack) {
            stack->stackChanged().onNotify(this, [this]() {
                updateItems();
            });
        }
    });

    auto stack = undoStack();
    if (stack) {
        stack->stackChanged().onNotify(this, [this]() {
            updateItems();
        });
    }

    m_undoItem = new MenuItem(actionsRegister()->action("undo"), this);
    m_redoItem = new MenuItem(actionsRegister()->action("redo"), this);

    updateItems();

    // Only to let QML know that `m_undoItem` and `m_redoItem` have been initialised;
    // changes to their properties will be communicated via those MenuItems' own signals.
    emit itemsChanged();
}

MenuItem* UndoRedoToolbarModel::undoItem() const
{
    return m_undoItem;
}

MenuItem* UndoRedoToolbarModel::redoItem() const
{
    return m_redoItem;
}

void UndoRedoToolbarModel::updateItems()
{
    auto stack = undoStack();

    if (m_undoItem) {
        const TranslatableString undoActionName = stack ? stack->topMostUndoActionName() : TranslatableString();
        ui::UiActionState state;
        state.enabled = stack ? stack->canUndo() : false;
        m_undoItem->setState(state);
        m_undoItem->setTitle(undoActionName.isEmpty()
                             ? TranslatableString("action", "Undo")
                             : TranslatableString("action", "Undo ‘%1’").arg(undoActionName));
    }

    if (m_redoItem) {
        const TranslatableString redoActionName = stack ? stack->topMostRedoActionName() : TranslatableString();
        ui::UiActionState state;
        state.enabled = stack ? stack->canRedo() : false;
        m_redoItem->setState(state);
        m_redoItem->setTitle(redoActionName.isEmpty()
                             ? TranslatableString("action", "Redo")
                             : TranslatableString("action", "Redo ‘%1’").arg(redoActionName));
    }
}

void UndoRedoToolbarModel::redo()
{
    if (undoStack()) {
        undoStack()->redo(nullptr);
    }
}

void UndoRedoToolbarModel::undo()
{
    if (undoStack()) {
        undoStack()->undo(nullptr);
    }
}

INotationUndoStackPtr UndoRedoToolbarModel::undoStack() const
{
    INotationPtr notation = context()->currentNotation();
    return notation ? notation->undoStack() : nullptr;
}
