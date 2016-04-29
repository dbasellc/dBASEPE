///////////////////////////////////////////////////////////////////////////////
// Filename:    ProjExp.WFM                                                  //
// Written for: dBASE (32-bit versions)                                      //
// Written by:  dBASE Inc. R&D Group                                         //
// Copyright:   Copyright 2002 to 2006, dataBased Intelligence, Inc.         //
// Purpose:     dBL Project Explorer                                         //
// Parameters:  ProjectFilename Optional                                     //
///////////////////////////////////////////////////////////////////////////////

#include ProjExp.h

//the main parameter is used to pass the projectname
parameters cProjectFileName
local cTalk, f, o
cTalk = set("talk")
set talk off

f = findinstance("PROJEXPFORM")
if empty(f)

   f = new projExpForm()
   f.cTalk = cTalk

   if empty(cProjectFileName) or ;
      upper(cProjectFileName) = 'UNTITLE'

      f.cProjectFileName = 'Untitled'
      // This is a new one

   else
      f.cProjectFileName = cProjectFileName

   endif

   f.init()
   f.open()
else

   if empty(cProjectFileName) or ;
      upper(cProjectFileName) = 'UNTITLE'
      f.setFocus()
      f.newProject()

   else
      f.setFocus()
      f.openProject(cProjectFileName)

   endif

endif
return