//#############################################################################
//#  resource.h
//#  ==========
//#
//#  Creation:       Tue May 14 2002
//#  Author:         Ralf Schneider  (ralf.schneider@b-connected.de)
//#  Copyright:      (C) 2002 by B-connected Linux Solutions GmbH
//#
//#  ------------------------------------------------------------------------
//#
//#  This file contains definitions that are global for all parts of the
//#  application.
//#
//#  ------------------------------------------------------------------------
//#
//#  This program is free software; you can redistribute it and/or modify it
//#  under the terms of the GNU General Public License as published by the
//#  Free Software Foundation; either version 2 of the License, or (at your
//#  option) any later version.
//#
//#############################################################################

#ifndef RESOURCE_H
#define RESOURCE_H

//=============================================================================
// Return codes
//=============================================================================
#define ERROR_LOGIN_CANCELED           1
#define ERROR_DB_NOT_RUNNING           2
#define ERROR_USER_NOT_FOUND           3


//=============================================================================
// Geometry settings
//=============================================================================
#define WINDOW_WIDTH                   930
#define GROUPBOX_WIDTH                 WINDOW_WIDTH - 325
#define LINEEDIT_WIDTH                 GROUPBOX_WIDTH - 175


//=============================================================================
// Application specific defines.
//=============================================================================
#define MAX_CURRENCY									 1000000000000.0


//=============================================================================
// ID's for menu and toolbar items
//=============================================================================
#define MENU_ITEM_NEW              1
#define MENU_ITEM_EDIT             2
#define MENU_ITEM_DELETE           3

#define TOOLBAR_ITEM_PATTERN       1
#define TOOLBAR_ITEM_REFRESH       1


//=============================================================================
// Misc ID's
//=============================================================================
#define ID_STATUS_MSG               1
#define ID_STATUS_DB                2
#define ID_STATUS_HOST              3
#define ID_STATUS_USER              4
#define ID_STATUS_GROUP             5

#define IDS_STATUS_DEFAULT          "   Bereit."

#define VERSION                     "0.1"

#endif // RESOURCE_H
