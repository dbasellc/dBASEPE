///////////////////////////////////////////////////////////////////////////////
// Filename:    PBULDER.CC                                                   // 
// Written for: dBASE (32-bit versions)                                      //
// Written by:  dBASE Inc. R&D Group                                         //
// Copyright:   Copyright © dBASE Inc. 2002                                  //
// Purpose:     Project Class                                                //
// Usage:       set procedure to :<sourceAlias>:PBuilder.CC additive         //
//              use as normal ToolBar                                        //
///////////////////////////////////////////////////////////////////////////////
// Versioning and History comments:
#define PBUILDER_VERSION  "1000"  // 04/30/1998 - Original version.

class pBuilderTBar of Toolbar

   with (this)
      flat = true
      floating = false
      //imageheight = 16
      //beforerelease = CLASS::BEFORERELEASE
   endwith

   this.oAppIni = null    // coreini object for plus.ini
   this.version =     PBUILDER_VERSION
   this.imageSuffix = iif(this.UseLargeImages(), "24", "16")

   this.newButton = new toolButton(this)
   //this.newButton.bitmapWidth = 18
   //this.newButton.bitmap = "RESOURCE #2060"
   this.newButton.bitmap = "RESOURCE NEW"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"

   //this.newButton.bitmap = "FILE :ProjExp:NEW.BMP"
   this.newbutton.speedTip = 'Create New Project'
   this.newButton.onClick = {;this.parent.form.newProject()}

   this.openButton = new toolButton(this)
   //this.openButton.bitmapWidth = 18
   //this.openButton.bitmap = "RESOURCE TS_OPEN"    
   this.openButton.bitmap = "RESOURCE OPEN"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
   this.openButton.speedTip = 'Open Existing Project...'
   this.openButton.onClick = {;this.parent.form.openProject()}

   this.saveButton = new toolButton(this)
   //this.saveButton.bitmapWidth = 18
   //this.saveButton.bitmap = "RESOURCE TS_SAVE"    
   //this.saveButton.bitmap = "RESOURCE SAVE :ProjExp:ProjExp.dll"
   this.saveButton.bitmap = "RESOURCE SAVE"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
   this.saveButton.speedTip = 'Save Project'
   this.saveButton.onClick = {;this.parent.form.saveProject(false)}

   this.separator1 = new toolButton(this)
   this.separator1.separator = true

   this.addButton = new toolButton(this)
   //this.addButton.bitmap = "RESOURCE TS_APPEND"
   this.addButton.bitmap = "RESOURCE PROJECTADDFILE"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
   this.addButton.speedtip = 'Add File to Project...'
   //this.addButton.onClick = {;this.parent.parent.addFiles()}
   this.addButton.onClick = {;this.parent.form.addFiles()}

   this.deleteButton = new toolButton(this)        
   //this.deleteButton.bitmap = "RESOURCE TS_DELETE"
   this.deleteButton.bitmap = "RESOURCE PROJECTDELETEFILE"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
   this.deletebutton.speedTip = 'Remove File from Project'
   this.deleteButton.onClick = {;this.parent.form.removeFiles()}

   this.separator3 = new toolButton(this)
   this.separator3.separator = true
  
   this.compileButton = new toolButton(this)        
   //this.compileButton.bitmap = "RESOURCE #616"
   this.compileButton.bitmap = "RESOURCE COMPILEFILE"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
   this.compileButton.speedTip = 'Compile File'
   this.compileButton.onClick = {;this.parent.form.compileProg()}

   this.compileAllButton = new toolButton(this)        
   //this.compileAllButton.bitmap = "RESOURCE COMPILEALL :ProjExp:ProjExp.dll"
   this.compileAllButton.bitmap = "RESOURCE COMPILEALL"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
   this.compileAllButton.speedTip = 'Compile All'
   this.compileAllButton.onClick = {;this.parent.form.compileAll()}

   this.designButton = new toolButton(this)        
   //this.designButton.bitmap = "RESOURCE DESIGN :ProjExp:ProjExp.dll"
   this.designButton.bitmap = "RESOURCE DESIGN"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
   //RESOURCE #2270"
   this.designButton.speedTip = 'Design File'
   this.designButton.onClick = {;this.parent.form.designProg()}
    
   this.editButton = new toolButton(this)        
   //this.editButton.bitmap = "RESOURCE TS_EDIT"
   this.editButton.bitmap = "RESOURCE PROJECTEDIT"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
   this.editButton.speedTip = 'Edit File'
   this.editButton.onClick = {;this.parent.form.editProg()}
    
   this.runButton = new toolButton(this)        
   //this.runButton.bitmap = "RESOURCE TS_RUN"
   this.runButton.bitmap = "RESOURCE PROJECTEXECUTE"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
   this.runButton.speedTip = 'Run File'
   this.runButton.onClick = {;this.parent.form.runFile()}
    
   this.separator2 = new toolButton(this)
   this.separator2.separator = true

   this.buildButton = new toolButton(this)        
   //this.buildButton.bitmap = "RESOURCE #2270"
   //this.buildButton.bitmap = "RESOURCE BUILD ProjExpResource.dll"
   //this.buildButton.bitmap = "RESOURCE BUILD :ProjExp:ProjExp.dll"
   this.buildButton.bitmap = "RESOURCE BUILDEXE"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
   this.buildButton.speedTip = 'Build Exe'
   this.buildButton.onClick = {;this.parent.form.buildExe()}
    
   this.buildAllButton = new toolButton(this)        
   //this.buildButton.bitmap = "RESOURCE #2270"
   //this.buildAllButton.bitmap = "RESOURCE BUILDALL :ProjExp:ProjExp.dll"
   this.buildAllButton.bitmap = "RESOURCE BUILDALL"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
   this.buildAllButton.speedTip = 'Rebuild All'
   this.buildAllButton.onClick = {;this.parent.form.rebuildAll()}
    
   this.HelpButton = new toolButton(this)
   //this.HelpButton.bitmap = "RESOURCE HELP :ProjExp:ProjExp.dll"
   this.HelpButton.bitmap = "RESOURCE HELP"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
   this.HelpButton.speedTip = 'Project Explorer Online Help'

   // QAID: 5093  - M.Kay, dBI Inc. - 11/21/2004
