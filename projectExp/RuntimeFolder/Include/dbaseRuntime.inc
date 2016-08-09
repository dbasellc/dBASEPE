#define issiError 0
#if 'true' == SM_dBASERuntime
  #ifndef dBSeparat
     #define issiErrorFunction "Missing: dBSeparat"
     #define issiError 1
  #endif
  #ifndef dBPlus
     #define issiErrorFunction "Missing: dBPlus"
     #define issiError 1
  #endif
  #ifndef dBASELang
     #define issiErrorFunction "Missing: dBASELang"
     #define issiError 1
  #elif "" == dBaseLang
      #define dBaseLang "All"
  #endif
  #ifndef dBaseRuntimeDir
     #define issiErrorFunction "Missing: dBaseRuntimeDir"
     #define issiError 1
  #endif
   #ifndef dBaseRuntimeInstDir
     #define issiErrorFunction "Missing: dBaseRuntimeInstDir"
     #define issiError 1
  #endif
  #ifndef dBaseBuild
     #define issiErrorFunction "Missing: dBaseBuild"
     #define issiError 1
  #endif
    #ifndef dBaseRuntimeInstVer
     #define issiErrorFunction "Missing: dBaseRuntimeInstVer"
     #define issiError 1
  #endif
  #ifndef BDEGroupEntries
     #define issiErrorFunction "Missing: BDEGroupEntries"
     #define issiError 1
  #endif
  #ifndef BDEVersion
     #define BDEVersion "5202"
  #endif
  #Ifndef SepDestDir
     #define SepDestDir "{app}"
  #endif
  #ifndef dBDestName
     #define dBDestName "dBASERuntime.exe"
  #endif
  #ifexist dBaseRuntimeDir
     #define dBaseEngineName dBaseRuntimeDir
  #endif
  #ifndef dBUninstall
     #define dBUninstall "true"
  #endif 
  #ifndef dbVista
     #define dbVista ""
  #endif
  #ifndef DBuac
    #define DBuac "1"
  #endif
[Files]  
  #if 'true' == dBPlus 
      #if 'false' == dBSeparat
; ### Changed 03/25/2015 - KK - if dBSeparat is false user is installing using our runtime installer
;     This is now handled completely differently
      
