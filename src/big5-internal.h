/***************************************************************************
 *   Copyright (C) YEAR~YEAR by Your Name                                  *
 *   your-email@address.com                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.              *
 ***************************************************************************/

#ifndef _FCITX_BIG5_INTERNAL_H_
#define _FCITX_BIG5_INTERNAL_H_

#include "big5.h"
#include <fcitx/instance.h>
#include <fcitx-config/fcitx-config.h>
#include <fcitx-config/hotkey.h>

#define _(x) dgettext("fcitx-big5", x)

typedef struct {
    FcitxGenericConfig gconfig;
} FcitxBig5Config;

typedef struct {
    FcitxBig5Config config;
    FcitxInstance *owner;
    FcitxHotkey hkToggle[2];
    boolean useBig5;
} FcitxBig5;

CONFIG_BINDING_DECLARE(FcitxBig5Config);

#endif