//   if not file(_app.sourceAliases['ProjExp']+'\PExplorer.hlp')
//      this.HelpButton.onClick = {;this.parent.form.qLang.iMsgBox('Check Gold Site for updated help file.','No Help File',16)}
//   else
//      this.HelpButton.onClick = {;set help to (_app.sourceAliases['ProjExp']+'\PExplorer.hlp');help}     
//   endif
   this.HelpButton.onClick = CLASS::HELP_ONCLICK
   // End QAID: 5093

	//D.Brooks 11/22/2004 - makes help not launch, should use method instead
//   if not file(_app.sourceAliases['ProjExp']+'\PExplorer.hlp')
//      this.HelpButton.onClick = {;this.parent.form.qLang.iMsgBox('Check Gold Site for updated help file.','No Help File',16)}
//   else
//      this.HelpButton.onClick = {;set help to (_app.sourceAliases['ProjExp']+'\PExplorer.hlp');help}     
//   endif



   function onUpdate
      if (type('this.form') # 'O') or (empty(this.form))
         return
      endif

      local cExe, n

      cExe = this.form.Project.getShortName(this.form.Project.ExeName)
      this.compileAllButton.enabled = this.form.project.areAnyToCompile()
      this.buildButton.enabled = this.compileAllButton.enabled
      this.buildButton.speedTip = this.form.qLang.iGetText("Build")+" "+cExe
      this.buildAllButton.enabled = this.compileAllButton.enabled

      if this.form.treeview1.selected.typ # "H"
        n = this.form.treeview1.selected.ContentsNo
        this.editButton.enabled    = this.form.project.contents[n].canBeEdited
        this.designButton.enabled  = not empty(this.form.project.contents[n].designer)
        this.compilebutton.enabled = this.form.project.contents[n].canBeCompiled
        this.runButton.enabled     = not empty(this.form.project.contents[n].runWith) and this.form.project.contents[n].runWith <> "-"
        this.deleteButton.enabled  = true
      else
        this.editButton.enabled    = false
        this.designButton.enabled  = false
        this.compilebutton.enabled = false
        this.deleteButton.enabled  = false
        this.runButton.enabled     = false
      endif
      if _app.Language#"EN"
         //already in english so dont be called
         if this.newbutton.speedTip#this.form.qLang.iGetText("Create New Project")
            class::SetLanguage()
         endif
      endif

      return

   function SetLanguage      
      with this
         newbutton.speedTip:=form.qLang.iGetText("Create New Project")
         addButton.speedtip = form.qLang.iGetText('Add File to Project')+'...'
         buildAllButton.speedTip = form.qLang.iGetText('Rebuild All')
         compileAllButton.speedTip = form.qLang.iGetText('Compile All')
         compileButton.speedTip = form.qLang.iGetText('Compile File')
         deletebutton.speedTip = form.qLang.iGetText('Remove File from Project')
         designButton.speedTip = form.qLang.iGetText('Design File')
         editButton.speedTip = form.qLang.iGetText('Edit File')
         HelpButton.speedTip = form.qLang.iGetText('Project Manager Online Help')
         openButton.speedTip = form.qLang.iGetText('Open Existing Project')+'...'
         runButton.speedTip = form.qLang.iGetText('Run File')
         saveButton.speedTip = form.qLang.iGetText('Save Project')
      endwith
      return




   // QAID: 5093  - M.Kay, dBI Inc. - 11/21/2004
   function Help_OnClick
      private m_helpFile, m_helpID, m_saveHelpFile

      //m_helpFile = _dbwinhome+"\HELP\PLUS9_EN.chm"   

      m_helpFile = setto("help")

      //m_helpID = "Project Explorer,Overview"
      m_helpID = "Project Explorer"

      if not file( m_helpFile )

         m_helpFile = _dbwinhome+"\HELP\PLUS10_EN.chm"   
         //m_helpID = "#2005"
         //m_helpID = "Project Explorer,Overview"

         if not file( m_helpFile )
            if (type('this.form') = 'O') and (not empty(this.form))
               this.form.qLang.iMsgBox('Cannot find help file.'+m_helpFile,'No Help File',16)
            else
               msgBox('Cannot find help file.'+m_helpFile,'No Help File',16)
            endif
            return
         endif
      endif

      set help to ( m_helpFile )

      if empty( m_helpID )
         Help
      else
         Help &m_helpID.
      endif

      set help to // reset to default help file

      return
   // End QAID: 5093 



   function checkEnabledStatus()
      // the project contents array contains all the files used in the project as read from the projects file
//      local n
//      if (type('this.form') # 'O') or (empty(this.form))
//         return false
//      endif
//
//      local n
//
//      if this.form.treeview1.selected.typ # "H"
//        n = this.form.treeview1.selected.ContentsNo
//        this.editButton.enabled    = this.form.project.contents[n].canBeEdited
//        this.designButton.enabled  = not empty(this.form.project.contents[n].designer)
//        this.compilebutton.enabled = this.form.project.contents[n].canBeCompiled
//        this.runButton.enabled     = not empty(this.form.project.contents[n].runWith) and this.form.project.contents[n].runWith <> "-"
//        this.deleteButton.enabled  = true
//      else
//        this.editButton.enabled    = false
//        this.designButton.enabled  = false
//        this.compilebutton.enabled = false
//        this.deleteButton.enabled  = false
//        this.runButton.enabled     = false
//      endif

   return



   function UseLargeImages
      // Retrieve current setting for
      //[Tools]
      //LargeButtons=1
      // from _app.inifile
      private lUseLargeImages

      if empty( this.oAppIni )
         this.oAppIni = new coreini( _app.inifile )
      endif

      lUseLargeImages = iif( this.oAppIni.getValue("Tools", "LargeButtons")=1, true, false)

      return lUseLargeImages



   function GetHasLargeImages
      return iif( this.imageSuffix == "24", true, false)


//   function CheckButtonSize
//      private newSuffix
//
//      newSuffix = iif(this.UseLargeImages(), "24", "16")
//      if not (newSuffix == this.imageSuffix)
//         this.imageSuffix = newSuffix
//
//         if (type('this.form') = 'O') and (not empty(this.form))
//            // This isn't working correctly yet!
//
//
//            //this.detach(this.form)
//
//            this.newButton.bitmap = "RESOURCE NEW"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
//            this.openButton.bitmap = "RESOURCE OPEN"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
//            this.saveButton.bitmap = "RESOURCE SAVE"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
//            this.addButton.bitmap = "RESOURCE PROJECTADDFILE"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
//            this.deleteButton.bitmap = "RESOURCE PROJECTDELETEFILE"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
//            this.compileButton.bitmap = "RESOURCE COMPILEFILE"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
//            this.compileAllButton.bitmap = "RESOURCE COMPILEALL"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
//            this.designButton.bitmap = "RESOURCE DESIGN"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
//            this.editButton.bitmap = "RESOURCE PROJECTEDIT"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
//            this.runButton.bitmap = "RESOURCE PROJECTEXECUTE"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
//            this.buildButton.bitmap = "RESOURCE BUILDEXE"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
//            this.buildAllButton.bitmap = "RESOURCE BUILDALL"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
//            this.HelpButton.bitmap = "RESOURCE HELP"+this.imageSuffix+" :PROJEXP:PER_2013.DLL"
//
//            //this.attach(this.form)
//         endif
//      endif
//      return


   function beforeRelease
      if not empty( this.oAppIni )
         this.oAppIni.release()
         this.oAppIni = null
      endif

endClass

// programm supplied by romain strieff
// will open an html file and display it using
// the correct programm eg. IE or Netscape
// usage:
// open_url(<filename>)

Function Win_Call(cURL,cParam)
#define SW_SHOWNORMAL 1
#define SW_SHOWMINIMIZED 2
#define SW_SHOWMAXIMIZED 3
#define SW_SHOWNOACTIVATE 4


#if __vdb__ >= 7
  #define WINAPI_A "A"
  #define WINAPI_SHELL "SHELL32"
#else
  #define WINAPI_A ""
  #define WINAPI_SHELL "SHELL" 
#endif

if type("ShellExecute") #"FP"
   extern cHandle   ShellExecute(cHandle, cstring, cstring, cstring, ;
                      cstring, CINT) WINAPI_SHELL ;
                       from "ShellExecute"+WINAPI_A
endif
local nParam, nHandle
private sd                 && macro needs private
sd=set("dire")             &&sometimes it _could_ change dirs, so prevent it
if pcount()<2
  nParam=""
else
  nParam=cParam
endif 
nHandle=ShellExecute(_app.framewin.hwnd,"open",;
cUrl,nParam,null,SW_SHOWNORMAL)
if sd#set("dire")
  set dire to "&sd."
endif
return nHandle 
