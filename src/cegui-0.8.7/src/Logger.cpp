/***********************************************************************
    created:    21/2/2004
    author:     Paul D Turner
    
    purpose:    Implementation of the Logger class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "CEGUI/Precompiled.h"

#include "CEGUI/Logger.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    /*************************************************************************
        Static Data Definitions
    *************************************************************************/
    // singleton instance pointer
    template<> Logger* Singleton<Logger>::ms_Singleton  = 0;


    /*************************************************************************
        Constructor
    *************************************************************************/
    Logger::Logger(void) :
            d_level(Standard)
    {
    }

    /*************************************************************************
        Destructor
    *************************************************************************/
    Logger::~Logger(void)
    {
    }

} // End of  CEGUI namespace section
