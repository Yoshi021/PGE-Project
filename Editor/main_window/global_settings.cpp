/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2014-2016 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "global_settings.h"

EditingSettings::EditingSettings()
{
    animationEnabled=true;
    collisionsEnabled=true;
    grid_snap=true;
    grid_override=false;
    customGrid.setWidth(0);
    customGrid.setHeight(0);
    grid_show=false;
    semiTransparentPaths=false;
}

QString GlobalSettings::locale          = "";
long GlobalSettings::animatorItemsLimit = 25000;
QString GlobalSettings::openPath        = ".";
QString GlobalSettings::savePath        = ".";
QString GlobalSettings::savePath_npctxt = ".";

QString GlobalSettings::tools_sox_bin_path  = "/tools/sox/";

EditingSettings  GlobalSettings::LvlOpts;
SETTINGS_ItemDefaults GlobalSettings::LvlItemDefaults;
SETTINGS_TestSettings GlobalSettings::testing; //Testing settings
SETTINGS_ScreenGrabSettings GlobalSettings::screenGrab;

bool GlobalSettings::autoPlayMusic  = false;
int  GlobalSettings::musicVolume    = 128;
bool GlobalSettings::recentMusicPlayingState = false;

bool GlobalSettings::MidMouse_allowDuplicate    = true;
bool GlobalSettings::MidMouse_allowSwitchToPlace= true;
bool GlobalSettings::MidMouse_allowSwitchToDrag = true;

bool GlobalSettings::Placing_dontShowPropertiesBox  = false;

int  GlobalSettings::historyLimit   = 300;

QString GlobalSettings::currentTheme= "";

bool GlobalSettings::ShowTipOfDay   = true;

QMdiArea::ViewMode      GlobalSettings::MainWindowView = QMdiArea::TabbedView;
QTabWidget::TabPosition GlobalSettings::LVLToolboxPos  = QTabWidget::North;
QTabWidget::TabPosition GlobalSettings::WLDToolboxPos  = QTabWidget::West;
QTabWidget::TabPosition GlobalSettings::TSTToolboxPos  = QTabWidget::North;

int GlobalSettings::lastWinType     = 0;

