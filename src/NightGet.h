/***************************************************************************
 *   Copyright (C) USIC 2008 by Sergiy Kibrik, Olha Yevtushenko   *
 *   sakib@meta.ua,caelum@meta.ua   *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef NIGHT_GET_H
#define NIGHT_GET_H
#include <sys/types.h>
#include <stdlib.h>
/*
* define download status so far
*/
#define NG_INIT 0x0
#define NG_FINISHED 0x1
#define NG_STARTED 0x2
#define NG_ERROR 0x3
#define NG_EMPTY 0x4


typedef unsigned short state_t;
typedef unsigned short priority_t;
typedef unsigned long node_id;


class NightGetNode;
class NightGetDownloadMethod;
class NightGetSimpleQueue;
class NightGetPriorityQueue;
class NightGetController;


/* everything starts here */
#include "NightGetController.h"

#endif // NIGHT_GET_H

