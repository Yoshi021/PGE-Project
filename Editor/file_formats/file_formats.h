/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2014 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef FILE_FORMATS_H
#define FILE_FORMATS_H

#include <QRegExp>
#include <QString>
#include <QFile>


#include "lvl_filedata.h"
#include "npc_filedata.h"
#include "wld_filedata.h"



//SMBX64 standard data
class SMBX64
{
public:
    SMBX64() {}

    // /////////////Validators///////////////
    //returns TRUE on wrong data
    static bool Int(QString in); // UNSIGNED INT
    static bool sInt(QString in); // SIGNED INT
    static bool sFloat(QString in); // SIGNED FLOAT
    static bool qStr(QString in); // QUOTED STRING
    static bool wBool(QString in); //Worded BOOL
    static bool dBool(QString in); //Worded BOOL

    //SMBX64 parameter string generators
    static QString IntS(long input);
    static QString BoolS(bool input);
    static QString qStrS(QString input);
};


class FileFormats
{
public:
    //File format read functions
    // SMBX64 LVL File
    static LevelData ReadLevelFile(QFile &inf);             //read
    static QString WriteSMBX64LvlFile(LevelData FileData);  //write

    // SMBX64 Lvl Data
    static LevelNPC dummyLvlNpc();

    // SMBX64 NPC.TXT File
    static NPCConfigFile ReadNpcTXTFile(QFile &inf, bool IgnoreBad=false); //read
    static QString WriteNPCTxtFile(NPCConfigFile FileData);                //write

    static NPCConfigFile CreateEmpytNpcTXTArray();

    // SMBX64 WLD File
    static WorldData ReadWorldFile(QFile &inf); //read



    //common
    static void BadFileMsg(QString fileName_DATA, int str_count, QString line);
    static QString removeQuotes(QString str); // Remove quotes from begin and end
};

#endif // FILE_FORMATS_H
