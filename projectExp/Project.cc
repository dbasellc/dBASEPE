///////////////////////////////////////////////////////////////////////////////
// Filename:    project.cc                                                   // 
// Written for: dBASE (32-bit versions)                                      //
// Written by:  dBASE Inc. R&D Group                                         //
// Copyright:   Copyright © dBASE Inc. 2002                                  //
// Purpose:     Project Class                                                //
// usage:       set procedure to :<sourceAlias>:project.cc additive          //
//              <oRef> = new Project()                                       //
//              <oref>.loadFileTypes()  // if you are creating a new project //
//              <oref>.load(<fileName>) // to load an existing projectFile   //
//              <oref>.save(<filename>,lForceSaveAs)                         //
//              <oref>.build()          // build the target executable       //
//              <oref>.rebuild()        // compile all and build             //
///////////////////////////////////////////////////////////////////////////////
#include ProjExp.h
#include winreg.h

CLASS project(f)
  
  LOCAL i

  this.form                   = f
  this.projectName            = ""       // new property
  this.FileName               = ""
  this.Folder                 = set("directory")
  this.ProjectFileName        = ""

  // M.Kay - 5/16/2008
  this.PrefPathFormat         = "R"   // Can be "R" - Relative, "S" - Source Alias, "F" - Full (explicit)
  this.OldPrefPathFormat      = ""    // Used to hold PrefPathFormat set at Load time to check at Save time

  this.Modified               = false
  this.MainFileName           = ""
  this.parameterList          = ""
  this.Author                 = ""
  this.description            = ""
  this.maxDescription         = 1024
  this.PreprocessorDirectives = ""
  this.logFileName            = ""
  this.exeName                = ""
  this.iconFileName           = ""
  this.splashFileName         = ""
  this.webApplication         = false
  this.deoOption              = false
  this.deoTarget0             = ""
  this.verbose                = true          // set to false to suppress msgboxes 
                                              // on errors
  this.projectStatus          = ""            // for compatability
                                              // actually the first line of a
                                              // .prj file
  this.SaveRspFile           = false
  this.VerboseLog            = true

  // this array contains a list of 10 target folders for deo-deployment
  // the list is pre-loaded
  this.deoList                = new Array(10)
  for i = 0 to 9
    this.deoList[i+1]         = ltrim(str(i))+"="
  next

  // this array contains 1 object for each file
  this.contents               = new Array()

  // a couple of counters
  this.compileCount = 0
  this.deoCount     = 0

  //for Inno Settings
  //J.Kwekkeboom 02/05/2005
  //Added for Windows XP Look support
  this.XPLook = false

  ** for runtime
  this.SM_dBASERuntime = false
  this.dBPlus          = true
  this.dBSeparat       = false
  this.Resource        = false
  this.BDEGroupEntries = false
  this.dBProgressbar   = false
  this.dBProgressbox   = false
  this.dBSilent        = false
  this.dBASELang       = _app.Language

  this.dBaseBuild      = Substr(ltrim(Substr(version(0),len(Version())+1)),2,AT(" ",ltrim(Substr(version(0),len(Version()+2))))-2)
  this.dBaseVersion    = stuff(ltrim(substr(version(),AT(" ",version(),2))),AT(".",ltrim(substr(version(),AT(" ",version(),2)))),1,"")
  
  this.dBaseRuntimeDir = Substr(home(),1,len(home())-4)+"Runtime" //KK (08/20/2012) - added because other old dBaseRuntimeDir references that were used for the Runtime installer as well as the Plus\Runtime directory had to be seperated
  this.INNO_RBoption = 3 //default is 'No runtime installation
  this.bdeonlyinstall = false
  this.bdeinstallfile = ""

  do while len(this.dBaseVersion)<4
     this.dBaseVersion+="0"
  enddo
  //J.Kwekkeboom 17/12/2004 - added for individual Engine filename
  //this.dBASEEngineName = this.dBaseRuntimeDir+"\PlusRuntime-b"+this.dBaseBuild+'.exe'

  //D.Brooks 12/22/2004 - updated to contain default runtime file name
  //this.dBaseRuntimeDir = Substr(home(),1,len(home())-4)+"Runtime"+"\PlusRuntime-b"+this.dBaseBuild+'.exe'
  
  //KK 07/25/2012 - PlusRuntime-b.... is not automatically included in Runtime folder so setting this initially to " " 
  //this.dBaseRuntimeDir = ""

  //Jonny Kwekkeboom 08/19/2012
  // we add at first the normal needed RuntimeEngine file
  this.dBaseRuntimeInstDir = Substr(home(),1,len(home())-4)+"Runtime"+"\PlusRuntime-b"+this.dBaseBuild+'.exe' //KK (08/20/2012) Change from this.dBaseRuntimeDir to dBaseRuntimeInstDir to separate it from the Runtime Directory
  if not File(this.dBaseRuntimeInstDir)
   // is not found the file so we set to the Runtime folder for find the normal runtime files in this folder
   this.dBaseRuntimeInstDir = ""  //KK (08/20/2012) - if a runtime installer is not found in the Plus\Runtime folder - set it to blank until the project file is read
  endif
    
  //J.Kwekkeboom 01/20/2007 - Removed not more need defaults in ISG
  //this.dBDestName      = "dBASERuntime.exe"
  this.SepDestDir      = "{app}"
  //this.BDEVersion      = "5202"
  //J.Kwekkeboom 01/20/2007 - Added for new Setup languages support
  this.SetupLang       = Lower(_app.Language)  
  this.dBDestRuntime   = ""
  this.dBDestBDE       = ""

  **for UninsHs
  this.UninsHS          = true
  //D.Brooks 12/22/2004 - Keep blank for refresh purposes
  this.UninsHSAppName	= ""
  //this.UninsHSAppName   = "New Application"
  this.UninsHSAppId     = ""
  //this.UninsHSExeDir = ""
  //fixed to "All" so all languages will be included also
  //Jonny Kwekkeboom 12/16/2004
  this.UninsHSLanguages = "All"
  
  //this.UninsHSLanguages = _app.Language
  //removed so not needed by "All" !
//  if this.UninsHSLanguages="JA"
     //Japan not suported inside UninsHS so we set the
     //language to english...
//     this.UninsHSLanguages:= "EN"
//  endif


  ** defaults
  this.AppName         = this.UninsHSAppName
  this.AppVersion      = "1.0.0.0"
  this.SourceFolder    = ""
  this.DestFolder      = "{pf}\{#AppName}"
  this.NeedDirs        = new Array()
  this.NeedFiles       = new Array()
  this.NeedGroup       = new Array()
  this.InnoScriptname  = ""
  this.LastDBVersion   = ""
  this.LastDBLanguage  = ""
  this.LastCreateDate  = ""
  this.IncUninsEntry   = true
  this.IncDispGroup    = true

  //J.Kwekkebbom 02/14/2005 - added Desktop and quicklaunch Icons
  this.IncDesktopEntry = false
  this.IncQuickEntry   = false

  //D.Brooks 12/08/2004 - Language files no longer required.