;         #ifdef dBaseEngineName
;Source: {#dBaseEngineName}; DestName: {#dBDestName}; DestDir: {tmp}; Flags: DontCopy
;         #else
;            #define dbPlusName "dBasePlusRuntimeEngine"
;            #if Int(dBaseBuild)>1800
;                #define dbPlusName "PlusRuntime-b"
;            #endif
;            #if POS('.exe',dBaseRuntimeInstDir)
;Source: {#dBaseRuntimeInstDir}; DestName: {#dBDestName}; DestDir: {tmp}; Flags: DontCopy
;            #else
;               #if 'All' == dBASELang
;Source: {#dBaseRuntimeInstDir}\{#dbPlusName}{#dBaseBuild}.exe; DestName: {#dBDestName}; DestDir: {tmp}; Flags: DontCopy
;               #else
;                  #if Pos('EN',dBASELang)>0
;Source: {#dBaseRuntimeInstDir}\{#dbPlusName}{#dBaseBuild}_en.exe; DestName: {#dBDestName}; DestDir: {tmp}; Flags: DontCopy
;                  #elif Pos('DE',dBASELang)>0
;Source: {#dBaseRuntimeInstDir}\{#dbPlusName}{#dBaseBuild}_de.exe; DestName: {#dBDestName}; DestDir: {tmp}; Flags: DontCopy
;                  #elif Pos('ES',dBASELang)>0
;Source: {#dBaseRuntimeInstDir}\{#dbPlusName}{#dBaseBuild}_es.exe; DestName: {#dBDestName}; DestDir: {tmp}; Flags: DontCopy
;                  #elif Pos('IT',dBASELang)>0
;Source: {#dBaseRuntimeInstDir}\{#dbPlusName}{#dBaseBuild}_it.exe; DestName: {#dBDestName}; DestDir: {tmp}; Flags: DontCopy
;                  #elif Pos('JA',dBASELang)>0
;Source: {#dBaseRuntimeInstDir}\{#dbPlusName}{#dBaseBuild}_ja.exe; DestName: {#dBDestName}; DestDir: {tmp}; Flags: DontCopy
;                  #endif 
;                #endif
;            #endif
;         #endif           
      #else
Source: {#dBaseRuntimeDir}\PLUSrun.exe; DestDir: {reg:HKLM\SOFTWARE\dBASE\PLUS\series10,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
         #if 'All' == dBASELang
             #define dBASELang "EN DE ES IT JA"
         #endif
         #if Pos('EN',dBASELang)>0
Source: {#dBaseRuntimeDir}\PLUSr_en.dll; DestDir: {reg:HKLM\SOFTWARE\dBASE\PLUS\series10,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
         #endif
         #if Pos('DE',dBASELang)>0
Source: {#dBaseRuntimeDir}\PLUSr_de.dll; DestDir: {reg:HKLM\SOFTWARE\dBASE\PLUS\series10,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
         #endif
         #if Pos('ES',dBASELang)>0
Source: {#dBaseRuntimeDir}\PLUSr_es.dll; DestDir: {reg:HKLM\SOFTWARE\dBASE\PLUS\series10,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
         #endif
         #if Pos('IT',dBASELang)>0
Source: {#dBaseRuntimeDir}\PLUSr_it.dll; DestDir: {reg:HKLM\SOFTWARE\dBASE\PLUS\series10,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
         #endif
         #if Pos('JA',dBASELang)>0
Source: {#dBaseRuntimeDir}\PLUSr_ja.dll; DestDir: {reg:HKLM\SOFTWARE\dBASE\PLUS\series10,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
         #endif
         #if 'true' == Resource
Source: {#dBaseRuntimeDir}\Resource.dll; DestDir: {reg:HKLM\SOFTWARE\dBASE\PLUS\series10,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
         #endif
         #define dBEFolder ReadReg(HKEY_LOCAL_MACHINE, "SOFTWARE\dBASE\PLUS\series10", "RuntimePath")
         #ifdef dbVista
           #if 'Admin' == dbVista
Source: {#dBEFolder}\PlusVistaAdmin.mf; DestName: PlusRun.exe.manifest; DestDir: {reg:HKLM\SOFTWARE\dBASE\PLUS\series10,RuntimePath|{#SepDestDir}}; Flags: SkipIfSourceDoesntExist IgnoreVersion; MinVersion: 0,6
Source: {#dBEFolder}\Include\PlusVistaAdmin.mf; DestName: PlusRun.exe.manifest; DestDir: {reg:HKLM\SOFTWARE\dBASE\PLUS\series10,RuntimePath|{#SepDestDir}}; Flags: SkipIfSourceDoesntExist IgnoreVersion; MinVersion: 0,6
           #endif
           #if 'Invoker' == dbVista
Source: {#dBEFolder}\PlusVistaInvoker.mf; DestName: PlusRun.exe.manifest; DestDir: {reg:HKLM\SOFTWARE\dBASE\PLUS\series10,RuntimePath|{#SepDestDir}}; Flags: SkipIfSourceDoesntExist IgnoreVersion; MinVersion: 0,6
Source: {#dBEFolder}\Include\PlusVistaInvoker.mf; DestName: PlusRun.exe.manifest; DestDir: {reg:HKLM\SOFTWARE\dBASE\PLUS\series10,RuntimePath|{#SepDestDir}}; Flags: SkipIfSourceDoesntExist IgnoreVersion; MinVersion: 0,6
           #endif
           #if 'Highest' == dbVista
Source: {#dBEFolder}\PlusVistaHighest.mf; DestName: PlusRun.exe.manifest; DestDir: {reg:HKLM\SOFTWARE\dBASE\PLUS\series10,RuntimePath|{#SepDestDir}}; Flags: SkipIfSourceDoesntExist IgnoreVersion; MinVersion: 0,6
Source: {#dBEFolder}\Include\PlusVistaHighest.mf; DestName: PlusRun.exe.manifest; DestDir: {reg:HKLM\SOFTWARE\dBASE\PLUS\series10,RuntimePath|{#SepDestDir}}; Flags: SkipIfSourceDoesntExist IgnoreVersion; MinVersion: 0,6
           #endif
        #endif
Source: {#dBEFolder}\PlusXP.mf; DestName: PlusRun.exe.manifest; DestDir: {reg:HKLM\SOFTWARE\dBASE\PLUS\series10,RuntimePath|{#SepDestDir}}; Flags: SkipIfSourceDoesntExist IgnoreVersion; MinVersion: 0,5.01.2600; OnlyBelowVersion: 0,6
Source: {#dBEFolder}\Include\PlusXP.mf; DestName: PlusRun.exe.manifest; DestDir: {reg:HKLM\SOFTWARE\dBASE\PLUS\series10,RuntimePath|{#SepDestDir}}; Flags: SkipIfSourceDoesntExist IgnoreVersion; MinVersion: 0,5.01.2600; OnlyBelowVersion: 0,6
      #endif
  #else
      #if 'false' == dBSeparat
         #ifdef dBaseEngineName
Source: {#dBaseEngineName}; DestName: {#dBDestName}; DestDir: {tmp}; Flags: DontCopy
         #else
            #if 'All' == DBASELang
Source: {#dBaseRuntimeDir}\db2kruntimeengine{#dBaseBuild}.exe; DestName: {#dBDestName}; DestDir: {tmp}; Flags: DontCopy
            #else
                #if Pos('EN',dBASELang)>0
Source: {#dBaseRuntimeDir}\db2kruntimeengine{#dBaseBuild}_en.exe; DestName: {#dBDestName}; DestDir: {tmp}; Flags: DontCopy
                #elif Pos('DE',dBASELang)>0
Source: {#dBaseRuntimeDir}\db2kruntimeengine{#dBaseBuild}_de.exe; DestName: {#dBDestName}; DestDir: {tmp}; Flags: DontCopy
                #elif Pos('ES',dBASELang)>0
Source: {#dBaseRuntimeDir}\db2kruntimeengine{#dBaseBuild}_es.exe; DestName: {#dBDestName}; DestDir: {tmp}; Flags: DontCopy
                #elif Pos('IT',dBASELang)>0
Source: {#dBaseRuntimeDir}\db2kruntimeengine{#dBaseBuild}_it.exe; DestName: {#dBDestName}; DestDir: {tmp}; Flags: DontCopy
                #elif Pos('JA',dBASELang)>0
Source: {#dBaseRuntimeDir}\db2kruntimeengine{#dBaseBuild}_ja.exe; DestName: {#dBDestName}; DestDir: {tmp}; Flags: DontCopy
                #endif
            #endif
         #endif
      #endif
      #if 'true' == dBSeparat
Source: {#dBaseRuntimeDir}\db2krun.exe; DestDir: {reg:HKLM\SOFTWARE\dBASE\dB2K\0.1,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
         #if 'All' == DBASELang
             #define DBASELang "EN DE IT ES JA"
         #endif
         #if Pos('EN', dBASELang)>0
Source: {#dBaseRuntimeDir}\db2k_en.dll; DestDir: {reg:HKLM\SOFTWARE\dBASE\dB2K\0.1,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
Source: {#dBaseRuntimeDir}\db2kr_en.dll; DestDir: {reg:HKLM\SOFTWARE\dBASE\dB2K\0.1,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
         #endif
         #if Pos('DE',dBASELang)>0
Source: {#dBaseRuntimeDir}\db2k_de.dll; DestDir: {reg:HKLM\SOFTWARE\dBASE\dB2K\0.1,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
Source: {#dBaseRuntimeDir}\db2kr_de.dll; DestDir: {reg:HKLM\SOFTWARE\dBASE\dB2K\0.1,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
         #endif
         #if Pos('ES',dBASELang)>0
 Source: {#dBaseRuntimeDir}\db2k_es.dll; DestDir: {reg:HKLM\SOFTWARE\dBASE\dB2K\0.1,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
 Source: {#dBaseRuntimeDir}\db2kr_es.dll; DestDir: {reg:HKLM\SOFTWARE\dBASE\dB2K\0.1,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
         #endif
         #if Pos('IT',dBASELang)>0
Source: {#dBaseRuntimeDir}\db2k_it.dll; DestDir: {reg:HKLM\SOFTWARE\dBASE\dB2K\0.1,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
Source: {#dBaseRuntimeDir}\db2kr_it.dll; DestDir: {reg:HKLM\SOFTWARE\dBASE\dB2K\0.1,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
         #endif
         #if Pos('JA',dBASELang)>0
Source: {#dBaseRuntimeDir}\db2k_ja.dll; DestDir: {reg:HKLM\SOFTWARE\dBASE\dB2K\0.1,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
Source: {#dBaseRuntimeDir}\db2kr_ja.dll; DestDir: {reg:HKLM\SOFTWARE\dBASE\dB2K\0.1,RuntimePath|{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
         #endif
         #if 'true' == Resource
Source: {#dBaseRuntimeDir}\Resource.dll; DestDir: {reg:HKLM\SOFTWARE\dBASE\dB2K\0.1,RuntimePath|{{#SepDestDir}}; Flags: IgnoreVersion uninsneveruninstall
         #endif
      #endif
  #endif
#endif
#if 'True' == UninsHs
  #ifndef UninsHsExeDir
    #if FileExists(dBaseRuntimeDir)
     #define UninsHsExeDir ExtractFilePath(dBaseRuntimeDir)+"\Include\Utilities\UninsHS"
    #endif
  #endif
Source: {#UninsHsExeDir}\UninsHs.exe; DestDir: {#AppUninstallDir}; DestName: UninsHs.exe; Flags: IgnoreVersion; Check: UninsHsCheckFile
  #ifdef AppName
     #define UninsHSAppName AppName
  #endif
  #ifndef AppID
     #define AppID AppName+"_ID"
  #endif
  #define UninsHSAppID AppID
  
[INI]
Filename: {#AppUninstallDir}\UninsHs.dat; Section: Common; Key: Software; String: {#UninsHSAppName}
Filename: {#AppUninstallDir}\UninsHs.dat; Section: Common; Key: Language; String: {language}
Filename: {#AppUninstallDir}\UninsHs.dat; Section: Common; Key: Install; String: {srcexe}
Filename: {#AppUninstallDir}\UninsHs.dat; Section: Common; Key: Remove; String: {uninstallexe}
Filename: {#AppUninstallDir}\UninsHs.dat; Section: Common; Key: group; String: {groupname}
Filename: {#AppUninstallDir}\UninsHs.dat; Section: Common; Key: Components; String: {code:ComponentList|x}
  #if Pos('{', UninsHSAppId)>0
Filename: {#AppUninstallDir}\UninsHs.dat; Section: Common; Key: Uninstall; String: {{#UninsHSAppId}
  #else
Filename: {#AppUninstallDir}\UninsHs.dat; Section: Common; Key: Uninstall; String: {#UninsHSAppId}
  #endif
Filename: {#AppUninstallDir}\UninsHs.dat; Section: Common; Key: Dir; String: {app}

[Icons]
  #if VER >= 0x05000000
Name: {group}\Uninstall Information\{cm:UninstallProgram,{#UninsHSAppName}}; Filename: {#AppUninstallDir}\UninsHs.exe; Parameters: /u0={#UninsHSAppName}
  #else
Name: {group}\Uninstall Information\{cm:UninstallProgram,{#UninsHSAppName}}; Filename: {#AppUninstallDir}\UninsHs.exe;
  #endif

[UninstallDelete]
Type: filesandordirs; Name: {#AppUninstallDir}\UninsHs.dat
  
[Run]
  #if Pos('{', UninsHSAppName)>0
Filename: {#AppUninstallDir}\UninsHs.exe; Parameters: /r={{#UninsHSAppName}; Flags: nowait runhidden runminimized skipifdoesntexist
  #else
Filename: {#AppUninstallDir}\UninsHs.exe; Parameters: /r={#UninsHSAppName}; Flags: nowait runhidden runminimized skipifdoesntexist
  #endif
#endif

#if 'true' == BDEGroupEntries
[Icons]
Name: {group}\BDE\BDE Administrator; Filename: {reg:HKLM\SOFTWARE\Borland\BLW32,BLAPIPATH}\BDEADMIN.EXE; WorkingDir: {app}
Name: {group}\BDE\Help SQL (BDE); Filename: {reg:HKLM\SOFTWARE\Borland\BLW32,BLAPIPATH}\LOCALSQL.HLP; WorkingDir: {app}; Comment: Help BDE SQL
Name: {group}\BDE\Help API (BDE); Filename: {reg:HKLM\SOFTWARE\Borland\BLW32,BLAPIPATH}\BDE32.HLP; WorkingDir: {app}; Comment: Help BDE API
#endif

#if 'false' == dBSeparat
#else
  #if '{app}' == SepDestDir
  #else
[Registry]
    #if 'true' == dBPlus
Root: HKLM; Subkey: SOFTWARE\dBASE\PLUS\series10; ValueType: string; ValueName: RuntimePath; ValueData: {#SepDestDir}; Flags: uninsdeletekeyifempty
    #else
Root: HKLM; Subkey: SOFTWARE\dBASE\dB2K\0.1; ValueType: string; ValueName: RuntimePath; ValueData: {#SepDestDir}; Flags: uninsdeletekeyifempty
    #endif
  #endif
#endif

#dim ErrorHandlingdb[13]
#define ErrorHandlingdb[0]  "Undefined value"
#define ErrorHandlingdb[1]  "Invalid number of defines"
#define ErrorHandlingdb[2]  "Invalid number of characters"
#define ErrorHandlingdb[3]  "File not found error"
#define ErrorHandlingdb[4]  "Option not implemented"
#define ErrorHandlingdb[5]  "Inno Setup out of date"
#define ErrorHandlingdb[6]  "ISSI Language file is out of date"
#define ErrorHandlingdb[7]  "Define cannot be empty"
#define ErrorHandlingdb[8]  "Illegal define value"
#define ErrorHandlingdb[9]  "Language not supported by Inno Setup"
#define ErrorHandlingdb[10] "Language not supported by ISSI"
#define ErrorHandlingdb[11] "An unknown error has occured"
#define ErrorHandlingdb[12] "Missing required defined directive"
#if issiError !=0    
    #define issiErrorMessage "dBase Include Error: "+str(issiError)+" : "+ErrorHandlingdb[issiError-1]
    #if len(issiErrorFunction)>0
        #define issiErrorMessage issiErrorMessage+" "+issiErrorFunction
    #endif
  #pragma error issiErrorMessage
#endif

[Code]
#ifdef UninsHs
{***** Block UninsHS Start}
function UninsHsCheckFile : Boolean;
begin
  Result:= not (Pos('/SP-', UpperCase(GetCmdTail))>0);
end;

#if VER >= 0x05000000
function ShouldSkipPage(PageID: Integer): Boolean;
#else
function SkipCurPage(CurPage: Integer): Boolean;
#endif
var I:Integer;
begin
  Result := false;
  if (Pos('/SP-', UpperCase(GetCmdTail))>0) then
    #if VER >= 0x05000000
    case PageID of
    #else
    case CurPage of
    #endif
      wpWelcome, wpLicense, wpPassword, wpInfoBefore, wpUserInfo,
      wpSelectDir, wpSelectProgramGroup, wpInfoAfter: Result:=True;
    end
    #if VER >= 0x05000000
      #ifdef CustomPageDataDir
    else
      if GetPreviousData('DataDir','')<>''
      then Result := (PageID=DataDirPage.ID);
      #endif
    #endif
end;

function ComponentList(Default: String): String;
begin
  Result:= WizardSelectedComponents(False);
end;
{***** Block UninsHS end}
#endif

#if 'true' == SM_dBASERuntime
#if 'true' == dBSeparat
function CalldBASERuntimeSetup(): Boolean;
begin
  Result:=true;
end;
#endif
#if 'false' == dBSeparat
const dBRunFilename='{#dBDestName}';
      WM_LBUTTONDOWN = 513;
      WM_LBUTTONUP   = 514;

var sErneuern,AllEnd: Boolean;
    ErrorCode,CallCounter: Integer;
    dBParameters,dBRuntimeUninstall,dBAppIsPath,
    sVersion,sVerCheck,s2,
    cDispText,cDispText1,cDispText2,cdbParam: String;
    dBaseButton: TButton;

function BDEVersCheck(): Boolean;
var cFileName: String;
begin
  sErneuern := true;
  sVerCheck := '{#BDEVersion}';
  cFileName := ExpandConstant('{reg:HKLM\Software\Borland\BLW32,BLAPIPATH}')+'\BDEADMIN.exe';
  if FileExists(cFilename)
  then sErneuern := false;

  if not sErneuern then
  begin
     GetVersionNumbersString(ExpandConstant('{reg:HKLM\Software\Borland\BLW32,BLAPIPATH}')+'\BDEADMIN.exe',sVersion)
     s2:=sVersion;
     StringChange(S2,'.','');
     if StrToInt(sVerCheck)>StrToInt(s2) then sErneuern:=true;
  end;
  Result := sErneuern;

  if  Result
  then Wizardform.READYLABEL.Caption:= Wizardform.READYLABEL.Caption+''+#13#13+cDispText1
  else Wizardform.READYLABEL.Caption:= Wizardform.READYLABEL.Caption+''+#13#13'BDE OK!';

  Wizardform.READYLABEL.Repaint;
end;

function dBRuntimeCheck(): Boolean;
begin
  sErneuern := true;
  sVerCheck:='{#dBaseVersion}';
  #if 'true' == dBPlus
  RegQueryStringValue(HKLM,'SOFTWARE\dBASE\PLUS\series10','RuntimePath',dBAppIsPath);
  if dBAppIsPath='' then
  begin
    RegQueryStringValue(HKLM,'SOFTWARE\dBASE\PLUS\series10','RootDir',dBAppIsPath);
    if dBAppIsPath<>'' then
    begin
      dBAppIsPath:=dBAppIsPath+'\Runtime';
      RegWriteStringValue(HKLM,'SOFTWARE\dBASE\PLUS\series10','RuntimePath',dBAppIsPath);
    end;
  end;
  if FileExists(dBAppIsPath+'\PLUSrun.exe')
  then sErneuern := false;
  #else
  RegQueryStringValue(HKLM,'SOFTWARE\dBASE\dB2K\0.1','RuntimePath',dBAppIsPath);
  if dBAppIsPath = '' then
  begin
    RegQueryStringValue(HKLM,'SOFTWARE\dBASE\dB2K\0.1','RootDir',dBAppIsPath);
    if dBAppIsPath <> ''
    then dBAppIsPath := dBAppIsPath+'\bin';
  end;
  if FileExists(dBAppIsPath+'\db2kRun.exe')
  then sErneuern := false;
  #endif
  if not sErneuern then
  begin
     #if 'true' == dBPlus
       GetVersionNumbersString(dBAppIsPath+'\PLUSrun.exe',sVersion);
     #else
       GetVersionNumbersString(dBAppIsPath+'\db2krun.exe',sVersion);
     #endif
     s2 := sVersion;
     StringChange(s2,'.','');
     if StrToInt(sVerCheck)>StrToInt(s2)
     then sErneuern := true;
  end;
  Result := sErneuern;

  if Result then
  begin
     Wizardform.READYLABEL.Caption := Wizardform.READYLABEL.Caption+''+#13#13+cDispText2;
     Sleep(100);
  end
  else Wizardform.READYLABEL.Caption:= Wizardform.READYLABEL.Caption+''+#13#13+'dBase Runtime OK!';
  Wizardform.READYLABEL.Repaint;  
end;

Procedure CheckFolder(cPara:String; const cWhat:String);
begin
  {Check/Create the destination folder of BDE/Runtime files if silent included it must be there.....}
  if Pos(cWhat,lowercase(cPara))>0 then
  begin
     cPara:=copy(cPara,Pos(cWhat,cPara)+length(cWhat),length(cPara));
     cPara:=AddBackslash(RemoveQuotes(cPara));
     if not DirExists(cPara)
     then ForceDirectories(cPara);
  end;
end;

Procedure OldParameterSet;
var Para1,Para2,Para3,Para4,Para5,cParaAdd,ParaReg: String;
begin
  cParaAdd:=' ';
  Para1:='{#dBProgressbar}';
  Para2:='{#dBProgressbox}';
  Para3:='{#dBSilent}';
  if (lowercase(Para3)='true') or
     (lowercase(Para3)='silent')
  then
  begin
     Para1:='-s';
     para2:='-sr';
     para3:='-s';
  end
  else Para3:='';
  
  Para4:=ExpandConstant('{#dBDestRuntime}');       
  Para5:=ExpandConstant('{#dBDestBDE}');

  if Para4<>'' then
  begin
     if RegQueryStringValue(HKLM,'Software\dBASE\PLUS\series10','RuntimePath',ParaReg)
     then Para4:=ParaReg;
     Para4:='r="'+Para4+'"';
  end;

  if Para5<>'' then
  begin
     if RegQueryStringValue(HKLM,'Software\Borland\BLW32','BLAPIPATH',ParaReg)
     then Para5:=ParaReg;
     Para5:='b="'+Para5+'"';
  end;

  if (Para2<>'') or (Para3<>'') or (Para4<>'') or (Para5<>'')
  then cParaAdd:=' -a ';

  if (Para1='') and (Para2='') and (Para3='') and (Para4='') and (Para5='')
  then dBParameters:=''
  else dBParameters:=Para1+cParaAdd+Para4+' '+Para5+' '+Para3+' '+Para2;

  if Para3<>'' then
  begin
     CheckFolder(Para4,'r=');
     CheckFolder(Para5,'b=');
  end;
end;

Procedure NewParameterSet;
var Para1,Para2,Para3,Para4,Para5,cParaAdd,ParaReg : String;
begin
  cParaAdd:=' ';
  Para1:='{#dBProgressbar}';
  Para2:='{#dBProgressbox}';

  Para3:='{#dBSilent}';
  
  if (lowercase(Para3)='true')  or
     (lowercase(Para3)='silent')
  then
  begin
     if StrToInt('{#dBaseRuntimeInstVer}') > 2200 //for release 2.8 and higher - new installer built from SetupBuilder (Lindersoft)
	 then
	 begin
	    Para1:='';
		Para2:='';
		Para3:='/S';
	 end
	 if StrToInt('{#dBaseRuntimeInstVer}') < 2200 //for release 2.7 and Lower
	 then
	 begin
     Para1:='s';
     para2:='-sr';
     para3:='-s'; 
	 end
  end
  else Para3:='';

  Para4:=ExpandConstant('{#dBDestRuntime}');       
  Para5:=ExpandConstant('{#dBDestBDE}');

  if Para4='' then 
  begin
     if RegQueryStringValue(HKLM,'Software\dBASE\PLUS\series10','RuntimePath',ParaReg)
     then Para4:=ParaReg;
  end;

  if Para5='' then  
  begin
     if RegQueryStringValue(HKLM,'Software\Borland\BLW32','BLAPIPATH',ParaReg)
     then Para5:=ParaReg;
  end;

  if Para4<>'' then  
  begin
	 if StrToInt('{#dBaseRuntimeInstVer}') > 2200
	 then Para4:='/X "'+Para4+'"';
	 
	 if StrToInt('{#dBaseRuntimeInstVer}') < 2200
	 then Para4:='r="'+Para4+'"'; 
  end;
  
  if Para5<>'' then  
  begin
     if StrToInt('{#dBaseRuntimeInstVer}') > 2200
	 then Para5:='/V COMMONBDEDIR "'+Para5+'"';

	 if StrToInt('{#dBaseRuntimeInstVer}') < 2200
	 then Para5:='b="'+Para5+'"';
  end;
  
  if StrToInt('{#dBaseRuntimeInstVer}') < 2200
  then
  begin
	  if (Para2<>'') or (Para3<>'') or (Para4<>'') or (Para5<>'')
	  then cParaAdd:='-a ';
	  
	  if (Para1='') and (Para2='') and (Para3='') and (Para4='') and (Para5='')
	  then dBParameters:=''
	  else
      begin
		  if (Para3<>'') or (Para4<>'') or (Para5<>'')  
		  then dBParameters:=Para3+' '+cParaAdd+Para4+' '+Para5+' '+Para1+' '+Para2
		  else dBParameters:=Para1+' '+Para2;
		  
		  if (Para3<>'') or (Para4<>'') or (Para5<>'') then
		  begin
			 CheckFolder(Para4,'r=');
			 CheckFolder(Para5,'b=');
		  end;		  
	   end;
   end; //dBaseRuntimeInstVer < 2200
   
  if StrToInt('{#dBaseRuntimeInstVer}') > 2200
  then dBParameters:=Para3+' '+Para4+' '+Para5;
  
end; //procedure

function dBCallInstall(): Boolean;
var
  nBack,nHandle: Integer;
  cText1,cText2,cVers: String;
begin
    Result:=true;    
    if CallCounter<2 then
    begin
       CallCounter := 2;
       {check for stop if dbase is running}
       nHandle := FindWindowByWindowName('dBase Plus');
       if nHandle>0 then
       begin
          cText1 := 'Setup has detected that dBase is running...';
          cText2 := 'Please close all instances of dBase before continue.';

          if lowercase(Activelanguage)='de' then
          begin
             cText1 := 'Setup hat entdeckt das dBase gestartet ist...';
             cText2 := 'Bitte schliessen Sie alle Instanzen von dBase um fortzusetzen.';
          end
          else

          if lowercase(Activelanguage)='it' then
          begin
             cText1 := 'Il setup ha rilevato che dBase è in esecuzione...';
             cText2 := 'Per favore, chiudere tutte le istanze di dBase prima di continuare.';
          end;
          
          while nHandle > 0 do
          begin
            nBack := msgbox(ctext1+#13#13+cText2,mbConfirmation,MB_YESNOCANCEL);

            if (nBack=IDYES) or 
               (nBack=IDNo)
            then nHandle:=FindWindowByWindowName('dBase Plus')
            else

            if nBack = IDCANCEL then
            begin
               nHandle := 0; {break out of while loop}
               cText1 := 'Required missing dBase Runtime files not installed!'#13#13'Canceled by user.';
               if lowercase(Activelanguage())='de'
               then cText1 := 'Unbedingt benötigte fehlende Runtime Dateien nicht installiert!'#13#13'Abbruch durch Benutzer';
               Wizardform.READYLABEL.Caption := cText1;
               WizardForm.Repaint;     
               WizardForm.BackButton.visible := false;
               Result := false;
               Exit;
            end;
          end;      
       end;
       
       if StrToInt('{#dBaseBuild}') > 1800
       then NewParameterSet
       else OldParameterSet;
       
       (** Now we call the needed UAC registry Key before Installing the Runtime Files.... **)
       RegQueryStringValue(HKLM,'Software\dbase\plus\series10','useUACPaths',cVers);
       if cVers <> 'Y' then
       begin
         cVers := GetWindowsVersionString;
         if cVers[1] = '6'
         then RegWriteStringValue(HKLM,'Software\dbase\plus\series10','useUACPaths','Y');
       end;
       
       #if VER >= 0x05000100
       if not Exec(ExpandConstant('{tmp}\'+dBRunFilename),dBParameters,ExpandConstant('{tmp}'),SW_ShowNormal,ewWaitUntilTerminated,ErrorCode) then
       begin
       #else
       if not InstExec(ExpandConstant('{tmp}\'+dBRunFilename),dBParameters,'',true,false,SW_SHOWNORMAL,ErrorCode) then
       begin
       #endif        
          if lowercase(Activelanguage)='it'
          then MsgBox('Il file: {#dBDestName} non può essere eseguito!'+SysErrorMessage(ErrorCode)+'.',mbError,MB_OK)
          else
          if lowercase(Activelanguage)='de'
          then MsgBox('Die Datei: {#dBDestName} konnte nicht gestartet werden!'+SysErrorMessage(ErrorCode)+'.',mbError,MB_OK)
          else MsgBox('The file: {#dBDestName} can''t be started!'+SysErrorMessage(ErrorCode)+'.',mbError,MB_OK);
          Result := false;
          Exit;
       end;
       #if 'true' == dBPlus
          RegQueryStringValue(HKLM,'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\dBASE PLUS series10 Runtime Engine','UninstallString',dBRuntimeUninstall);
          if dBRuntimeUninstall=''
          then RegQueryStringValue(HKLM,'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\dBASE PLUS series10','UninstallString',dBRuntimeUninstall);
       #else
          RegQueryStringValue(HKLM,'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\dB2K 0.1 Runtime Engine','UninstallString',dBRuntimeUninstall);
          if dBRuntimeUninstall=''
          then RegQueryStringValue(HKLM,'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\dB2K 0.1','UninstallString',dBRuntimeUninstall);
       #endif
       if dBRuntimeUninstall<>'' then
       begin
          Result:=true;
          WizardForm.Visible := true;
          BringToFrontAndRestore;       
       end
       else

       begin
          if lowercase(Activelanguage)='de'
          then cText1 := 'Kann die Anwendung nicht installieren, Abbruch durch Benutzer'
          else

          if lowercase(Activelanguage)='it'
          then cText1:='Spiacente, impossbile installare questo setup perchè annullato dall''utente'
          else cText1:='Sorry can''t install this setup cancelled by user';

          msgbox(cText1,mbInformation,mb_OK);
          Result := False;
       end;
    end;
end;

procedure dBaseButtonOnClick(Sender: TObject);
var
  bBool: Boolean;
  #ifdef dBMainFile
    cVers: String;
    cMainFile: String;
  #endif
begin
  AllEnd:=false;
  if lowercase(Activelanguage)='de' then
  begin
     cDispText  := 'Überprüfe unbedingt benötigte dBASE Runtime Dateien...'#13#13'Hinweis: Dies kann einige Sekunden dauern...'
     cDispText1 := 'Die BDE muss erneuert werden!';
     cDispText2 := 'Die dBase Runtime Dateien müssen erneuert werden!';
  end
  else

  if lowercase(Activelanguage)='it' then
  begin
     cDispText  := 'Verifica dei files richiesti del Runtime dBASE...'#13#13'Sono necessari alcuni secondi...';
     cDispText1 := 'The BDE must be updated!';
     cDispText2 := 'The dBase Runtime files must be updated!';
  end
  else

  begin
     cDispText  := 'Check for required dBASE Runtime files...'#13#13'Info: This can take several seconds...';
     cDispText1 := 'The BDE must be updated!';
     cDispText2 := 'The dBase Runtime files must be updated!';
  end;
  
  dBaseButton.Enabled := False;
  Wizardform.READYLABEL.Caption := cDispText;
  Wizardform.ReadyMemo.visible := False;
  Wizardform.PAGENAMELABEL.Caption:='dBASE PLUS Runtime';
  Wizardform.PAGEDESCRIPTIONLABEL.Caption:='dBASE PLUS Runtime Engine check';
  WizardForm.Repaint;     
  WizardForm.BackButton.visible := False;
  WizardForm.CancelButton.visible := False;

  CallCounter := 2;

  if BDEVersCheck() then
  begin
     ExtractTemporaryFile(dBRunFilename);
     CallCounter:=1;
  end;

  if CallCounter = 2 then
  if dBRuntimeCheck() then
  begin
    ExtractTemporaryFile(dBRunFilename);
    CallCounter := 1;
  end;

  if CallCounter = 1
  then bBool := dBCallInstall()
  else bBool := true;

  Wizardform.Visible := true;
  WizardForm.CancelButton.visible := true;
  WizardForm.NextButton.visible := bBool;
  dBaseButton.Visible := not bBool;
  BringToFrontAndRestore;     

  if bBool then
  begin
    PostMessage(WizardForm.NextButton.Handle, WM_LBUTTONDOWN, 0, 0);
    PostMessage(WizardForm.NextButton.Handle, WM_LBUTTONUP, 0, 0);
    #ifdef dBMainFile
      cVers := GetWindowsVersionString;
      if cVers[1] = '6' then
      begin
        cMainFile := ExpandConstant('{#dBMainFile}');
        if pos('.exe',cMainFile)>0 then
        begin
          #if "1" == DBuac
            //RegWriteStringValue(HKLM,'SOFTWARE\dBASE\PLUS\RuntimeApps\'+cMainFile,'useUACPaths','Y');
          #elif "2" == DBuac
            RegWriteStringValue(HKLM,'SOFTWARE\dBASE\PLUS\RuntimeApps\'+cMainFile,'useUACPaths','Y');
          #elif "3" == DBuac
            RegWriteStringValue(HKLM,'SOFTWARE\dBASE\PLUS\RuntimeApps\'+cMainFile,'useUACPaths','N');
          #endif
        end;
      end;
    #endif
  end
  else

  begin
    WizardForm.BackButton.visible := false;
    PostMessage(WizardForm.CancelButton.Handle, WM_LBUTTONDOWN, 0, 0);
    PostMessage(WizardForm.CancelButton.Handle, WM_LBUTTONUP, 0, 0);
  end;
end;

procedure dBaseWizardButtton();
var NextButton: TButton;
begin
  NextButton          := WizardForm.NextButton;
  dBaseButton         := TButton.Create(WizardForm);
  dBaseButton.Top     := NextButton.Top;
  dBaseButton.Left    := NextButton.Left;
  dBaseButton.Width   := NextButton.Width+8;
  dBaseButton.Height  := NextButton.Height;
  dBaseButton.Caption := '&Install';
  dBaseButton.OnClick := @dBaseButtonOnClick;
  dBaseButton.Visible := true;
  dBaseButton.Parent  := WizardForm;
end;


function CalldBASERuntimeSetup(): Boolean;
begin
  dBaseWizardButtton;
end;
#endif {dBSeparat}  
#endif {SM_dBASERuntime}

{Let this functions always defined and check for dBase settings only}
#if 'true' == dBUninstall
#ifDef SM_dBASERuntime
function IsOnlyRuntime(): Boolean;
var dbRunFolder: String;
begin
    #if 'true' == dBPlus        
  RegQueryStringValue(HKLM,'SOFTWARE\dBASE\PLUS\series10','RootDir',dbRunFolder);
  Result := not FileExists(dbRunFolder+'\Plus.exe');
    #else
  RegQueryStringValue(HKLM,'SOFTWARE\dBASE\dB2K\0.1','RootDir',dbRunFolder);
    Result := not FileExists(dbRunFolder+'\db2k.exe');
    #endif
end;
#endif {SM_dBASERuntime}
#endif {dBUninstall}

procedure dBase_CurPageChanged(CurPageID: Integer);
begin
   #ifDef SM_dBASERuntime
     #if 'true' == SM_dBASERuntime
         #if 'false' == dBSeparat
  if CurPageID = wpReady
  then CalldBASERuntimeSetup;
         #endif
     #endif
  #endif {SM_dBASERuntime}
end;  

procedure dBase_DeinitializeUninstall();
#if 'true' == dBUninstall
   var cFile,cParam: String;
       dbErrorCode: Integer;
#endif
begin
  #ifDef SM_dBASERuntime
    #if 'true' == dBUninstall
  if IsOnlyRuntime then
  begin
    RegQueryStringValue(
      HKLM,
      'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\dBASE PLUS series10 Runtime Engine',
      'UninstallString',
      cFile);
    if Length( cFile ) > 10 then
    begin
      cParam := copy( cFile, Pos( ' ', cFile )+1, Length( cFile ) );
      cFile := copy( cFile, 1, Pos( ' ', cFile )-1 );
      if FileExists( cFile )
      #if VER >= 0x05000100
      then Exec(cFile, cParam, ExpandConstant('{win}'),
                        SW_ShowNormal,ewWaitUntilTerminated,dbErrorCode);
      #else
      then InstExec(cFile,cParam,'',true,false,SW_SHOWNORMAL,dbErrorCode);
      #endif
    end;
  end;
    #endif
  #else
    Exit;
  #endif {SM_dBASERuntime}
end;
