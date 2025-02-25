/*
    Copyright (C) 2001, S.R.Haque <srhaque@iee.org>.
    Copyright (C) 2002, David Faure <david@mandrakesoft.com>
    Copyright (C) 2004, Arend van Beelen jr. <arend@auton.nl>
    This file is part of the KDE project

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2, as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LGPL-2.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef KFIND_P_H
#define KFIND_P_H

#include "kfind.h"

#include <QDialog>
#include <QHash>
#include <QList>
#include <QPointer>
#include <QString>

struct Q_DECL_HIDDEN KFind::Private {
    Private(KFind *q)
        : q(q)
        , findDialog(nullptr)
        , currentId(0)
        , customIds(false)
        , patternChanged(false)
        , matchedPattern(QLatin1String(""))
        , emptyMatch(nullptr)
    {
    }

    ~Private()
    {
        if (dialog) {
            dialog->deleteLater();
        }
        dialog = nullptr;
        data.clear();
        delete emptyMatch;
        emptyMatch = nullptr;
    }

    struct Match {
        Match() : dataId(-1), index(-1), matchedLength(-1) {}
        bool isNull() const
        {
            return index == -1;
        }
        Match(int _dataId, int _index, int _matchedLength) :
            dataId(_dataId),
            index(_index),
            matchedLength(_matchedLength)
        {
            Q_ASSERT(index != -1);
        }

        int dataId;
        int index;
        int matchedLength;
    };

    struct Data {
        Data() { }
        Data(int id, const QString &text, bool dirty = false) :
            text(text),
            id(id),
            dirty(dirty)
        { }

        QString text;
        int     id = -1;
        bool    dirty = false;
    };

    void init(const QString &pattern);
    void startNewIncrementalSearch();

    void _k_slotFindNext();
    void _k_slotDialogClosed();

    KFind *q;
    QPointer<QWidget>  findDialog;
    int                   currentId;
    bool                  customIds : 1;
    bool                  patternChanged : 1;
    QString               matchedPattern;
    QHash<QString, Match>  incrementalPath;
    Match                *emptyMatch;
    QList<Data>           data; // used like a vector, not like a linked-list

    QString pattern;
    QRegExp *regExp;
    QDialog *dialog;
    long options;
    unsigned matches;

    QString text; // the text set by setData
    int index;
    int matchedLength;
    bool dialogClosed : 1;
    bool lastResult : 1;
};

#endif // KFIND_P_H