//  //italian is not inside INNO official language folder!
//  //so the italian languge file can be included here to find it!
//  this.ITLanguagefile  =""
//  //spain is not inside INNO official language folder!
//  //so the spain languge file can be included here to find it!
//  this.ESLanguagefile  =""
//  //Japanese is not inside INNO official language folder!
//  //so the japanese languge file can be included here to find it!
//  this.JALanguagefile  =""

  this.ENLicense       =""
  this.ENBefore        =""
  this.ENAfter         =""
  this.DELicense       =""
  this.DEBefore        =""
  this.DEAfter         =""
  this.ESLicense       =""
  this.ESBefore        =""
  this.ESAfter         =""
  this.ITLicense       =""
  this.ITBefore        =""
  this.ITAfter         =""
  this.JALicense       =""
  this.JABefore        =""
  this.JAAfter         =""
  //Other suported languages too! France, Dutch user
  //so we let Inno compiled setup.exe display this language too!!
  this.FRLicense       =""
  this.FRBefore        =""
  this.FRAfter         =""
  this.NLLicense       =""
  this.NLBefore        =""
  this.NLAfter         =""
  //J.Kwekkeboom 01/20/2007 - added for support Dansk and Polish
  this.DKLicense       =""
  this.DKBefore        =""
  this.DKAfter         =""
  this.PLLicense       =""
  this.PLBefore        =""
  this.PLAfter         =""

  //BDE Settings
  this.BDEAlias         = new Array()
  this.BDEUseSettings	= false
  this.BDEMaxFileHandle = "255"
  this.BDELangDriver    = "'ascii' ANSI"
  this.BDELevel         = "7"
  this.BDEMemoBlockSize = "1024"
  this.BDEMDXBlockSize  = "1024"

  //KK added to support additional UAC and INI option in build command
  this.enableuac = 0 //default 0-do not use UAC -- 1-use UAC in build
  this.inioption = 1 //default 1-Standard ini 0-No ini 2-roaming ini
  
  //KK added 03/23/2015 new BUILD option to include runtime in the exe
  this.runtimeinbuild = 0 //default 0-do not include runtime in the .exe
  this.bdeoption = 1 //default 0- load bde during application startup
  
  //KK added to support INI option in INNO script
  //this.INIStandard      = false  
  this.INNOINI = false
  this.INIErrorHandling = false
  this.INITrap          = 0
  this.ININoBDE         = false
  this.INIAppThemeType  = 4 //KK added 02/26/2014 for dbase plus 9.0.0.0
  this.INIFullThemeName = "4 - Office2 Blue" //KK added 02/26/2014 for dbase plus 9.0.0.0
  this.INILargeButtons       = true //KK added 02/26/2014 for dbase plus 9.0.0.0
  this.UACRB            = 1   //Default Uac Setting
  **End of Inno Settings
    
  
  ///////////////////////////////////////////
  // methods belonging to the project class
  ///////////////////////////////////////////

  //added for get Source folder same as Inno {app}\Subfolders...
  //10/14/2005 J. Kwekkeboom
  Function InnoDestinationGetFolder(cFileName)
  Local cResult, cMainFolder
     cResult='{app}'
     cFileName:=this.form.CheckSourceAlias(cFileName)        
     if File(cFilename)
        this.Form.FNSplit.FullPath := ltrim(trim(this.getFullPath(this.MainFileName)))
        this.Form.FNSplit.SplitFileName()
        cMainFolder=this.Form.FNSplit.DriveDir
        this.Form.FNSplit.FullPath := cFilename
        this.Form.FNSplit.SplitFileName()
        cResult:=this.Form.FNSplit.DriveDir

        // QAID:5614 - 11/01/2005 - Dave Casler 
        // Add Jonny Kwekkeboom's update to non-main folder cases
        // cResult:=STUFF(cResult,1,len(cMainFolder), '{app}')
        if At(cMainfolder, cResult)>0
           cResult:=STUFF(cResult,1,len(cMainFolder), '{app}')
        else
           cResult = '{app}'
        endif
        // end QAID:5614

     endif
  return(cResult)

  Procedure CreateXPManiFest
  Local H1,cTF
     //cTF=_app.sourceAliases[ "Samples" ]+'\PlusRun.exe.manifest'
     cTF = this.sourceFolder + '\PLUSrun.exe.manifest'
     if file(cTF)
        return
     endif
     h1 = new file()
     h1.create(cTF)
      
     h1.puts('<?xml version="1.0" encoding="UTF-8" standalone="yes"?>')
     h1.puts('<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">')
     h1.puts('  <assemblyIdentity') 
	  h1.puts('    version="1.0.0.0"') 
	  h1.puts('    processorArchitecture="x86"') 
	  h1.puts('    name="dBase PlusRun"') 
	  h1.puts('    type="win32"') 
     h1.puts('  />') 
     h1.puts('  <description>')
	  h1.puts('     dBase PlusRun Application')
     h1.puts('  </description>')
     h1.puts('  <dependency>')
	  h1.puts('    <dependentAssembly>')
	  h1.puts('	     <assemblyIdentity') 
	  h1.puts('		     type="win32"') 
	  h1.puts('		     name="Microsoft.Windows.Common-Controls"') 
	  h1.puts('		     version="6.0.0.0"') 
	  h1.puts('		     processorArchitecture="x86"') 
	  h1.puts('		     publicKeyToken="6595b64144ccf1df"') 
	  h1.puts('		     language="*"') 
	  h1.puts('	     />')
	  h1.puts('    </dependentAssembly>')
     h1.puts('  </dependency>')
     h1.puts('</assembly>')
     h1.close()
  return

  //J.Kwekkeboom 01/20/2007
  //Added Setup languages support for not always add all Setup languages
  function Language(cValue,isValue)
  local isValue, tValue
     tValue = ''
     do case
        case cValue = "English"
             tValue:='en'
        case cValue = "Deutsch"
             tValue:='de'
        case cValue = "Espaniol"
             tValue:='es'
        case cValue = "Italian"
             tValue:='it'
        case cValue = "Japan"
             tValue:='ja'
        case cValue = "Netherland"
             tValue:='nl'
        case cValue = "France"
             tValue:='fr'
        case cValue = "Dansk"
             tValue:='dk'
        case cValue = "Polish"
             tValue:='pl'
     endcase
     if not Empty(tValue)
        if AT(tValue,isValue)=0
           isValue += ' '+ tValue
           this.SetupLang := isValue
        endif
     endif
  return Trim(lTrim( isValue ))

  Function Load(cFn)
     PRIVATE sExact
     LOCAL h1, lCnt, phase, x, keyWord, data, inBuffer, bIsOk, bFound, bNewLangFound
     // load a project file given the fullpath filename
     // Note that the first line is machine language dependent and NOT db2K language
     // the parameter lCustom would be used to indicate if this is a custom project being loaded
     // this feature is unused at present
     // the load method returns false if an error occured reading the project file

     // check if the fileTypes array has been loaded
     if type("this.FileTypes") = "U"
       class::loadFileTypes()
     endif

     if not File(cFN)

       bIsOk = false

       if at('.',cFn) = 0  // if there's no extension

          // Try adding .prj
          cFN +='.prj'
          if file(cFN)

             // If this one exists, it's ok
             bISOk = true
          endif
       endif

       if not bIsOK
          this.form.qLang.iMsgBox("Cannot find file: %1",'Unable to Open',16,cFn)            
          return false
       endif
    
     endif

     sExact = set("exact")
     //set exact off//BMM
     set exact on
     h1=new file()
     h1.open(cFn)
     lCnt = 0
     Phase = 0
     bFound = false
     //J.Kwekkeboom 01/21/2007 - Added for new Language support DK PL
     bNewLangFound = false
     do while not h1.eof()
        inBuffer = h1.gets()
        //if empty(inBuffer) or left(inBuffer,1) == ";"
        //BMM - with Ldriver=BLLT1DA0, '[' == ';'!!
        if empty(inBuffer) or asc(left(inBuffer,1)) == 59
           loop
        endif
        lCnt += 1
        x=at("=",inBuffer)
        if x > 0
          keyword = upper( trim( left(inBuffer,x-1) ) )
          data = trim(ltrim(substr(inBuffer,x+1)))
        else
          keyWord = inBuffer
          data = ""
        endif
        do case
        case lCnt = 1
           this.ProjectStatus = trim(inBuffer)
        case upper(left(inBuffer,9)) = "[PROJECT]"
           Phase = 1
        case upper(left(inBuffer,10)) = "[CONTENTS]"
           if Phase # 1 // [PROJECT] section missed, must be invalid .PRJ
              exit
           endif
           Phase = 2
        case upper(left(inBuffer,5)) = "[DEO]"
           if Phase # 2 // [CONTENTS] section missed, must be invalid .PRJ
              exit
           endif
           Phase = 3
        case upper(left(inBuffer,6)) = "[INNO]"
           if Phase < 2 // [CONTENTS] section missed, must be invalid .PRJ
              exit
           endif
           Phase = 4

        case phase = 1
           do case
              case upper(keyword) = "DEFAULTDIR"
                   // ignored at present
              case upper(keyword) = "MAINPROJECTFILENAME"                               
                   if not empty(data)
                      this.MainFileName = data
                   endif
              case upper(keyword) = "NAME"
                   this.projectName = data
              case upper(keyword) = "PARAMETERS"
                   this.ParameterList = data
              case upper(keyword) = "AUTHOR"
                   this.Author = data
              case upper(keyword) = "DESCRIPTION"
                   this.description = data
              case upper(keyword) = "PREPROCESSOR"
                   this.PreprocessorDirectives = data
              case upper(keyword) = "LOGFILENAME"
                   this.logFileName = data
              case upper(keyword) = "TARGETFILENAME"
                   this.exeName = data
              case upper(keyword) = "ICONFILENAME"
                   this.iconFileName = data
              case upper(keyword) = "SPLASHFILENAME"
                   this.splashFileName = data
              case upper(keyword) = "MOSTRECENTSELECTEDFILENAME"
                   // ignored at present
              case upper(keyword) = "WEBAPPLICATION"
                   this.webapplication = iif(upper(data)="FALSE",false,true)
                   // deo processing - extended version
              case upper(keyword) ="DEOOPTION"
                   this.deoOption = iif(data="True",True,False)

              // M.Kay - 05/16/2008
              case upper(keyword) = "PREFPATHFORMAT"
                   this.PrefPathFormat    = data
                   this.OldPrefPathFormat = this.PrefPathFormat

              //K.kolosky - 12/01/2010
              case upper(keyword) = "ENABLEUAC"
                 this.enableuac = data
              case upper(keyword) = "INIOPTION"
                 this.inioption = data
				
				  //K.kolosky - 3/23/2015
              case upper(keyword) = "RUNTIMEINBUILD"
                 this.runtimeinbuild = data
					  
				  //K.kolosky - 03/23/2015
				  case upper(keyword) = "BDEOPTION"
				     this.bdeoption = data

           endcase //phase1

        case phase = 2
           class::addContentLine(inBuffer, false)
        case phase = 3
           // the deo Entries are in the form DEOn=path
           deoNumber = val(substr(keyword,4,1))
           this.deoList[deonumber + 1] = ltrim(str(deoNumber,1))+"="+data
        
        //Inno processing
        case phase = 4
             do case
                case upper(keyWord) = "INNO_RBOPTION"
                     this.INNO_RBoption = data
					 case upper(keyWord) = "BDEONLYINSTALL"
							this.bdeonlyinstall = iif(upper(data)="TRUE",true,false)
					 case upper(keyWord) = "BDEINSTALLFILE"
							this.bdeinstallfile = data
                case upper(keyWord) = "XPLOOK"
                     this.XPLook:=iif(upper(data)="TRUE",true,false)
                case upper(keyWord) = "SM_DBASERUNTIME"
                     this.SM_dBASERuntime:=iif(upper(data)="TRUE",true,false)
                case upper(keyWord) = "DBPLUS"
                     this.dBPlus:=iif(upper(data)="FALSE",false,true)
                case upper(keyWord) = "DBSEPARAT"
                     this.dBSeparat:=iif(upper(data)="TRUE",true,false)
                case upper(keyWord) = "RESOURCE"
                     this.Resource:=iif(upper(data)="TRUE",true,false)
                case upper(keyWord) = "SEPDESTDIR"
                     //D.Brooks 11/12/2004 - variable can contain a value
                     if upper(data)=="FALSE"
                     	this.SepDestDir:=false
                     else
                     	this.SepDestDir:=data
                     endif
                     this.SepDestDir:=iif(upper(data)="TRUE",true,false)
                case upper(keyWord) = "BDEGROUPENTRIES"
                     this.BDEGroupEntries:=iif(upper(data)="TRUE",true,false)
                case upper(keyWord) = "DBPROGRESSBAR"
                     this.dBProgressbar:=iif(upper(data)="TRUE",true,false)
                case upper(keyWord) = "DBPROGRESSBOX"
                     this.dBProgressbox:=iif(upper(data)="TRUE",true,false)
                case upper(keyWord) = "DBSILENT"
                     this.dBSilent:=iif(upper(data)="TRUE",true,false)             
                case upper(keyWord) = "APPNAME"
                     this.AppName:=data
                case upper(keyWord) = "APPVERSION"
                     this.AppVersion:=data
                case upper(keyWord) = "DBASELANG"
                     this.dBASELang:=data
                     	//KK (08/20/2012 - added this because it is referencing the Runtime Installer .exe directory - not necessarily the acutal Plus\Runtime dir
                case upper(keyWord) = "DBASERUNTIMEINSTDIR"
                     this.dBaseRuntimeInstDir:=data
                case upper(keyWord) = "DBASERUNTIMEDIR"
                     if ".exe" $ data
                        nlastBackSlash = RAT("\",data)
                        this.dBaseRuntimeDir := LEFT(data,nLastBackSlash-1)
                     else
                        this.dBaseRuntimeDir:=data     	
                     endif
                case upper(keyWord) = "DBASEBUILD"
                     this.dBaseBuild:=data
                case upper(keyWord) = "DBASEVERSION"
                     this.dBaseVersion:=data
                //J.Kwekkeboom 01/20/2007 - Added for new Setup languages support
                case upper(keyWord) = "SETUPLANG"
                     this.SetupLang := data
                     this.Form.NOTEBOOK1.INNONOTEBOOK.I_SETUPLANGRESULT.Value:=data
                //J.Kwekkeboom 01/20/2007 - Removed not more needed in ISG
                //case upper(keyWord) = "DBDESTNAME"
                //     this.dBDestName:=data
                //case upper(keyWord) = "BDEVERSION"
                //     this.BDEVersion:=data
                case upper(keyWord) = "DBDESTRUNTIME"
                     this.dBDestRuntime:=data
                //J.Kwekkeboom 17/12/2004 - load engine filename
//                case upper(keyWord) = "DBASEENGINENAME"
//                     this.dBaseEngineName:=data
                case upper(keyWord) = "DBDESTBDE"
                     this.dBDestBDE:=data                
                case upper(keyWord) = "SOURCEFOLDER"
                     this.SourceFolder:=data
                //D.Brooks 11/12/2004 - load DefaultDirName
                case upper(keyWord) = "DEFAULTDIRNAME"
                		this.DestFolder:=data
                case upper(keyWord) = "INNOSCRIPTNAME"
                     this.InnoScriptname:=data
                case upper(keyWord) = "LASTDBVERSION"
                     this.LastDBVersion:=data
                case upper(keyWord) = "LASTDBLANGUAGE"
                     this.LastDBLanguage:=data
                case upper(keyWord) = "LASTCREATEDATE"
                     this.LastCreateDate:=data

                case upper(keyWord) = "INCDESKTOPENTRY"
                     this.IncDesktopEntry:=iif(upper(data)="TRUE",true,false)
                case upper(keyWord) = "INCQUICKENTRY"
                     this.IncQuickEntry:=iif(upper(data)="TRUE",true,false)
                                                                              
                case upper(keyWord) = "INCUNINSENTRY"
                     //D.Brooks 11/12/2004 - needs to be logical
                     this.IncUninsEntry:=iif(upper(data)="TRUE",true,false)
                     //this.IncUninsEntry:=data
                case upper(keyWord) = "INCDISPGROUP"
                     //D.Brooks 11/12/2004 - needs to be logical
                     this.IncDispGroup:=iif(upper(data)="TRUE",true,false)
                     //this.IncDispGroup:=data

                //D.Brooks 12/08/2004 - Language files no longer required.     
//                case upper(keyWord) = "ITLANGUAGEFILE"
//                     this.ITLanguagefile:=data                     
//                case upper(keyWord) = "ESLANGUAGEFILE"
//                     this.ESLanguagefile:=data                     
//                case upper(keyWord) = "JALANGUAGEFILE"
//                     this.JALanguagefile:=data                     
                case upper(keyWord) = "ENLICENSE"
                     this.ENLicense:=data
                     this.form.LizArr.add("en.Lic."+data)
                case upper(keyWord) = "ENBEFORE"
                     this.ENBefore:=data
                     this.form.LizArr.add("en.Bef."+data)
                case upper(keyWord) = "ENAFTER"
                     this.ENAfter:=data
                     this.form.LizArr.add("en.Aft."+data)
                case upper(keyWord) = "DELICENSE"
                     this.DELicense:=data
                     this.form.LizArr.add("de.Lic."+data)
                case upper(keyWord) = "DEBEFORE"
                     this.DEBefore:=data
                     this.form.LizArr.add("de.Bef."+data)
                case upper(keyWord) = "DEAFTER"
                     this.DEAfter:=data
                     this.form.LizArr.add("de.Aft."+data)
                case upper(keyWord) = "ESLICENSE"
                     this.ESLicense:=data
                     this.form.LizArr.add("es.Lic."+data)
                case upper(keyWord) = "ESBEFORE"
                     this.ESBefore:=data
                     this.form.LizArr.add("es.Bef."+data)
                case upper(keyWord) = "ESAFTER"
                     this.ESAfter:=data
                     this.form.LizArr.add("es.Aft."+data)
                case upper(keyWord) = "ITLICENSE"
                     this.ITLicense:=data
                     this.form.LizArr.add("it.Lic."+data)
                case upper(keyWord) = "ITBEFORE"
                     this.ITBefore:=data
                     this.form.LizArr.add("it.Bef."+data)
                case upper(keyWord) = "ITAFTER"
                     this.ITAfter:=data
                     this.form.LizArr.add("it.Aft."+data)
                case upper(keyWord) = "JALICENSE"
                     this.JALicense:=data
                     this.form.LizArr.add("ja.Lic."+data)
                case upper(keyWord) = "JABEFORE"
                     this.JABefore:=data
                     this.form.LizArr.add("ja.Bef."+data)
                case upper(keyWord) = "JAAFTER"
                     this.JAAfter:=data                     
                     this.form.LizArr.add("ja.Aft."+data)
                case upper(keyWord) = "FRLICENSE"
                     this.FRLicense:=data
                     this.form.LizArr.add("fr.Lic."+data)
                case upper(keyWord) = "FRBEFORE"
                     this.FRBefore:=data
                     this.form.LizArr.add("fr.Bef."+data)
                case upper(keyWord) = "FRAFTER"
                     this.FRAfter:=data                     
                     this.form.LizArr.add("fr.Aft."+data)

                //J.Kwekkeboom 01/20/2007 - added for Dansk and Polish                
                case upper(keyWord) = "DKLICENSE"
                     this.DKLicense:=data
                     this.form.LizArr.add("dk.Lic."+data)
                     bNewLangFound = true
                case upper(keyWord) = "DKBEFORE"
                     this.DKBefore:=data
                     this.form.LizArr.add("dk.Bef."+data)
                case upper(keyWord) = "DKAFTER"
                     this.DKAfter:=data                     
                     this.form.LizArr.add("dk.Aft."+data)
                
                case upper(keyWord) = "PLLICENSE"
                     this.PLLicense:=data
                     this.form.LizArr.add("pl.Lic."+data)
                     bNewLangFound = true
                case upper(keyWord) = "PLBEFORE"
                     this.PLBefore:=data
                     this.form.LizArr.add("pl.Bef."+data)
                case upper(keyWord) = "PLAFTER"
                     this.PLAfter:=data                     
                     this.form.LizArr.add("pl.Aft."+data)
                //End of adding 01/20/2007 

                case upper(keyWord) = "NLLICENSE"
                     this.NLLicense:=data
                     this.form.LizArr.add("nl.Lic."+data)
                case upper(keyWord) = "NLBEFORE"
                     this.NLBefore:=data
                     this.form.LizArr.add("nl.Bef."+data)
                case upper(keyWord) = "NLAFTER"
                     this.NLAfter:=data                     
                     this.form.LizArr.add("nl.Aft."+data)
                
                case upper(keyWord) = "UNINSHS"
                     this.UninsHS:=iif(upper(data)="TRUE",true,false)
                case upper(keyWord) = "UNINSHSHSAPPNAME"
                     this.UninsHSAppName:=data
                case upper(keyWord) = "UNINSHSAPPID"
                     this.UninsHSAppId:=data
                case upper(keyWord) = "UNINSHSLANGUAGES"
                     this.UninsHSLanguages:=data
                //case upper(keyWord) = "UNINSHSEXEDIR"
                //     this.UninsHSExeDir:=data

                //D.Brooks 11/24/2004 - load BDEUseSettings value
                case upper(keyWord) = "BDEUSESETTINGS"
                		this.BDEUseSettings =iif(upper(data)="TRUE",true,false)
                case upper(keyWord) = "BDEMAXFILEHANDLE"
                     this.BDEMaxFileHandle =data
                case upper(keyWord) = "BDELANGDRIVER"
                     this.BDELangDriver =data
                case upper(keyWord) = "BDELEVEL"
                     this.BDELevel =data
                case upper(keyWord) = "BDEMEMOBLOCKSIZE"
                     this.BDEMemoBlockSize =data
                case upper(keyWord) = "BDEMDXBLOCKSIZE"
                     this.BDEMDXBlockSize =data
                case upper(keyWord) = "INNOINI" 
                     this.INNOINI = iif(upper(data)="TRUE",true,false)
                case upper(keyWord) = "INIERRORHANDLING"
                     this.INIErrorHandling = iif(upper(data)="TRUE",true,false)
                case upper(keyWord) = "INITRAP"
                     this.INITrap = iif(upper(data)="TRUE",0,1)
                case upper(keyWord) = "ININOBDE"
                     this.ININoBDE = iif(upper(data)="TRUE",true,false)   
                case upper(keyWord) = "INIAPPTHEMETYPE"    //KK added 02/26/2014 for 9.0.0.0
                     this.INIAppThemeType = data           //KK added 02/26/2014 for 9.0.0.0
                case upper(keyWord) = "INIFULLTHEMENAME"      //KK added 02/26/2014 for 9.0.0.0
                     this.INIFullThemeName = data				  //KK added 02/26/2014 for 9.0.0.0	
                case upper(keyWord) = "INILARGEBUTTONS"         //KK added 02/26/2014 for 9.0.0.0
                     this.INILargeButtons = iif(upper(data)="TRUE",true,false) //KK added 02/26/2014 for 9.0.0.0
                case upper(keyWord) = "UACRB"
                     this.UACRB =data
                     
                case left(upper(keyword),3) = "DIR"
                     this.NeedDirs.add(data)
                case left(upper(keyword),4) = "ICON"
                     this.NeedGroup.add(data)
                case left(upper(keyword),5) ="FILES"
                     bFound:=true
                     this.NeedFiles.add(data)
                     //MsgBox("LOAD Phase 4 - Inno setup this.NeedFiles.add["+data+"]")
                case left(upper(keyword),8) ="BDEALIAS"     
                     this.BDEAlias.Add(data)
             endcase //phase4
        endcase
     enddo
     h1.close()
     if Phase < 2
       if this.verbose = true
          this.form.qLang.iMsgBox("The specified Project File has an invalid format.",'Error in Project File',16)
       endif
     else
       this.FileName = cFN     // set the filename only if succesful
     endif
     if not bFound
        //So we get all files for inno deploy now
        this.Form.SetAllFilesToInno()
     endif

     //J.Kwekkeboom 01/21/2007 - added for new Language support DK PL
     if not bNewLangFound
        this.form.LizArr.add("dk.Lic.")
        this.form.LizArr.add("dk.Bef.")
        this.form.LizArr.add("dk.Aft.")
        this.form.LizArr.add("pl.Lic.")
        this.form.LizArr.add("pl.Bef.")
        this.form.LizArr.add("pl.Aft.")
     endif

      this.Form.FNSplit.FullPath := ltrim(trim(this.FileName))
      this.Form.FNSplit.SplitFileName()
      this.Folder := this.Form.FNSplit.DriveDir

      this.ProjectFileName := this.Form.FNSplit.Name

     set exact &sExact.
     return Phase >= 2

  Function AddContentLine(cT, lCustomFile)
     // add a content line to the contents table
     // each line in the old prj files contains the filename and two parameters
     // parameter 1 : 0 = normal 1 = exclude from build
     // parameter 2 : 0 = normal 1 = include in executable
     // new
     // parameter 3 : 0 = normal 1 = DEO-Deployed Module
     LOCAL x, fn, currentObject, rst, shortName, ext, nCommaPos, nComma, nPeriod, nPeriodPos
     if empty(cT)
       return
     endif

      nPos = at(',',cT)
      if nPos = 0
         fn = cT
      else
         // comma is legit char in filename!
         // find first PE code (',0,' or ',1,)
         nPECodePos1 = at(',0,',cT)
         nPECodePos2 = at(',1,',cT)
         if nPECodePos1 > 0 and nPECodePos2 > 0
            nPos = min(nPECodePos1,nPECodePos2)
         elseif nPECodePos1 = 0 and nPECodePos2 > 0
            nPos = nPECodePos2
         elseif nPECodePos1 > 0 and nPECodePos2 = 0
            nPos = nPECodePos1
         elseif nPECodePos1 = 0 and nPECodePos2 = 0
            //old PRJ file?
         endif
         fn = substr(cT,1,nPos-1)
      endif

     // add this file reference as a new project object to the contents array
     this.contents.add(new ProjectObject())
     CurrentObject = this.contents[alen(this.Contents)]

     // M.Kay - 05/16/2008
     // Save file path & name as read from project file so it can be stored the same way
     //  later on
     CurrentObject.SavedPath = fn

     //Set now Source Alias if inside (Jonny)
     // M.Kay - 5/16/2008
     // This makes very little sense
     // Why convert explicit path to Source Alias here - what purpose does it serve?
     //fn:= this.form.SetSourceAlias(Fn)
 
     // get shortname and type of file
      x = rat("\",fn)
      if x = 0
        shortname = fN
      else
        shortName = substr(fN,x+1)
      endif
      shortName = this.getShortName( fN )
      CurrentObject.Shortname = shortName
      x=rat(".",shortName)
      if x > 0
        // Not using lower() version of ext anywhere, just using upper()
        //ext = lower(substr(shortName,x))
        ext = substr(shortName,x)
      else
        ext = ""
      endif
      CurrentObject.FileType = upper(ext)
      CurrentObject.Custom   = lCustomFile   // unused
      // set marker for main program
      if lower(fn) == lower(this.MainfileName)
        CurrentObject.isMain = true
      endif
 
      CurrentObject.FileName   = fn
      
      // rst = ltrim(substr(cT,x+1))

      // set the standard properties for a file of this this type
      class::setFileProperties(currentObject)

      // Include and exclude must be done after file properties have
      // been determinded - since they vary by file type - compilable or not

 
      // Get right side, the 1's and 0's
      fn = substr(cT,at(',',cT)+1)
      if len(trim(fn)) = 0
        fn = cT
      else 

         // filename,0,1,isDEO,DEOlibNo    
         // If object is compilable
         if currentObject.canBeCompiled
            // included in exe is first param =1
            currentObject.included = substr(fn,1,1) = '0'
         else
            // included in exe is second param = 0
            currentObject.included = substr(fn,3,1) # '0'
         endif

         // Check to see if this is a DEO object 
         currentObject.deoObject = substr(fn,5,1) = '1' 

         // Get DEO library number
         if currentObject.deoObject and ;
            len(trim(substr(fn,7,1))) > 0 
            currentObject.deoLibNo = substr(fn,7,1)
         endif

      endif
   

      return true

   Function setFileProperties(oFile)
      // each file object in a project has properties such as canBeCompiled ...
      // these properties are stored directly in the contents array to save
      // constantly doing lookups in the filesType array when navigating between
      // the files later
      // oFile is the current entry from the contents array
      // At present a Project is divieded into 8 File file Types
      //    Forms, Reports, Progs, Desktop Tables, SQL, Datamodules, Graphics, Others
      // files of unknown type will be assigned to Group 8 (others)

      LOCAL typ
      typ = oFile.FileType
      if this.fileTypes.iskey(typ)
         oFile.FileGroup     = this.fileTypes[typ].FileGroup
         if oFile.FileGroup = 8
            oFile.icon = "Resource ocx "+this.form.ExplorerDllFile
         else
            oFile.Icon          = this.fileTypes[typ].Icon
         endif
         oFile.canBeMain     = this.fileTypes[typ].canBeMain
         oFile.canBeExcluded = this.fileTypes[typ].canBeExcluded
         oFile.canBeIncluded = this.fileTypes[typ].canBeIncluded
         oFile.runWith       = this.fileTypes[typ].runWith
         oFile.canBeEdited   = this.fileTypes[typ].canBeEdited
         oFile.canBeCompiled = this.fileTypes[typ].canBeCompiled
         oFile.designer      = this.fileTypes[typ].designer
      else
         // this type of file is unknown so....
         oFile.FileGroup = 9     
         oFile.Icon      = "resource #240"
         oFile.canBeCompiled = false
      endif
      return true


   // M.Kay - 05/16/2008
   // Given a reference to a projectObject for a file within the project
   // determine the file path to write to the project file.
   //
   // If possible save using same path as was previously saved in the project file
   // If file is new to project or file path has been changed, determine new path
   // to save for file.
   //
   // lNeedFiles is used to determine wether or not resolveSavePath is resolving
   // the path of the files included in the inno script (as deo files)
   //
   //    New path should be based on preference set for project
   //       For files added to project
   //          Use Relative Paths
   //          Use Source Aliases
   //          Use Full (Explicit) Paths

   Function resolveSavePath( oProjObject, lApplyFormat, lNeedFiles )
      local cContentPath, cSavedPath

      cContentPath = ""
      cSavedPath = ""
      //lNeedFiles is 
      if pCount()<3
         lNeedFiles = false
      endif

      // KK 04/13/2010
      //evaluating the list of NeedFiles that go with INNO script
      if lNeedFiles
         cSavedPath = oProjObject
      //evaluating a oProjObject file
      else  
         //test to see if a previously SavedPath Exists 
         if NOT EMPTY(oProjObject.SavedPath)
            cSavedPath = oProjObject.SavedPath
         endif 
      endif
      
     
      if NOT lNeedFiles //evaluating oProjObject file
         if not empty( cSavedPath )
            if NOT lApplyFormat
               // See if FileName points to same file as SavedPath
               if this.getFullPath( oProjObject.FileName ) == this.getFullPath( cSavedPath )
                  // File was previously loaded from project file, so Save using previous SavedPath
                  cContentPath = cSavedPath
               endif
            else
               cContentPath = oProjObject.FileName
            endif
         else //File is new - No savedPath
            cContentPath = oProjObject.FileName
         endif
      else //evaluting an Inno file 
         cContentPath = cSavedPath
      endif


      if lApplyFormat
         // Construct appropriate path to save file
         do case
         case this.PrefPathFormat = "R"
            // Use Relative Path
            cContentPath = this.getRelativePath( this.Folder, this.getFullPath( cContentPath ))
         case this.PrefPathFormat = "S"
            // Use Source Alias (if possible)
            cContentPath = this.form.SetSourceAlias( this.getFullPath(cContentPath) )
         case this.PrefPathFormat = "F"
            // Use Full (explicit) path
            cContentPath = this.getFullPath( cContentPath )
         endcase
      endif

      return cContentPath



   Function save(lSaveAs)
      // lParam true means force a save as
      LOCAL cFileName, cTF, h1, x, i, backupFilename, cContentFilePath, nChangePathFormat, lApplyPathFormat
      cFileName = this.FileName
      if empty(cFileName)  or lSaveAs
         //J.Kwekkeboom 12/25/2004 - add for let not ScriptMaker destination folder
         //including here so we get always the main file folder...
         if empty(this.SourceFolder)
            this.SourceFolder:=Substr(this.MainFileName,1,rat("\",this.MainFileName)-1)
         endif
         cFileName:= putFile("New Project",this.SourceFolder+"\*","prj")
         //Disabled for change with this lines above!
         //cFileName = putFile("New Project",this.SourceFolder+"\*","prj")
         
         if empty(cFileName)
            return false
         endif
      endif

      // M.Kay - 05/16/2008
      lApplyPathFormat = false
      if not empty(this.OldPrefPathFormat)
         if this.OldPrefPathFormat <> this.PrefPathFormat
            // Ask user if they want paths of previously saved
            // files modified to match new setting
            cSrcPathFormatName = ""

            do case
            case this.PrefPathFormat = "R"
               cSrcPathFormatName = "Use Relative Paths"

            case this.PrefPathFormat = "S"
               cSrcPathFormatName = "Use Source Aliases"

            case this.PrefPathFormat = "F"
               cSrcPathFormatName = "Use Full Paths"
            endcase

            nChangePathFormat = msgbox("Do you want the Source Path Format ("+cSrcPathFormatName+")"+chr(13) ;
                                       +"applied to all files previously saved to the project file?",;
                                       "Source Path Format Changed",4)
            if nChangePathFormat = 6
               // Yes - Change paths to match current format
               lApplyPathFormat = true
            endif
         endif
      endif

      // we write all the info to a temp file always
      cTF = funique("prj?????.tmp")
      h1 = new file()
      //KK 09/15/2012 - added try catch because if in a dir that is protected by UAC 
      //funique returns an empty string and all of the backup files will not be able to be created
      try
         h1.create(cTF)
      catch (exception e)
         MsgBox("Unable to create new files in this directory due to security restraints."+chr(13)+"Please re-start dBASE Plus with Administrator Rights and try again.","Alert",16)
         return false
      endtry

      h1.puts("Project File generated "+dtoc(date())+" "+time())
      h1.puts("")
      h1.puts("[Project]")
      h1.puts("Name="+this.projectName)
      h1.puts("DefaultDir=.")              // incomplete
      x = trim(this.mainFileName)
      if left(x,1) = "<"    // indicates <none>
        x = ""
      endif
      h1.puts("MainProjectFileName=" + x)
      h1.puts("Parameters=" + class::noCR(this.ParameterList,this.maxDescription)) // no longer supported
      h1.puts("Author=" + this.Author)
      h1.puts("Description=" + class::noCR(this.description,this.maxDescription))
      h1.puts("Preprocessor=" + this.preProcessorDirectives)
      h1.puts("LogFileName=" + this.logFileName)
      if not empty(this.exeName) and at('.',this.exeName) = 0
         this.exeName = trim(this.exeName)+'.exe'
      endif

      // M.Kay - 05/16/2008
      h1.puts("PrefPathFormat=" + this.PrefPathFormat )

      h1.puts("TargetFileName=" + this.exeName)
      x = trim(this.iconFileName)
      if left(x,1) = "<"        // indicates <none>
        x = ""
      endif
      h1.puts("IconFileName=" + x)
      x = trim(this.splashFileName)
      if left(x,1) = "<"        // indicates <none> 
        x = ""
      endif
      h1.puts("SplashFileName=" + x)
      h1.puts("MostRecentSelectedFileName=")  // unused at present
      h1.puts("WebApplication="+iif(this.webApplication = false,"false","True"))
      h1.puts("DeoOption="+iif(this.deoOption = false,"False","True"))
      //KK 12/01/2010 - added support for build option UAC 
      h1.puts("enableUAC="+this.enableuac)
      //KK - added to accomodate the new INI options
      h1.puts("inioption="+this.inioption)
		h1.puts("runtimeinbuild="+this.runtimeinbuild)
		h1.puts("bdeoption="+this.bdeoption)
      // the following emtpy line must be length 0 or the original project explorer will ignor
      // all of the contents lines following
      h1.puts("")
      h1.puts("[Contents]")

      // the contents table can contain empty entries if files were removed

      // First find mainFileName
      nMainFileIndex = 0
      // If there is a mainFileName

      if not empty(this.mainFileName)
         // Traverse the contents array to find a match
         for i =1 to alen(this.contents)

            // If you got it, write this one first
            if this.contents[i].isMain
               // M.Kay - 5/16/2008
               // Determine file path to save
               cContentFilePath = this.resolveSavePath( this.contents[i], lApplyPathFormat )

               //h1.puts(this.contents[i].FileName + ',0,0'+;
                 //"," + iif(this.contents[i].deoObject=true,"1","0") + ;
                 //"," + this.contents[i].deoLibNo )

               h1.puts( cContentFilePath + ',0,0'+;
                "," + iif(this.contents[i].deoObject=true,"1","0") + ;
                "," + this.contents[i].deoLibNo )

                nMainFileIndex = i
                exit

            endif
         next
      endif

      // Now write the rest of the contents
      private cParmString

      for i = 1 to alen(this.Contents)

        // If filename exists and index is not mainFileName, write it
        if (not empty(this.contents[i].FileName)) and ;
           (not this.contents[i].isMain)


          // If this is a compilable file,
          if this.contents[i].canBeCompiled
             // if not DEO, do not exclude Obj, do not include source
             if this.contents[i].included
                //cParmString = '0,0,'
                cParmString = '0,0'
             else
                // If it is DEO, exclude both Obj and source
                cParmString = '1,0'
             endif

          else // A non-compilable file

             // Do not exclude, do include

             cParmString = '0,'+iif(this.contents[i].included,'1','0')
    
          endif

          // M.Kay - 5/16/2008
          // Determine file path to save
          cContentFilePath = this.resolveSavePath( this.contents[i], lApplyPathFormat )

          //h1.puts(this.contents[i].FileName + ','+cParmString+;
           //"," + iif(this.contents[i].deoObject=true,"1","0") + ;
           //"," + this.contents[i].deoLibNo )

          h1.puts( cContentFilePath + ','+cParmString+;
          "," + iif(this.contents[i].deoObject=true,"1","0") + ;
          "," + this.contents[i].deoLibNo )

        endif
      next

      h1.puts("")

      // the deo enhancement writes a DEO section followed by each deo folder
      h1.puts("[DEO]")
      for i = 1 to 10
         if not empty(substr(this.deolist[i],3))
            h1.puts("DEO" + this.deolist[i])
         endif
      next

      h1.puts("")

      //The Inno entries
      if empty(this.SourceFolder)
         this.SourceFolder:=Substr(this.MainFileName,1,rat("\",this.MainFileName)-1)
      endif
      h1.puts("[INNO]")
      //needed for import inside SM application this entries too...
      // SM MenuItem-> Project->Import form dBASE Project file
      // !! only write not read settings !!
      h1.puts("ProjectExeFileName="+this.getFullPath(this.exename))
      h1.puts("ProjectMainFileName="+this.InnoScriptname)
      h1.puts("ProjectdBaseVersion="+version(0))
      h1.puts("DefaultDirName="+this.DestFolder)
      ** End of no read entries

      //defaults
      h1.puts("AppName="+this.AppName)
      h1.puts("AppVersion="+this.AppVersion)
      //For Runtime settings
      h1.puts("SM_dBASERuntime="+this.SM_dBASERuntime)
      h1.puts("dBPlus="+this.dBPlus)
      h1.puts("dBSeparat="+this.dBSeparat)
      h1.puts("Resource="+this.Resource)
      h1.puts("dBASELang="+this.dBASELang)
      
      //h1.puts("dBaseRuntimeDir="+this.dBaseRuntimeDir)
      //KK (08/20/2012) - using dBaseRuntimeInstDir instead of dBaseRuntimeDir - to distinguish it from the actual Plus\Runtime directory
      h1.puts("dBaseRuntimeInstDir="+this.dBaseRuntimeInstDir)
      h1.puts("dBaseRuntimeDir="+this.dBaseRuntimeDir)
      h1.puts("INNO_RBoption="+this.INNO_RBoption)
		h1.puts("bdeonlyinstall="+this.bdeonlyinstall)
		h1.puts("bdeinstallfile="+this.bdeinstallfile)
      h1.puts("XPLook="+this.XPLook)

      //J.Kwekkeboom 12/17/2004 - save new properties
      //h1.puts("dBaseEngineName="+this.dBaseEngineName)

      h1.puts("dBaseBuild="+this.dBaseBuild)
      //J.Kwekkeboom 03/09/2007 check for ONLY 4 chars !
      if len(this.dBaseVersion)#4 
         if len(this.dBaseVersion)>4 
            this.dBaseVersion:= Left(this.dBaseVersion,4)
         else
            do while len(this.dBaseVersion)<4
               this.dBaseVersion+="0"
            enddo    
         endif
      endif
      h1.puts("dBaseVersion="+this.dBaseVersion)
      //J.Kwekkeboom 01/20/2007 - Added for new Setup languages support
      h1.puts('SetupLang='+this.SetupLang)
      //J.Kwekkeboom removed in isg //01/15/2007
      //h1.puts("dBDestName="+this.dBDestName)
      h1.puts("SepDestDir="+this.SepDestDir)
      h1.puts("BDEGroupEntries="+this.BDEGroupEntries)
      //J.Kwekkeboom removed in isg //01/15/2007
      //h1.puts("BDEVersion="+this.BDEVersion)
      h1.puts("dBProgressbar="+this.dBProgressbar)
      h1.puts("dBProgressbox="+this.dBProgressbox)
      h1.puts("dBSilent="+this.dBSilent)

      h1.puts("dBDestRuntime="+this.dBDestRuntime)
      h1.puts("dBDestBDE="+this.dBDestBDE)
      h1.puts("SourceFolder="+this.SourceFolder)
      h1.puts("InnoScriptname="+this.InnoScriptname)
      this.LastDBVersion:=this.dBaseVersion
      h1.puts("LastDBVersion="+this.LastDBVersion)
      this.LastDBLanguage:=this.dBASELang
      h1.puts("LastDBLanguage="+this.LastDBLanguage)
      h1.puts("LastCreateDate="+this.LastCreateDate)
      h1.puts("IncDispGroup="+this.IncDispGroup)
      h1.puts("IncUninsEntry="+this.IncUninsEntry)
      h1.puts("IncDesktopEntry="+this.IncDesktopEntry)
      h1.puts("IncQuickEntry="+this.IncQuickEntry)
      

      //D.Brooks 12/08/2004 - Language files no longer required.
//      h1.puts("ITLanguagefile="+this.ITLanguagefile)
//      h1.puts("ESLanguagefile="+this.ESLanguagefile)
//      h1.puts("JALanguagefile="+this.JALanguagefile)
      h1.puts("ENLicense="+this.ENLicense)
      h1.puts("ENBefore="+this.ENBefore)
      h1.puts("ENAfter="+this.ENAfter)
      h1.puts("DELicense="+this.DELicense)
      h1.puts("DEBefore="+this.DEBefore)
      h1.puts("DEAfter="+this.DEAfter)
      h1.puts("ESLicense="+this.ESLicense)
      h1.puts("ESBefore="+this.ESBefore)
      h1.puts("ESAfter="+this.ESAfter)
      h1.puts("ITLicense="+this.ITLicense)
      h1.puts("ITBefore="+this.ITBefore)
      h1.puts("ITAfter="+this.ITAfter)
      h1.puts("JALicense="+this.JALicense)
      h1.puts("JABefore="+this.JABefore)
      h1.puts("JAAfter="+this.JAAfter)
      h1.puts("FRLicense="+this.FRLicense)
      h1.puts("FRBefore="+this.FRBefore)
      h1.puts("FRAfter="+this.FRAfter)
      h1.puts("DKLicense="+this.DKLicense)
      h1.puts("DKBefore="+this.DKBefore)
      h1.puts("DKAfter="+this.DKAfter)
      h1.puts("PLLicense="+this.PLLicense)
      h1.puts("PLBefore="+this.PLBefore)
      h1.puts("PLAfter="+this.PLAfter)
      h1.puts("NLLicense="+this.NLLicense)
      h1.puts("NLBefore="+this.NLBefore)
      h1.puts("NLAfter="+this.NLAfter)

      h1.puts("UninsHS="+this.UninsHS)
      h1.puts("UninsHSAppName="+this.UninsHSAppName)
      h1.puts("UninsHSAppId="+this.UninsHSAppId)
      h1.puts("UninsHSLanguages="+this.UninsHSLanguages)
      //h1.puts("UninsHSExeDir="+this.UninsHSExeDir)

      //D.Brooks 11/24/2004 - save BDEUseSettings value
      h1.puts("BDEUseSettings="+this.BDEUseSettings)
      h1.puts("BDEMaxFileHandle="+this.BDEMaxFileHandle)
      h1.puts("BDELangDriver="+this.BDELangDriver)
      h1.puts("BDELevel="+this.BDELevel)
      h1.puts("BDEMemoBlockSize="+this.BDEMemoBlockSize)
      h1.puts("BDEMDXBlockSize="+this.BDEMDXBlockSize)
      
      //h1.puts("INISTANDARD="+this.INIStandard)
      h1.puts("INNOINI="+this.INNOINI)
      h1.puts("INIERRORHANDLING="+this.INIErrorHandling)
      h1.puts("INITRAP="+iif(this.INITrap=0,true,false))
      h1.puts("ININOBDE="+this.ININoBDE)
      //MsgBox("about to save them and button settings in proj file")
      h1.puts("INIAPPTHEMETYPE="+this.INIAppThemeType) //KK added 02/26/2014 for 9.0.0.0
      h1.puts("INIFULLTHEMENAME="+this.INIFullThemeName) //KK added 02/26/2014 for 9.0.0.0
      h1.puts("INILARGEBUTTONS="+this.INILargeButtons) //KK added 02/26/2014 for 9.0.0.0
      
      h1.puts("UACRB="+this.UACRB)

      for i=1 to this.BDEAlias.size
          h1.puts("BDEAlias"+i+"="+this.BDEAlias[i])
      next
      for i=1 to this.NeedGroup.size
         h1.puts("Icon"+i+"="+this.NeedGroup[i])
      next
      for i=1 to this.NeedDirs.size
         h1.puts("Dir"+i+"="+this.NeedDirs[i])
      next      
      for i=1 to this.Needfiles.size
         // KK 04/13/2010 - added resolveSavePath() to get the right Path type
         NeedFile_FileName = LEFT(this.NeedFiles[i],AT(" >>",this.NeedFiles[i])-1)
         NeedFile_Destination = RIGHT( this.NeedFiles[i],LEN(this.NeedFiles[i])-LEN(NeedFile_FileName) )
         NeedFile_FileName = this.resolveSavePath( NeedFile_FileName, lApplyPathFormat, true ) 
         h1.puts("Files"+i+"="+NeedFile_FileName+NeedFile_Destination )
      next
      h1.close()

      // now make a backup of the old project file and rename the temp file
      if file(cFileName)
         x=rat("\",cFileName)
         if x>0
           backupFileName = left(cFileName,x) + this.form.qlang.iGetText("Backup of") +" " + substr(cFileName,x+1)
         else
           backupFileName = this.form.qlang.iGetText("Backup of")+" " + cFileName
         endif
         if file(backupFileName)
           //D.Brooks 12/17/204 - QAID 5112 - will not work if file is read-only.
           try
           		erase (backupFileName)
           catch(exception e)
     //QAID: 5316 - D.Brooks - 03/30/2005
         		this.form.qlang.iMsgbox(e.message,"Cannot Save File")
           	   //msgbox(this.form.qlang.iGetText("Cannot save file"))
               return false
           endtry
         endif
         copy file (cFileName) to (backupFileName)
         //rename (cFileName) to (backupFileName)
      endif
      //D.Brooks 11/24/2004 - make sure file exists
      try
         erase (cFileName)
      	rename (cTF) to (cFileName)
      catch (exception e)         
         this.form.qlang.iMsgbox(e.message,"Cannot Save File")
      	//form.qlang.iMsgbox("Cannot save file")
      //End QAID: 5316
         return false
      endtry
      this.Modified = false
      this.fileName = cFileName
      // Update MRU list on save
      this.form.root.saveMRUItem(cFileName)

      this.Form.FNSplit.FullPath := ltrim(trim(this.FileName))
      this.Form.FNSplit.SplitFileName()
      this.Folder := this.Form.FNSplit.DriveDir
      this.ProjectFileName := this.Form.FNSplit.Name
      return true

   function noCR(cT, nMax)
     // method removes carriage returns from a field and limits it's length
     // used for project description
     LOCAL ret, i, c
     ret = ""
     for i = 1 to len(cT)
       c = substr(cT,i,1)
       do case
       case c = chr(13)
          ret += " "
       case c = chr(10)
       otherwise
           ret += c
       endcase
     next
     if len(ret) > nMax
       ret = left(ret,nMax)
     endif
     return ret

   function releaseLog()
      // this is a workaraound for releasing the datalink from an editor
      // it is quick and dirty
      // M.Kay - TEST - 03/06/2008
      //  Try checking for empty before setting it to null
      // QAID: 6780 - M.Kay - dBase, LLC - 05/11/2012
      // Add Try...Catch ...Endtry to catch error
      // if dBASE is closed while Project Explorer is open
      try
         if not empty(this.form.notebook1.logEditor.datalink)
            this.form.notebook1.logEditor.datalink := null
         endif
      catch (exception e)
      endtry

      return

   function datalinkLog
      try
         // M.Kay TEST - 3/6/08
         //this.form.notebook1.logEditor.datalink:= 'file '+this.getFullPath(trim(this.logFileName))
         this.form.notebook1.logEditor.datalink:= 'file '+this.getFullPath(trim(this.logFileName))
      catch (exception e)
      endtry



   function compileProg
      local bSuccess, cSourceFileName, oCurrentFileObj, ObjFile, cErrMsg
      bSuccess = false
      oCurrentFileObj = this.Form.currentFileObject
      cSourceFileName = this.getFullPath(oCurrentFileObj.FileName)
      this.releaselog()
      oLog = new ProjectLog( (this) )
      oLog.StartLog()

      oLog.SetNotifiee( this.form.notebook1.logEditor )  // MK Test - 03/06/2008


      oLog.PutProcessHeader(this.form.qLang.iGetText("Compile"))
      oLog.CompileCount += 1
      try
      	//D.Brooks 12/17/2004 - QAID 5111 - Change directory before compile
         Local StartPath,ActionPath
         StartPath=Set("directory")
         //D.Brooks 12/29/2004 - QAID 5120
         cName = this.compiledExt(this.getFullPath(this.mainFileName))
         //cName = this.getFullPath(this.compiledExt(this.mainFileName))
         ActionPath = left(cName,rat("\",cName)-1)
         if not empty(ActionPath)
         	set path to (ActionPath)
         	set directory to (ActionPath)
         endif
         //compile ( cSourceFileName ) log ( oLog.CompileLog )
			//KK changed below to include [Project] / Preprocessor= setting
			//?"this.preprocessordirectives",this.preprocessordirectives
			if empty(this.PreprocessorDirectives)
				compile ( cSourceFileName ) log ( oLog.CompileLog )
			else
				cCompile = "compile /D"+this.PreprocessorDirectives+" ["+cSourceFileName+"] log ["+oLog.CompileLog+"]"
				&cCompile.
			endif   
         set path to (StartPath)
         set directory to (StartPath)
         bSuccess = oLog.PutCompileStats()
// M.Kay - 12/13/2010
// Moved deoCopy() code after endtry as it
//  contains its own try...catch to handl copy errors
//  and they were being reported incorrectly when deoCopy()
//  was called from within this try...catch
//         if bSuccess
//            if oCurrentFileObj.deoObject and not ;
//               empty(oCurrentFileObj.deoLibNo)
//               ObjFile =  left( cSourceFileName, len(trim(cSourceFileName))-1 ) + "o"
//               this.deoCopy(ObjFile,oCurrentFileObj,oLog)
//            endif
//         endif
      catch (exception e)
         cErrMsg = "Compile results for:"+chr(13)+chr(10)+cSourceFileName+chr(13)+chr(10)+;
                   "COMPILE COMMAND FAILED: "+e.message
         oLog.PutCompileStats(true,cErrMsg)
      endtry

      if bSuccess
         if oCurrentFileObj.deoObject and not empty(oCurrentFileObj.deoLibNo)
            ObjFile =  left( cSourceFileName, len(trim(cSourceFileName))-1 ) + "o"
            this.deoCopy(ObjFile,oCurrentFileObj,oLog)
         endif
      endif

      oLog.PutCompileFooter()
      oLog.StopLog()

      // M.Kay - TEST - 03/06/2008
      this.form.notebook1.logEditor.value := ""
      this.form.notebook1.logEditor.scrollBar := 2  // Turn on scrollbars in case they were turned off

      this.datalinklog()

      return bSuccess

   function areAnyToCompile
      local i, bAreAnyToCompile
      bAreAnyToCompile = false
      for i = 1 to this.Contents.size
         if not empty(this.Contents[i].FileName) and this.Contents[i].canBeCompiled
            bAreAnyToCompile = true
            exit
         endif
      next
      return bAreAnyToCompile

   function compileAll
      local i, oLog, SourceFile, bCompleteSuccess, bCompileSuccess, cErrMsg

      bCompleteSuccess = true

      this.releaseLog()
      oLog = new ProjectLog((this))
      oLog.StartLog()

      oLog.SetNotifiee( this.form.notebook1.logEditor )  // MK Test - 03/06/2008

      oLog.PutProcessHeader(this.form.qLang.iGetText("Compile All"))

      //D.Brooks 12/17/2004 - QAID 5111 - Change directory before compile
      Local StartPath,ActionPath
      StartPath=Set("directory")
      //D.Brooks 12/29/2004 - QAID 5120
      cName = this.compiledExt(this.getFullPath(this.mainFileName))
      //cName = this.getFullPath(this.compiledExt(this.mainFileName))
      ActionPath = left(cName,rat("\",cName)-1)
      if not empty(ActionPath)
      	set path to (ActionPath)
      	set directory to (ActionPath)
      endif

      for i = 1 to alen(this.Contents)

         SourceFile = this.getFullPath(this.Contents[i].FileName)

         if this.Contents[i].canBeCompiled

            if not file( SourceFile )
               oLog.PutNotFound( SourceFile )
               oLog.MissingFiles += 1
               bCompleteSuccess = false

            else

               oLog.CompileCount += 1
               try
                  //compile ( SourceFile ) log ( oLog.CompileLog )
						//KK changed below to include [Project] / Preprocessor= setting
						//?"this.preprocessordirectives",this.preprocessordirectives
						if empty(this.PreprocessorDirectives)
							compile ( SourceFile ) log ( oLog.CompileLog )
						else
							cCompile = "compile /D"+this.PreprocessorDirectives+" ["+SourceFile+"] log ["+oLog.CompileLog+"]"
							&cCompile.
						endif   
                  bCompileSuccess = oLog.PutCompileStats()
                  if not bCompileSuccess
                     bCompleteSuccess = false
                  endif

                  // all the compilable objects will create a file ending with "o"
                  // if this is a deo-object then it can be copied to the deo-target folder
                  if this.contents[i].deoObject and not ;
                     empty(this.contents[i].deoLibNo)
                     ObjFile =  left( SourceFile, len(trim(SourceFile))-1 ) + "o"
                     this.deoCopy(ObjFile,this.contents[i],oLog)
                  endif
               catch (exception e)
                  bCompleteSuccess = false
                  //QAID: 5320 - D.Brooks - 04/08/2005 
                  cErrMsg = "Compile results for:"+chr(13)+chr(10)+SourceFile+chr(13)+chr(10)+;
                            "COMPILE COMMAND FAILED: "+e.message
                  //cErrMsg = "Compile results for:"+chr(13)+chr(10)+cSourceFileName+chr(13)+chr(10)+;
                            "COMPILE COMMAND FAILED: "+e.message
                  oLog.PutCompileStats(true,cErrMsg)
               endtry

            endif

         endif

      next

      set path to (StartPath)
      set directory to (StartPath)

      oLog.putCompletedFooter()
      oLog.putCompileFooter()
      oLog.StopLog()

      // M.Kay - TEST - 03/06/2008
      //oLog.SetNotifiee( NULL )  // MK Test - 03/06/2008
      this.form.notebook1.logEditor.value := ""
      this.form.notebook1.logEditor.scrollBar := 2  // Turn on scrollbars in case they were turned off

      this.datalinkLog()

      return bCompleteSuccess

   //02/22/2005 J.Kwekkeboom
   //Added for check of need to compile....
   function NeedCompile( origFilename, compFileName )
   // this method checked of needed to be compile the current file
   Local bRet,dDate1,dDate2,tTime1,tTime2
     if not file( (compFileName) )
        //msgbox('Filename: ' + compFileName +' not found! (compile it)')
        return(true)
     endif
     if not file( (origFilename) )
        //msgbox('Filename: ' + origFilename +' not found! (can not compile it)')
        return(false)
     endif
     tTime1 = new File( ).time( (origFilename) )
     tTime2 = new File( ).time( (compFileName) )
     dDate1 = new File( ).date( (origFilename) )
     dDate2 = new File( ).date( (compFileName) )

     bRet = ( dDate1 > dDate2 )
     if not bRet
        if dDate1 = dDate2
           bRet:= ( tTime1>tTime2 )
        endif
     endif
     //msgbox(origFilename+chr(13)+'> '+dDate1+' '+tTime1+chr(13)+;
              compFileName+chr(13)+'> '+dDate2+' '+tTime2+chr(13)+;
             'Need to compile: '+bRet)
   return(bRet)

   function build( oLog, cUACopt, cRUNTIME, nBDEOption )
	//?"begin build"
      // this method builds the target executable
      // returns true if successful
      local fOut, bSuccess, bCalledFromRebuild, cName, bDEOLibFound, cRspFile, i, cBDEOption
		cBDEOPTION = ""
      cUACopt = ""+cUACopt

      //KK - all calls to build if using cUACopt must have more than one parameter
      bCalledFromRebuild = (argCount()=1)  

 		
      //MsgBox("cUACopt='"+cUACopt)
      // QAID: 6409 - M.Kay, dBI Inc. - 04/08/2008
      if "PERSONAL"$upper(version())
         // If Personal dBASE, return without doing anything
         return false
      endif
		
		     //set default UAC option
      if type("cUACopt") # "C"
         cUACopt = "NONE"
      else if cUACopt == "ON"
         cUACopt = "ON"
		else if cUACopt == "OFF"
         cUACopt = "OFF"
      else
			cUACopt = "NONE"
      endif
		
		if type("cRUNTIME") # "C"
		   cRUNTIME = ""
		else if cRUNTIME <> "RTEXE"
         cRUNTIME = ""
		endif
		
		if type("nBDEOption") # "N"
		   cBDEOption = ""
		else if nBDEOption = 0 
		   //?"nBDEOption:"+nBDEOption
         cBDEOption = "BDE OFF"
		else
         cBDEOption = ""		
		endif		
		//?"bde option is ",cBDEOption
      // Issue #1582 - M.Kay, dBase, LLC - 03/30/2015
		// Ensure UAC and RTEXE are handled when user chooses Rebuild All
      if bCalledFromRebuild
			if this.enableuac = 0
				cUACopt = "NONE"
			else if this.enableuac = 1
				cUACopt = "ON"
			else if this.enableuac = 2
				cUACopt = "OFF"
			endif
			
			if this.runtimeinbuild = 1
				cRUNTIME = "RTEXE"
			else
				cRUNTIME = ""
			endif		
			
			//kk added BDE build option is called from rebuild
			//?"this.bdeoption is "+this.bdeoption
			?"this.bdeoption is "+this.bdeoption
			if this.bdeoption = 0
			   cBDEOption = "BDE OFF"
			endif
			
      endif		

//msgbox("In build() - bCalledFromRebuild =>"+iif(bCalledFromRebuild,"True","False")+"<="+chr(13)+"oLog=>"+iif(empty(oLog),"NULL","Object")+"<=")
      if empty(this.fileName)
        if this.verbose = true
          this.form.qLang.iMsgBox("No project file specified",'Missing Information',16)
        endif
        return false
      endif

      if empty(this.exename)
        if this.verbose = true
          this.form.qLang.iMsgBox("No target executable specified",'Missing Information',16)
        endif
        return false
      endif

      if not bCalledFromRebuild
         this.releaseLog()
         oLog = new ProjectLog((this))
         oLog.StartLog()

         oLog.SetNotifiee( this.form.notebook1.logEditor )  // MK Test - 03/06/2008

         oLog.PutProcessHeader(this.form.qLang.iGetText("Build"))
      endif
		
      this.SaveRspFile = true
      if this.SaveRspFile
         cRspFile = this.Folder + "\" + this.ProjectFileName + ".rsp"
      else
         cRspFile = funique(getTempFolder()+"\PE??????.rsp")
      endif

      bSuccess = true

      try
         // create new response file 
         fOut = new file()
         fOut.create(cRspFile)

        // Put out included file list
         fOut.puts('')
         fOut.puts('FILES')
         // Put out the main file if one is specified
         if empty(this.mainFileName)
            this.form.qLang.iMsgBox('No main program file specified!','Cannot Build',16)
            return false
         endif

         Local StartPath,ActionPath
         StartPath= Set("directory")
         //D.Brooks 12/29/2004 - QAID 5120
         cName = this.compiledExt(this.getFullPath(this.mainFileName))
         //cName = this.getFullPath(this.compiledExt(this.mainFileName))
         ActionPath = left(cName,rat("\",cName)-1)        
         if not bCalledFromRebuild and not file(cName)
            set path to (ActionPath)
            set directory to (ActionPath)
               //compile ( this.getFullPath(this.mainFileName) ) log ( oLog.CompileLog )
					//KK changed below to include [Project] / Preprocessor= setting
					//?"this.preprocessordirectives",this.preprocessordirectives
					if empty(this.PreprocessorDirectives)
						compile ( this.getFullPath(this.mainFileName) ) log ( oLog.CompileLog )
					else
						cCompile = "compile /D"+this.PreprocessorDirectives+" ["+this.getFullPath(this.mainFileName)+"] log ["+oLog.CompileLog+"]"
						&cCompile.
					endif   
               oLog.CompileCount += 1
               oLog.PutCompileStats()
            set path to (StartPath)
            set directory to (StartPath)
         endif
         fOut.puts('   '+cName)

         // Put out all files
         for i = 1 to alen(this.contents)
            // but only if they're to be included and
            // NOT the main startup file, which has
            // already been output
            if this.contents[i].included and not this.contents[i].isMain

               //D.Brooks 12/29/2004 - QAID 5120
               cName = this.compiledExt(this.getFullPath(this.contents[i].FileName))
               //cName = this.getFullPath(this.compiledExt(this.contents[i].FileName))
               ActionPath = left(cName,rat("\",cName)-1)
               // Compile missing object files
               if not bCalledFromRebuild and this.isObjectFile(cName) and not file(cName)
                  set path to (ActionPath)
                  set directory to (ActionPath)
                    //02/22/2005 J.Kwekkeboom
                    //Fixed for correct compile all files also by create EXE
                    //if eg. MNU date and time > MNO 

                    if this.NeedCompile(this.contents[i].FileName,cName)
                       //compile ( this.getFullPath(this.contents[i].FileName) ) log ( oLog.CompileLog )
							  //KK added preprocessor check in [Project] / processor= setting
							  //?"this.preprocessordirectives",this.preprocessordirectives
							   if empty(this.PreprocessorDirectives)
									compile ( this.getFullPath(this.contents[i].FileName)  ) log ( oLog.CompileLog )
								else
									cCompile = "compile /D"+this.PreprocessorDirectives+" ["+this.getFullPath(this.contents[i].FileName) +"] log ["+oLog.CompileLog+"]"
									&cCompile.
								endif         
                       oLog.CompileCount += 1
                       oLog.PutCompileStats()
                    endif
                    //compile ( this.getFullPath(this.contents[i].FileName) ) log ( oLog.CompileLog )
                    //oLog.CompileCount += 1
                    //oLog.PutCompileStats()

                  ** Info: Must be here! 
                  ** For correct build include in Exe files
                  set path to (StartPath)
                  set directory to (StartPath)                  
               endif
               fOut.puts('   '+cName)
            endif
         next
         set path to (StartPath)
         set directory to (StartPath)

         // Put out the destination path
         fOut.puts( 'TO '+this.getFullPath(this.exename) )

         // If ICON exists, put it out too
         if not empty(this.iconFileName)
            fOut.puts('ICON '+this.getFullPath(this.iconFileName))
         endif

         // If splash exists, put it out too
         if not empty(this.splashFileName)
            fOut.puts('SPLASH '+this.getFullPath(this.splashFileName))
         endif

         // Put out WebApplication keyword status
         fOut.puts('WEBAPPLICATION '+iif(this.webApplication,'True','False')) 

         //Put out INI options
         DO CASE
         CASE this.form.NOTEBOOK1.INICOMBO.value == "STANDARD"
            //do nothing - no INI option in the BUILD command will 
            // default to original ini behavior
         CASE this.form.NOTEBOOK1.INICOMBO.value == "ROAMING"
            //MsgBox("Putting out INI ROAM")
            fOut.puts('INI ROAM')        
         CASE this.form.NOTEBOOK1.INICOMBO.value == "NONE"
            fOut.puts('INI OFF')
         ENDCASE

         //put out UAC options
         DO CASE
         CASE cUACopt == "OFF"
            fOut.puts('UAC OFF')
         CASE cUACopt == "ON"
            fOut.puts('UAC ON')
			CASE cUACopt == "NONE"
				//no UAC switch - uses default registry entries
         ENDCASE
			
			//put out Runtime options
         if cRUNTIME == "RTEXE"
             fOut.puts('RTEXE')
         endif
			
			//put out BDE option
			if cBDEOption = "BDE OFF"
			   fOut.puts('BDE OFF')
			endif	
			
 
         fOut.close()
         /*testing KK 
            Private crspopen
            crspopen = "!"+cRspFile
            &crspopen.
        */
         
         build from (cRspFile)

         oLog.putCompletedFooter()
         oLog.putCompileFooter()
         oLog.putExeFooter()
         bDEOLibFound = false
         for i = 1 to this.DEOList.size
            if not empty(substr(this.DEOlist[i],3))
               bDEOLibFound = true
               exit
            endif
         next
         if bDEOLibFound
            this.WriteExeIni( oLog )
         endif

         if not bCalledFromRebuild
            oLog.StopLog()

            // M.Kay - TEST - 03/06/2008
            this.form.notebook1.logEditor.value := ""
            this.form.notebook1.logEditor.scrollBar := 2  // Turn on scrollbars in case they were turned off

            this.datalinkLog()
         endif
      catch(exception e)

         oLog.putCompletedFooter()
         oLog.putCompileFooter()
         oLog.PutBuildFailedFooter( e.message )
         if not bCalledFromRebuild
            oLog.StopLog()

            // M.Kay - TEST - 03/06/2008
            this.form.notebook1.logEditor.value := ""
            this.form.notebook1.logEditor.scrollBar := 2  // Turn on scrollbars in case they were turned off

            this.datalinkLog()
         endif

         #ifdef _DEBUG
         msgbox(e.message+" ("+e.lineno+")",this.form.qLang.iGetText('Build Failed!'),16)
         #else
         msgbox(e.message,this.form.qLang.iGetText('Build Failed!'),16)
         #endif
         bSuccess = false
      endTry

      // kill off temporary response file
      try
         if file(cRspFile) and not this.SaveRspFile
            dele file (cRspFile)
         endif
      catch(exception e)
         // kill any error messages
      endTry
      return bSuccess

   function rebuild()
      // rebuild the entire project and create the target executable
      // when specified copy the files marked as deo  to their respective target folders

      local oLog, SourceFile, x, baseName, mdxName, dbtName, ProjectObj, i 

//msgbox("In project.rebuild()")            
      if empty(this.fileName)
        if this.verbose = true
          this.form.qLang.iMsgBox("No project file specified",'Missing Information',16)
        endif
        return false
      endif
      if empty(this.exename)
        if this.verbose = true
          this.form.qLang.iMsgBox("No target executable specified",'Missing Information',16)
        endif
        return false
      endif

      this.releaseLog()
      oLog = new ProjectLog((this))
      oLog.SetNotifiee( this.form.notebook1.logEditor )  // MK Test - 03/06/2008

      oLog.StartLog()

      oLog.PutProcessHeader(this.form.qLang.iGetText("Rebuild All"))

      //D.Brooks 12/17/2004 - QAID 5111 - Change directory before build
      Local StartPath,ActionPath
      StartPath=Set("directory")
      //D.Brooks 12/29/2004 - QAID 5120
      cName = this.compiledExt(this.getFullPath(this.mainFileName))
      //cName = this.getFullPath(this.compiledExt(this.mainFileName))
      ActionPath = left(cName,rat("\",cName)-1)
      set path to (ActionPath)
      set directory to (ActionPath)

      // MK TEST
      _app.allowYieldOnMsg := true

      for i = 1 to alen(this.Contents)

        ProjectObj = this.Contents[i]
        SourceFile = this.getFullPath(ProjectObj.FileName)

        do case
        case not file( SourceFile )
           oLog.PutNotFound( SourceFile )
               oLog.MissingFiles += 1

        case ProjectObj.canBeCompiled

            oLog.CompileCount += 1
            try
               //compile ( SourceFile ) log ( oLog.CompileLog )
					//KK added preprocessor check for [Project] / processor= setting
					//?"this.preprocessordirectives",this.preprocessordirectives
					if empty(this.PreprocessorDirectives)
						compile ( SourceFile ) log ( oLog.CompileLog )
					else
						cCompile = "compile /D"+this.PreprocessorDirectives+" ["+SourceFile+"] log ["+oLog.CompileLog+"]"
						&cCompile.
					endif         
               oLog.PutCompileStats()

               // all the compilable objects will create a file ending with "o"
               // if this is a deo-object then it can be copied to the deo-target folder
               if ProjectObj.deoObject and not ;
                  empty(ProjectObj.deoLibNo)
                  ObjFile =  left( SourceFile, len(trim(SourceFile))-1 ) + "o"
                  this.deoCopy(ObjFile,ProjectObj,oLog)

               endif
            catch (exception e)
               //D.Brooks 11/30/2004 - caused program alert,
               //cSourceFile not a valid variable name.
               cErrMsg = "Compile results for:"+chr(13)+chr(10)+SourceFile+chr(13)+chr(10)+;
                         "COMPILE COMMAND FAILED: "+e.message
//               cErrMsg = "Compile results for:"+chr(13)+chr(10)+cSourceFile+chr(13)+chr(10)+;
//                         "COMPILE COMMAND FAILED: "+e.message
               oLog.PutCompileStats(true,cErrMsg)
            endtry


        otherwise
          // non-compilable objects can also be marked for copy to the deo target folder
           
           if ProjectObj.deoObject = true and ;
                     file(SourceFile) = true  and ;
                     not empty(ProjectObj.deoLibNo)

               class::deoCopy(SourceFile,ProjectObj,oLog)

               if ProjectObj.FileType = ".DBF"
                  this.Form.FNSplit.FullPath := SourceFile
                  this.Form.FNSplit.SplitFileName()
                  baseName = this.Form.FNSplit.DriveDirName
                  mdxname = basename + ".mdx"
                  if file(mdxname)
                     class::deoCopy(mdxName,ProjectObj,oLog)
                  endif
                  dbtname = basename + ".dbt"
                  if file(dbtname)
                     class::deoCopy(dbtName,ProjectObj,oLog)
                  endif
               endif

           endif
        endcase

      next

      set path to (StartPath)
      set directory to (StartPath)

      bBuildSuccess = this.build( oLog )

      oLog.StopLog()

      // M.Kay - TEST - 03/06/2008
      //oLog.SetNotifiee( NULL )  // MK Test - 03/06/2008
      this.form.notebook1.logEditor.value := ""
      this.form.notebook1.logEditor.scrollBar := 2  // Turn on scrollbars in case they were turned off

      this.datalinkLog()

      return bBuildSuccess

   function setAllBDE
      **All BDE Langdriver Settings
      this.form.bde_langdriver1 = new ARRAY()
      this.form.bde_langdriver1.add("")

      this.form.bde_langdriver2 = new ARRAY( 116,2 )
      this.form.bde_langdriver2.fill("")

      this.form.bde_langdriver2[  1,1] = "'ascii' ANSI"
      this.form.bde_langdriver2[  1,2] = "DBWINUS0"
      this.form.bde_langdriver2[  2,1] = "'Spanish' ANSI"
      this.form.bde_langdriver2[  2,2] = "DBWINES0"
      this.form.bde_langdriver2[  3,1] = "'WEurope' ANSI"
      this.form.bde_langdriver2[  3,2] = "DBWINWE0"
      this.form.bde_langdriver2[  4,1] = "Access General"
      this.form.bde_langdriver2[  4,2] = "ACCGEN"
      this.form.bde_langdriver2[  5,1] = "Access Greece"
      this.form.bde_langdriver2[  5,2] = "ACCGREEK"
      this.form.bde_langdriver2[  6,1] = "Access Japanese"
      this.form.bde_langdriver2[  6,2] = "ACCJAPAN"
      this.form.bde_langdriver2[  7,1] = "Access Nord/Danish"
      this.form.bde_langdriver2[  7,2] = "ACCNRDAN"
      this.form.bde_langdriver2[  8,1] = "Access Swed/Finnish"
      this.form.bde_langdriver2[  8,2] = "ACCSWFIN"
      this.form.bde_langdriver2[  9,1] = "Borland ANSI Arabic"
      this.form.bde_langdriver2[  9,2] = "BLWINAR0"
      this.form.bde_langdriver2[ 10,1] = "Borland DAN Latin-1"
      this.form.bde_langdriver2[ 10,2] = "BLLT1DA0"
      this.form.bde_langdriver2[ 11,1] = "Borland DEU Latin-1"
      this.form.bde_langdriver2[ 11,2] = "BLLT1DE0"
      this.form.bde_langdriver2[ 12,1] = "Borland ENG Latin-1"
      this.form.bde_langdriver2[ 12,2] = "BLLT1UK0"
      this.form.bde_langdriver2[ 13,1] = "Borland ENU Latin-1"
      this.form.bde_langdriver2[ 13,2] = "BLLT1US0"
      this.form.bde_langdriver2[ 14,1] = "Borland ESP Latin-1"
      this.form.bde_langdriver2[ 14,2] = "BLLT1ES0"
      this.form.bde_langdriver2[ 15,1] = "Borland FIN Latin-1"
      this.form.bde_langdriver2[ 15,2] = "BLLT1FI0"
      this.form.bde_langdriver2[ 16,1] = "Borland FRA Latin-1"
      this.form.bde_langdriver2[ 16,2] = "BLLT1FR0"
      this.form.bde_langdriver2[ 17,1] = "Borland FRC Latin-1"
      this.form.bde_langdriver2[ 17,2] = "BLLT1CA0"
      this.form.bde_langdriver2[ 18,1] = "Borland ISL Latin-1"
      this.form.bde_langdriver2[ 18,2] = "BLLT1IS0"
      this.form.bde_langdriver2[ 19,1] = "Borland ITA Latin-1"
      this.form.bde_langdriver2[ 19,2] = "BLLT1IT0"
      this.form.bde_langdriver2[ 20,1] = "Borland NLD Latin-1"
      this.form.bde_langdriver2[ 20,2] = "BLLT1NL0"
      this.form.bde_langdriver2[ 21,1] = "Borland NOR Latin-1"
      this.form.bde_langdriver2[ 21,2] = "BLLT1NO0"
      this.form.bde_langdriver2[ 22,1] = "Borland PTG Latin-1"
      this.form.bde_langdriver2[ 22,2] = "BLLT1PT0"
      this.form.bde_langdriver2[ 23,1] = "Borland SVE Latin-1"
      this.form.bde_langdriver2[ 23,2] = "BLLT1SV0"
      this.form.bde_langdriver2[ 24,1] = "DB2 SQL ANSI DEU"
      this.form.bde_langdriver2[ 24,2] = "DB2ANDEU"
      this.form.bde_langdriver2[ 25,1] = "dBASE BUL 868"
      this.form.bde_langdriver2[ 25,2] = "BGDB868"
      this.form.bde_langdriver2[ 26,1] = "dBASE CHS cp936"
      this.form.bde_langdriver2[ 26,2] = "DB936CN0"
      this.form.bde_langdriver2[ 27,1] = "dBASE CHT cp950"
      this.form.bde_langdriver2[ 27,2] = "DB950TW0"
      this.form.bde_langdriver2[ 28,1] = "dBASE CSY cp852"
      this.form.bde_langdriver2[ 28,2] = "DB852CZ0"
      this.form.bde_langdriver2[ 29,1] = "dBASE CSY cp867"
      this.form.bde_langdriver2[ 29,2] = "DB867CZ0"
      this.form.bde_langdriver2[ 30,1] = "dBASE DAN cp865"
      this.form.bde_langdriver2[ 30,2] = "DB865DA0"
      this.form.bde_langdriver2[ 31,1] = "dBASE DEU cp437"
      this.form.bde_langdriver2[ 31,2] = "DB437DE0"
      this.form.bde_langdriver2[ 32,1] = "dBASE DEU cp850"
      this.form.bde_langdriver2[ 32,2] = "DB850DE0"
      this.form.bde_langdriver2[ 33,1] = "dBASE ELL GR437"
      this.form.bde_langdriver2[ 33,2] = "DB437GR0"
      this.form.bde_langdriver2[ 34,1] = "dBASE ENG cp437"
      this.form.bde_langdriver2[ 34,2] = "DB437UK0"
      this.form.bde_langdriver2[ 35,1] = "dBASE ENG cp850"
      this.form.bde_langdriver2[ 35,2] = "DB850UK0"
      this.form.bde_langdriver2[ 36,1] = "dBASE ENU cp437"
      this.form.bde_langdriver2[ 36,2] = "DB437US0"
      this.form.bde_langdriver2[ 37,1] = "dBASE ENU cp850"
      this.form.bde_langdriver2[ 37,2] = "DB850US0"
      this.form.bde_langdriver2[ 38,1] = "dBASE ESP cp437"
      this.form.bde_langdriver2[ 38,2] = "DB437ES1"
      this.form.bde_langdriver2[ 39,1] = "dBASE ESP cp850"
      this.form.bde_langdriver2[ 39,2] = "DB850ES0"
      this.form.bde_langdriver2[ 40,1] = "dBASE FIN cp437"
      this.form.bde_langdriver2[ 40,2] = "DB437FI0"
      this.form.bde_langdriver2[ 41,1] = "dBASE FRA cp437"
      this.form.bde_langdriver2[ 41,2] = "DB437FR0"
      this.form.bde_langdriver2[ 42,1] = "dBASE FRA cp850"
      this.form.bde_langdriver2[ 42,2] = "DB850FR0"
      this.form.bde_langdriver2[ 43,1] = "dBASE FRC cp850"
      this.form.bde_langdriver2[ 43,2] = "DB850CF0"
      this.form.bde_langdriver2[ 44,1] = "dBASE FRC cp863"
      this.form.bde_langdriver2[ 44,2] = "DB863CF1"
      this.form.bde_langdriver2[ 45,1] = "dBASE HUN cp852"
      this.form.bde_langdriver2[ 45,2] = "DB852HDC"
      this.form.bde_langdriver2[ 46,1] = "dBASE ITA cp437"
      this.form.bde_langdriver2[ 46,2] = "DB437IT0"
      this.form.bde_langdriver2[ 47,1] = "dBASE ITA cp850"
      this.form.bde_langdriver2[ 47,2] = "DB850IT1"
      this.form.bde_langdriver2[ 48,1] = "dBASE JPN cp932"
      this.form.bde_langdriver2[ 48,2] = "DB932JP0"
      this.form.bde_langdriver2[ 49,1] = "dBASE JPN Dic932"
      this.form.bde_langdriver2[ 49,2] = "DB932JP1"
      this.form.bde_langdriver2[ 50,1] = "dBASE KOR cp949"
      this.form.bde_langdriver2[ 50,2] = "DB949KO0"
      this.form.bde_langdriver2[ 51,1] = "dBASE NLD cp437"
      this.form.bde_langdriver2[ 51,2] = "DB437NL0"
      this.form.bde_langdriver2[ 52,1] = "dBASE NLD cp850"
      this.form.bde_langdriver2[ 52,2] = "DB850NL0"
      this.form.bde_langdriver2[ 53,1] = "dBASE NOR cp865"
      this.form.bde_langdriver2[ 53,2] = "DB865NO0"
      this.form.bde_langdriver2[ 54,1] = "dBASE PLK cp852"
      this.form.bde_langdriver2[ 54,2] = "DB852PO0"
      this.form.bde_langdriver2[ 55,1] = "dBASE PTB cp850"
      this.form.bde_langdriver2[ 55,2] = "DB850PT0"
      this.form.bde_langdriver2[ 56,1] = "dBASE PTG cp860"
      this.form.bde_langdriver2[ 56,2] = "DB860PT0"
      this.form.bde_langdriver2[ 57,1] = "dBASE RUS cp866"
      this.form.bde_langdriver2[ 57,2] = "DB866RU0"
      this.form.bde_langdriver2[ 58,1] = "dBASE SLO cp852"
      this.form.bde_langdriver2[ 58,2] = "DB852SL0"
      this.form.bde_langdriver2[ 59,1] = "dBASE SVE cp437"
      this.form.bde_langdriver2[ 59,2] = "DB437SV0"
      this.form.bde_langdriver2[ 60,1] = "dBASE SVE cp850"
      this.form.bde_langdriver2[ 60,2] = "DB850SV1"
      this.form.bde_langdriver2[ 61,1] = "dBASE THA cp874"
      this.form.bde_langdriver2[ 61,2] = "DB874TH0"
      this.form.bde_langdriver2[ 62,1] = "dBASE TRK cp857"
      this.form.bde_langdriver2[ 62,2] = "DB857TR0"
      this.form.bde_langdriver2[ 63,1] = "FoxPro Czech 1250"
      this.form.bde_langdriver2[ 63,2] = "FOXCZWIN"
      this.form.bde_langdriver2[ 64,1] = "FoxPro Czech DOS895"
      this.form.bde_langdriver2[ 64,2] = "FOXCZ895"
      this.form.bde_langdriver2[ 65,1] = "FoxPro German 1252"
      this.form.bde_langdriver2[ 65,2] = "FOXDEWIN"
      this.form.bde_langdriver2[ 66,1] = "FoxPro German 437"
      this.form.bde_langdriver2[ 66,2] = "FOXDE437"
      this.form.bde_langdriver2[ 67,1] = "FoxPro Nordic 1252"
      this.form.bde_langdriver2[ 67,2] = "FOXNOWIN"
      this.form.bde_langdriver2[ 68,1] = "FoxPro Nordic 437"
      this.form.bde_langdriver2[ 68,2] = "FOXNO437"
      this.form.bde_langdriver2[ 69,1] = "FoxPro Nordic 850"
      this.form.bde_langdriver2[ 69,2] = "FOXNO850"
      this.form.bde_langdriver2[ 70,1] = "Hebrew dBASE"
      this.form.bde_langdriver2[ 70,2] = "DBHEBREW"
      this.form.bde_langdriver2[ 71,1] = "MSSQL ANSI Greek"
      this.form.bde_langdriver2[ 71,2] = "MSSGRWIN"
      this.form.bde_langdriver2[ 72,1] = "Oracle SQL WE850"
      this.form.bde_langdriver2[ 72,2] = "ORAWE850"
      this.form.bde_langdriver2[ 73,1] = "Paradox 'ascii'"
      this.form.bde_langdriver2[ 73,2] = "ASCII"
      this.form.bde_langdriver2[ 74,1] = "Paradox 'hebrew'"
      this.form.bde_langdriver2[ 74,2] = "HEBREW"
      this.form.bde_langdriver2[ 75,1] = "Paradox 'intl'"
      this.form.bde_langdriver2[ 75,2] = "INTL"
      this.form.bde_langdriver2[ 76,1] = "Paradox 'intl' 850"
      this.form.bde_langdriver2[ 76,2] = "INTL850"
      this.form.bde_langdriver2[ 77,1] = "Paradox 'japan'"
      this.form.bde_langdriver2[ 77,2] = "JAPAN"
      this.form.bde_langdriver2[ 78,1] = "Paradox 'nordan'"
      this.form.bde_langdriver2[ 78,2] = "NORDAN"
      this.form.bde_langdriver2[ 79,1] = "Paradox 'nordan40'"
      this.form.bde_langdriver2[ 79,2] = "NORDAN40"
      this.form.bde_langdriver2[ 80,1] = "Paradox 'swedfin'"
      this.form.bde_langdriver2[ 80,2] = "SWEDFIN"
      this.form.bde_langdriver2[ 81,1] = "Paradox 'turk'"
      this.form.bde_langdriver2[ 81,2] = "TURK"
      this.form.bde_langdriver2[ 82,1] = "Paradox ANSI HEBREW"
      this.form.bde_langdriver2[ 82,2] = "ANHEBREW"
      this.form.bde_langdriver2[ 83,1] = "Paradox BUL 868"
      this.form.bde_langdriver2[ 83,2] = "BULGARIA"
      this.form.bde_langdriver2[ 84,1] = "Paradox China 936"
      this.form.bde_langdriver2[ 84,2] = "CHINA"
      this.form.bde_langdriver2[ 85,1] = "Paradox Cyrr 866"
      this.form.bde_langdriver2[ 85,2] = "CYRR"
      this.form.bde_langdriver2[ 86,1] = "Paradox Czech 852"
      this.form.bde_langdriver2[ 86,2] = "CZECH"
      this.form.bde_langdriver2[ 87,1] = "Paradox ESP 437"
      this.form.bde_langdriver2[ 87,2] = "SPANISH"
      this.form.bde_langdriver2[ 88,1] = "Paradox Greek GR437"
      this.form.bde_langdriver2[ 88,2] = "GRCP437"
      this.form.bde_langdriver2[ 89,1] = "Paradox Hun 852 DC"
      this.form.bde_langdriver2[ 89,2] = "HUN852DC"
      this.form.bde_langdriver2[ 90,1] = "Paradox ISL 861"
      this.form.bde_langdriver2[ 90,2] = "ICELAND"
      this.form.bde_langdriver2[ 91,1] = "Paradox Korea 949"
      this.form.bde_langdriver2[ 91,2] = "KOREA"
      this.form.bde_langdriver2[ 92,1] = "Paradox Polish 852"
      this.form.bde_langdriver2[ 92,2] = "POLISH"
      this.form.bde_langdriver2[ 93,1] = "Paradox Slovene 852"
      this.form.bde_langdriver2[ 93,2] = "SLOVENE"
      this.form.bde_langdriver2[ 94,1] = "Paradox Taiwan 950"
      this.form.bde_langdriver2[ 94,2] = "TAIWAN"
      this.form.bde_langdriver2[ 95,1] = "Paradox Thai 874"
      this.form.bde_langdriver2[ 95,2] = "THAI"
      this.form.bde_langdriver2[ 96,1] = "Pdox ANSI Bulgaria"
      this.form.bde_langdriver2[ 96,2] = "BGPD1251"
      this.form.bde_langdriver2[ 97,1] = "Pdox ANSI Cyrillic"
      this.form.bde_langdriver2[ 97,2] = "ANCYRR"
      this.form.bde_langdriver2[ 98,1] = "Pdox ANSI Czech"
      this.form.bde_langdriver2[ 98,2] = "ANCZECH"
      this.form.bde_langdriver2[ 99,1] = "Pdox ANSI Greek"
      this.form.bde_langdriver2[ 99,2] = "ANGREEK1"
      this.form.bde_langdriver2[100,1] = "Pdox ANSI Hun. DC"
      this.form.bde_langdriver2[100,2] = "ANHUNDC"
      this.form.bde_langdriver2[101,1] = "Pdox ANSI Intl"
      this.form.bde_langdriver2[101,2] = "ANSIINTL"
      this.form.bde_langdriver2[102,1] = "Pdox ANSI Intl850"
      this.form.bde_langdriver2[102,2] = "ANSII850"
      this.form.bde_langdriver2[103,1] = "Pdox ANSI Nordan4"
      this.form.bde_langdriver2[103,2] = "ANSINOR4"
      this.form.bde_langdriver2[104,1] = "Pdox ANSI Polish"
      this.form.bde_langdriver2[104,2] = "ANPOLISH"
      this.form.bde_langdriver2[105,1] = "Pdox ANSI Slovene"
      this.form.bde_langdriver2[105,2] = "ANSISLOV"
      this.form.bde_langdriver2[106,1] = "Pdox ANSI Spanish"
      this.form.bde_langdriver2[106,2] = "ANSISPAN"
      this.form.bde_langdriver2[107,1] = "Pdox ANSI Swedfin"
      this.form.bde_langdriver2[107,2] = "ANSISWFN"
      this.form.bde_langdriver2[108,1] = "Pdox ANSI Turkish"
      this.form.bde_langdriver2[108,2] = "ANTURK"
      this.form.bde_langdriver2[109,1] = "pdx ANSI Czech 'CH'"
      this.form.bde_langdriver2[109,2] = "ANCZECHW"
      this.form.bde_langdriver2[110,1] = "pdx ANSI ISO L_2 CZ"
      this.form.bde_langdriver2[110,2] = "ANIL2CZW"
      this.form.bde_langdriver2[111,1] = "pdx Czech 852 'CH'"
      this.form.bde_langdriver2[111,2] = "CZECHW"
      this.form.bde_langdriver2[112,1] = "pdx Czech 867 'CH'"
      this.form.bde_langdriver2[112,2] = "CSKAMENW"
      this.form.bde_langdriver2[113,1] = "pdx ISO L_2 Czech"
      this.form.bde_langdriver2[113,2] = "IL2CZW"
      this.form.bde_langdriver2[114,1] = "SQL Link ROMAN8"
      this.form.bde_langdriver2[114,2] = "BLROM800"
      this.form.bde_langdriver2[115,1] = "Sybase SQL Dic437"
      this.form.bde_langdriver2[115,2] = "SYDC437"
      this.form.bde_langdriver2[116,1] = "Sybase SQL Dic850"
      this.form.bde_langdriver2[116,2] = "SYDC850"

      for i = 1 to alen(this.form.bde_langdriver2,1)
          this.form.bde_langdriver1.add(this.form.bde_langdriver2[i,1])
      next
      this.form.notebook1.Innonotebook.BDE_LANGDRIVER.dataSource:= "Array this.form.bde_langdriver1"
    return

   function WriteExeIni( oLog )
      local oIni, i, cIniFile, DEOLib, nObjPath, cExePath
      cExePath = this.getFullPath(this.EXEname)
      this.Form.FNSplit.FullPath := cExePath
      this.Form.FNSplit.SplitFileName()
      cIniFile = this.Form.FNSplit.DriveDirName + ".ini"
      oIni = new CoreIni( cIniFile )
      oIni.DeleteSection('ObjectPath')
      nObjPath = 0
      for i = 1 to this.DEOList.size
         DEOLib = substr(this.DEOlist[i],3)
         if not empty( DEOLib )
            oIni.setValue('ObjectPath',"objPath"+nObjPath,DEOLib)
            nObjPath++
         endif
      next
      oIni.flush()
      oLog.PutIniFooter( cIniFile )
      msgBox("INI FILE: "+cIniFile)
      return true

   function deoCopy(cFilename, oFileObj, oLog)
      // copy the given file to the target directory specified in the deoLib property
      LOCAL deoLib, x, deoName, bCopySuccess

      bCopySuccess = true
      //deoLib = substr(this.deoList[val(this.contents[nItem].deoLibNo) + 1],3)
      deoLib = substr(this.deoList[val(oFileObj.deoLibNo) + 1],3)
      x = rat("\",cFilename)
      if x = 0 
         deoName = deoLib + "\" + cFileName
      else
         deoName = deoLib + substr(cFileName,x)
      endif

      local cSafety
      cSafety = set('Safety')
      set safety off

      try

         copy file (cFileName) to (deoName)

      catch (exception e)

         oLog.putDEOCopyFail( cFileName, deoName )
         bCopySuccess = false
         if this.Verbose
            msgbox(e.message,this.form.qLang.iGetText('DEO Copy Error'),16)
         endif

      endtry

      if bCopySuccess

         oLog.deoCount += 1
         oLog.putDEOCopySuccess( cFileName, deoName )

      endif

      if cSafety = "ON"
         set safety on
      else
         set safety off
      endif

      return bCopySuccess

   //28/07/2012 J.Kwekkeboom 
   //Moved all needed files to Runtime\Include for next releases...
   //so we get here this folder from dBase registry
   function getRunIncludeFolder
   Privat reg,REG_KEY,cValue
     // #include <winreg.h>
      set procedure to :coreshared:registry.prg additive      

      REG_KEY="SOFTWARE\dBASE\PLUS\series11"      
      reg = new RegistryDb(HKEY_LOCAL_MACHINE, REG_KEY )       
      cValue = reg.queryValue("RuntimePath")
      try
        if not cValue
           cValue:=''        
        endif
      catch(exception e)
      endtry  
      close procedure :coreshared:registry.prg           
   return cValue

   //28/07/2012 J. Kwekkeboom
   function getInnoAppFolder
   Privat reg,REG_KEY,cValue
      Set procedure to :coreshared:registry.prg additive
      REG_KEY="SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Inno Setup 5_is1"      
      reg = new RegistryDb(HKEY_LOCAL_MACHINE, REG_KEY )
      cValue = reg.queryValue("InstallLocation")
      try
        if not cValue
           cValue:=''
        endif
      catch(exception e)
      endtry      
      close procedure :coreshared:registry.prg           
   return cValue
     

   function getSMFolder
   Privat reg,REG_KEY,cValue
     // #include <winreg.h>
      set procedure to :coreshared:registry.prg additive
      //REG_KEY="SOFTWARE\Jonny Kwekkeboom\ScriptMaker Delphi\Settings"
      REG_KEY="SOFTWARE\Jonny Kwekkeboom\ISG\Update"
      reg = new RegistryDb(HKEY_LOCAL_MACHINE, REG_KEY )
      cValue = reg.queryValue("Path")
      try
        if not cValue
           cValue:=''
        endif
      catch(exception e)
      endtry      
      close procedure :coreshared:registry.prg           
   return cValue

   function getISGDefFolder
   Privat reg,REG_KEY,cValue
     // #include <winreg.h>
      set procedure to :coreshared:registry.prg additive
      //HKEY_CURRENT_USER\Software\Jonny Kwekkeboom\ISG
      REG_KEY="SOFTWARE\Jonny Kwekkeboom\ISG\Update"
      reg = new Registry(HKEY_CURRENT_USER, REG_KEY )
      cValue = reg.queryValue("LocalFolder")
      try
        if not cValue
           cValue:=''
        endif
      catch(exception e)
      endtry      
      close procedure :coreshared:registry.prg           
   return cValue

	//D.Brooks - 09/30/2005 - method to retrieve Inno version from registry
   function getInnoVersion
   	private reg,REG_KEY,cValue,nValue

      nValue = 0

      set procedure to :coreshared:registry.prg additive
      REG_KEY="SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Inno Setup 5_is1"
      reg = new Registry(HKEY_LOCAL_MACHINE, REG_KEY )
      cValue = reg.queryValue("Inno Setup: Setup Version")
      try
        if type("cValue") = "L" and not cValue
           nValue:=0
        else
        	  do while at(".", cValue) > 0
              cValue = stuff(cValue, at(".",cValue), 1, "")
           enddo

           if cValue.length == 1
           	  cValue = cValue + "000"
           elseif cValue.length == 2
              cValue = cValue + "00"
           elseif cValue.length == 3
              cValue = cValue + "0"
           endif

           nValue = val(cValue)
        endif
      catch(exception e)
      endtry      
      close procedure :coreshared:registry.prg           
   return nValue
   //End D.Brooks - 09/30/2005

	//D.Brooks - 10/03/2005 - method to retrieve Inno version from registry
   function getSMVersion
   	private reg,REG_KEY,cValue,nValue

      nValue = 0

      set procedure to :coreshared:registry.prg additive
      //REG_KEY="SOFTWARE\Jonny Kwekkeboom\ScriptMaker Delphi\Update"
      REG_KEY="SOFTWARE\Jonny Kwekkeboom\ISG\Update"
      reg = new Registry(HKEY_LOCAL_MACHINE, REG_KEY )
      cValue = reg.queryValue("LastRunBeforeUpdateVersion")
      try
        if type("cValue") = "L" and not cValue
           nValue:=0
        else
           nValue = val(cValue)
        endif
      catch(exception e)
      endtry      
      close procedure :coreshared:registry.prg           
   return nValue
   //End D.Brooks - 10/03/2005


   Function LoadFileTypes()
      // the fileTypes AssocArray is loaded from a standard .ini file
      // each known file type (file extension) is an object in the assoc array
      // if a project is loaded the fileTypes array will automatically be loaded first
      // if a new project is being created then this method must be
      // called explicitly
      LOCAL iniFile, typ, inBuffer, x, keyWord, data
      this.fileTypes              = new AssocArray()

      // load the fileTypes array from the Ini file
      // The ini file is contained in a central dBASE folder
      // pointed to by the source alias ProjExp

      // M.Kay - TEST - 07/16/2013
      iniPath = _app.currentuserpath+'\dBLCore\ProjExp\projexp.ini'
      if not file( iniPath )
         this.form.recreateIni( iniPath )
      endif


      iniFile = new file()
      iniFile.open(_app.currentuserpath+"\dBLcore\ProjExp\projexp.ini")

      typ = "UNKNOWN"

      do while not iniFile.eof()
        // M.Kay - TEST - specifying end of line characters - 07/16/2013
        //inBuffer = ltrim( trim( iniFile.gets( 2048, chr(10)) ) )
        inBuffer = ltrim( trim( iniFile.gets() ) )
        if left(inBuffer,1) == ';'
          loop
        endif
        if left(inBuffer,1) == "["
          // only 1 section at present so ignore it
          loop
        endif
        x=at("=",inBuffer)
        if x > 0
          keyword = trim( left(inBuffer,x-1) ) 
          data    = ltrim( substr(inBuffer,x+1) )
        else
          keyWord = inBuffer
        endif
        do case
        case upper(keyword)="FILETYPE"
          // the file extension is always set to upper and starts with a "."
          typ = upper(trim(data))
          this.fileTypes[typ] = new object()
          this.fileTypes[typ].FileGroup = 9
          this.fileTypes[typ].canBeEdited = false
          this.fileTypes[typ].canbeMain = false
          this.fileTypes[typ].canBeIncluded = false
          this.fileTypes[typ].canBeExcluded = false
          this.fileTypes[typ].designer = ""
          this.fileTypes[typ].icon = "#50"
          this.fileTypes[typ].RunWith = "ex"
        case upper(keyword)="FILEGROUP"
          this.fileTypes[typ].fileGroup     = val(data)
        case upper(keyword)="CANBEEDITED"
          this.fileTypes[typ].canBeEdited   = iif(upper(data)="TRUE",true,false)
        case upper(keyword)="CANBEMAIN"
          this.fileTypes[typ].canBeMain     = iif(upper(data)="TRUE",true,false)
        case upper(keyword)="CANBEEXCLUDED"
          this.fileTypes[typ].canBeExcluded = iif(upper(data)="TRUE",true,false)
        case upper(keyword)="CANBEINCLUDED"
          this.fileTypes[typ].canBeIncluded = iif(upper(data)="TRUE",true,false)
        case upper(keyword)="CANBECOMPILED"
          this.fileTypes[typ].canBeCompiled = iif(upper(data)="TRUE",true,false)
        case upper(keyword)="USEDESIGNER"
          this.fileTypes[typ].designer      = data
        case upper(keyword)="ICON"
          this.fileTypes[typ].icon          = data
        case upper(keyword)="RUNWITH"
          this.fileTypes[typ].runWith       = data
        endcase
      enddo
      iniFile.close()
      return


      /////---------------------- Method: CompiledExt -----------------------/////
      ///// Purpose: returns "compiled" version of a compilable filename
      function compiledExt(cFilePath)
         local cExt, cPath

         //KK - 03/30/2010 took out UPPER(trim(cfilePath))  
         // Kill any trailing spaces
         cPath = trim(cFilePath)
         // get extension
         if at('.',cPath) > 0
            // Extract the extension
            cExt = UPPER(substr(cPath,rat('.',cPath)+1))
            if at(cExt,'WFM|CFM|MNU|POP|DMD|CDM|CC|PRG|REP|LBL|LAB|QBE|CRP') > 0
               return substr(cPath,1,len(cPath)-1)+'O'
            else
               return cPath
            endif
         endif

         return cPath

   ****************
   //added out of BDEAlias.cc file for let also support database drop by table
   function dataBaseFound( cDatabase )
      // Code by Gary White
      local d, dbArray, lFound, i
      d = new database()
      dbArray=d.getschema( "Databases" )
      release object d
      lFound = false
      for i = 1 to dbArray.size
         if upper( cDatabase ) == upper( dbArray[i] )
            lFound := true
            exit
         endif
      endfor
   return lFound

   **************

   function isObjectFile( cFilePath )
      local cExt, cPath
      cPath = upper(trim(cFilePath))
      cExt = substr(cPath,rat('.',cPath)+1)
      return cExt $ 'WFO|CFO|MNO|POO|DMO|CDO|CO|PRO|REO|LBO|LAO|QBO|CRO'

   function getShortName( cFilePath )
	//	?"cFilePath = "+cFilePath
   //	?"type this.Form.FNSplit.FullPath:"+type("this.Form.FNSplit.FullPath")
	//	?
	  try
			this.Form.FNSplit.FullPath := ltrim(trim(cFilePath))
			this.Form.FNSplit.SplitFileName()
		catch (exception e)
		   return
		endtry
      return this.Form.FNSplit.Name + this.Form.FNSplit.Ext

   function getShortNameWithOutExt( cFilePath )
      try
      this.Form.FNSplit.FullPath := ltrim(trim(cFilePath))
      this.Form.FNSplit.SplitFileName()
      catch(exception e)
        //eat errors on Windows xClose
        return( cFilePath)
      endtry
      return this.Form.FNSplit.Name

   function getFullPath( cFN )
      Local cFileName, cNewFileName, nChar
      //changed above parameter from cFileName to cFN
      // and added the line below (KK 01/22/10)
      //found that some memVar's passed as parameters were
      // having their values changed unexpectedly.
      // for example in ResolveSavePath - the test for 
      // == this.getFullPath( cSavedPath ) was actually changing
      // cSavedPath to a full path every time.
      cFileName = ""+cFN

      // If arg doesn't include full path, it is
      // considered relative to .PRJ home folder
      if empty( cFileName )
         return ""
      else
         //added try catch block for not crash by type projectname and 
         //Windows red xClose 
         //J.Kwekkeboom 01/17/2007
         try
            // Expand source alias to explicit path (if any) in cFileName
            cFilename=this.form.CheckSourceAlias(cFileName)
         catch(exception e)
            return( cFileName)
         endtry

         local oSplit
         oSplit = this.Form.FNSplit
         oSplit.FullPath := ltrim(trim(cFileName))

         oSplit.SplitFileName()

         //D.Brooks 11/24/2004 - adding code from original fvcs version 
         //to make relative paths work (QAID 4827)
         if oSplit.BadSourceAlias
            return cFileName
         elseif not empty( oSplit.Drive ) // Full path
            return oSplit.FullPath
            //return cFileName
         //03-19-2005 J.Kwekkeboom - Added support for included UNC Path 
         elseif left(cFileName,2) == "\\"
            return cFileName
         // Relative path
         /*
         elseif left(cFileName,3) == "..\"
            MsgBox("this.Folder+'\'+cFileName='"+this.folder+"\"+cFileName)
            return this.Folder + "\" + cFileName
         elseif left(cFileName,2) == ".\"
            return this.Folder + "\" + cFileName
         elseif left(cFileName,1) == "\"
            return this.Folder + oSplit.Dir + "\" + oSplit.Name + oSplit.Ext
         */
         //KK - 03/26/2010
         //Modified above to use API to get relative path 
         //this fixes issues where "..\" or ".\" could be left
         //in the middle of the cFileName string
         elseif  "\" $ cFileName //"..\" $ cFileName OR ".\" $ cFileName OR LEFT(cFileName,1) == "\" 

            //if the recursion is in the front of the FileName dir 
            //we need to make sure the Proj File's folder is at the start
            //of the Path before calling PathResolve
            DO CASE
            CASE left(cFileName,3) == "..\"
               cFileName = this.Folder + "\" + cFileName
            CASE left(cFileName,2) == ".\"
               cFileName = this.Folder + "\" + cFileName
            CASE left(cFileName,1) == "\"  
               cFileName = this.Folder + oSplit.Dir + "\" + oSplit.Name + oSplit.Ext
            OTHERWISE  //no backslash at all
               cFileName = this.Folder + "\" + oSplit.Dir + "\" + oSplit.Name + oSplit.Ext
            ENDCASE

            #define MAX_PATH 260
            #define PRF_VERIFYEXISTS 0x0001
            cFileName = cFileName+chr(0)
            cFileName = cFileName+REPLICATE(" ",MAX_PATH-LEN(cFileName))
            if type("dB_PathResolve")#"FP"
               extern CLOGICAL dB_PathResolve( CPTR, CPTR, CUINT ) Shell32.dll from "PathResolve"
            endif
            CExists = dB_PathResolve( cFileName , NULL , PRF_VERIFYEXISTS )
            //make sure full path is obtained and take out 
            //null terminator

            if cExists
               cFileName = trim( cFileName )
               cFileName = LEFT(cFileName,AT(CHR(0),cFileName)-1)
            endif
            return cFileName

         
         else // No path
            //MsgBox("no path : "+this.folder+"\" + oSplit.Name + oSplit.Ext)
            return this.Folder + "\" + oSplit.Name + oSplit.Ext
         endif
         
      endif
      return cFileName

      Function getRelativePath( cRootPath, cFullPath )

         //KK - 03/30/2010 - added handling of UNC and paths with diff drive
         //if UNC path return full path
         if LEFT(cFullPath,2) == "\\"
            return cFullPath
         endif
         //If file is in a different drive
         //return full path
         if ":\" $ LEFT(cFullPath,3) AND (upper(LEFT(cFullPath,3)) <> upper(LEFT(cRootPath,3)))
            return cFullPath
         endif
             
         // Returns the relative path to a file given a root path and the files full path
         local cRelativePath, cBuffer, lResult

         cRelativePath = ""

         if type("dB_PathRelativePathTo") # "FP"
            //BOOL PathRelativePathTo( LPTSTR pszPath,
            //                         LPCTSTR pszFrom,
            //                         DWORD dwAttrFrom,
            //                         LPCTSTR pszTo,
            //                         DWORD dwAttrTo
            //                        );

            extern CLOGICAL dB_PathRelativePathTo( CPTR, CPTR, CUINT, CPTR, CUINT ) shlwapi.dll ;
                  from "PathRelativePathToW"
         endif

         lResult = FALSE

         #define MAX_PATH  260
         #define FILE_ATTRIBUTE_DIRECTORY        0x00000010
         #define FILE_ATTRIBUTE_NORMAL           0x00000080

         cBuffer = space( MAX_PATH )

         lResult = dB_PathRelativePathTo( cBuffer, ;
                                          cRootPath, ;
                                          FILE_ATTRIBUTE_DIRECTORY, ;
                                          cFullPath, ;
                                          FILE_ATTRIBUTE_NORMAL )            
//? "lResult = ",lResult
//? "cBuffer=>"+cBuffer+"<="
         if lResult
            cRelativePath = trim( cBuffer )
            if substr(cRelativePath, len(cRelativePath))==chr(0)
               // Remove trailing null if any
               cRelativePath = substr(cRelativePath,1,len(cRelativePath)-1)
            endif
            if substr(cRelativePath,1,2) == ".\"
               cRelativePath = substr(cRelativePath,3)
            endif

         endif
         return cRelativePath

ENDCLASS

///////////////////////////////////////////////////////////////////////////////
// Filename:    projectObject.cc                                             //
// Written for: dBASE (32-bit versions)                                      //
// Written by:  dBASE Inc. R&D Group                                         //
// Copyright:   Copyright © dBASE Inc. 2002                                  //
// Purpose:     Project Object Class                                         //
//              used for managing files contained in a project               //
///////////////////////////////////////////////////////////////////////////////
CLASS projectObject

  this.FileName      = ""        // the long fileName

  // M.Kay - 05/16/2008
  //this.RelPath       = ""        // Relative path to file from project folder
  //this.FullPath      = ""        // Full (explicit) path to file
  this.SavedPath     = ""        // File path and name as read from or stored to project file

  this.excluded      = false     // object is excluded from build
  this.included      = false     // object is included in build
  this.ShortName     = ""        // object ShortName
  this.FileType      = ""        // object type (file extension)
  this.isMain        = false     // object is the main Program
  this.deoObject     = false     // object designated for DEO deployment
  this.deoLibNo      = " "       // deo Library No
  // the following properties are set based on the control information in
  // the project builder .ini file
  // they are initialised as for unknown file types
  this.FileGroup     = 9         // group to which this object belongs
  this.Icon          = "resource #240"  // icon to be used to display this object
  this.custom        = false     // unused
  this.tree          = null      // direct reference to tree object for this object
  this.canBeMain     = false     // can be a main program
  this.canBeExcluded = false     // can be excluded from build
  this.canBeIncluded = true      // can be included in build
  this.runWith       = ""        // run this object with ....
  this.canBeEdited   = false     // object can be edited
  this.canBeCompiled = false     // object can be compiled
  this.designer      = ""        // the designer to be used if any
  this.InnoFolder    = "{app}"   // Destination folder for INNO deployment

ENDCLASS

class ProjectLog( oProject ) of file

   this.Project = oProject
   this.Name = this.Project.getFullPath(oProject.LogFileName)
   if empty(this.Name)
      this.SetDefaultLog()
   endif
   this.MaxBytes = MAX_LOG_SIZE
   this.CompileLog = funique(getTempFolder()+"\PE??????.tmp")

   // QAID: 6383 - M.Kay, dBI Inc. - 01/17/2007
   this.Notifiee = null  // object ref to object to notify of log change

   this.Reset()

   // QAID: 6383 - M.Kay, dBI Inc. - 01/17/2007
   // Add mechanism to have compile log update during project build
   // in Project Manager's Editor component
   function SetNotifiee( oNotifiee )
      oNotifiee.reinit()
      this.Notifiee = oNotifiee
      return


   function ClearNotifiee()
      this.Notifiee := null
      return


   function Notify( cLog )
      if not empty( this.Notifiee )
         try
            this.Notifiee.OnNotify( cLog )
         catch (exception e )
            // Unable to notify 
         endtry
      endif
      return
   // End QAID: 6383 - M.Kay, dBI Inc. - 01/17/2007

   function Reset
      try
         this.close()
      catch (exception e)
      endtry
      this.Started = false
      this.CompileCount = 0
      this.DEOCount = 0
      this.CompileErrors = 0
      this.MissingFiles  = 0
      this.StartTime = ""
      this.EndTime = ""
      this.Verbose = this.Project.VerboseLog
      return true

   function StartLog
      local bSuccess
      bSuccess = true
      this.Reset()
      this.StartTime = time()
      if file(this.Name)
         try
            bSuccess = this.open(this.Name,"A")
         catch (exception e)
            bSuccess = false
         endtry
      else
         try
            bSuccess = this.create(this.Name)
         catch (exception e)
            bSuccess = false
         endtry
         if bSuccess
            this.PutMainHeader(true)
         endif
      endif
      this.Started = bSuccess
      return bSuccess

   function StopLog
      local nBytes
      try
         this.puts("")
         this.close()
      catch (exception e)
      endtry
      this.Started = false
      //D.Brooks 11/30/2004 - Prevent Program Alert when invalid path for log file
      try
      	nBytes = this.size(this.Name)
      catch (exception e)
         msgbox(e.message)
         return
      endtry
//      if nBytes > this.MaxBytes
//         this.ReWrite()
//      endif

      // QAID: 6383 - M.Kay, dBI Inc. - 03/06/2008
      this.ClearNotifiee()

      release object (this)

   function ReWrite
      local nOffset, buffer, cLine
      try
         this.open(this.Name,"R")

         //move MaxBytes back from end of file
         nOffSet = -1*this.MaxBytes
         this.seek(nOffSet,2)

         // move forward to next process section
         do
            cLine = this.gets()
         until left(cLine,10) = "==========" or this.eof()

         buffer = cLine+chr(13)+chr(10)
         buffer += this.read(this.MaxBytes)
         this.close()
         this.create(this.Name)
         this.PutMainHeader()
         this.puts("")
         this.write(buffer)
         this.puts("")
         buffer = null
      catch (exception e)
      endtry
      try
         this.close()
      catch (exception e)
      endtry
      return true

   function SetDefaultLog
      if empty(this.Project.ProjectFileName)
         store this.Project.getFullPath("Untitled.log") to this.Name,;
                                this.project.logFileName,;
                                this.Project.form.NOTEBOOK1.LOGFILENAME.value
      else
         store this.Project.getFullPath(this.Project.ProjectFileName+".log") to this.Name,;
                                this.project.logFileName,;
                                this.Project.form.NOTEBOOK1.LOGFILENAME.value
      endif
      this.Project.form.logFileName_onChange()
      return true

   // QAID: 6383 - M.Kay, dBI Inc. - 01/18/2008
   // Write cStr to log file and notify this.notifiee as well
   function putNotify( cStr )
      this.puts( cStr )
      this.notify( cStr )
      return

   function PutMainHeader( bForceIt )
      if this.Started or bForceIt
         //this.puts("dBASE PLUS "+this.Project.form.qLang.iGetText("Project")+" Explorer Log: "+this.Project.GetShortName(this.name))
         //this.puts(this.Project.form.qLang.iGetText("Project name")+": "+this.Project.projectname)

         // QAID: 6383 - M.Kay, dBI Inc. - 03/06/2008
         //this.notify()
         this.putNotify("dBASE PLUS "+this.Project.form.qLang.iGetText("Project")+" Explorer Log: "+this.Project.GetShortName(this.name))
         this.putNotify(this.Project.form.qLang.iGetText("Project name")+": "+this.Project.projectname)

      endif
      return true

   function PutProcessHeader( cProcess )
      if this.Started
//         this.puts('')
//         this.puts(replicate("=",65))
//         if this.Verbose
//         	this.puts(this.Project.form.qLang.iGetText("%1 process started: %2",upper(cProcess),dtoc(date())+" "+time()))
//         	this.puts(version(1))
//         	this.puts(this.Project.form.qLang.iGetText("Project")+" Explorer b"+PE_BUILD)
//         else
//            this.puts(upper(cProcess) + " " + dtoc(date()) + " " + time())
//         endif
//         this.puts(replicate("-",65))

         // QAID: 6383 - M.Kay, dBI Inc. - 03/06/2008
         //this.notify()
         this.putNotify('')
         this.putNotify(replicate("=",65))
         if this.Verbose
         	this.putNotify(this.Project.form.qLang.iGetText("%1 process started: %2",upper(cProcess),dtoc(date())+" "+time()))
         	this.putNotify(version(1))
         	this.putNotify(this.Project.form.qLang.iGetText("Project")+" Explorer b"+PE_BUILD)
         else
            this.putNotify(upper(cProcess) + " " + dtoc(date()) + " " + time())
         endif
         this.putNotify(replicate("-",65))

      endif
      return true

   function PutCompileStats(bCompileErr,cErrMsg)
      local f, cLine, bCompileSuccess, aBuffer, i
      bCompileSuccess = true
      try
         if this.Started
            if bCompileErr
//               this.puts(cErrMsg)
//               this.puts("")

               // QAID: 6383 - M.Kay - 03/06/2008
               this.putNotify(cErrMsg)
               this.putNotify("")
               this.CompileErrors++
            else
               aBuffer = new array()
               f = new file()
               f.open(this.CompileLog,"R")
               this.puts(this.Project.form.qLang.iGetText("Compile results for")+":")
               do while not f.eof()
                  cLine = f.gets()
                  if "Error in line" $ cLine
                     bCompileSuccess = false
                     this.CompileErrors++
                  endif
                  if not this.Verbose and left(cLine,15) == "Warning in line" or;
                     not this.Verbose and left(cLine,16) == "Warnung in Zeile"
                     // Delete the previous line
                     aBuffer.size := aBuffer.size-1
                  else
                     aBuffer.add(cLine)
                  endif
               enddo

               for i = 1 to aBuffer.size
                  // QAID: 6383 - M.Kay - 03/06/2008
                  //this.puts(aBuffer[i])
                  this.putNotify(aBuffer[i])
               next
               f.close()
            endif
         endif
         f.delete(this.CompileLog)
      catch (exception e)
      endtry
      return bCompileSuccess

   function PutCompletedFooter
      if this.Started and this.Verbose
         this.EndTime = time()
         //this.puts(this.Project.form.qLang.iGetText("Process completed at: %1 (%2 seconds elapsed)",time(),""+elapsed(this.EndTime,this.StartTime)))

         // QAID: 6383 - M.Kay, dBI Inc. - 03/06/2008
         //this.notify()
         this.putNotify(this.Project.form.qLang.iGetText("Process completed at: %1 (%2 seconds elapsed)",time(),""+elapsed(this.EndTime,this.StartTime)))
      endif
      return true

   function PutExeFooter
      if this.Started
         local f, nSize
         f = new file()
         nSize = f.size(this.Project.getFullPath(this.Project.exename))
//         this.puts(this.Project.form.qLang.iGetText("Executable file")+": "+this.Project.getFullPath(this.Project.exename))
//         this.puts(this.Project.form.qLang.iGetText("Executable file size")+": "+ltrim(transform(nSize,"999,999,999")))

         // QAID: 6383 - M.Kay, dBI Inc. - 03/06/2008
         //this.notify()
         this.putNotify(this.Project.form.qLang.iGetText("Executable file")+": "+this.Project.getFullPath(this.Project.exename))
         this.putNotify(this.Project.form.qLang.iGetText("Executable file size")+": "+ltrim(transform(nSize,"999,999,999")))
      endif
      return true

   function PutCompileFooter
      if this.Started
//         this.puts(this.Project.form.qLang.iGetText("File compiles attempted: %1",""+this.CompileCount))
//         this.puts(this.Project.form.qLang.iGetText("File compile errors: %1",""+this.CompileErrors))
//         this.puts(this.Project.form.qLang.iGetText("DEO files copied: %1",""+this.DEOCount))

         // QAID: 6383 - M.Kay, dBI Inc. - 03/06/2008
         //this.notify()
         this.putNotify(this.Project.form.qLang.iGetText("File compiles attempted: %1",""+this.CompileCount))
         this.putNotify(this.Project.form.qLang.iGetText("File compile errors: %1",""+this.CompileErrors))
         this.putNotify(this.Project.form.qLang.iGetText("DEO files copied: %1",""+this.DEOCount))


         // QAID: ? - M.Kay, dBI Inc. - 04/09/2008
         // Add line to show count of missing files (if any)
         if this.MissingFiles > 0
            this.putNotify(this.Project.form.qLang.iGetText("Missing Files: %1",""+this.MissingFiles))
         endif

      endif
      return true

   function PutIniFooter( cIniFileName )
      if this.Started
         //this.puts(this.Project.form.qLang.iGetText("%1 created or updated",cIniFileName)+".")

         // QAID: 6383 - M.Kay, dBI Inc. - 03/06/2008
         //this.notify()
         this.putNotify(this.Project.form.qLang.iGetText("%1 created or updated",cIniFileName)+".")
      endif
      return true

   function PutNotFound( cFileName )
      if this.Started
//         this.puts( cFilename + " "+this.Project.form.qLang.iGetText("not found")+"." )

         // QAID: 6383 - M.Kay, dBI Inc. - 03/06/2008
//         this.notify()
         this.putNotify( cFilename + " "+this.Project.form.qLang.iGetText("not found")+"." )
         this.putNotify('')
      endif
      return true

   function putDEOCopyFail( ObjFile, DEOName )
      if this.Started
//         this.puts(this.Project.form.qLang.iGetText("FAILED")+": "+ObjFile + " "+this.form.qLang.iGetText("copy to DEO Target")+": "+DEOName)
//         this.puts('')

         // QAID: 6383 - M.Kay, dBI Inc. - 03/06/2008
//         this.notify()
//         this.putNotify(this.Project.form.qLang.iGetText("FAILED")+": "+ObjFile + " "+this.form.qLang.iGetText("copy to DEO Target")+": "+DEOName)
         // M.Kay - 12/13/2010
         // Fixed coding error:
         //  Changed this.form... to this.Project.form
         this.putNotify(this.Project.form.qLang.iGetText("FAILED")+": "+ObjFile + " "+this.Project.form.qLang.iGetText("copy to DEO Target")+": "+DEOName)
         this.putNotify('')
      endif
      return true

   function putDEOCopySuccess( ObjFile, DEOName )
      if this.Started and this.Verbose
//         this.puts(ObjFile + " "+this.Project.form.qLang.iGetText("copied to DEO Target")+": "+DEOName)
//         this.puts('')

         // QAID: 6383 - M.Kay, dBI Inc. - 03/06/2008
//         this.notify()
         this.putNotify(ObjFile + " "+this.Project.form.qLang.iGetText("copied to DEO Target")+": "+DEOName)
         this.putNotify('')
      endif
      return true

   function PutBuildFailedFooter( cErrorMessage )
      if this.Started
//         this.puts(this.Project.form.qLang.iGetText("Build Failed!"))
//         this.puts(cErrorMessage)

         // QAID: 6383 - M.Kay, dBI Inc. - 03/06/2008
//         this.notify()
         // QAID: ? - M.Kay, dBI Inc. - 05/01/2008
         // Fixed text for Build Failed line
         this.putNotify("Build Failed!")
         this.putNotify('')
      endif
      return true

   function EmptyLog
      try
         this.Project.ReleaseLog()
         this.create( this.Name )
         this.PutMainHeader( true )
         this.close()
         this.Project.datalinkLog()
      catch (exception e)
         return false
      endtry
      return true

endclass


// QAID: 6383 - M.Kay, dBI Inc. - 03/06/2008
class ProjLogEditor( oParent ) of Editor( oParent )

   this.numLines = 0     // Current Number of lines displaying in editor during build or compile sequence
   this.maxLines = 0     // Maximum # of lines that will fit in current editor height

   function onNotify( cLog )
      // Called from projectLog notify method
      //  Reload Editor to update display and add new line of text (cLog) to display

      //? "In ProjLogEditor.onNotify()...datalink=>"+this.datalink+"<="
      if this.maxLines < 1
         this.init()
      endif

      if this.numLines >= this.maxLines
         // Remove a line each time an additional line is added
         this.value := substr(this.value, at(chr(13), this.value)+2 ) + chr(13)+chr(10)+cLog
      else
         // Add a line until editor.value has enough lines to fill current height
         this.value := this.value + chr(13)+chr(10)+cLog
         this.numLines++
         this.lineNo++
      endif

      return


   function init
      // Called first time onNotify() is called during a build or compile sequence
      // Used to set maxLines, clear numLInes, and turn off editor's scrollbars
      //   to speed up display and overall build sequence.
      local fontSizePoints

      // Calculate number of log lines that will fit in current editor height
      // so that we only keep the visible lines in this.value.  Otherwise, during
      // a lengthy build sequence, this.value will get very large and the editor
      // refresh becomes extemely slow.
      fontSizePoints = this.lineHeightPointsToPixels( this.fontSize )
      this.maxLines := int(this.height / fontSizePoints) + 1

      this.numLines := 0

      this.scrollbar := 0 // turn off scrollbar

      return


   function reinit
      // Called from projectLog.setNotifiee() to reset this editor to display log data
      // during a new build or compile sequence.  Clears maxLines so that init() will
      // be called the first time onNotify gets called.
      this.maxLines  := 0
      this.numLines  := 0
      this.scrollbar := 0 // turn off scrollbar

      return
       

   function lineHeightPointsToPixels( nFontSizePoints )
      // Converts nFontSizePoints (font size specified in points)
      // to logical pixels (assuming 96 pixels per inch)
      // Used by init() method to estimate the number of log lines that will
      //  fit in current editor height.
#define POINTSPERINCH  72
#define PIXELSPERINCH  96
      // add 3 to nFontSizePoints to adjust for ascenders and descenders of characters
      return ((nFontSizePoints+3)/POINTSPERINCH) * PIXELSPERINCH
      
// Partial code to obtain more accurate log line height in pixels
//      local hDC, nPixelsPerInchY, result
//
//#define LOGPIXELSY     90
//#define POINTSPERINCH  72
//      result = 0
//
//      if type("plGetDC") # "FP"
//		   extern CLONG plGetDC(CLONG) User32 From "GetDC"
//	   endif
//
//      if type("plGetDeviceCaps") # "FP"
//         extern CINT plGetDeviceCaps(CHANDLE, CINT) GDI32 from "GetDeviceCaps"
//      endif
//
//      if type("plGetWindowExtEx") # "FP"
//         extern BOOL plGetWindowExtEx(HDC, CPTR ) gdi32 from "GetWindowExtEx"
//      endif
//
//      if type("plGetViewportExtEx") # "FP"
//         extern BOOL plGetViewportExtEx(HDC, CPTR ) gdi32 from "GetViewportExtEx"
//      endif
//
//      hDC = plGetDC( this.hWnd )
//
//      if hDC <> 0
//         nPixelsPerInchY = plGetDeviceCaps( hDC, LOGPIXELSY )
//
//          
//         nLogUnitsY      = plGetWindowExtEx( hDC, 
//      endif
//
//
//      return result

// Sample method from dBASE Plus C++ source in contbut.cpp
//int CTextAttribute::FontHeightPointsToPixels(CDC *pdc, int points)
//{
//   const DoubleType POINTSPERINCH = 72.0;
//   DoubleType pixelsPerInchY = pdc->GetDeviceCaps(LOGPIXELSY);
//   DoubleType lusY = pdc->GetWindowExt().cy;   // logical units Y
//   DoubleType pusY = pdc->GetViewportExt().cy; // physical units Y
//   return fixd((points * pixelsPerInchY * lusY) / (POINTSPERINCH * pusY));
//}


endclass
// End QAID: 6383



// QAID: 6409 - M.Kay, dBI Inc. - 04/08/2008
// Function to disable controls on the InnoNotebook
//  Used for Personal dBASE
function DisableInnoNotebook( oNoteBook )

   private obj
   obj = oNoteBook.first // First control in z-order
   do
      if obj.className == "TEXT" or obj.className == "TEXTLABEL"
         obj.colorNormal := "graytext/BtnFace"
      else
         if type("obj.enabled") = "L"
            obj.enabled := false
         endif
      endif
      obj := obj.before           // Next control in z-order
   until obj == oNoteBook.first   // Until you get back to first control
   return



// Moved hourGlass() here from ProjExp.wfm due to having
//  Too many procedures error in ProjExp.wfm - 04/09/2008
//QAID: 5176 - D.Brooks - 09/23/2005
//---------------------------------Function Hourglass -----------------------
// Not assigned to Class, may be moved to a Utility Class with doMenu etc.
 //x.hourGlass(True, _app.FrameWin.hWND)  // Turn on hourglass mouse cursor

function hourGlass(lWait, hWND)

   If Type("dB_GetDesktopWindow") # "FP"
      extern CHANDLE dB_GetDesktopWindow() User32;
                  From "GetDesktopWindow"
   EndIf
   If Type("dB_LoadCursorByNum") # "FP"
      extern CHANDLE dB_LoadCursorByNum(CHANDLE, CLONG) User32;
                  From "LoadCursorA"
   EndIf
//      #define IDC_WAIT        32514
   If Type("dB_ReleaseCapture") # "FP"
      extern CLOGICAL dB_ReleaseCapture() User32;
                   From "ReleaseCapture"
   EndIf
   If Type("dB_SetCapture") # "FP"
      extern CHANDLE dB_SetCapture(CHANDLE) User32;
                  From "SetCapture"
   EndIf
   If Type("dB_SetCursor") # "FP"
      extern CHANDLE dB_SetCursor(CHANDLE) User32;
                  From "SetCursor"
   EndIf
   If lWait
#define IDC_WAIT     32514 // wait
      dB_SetCursor(dB_LoadCursorByNum(0, IDC_WAIT))
      dB_SetCapture(IIf(pCount() > 1, hWND, dB_GetDesktopWindow()))
      Return
   EndIf
   Clear Typeahead
   dB_ReleaseCapture()
//End QAID: 5176


/////////////////////////////////////////////////////////////////////////
//  Class FileEx
//
//  Started: 07/16/2013
//  Updated: 07/16/2013
//
//  Purpose: subclasses file class to add a geteol() method
//           to determine end of line characters used in file
//
/////////////////////////////////////////////////////////////////////////
class fileEx of file

   // Method: geteol()
   //    searches for standard end of line characters
   //    and returns string contains the ones found
   //    for use in calls to method gets()
   function geteol()
      local ceol, cBuf, nPos, eCharPos

      // save current pointer position
      nPos = this.position
            
      // read entire file into memory
      cBuf = this.read( this.size )

      i = 1
      do while i <= len(cbuf)
         testchr  = substr(cBuf,i,1)

         // check if null - how?
         //if testchar == chr(0)

         // check if other supported character
         //   use getbyte() 
         eCharPos = at( testchr, CHR(13)+CHR(10)+CHR(138)+CHR(141)+CHR(255) )
      enddo

      // restore pointer position
      this.seek( nPos )

      return ceol


endclass

