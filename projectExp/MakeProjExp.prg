//=== MakeProjExp.prg - Project Explorer Build Creator ===
//
//  Compiles Project Explorer files and copies all required files
//  to the following output folder:
//
//    C:\fvcsprojects\projectmanager\ProjExp 
//
//
//  07/25/2006 - Created by Dave Casler
//  01/09/2007 - QAID:4784 - Dave Casler - Add PBuilderProp.wfm module
//             - QAID:none - Dave Casler - Remove Read-Only from ProjExp.ini

//cd "C:\fvcsprojects\projectmanager"

cd "C:\dBLCore\projectExp"

//=== Compile the files that need to be compiled ===
delete file "ProjExp.log"
compile ProjExp.wfm,      ;
        Resize.cc,        ;
        Project.cc,       ;
        Pbuilder.cc,      ;
        Pbuilderplus.mnu, ;
        Pbuilderplus.pop, ;
        PbuilderProp.wfm  ;
        AUTO Log "ProjExp.log"

//=== Create the output folder ===
if ( NOT (makeDirTree("ProjExp\dtf")) )
   mkdir "ProjExp"
   cd "ProjExp"
   mkdir "dtf"
   cd ".."
endif

//=== Copy Build Files to Output Folder ===
copy file "ProjExp.wfo"             to "ProjExp\ProjExp.wfo"
copy file "Resize.co"               to "ProjExp\Resize.co"
copy file "Project.co"              to "ProjExp\Project.co"
copy file "Pbuilder.co"             to "ProjExp\Pbuilder.co"
copy file "Pbuilderplus.mno"        to "ProjExp\Pbuilderplus.mno"
copy file "Pbuilderplus.poo"        to "ProjExp\Pbuilderplus.poo"
copy file "PbuilderProp.wfo"        to "ProjExp\PbuilderProp.wfo"  // QAID:4784

copy file "ProjExp.ini"             to "ProjExp\ProjExp.ini"
copy file "ProjExp.dll"             to "ProjExp\ProjExp.dll"

copy file "dtf\Pbuilderplus_DE.dtf" to "ProjExp\dtf\Pbuilderplus_DE.dtf"
copy file "dtf\Pbuilderplus_EN.dtf" to "ProjExp\dtf\Pbuilderplus_EN.dtf"
copy file "dtf\Pbuilderplus_IT.dtf" to "ProjExp\dtf\Pbuilderplus_IT.dtf"
copy file "dtf\Pbuilderplus_PT.dtf" to "ProjExp\dtf\Pbuilderplus_PT.dtf"

set alternate to "ProjExp.log" additive
set alternate on

// QAID:none - 01/09/2007 - Dave Casler
// Make sure distributed version on ProjExp.ini in Not Read-Only
run attrib -r ProjExp\ProjExp.ini

?"=== Build Results to C:\dBLCore\projectexp\ProjExp Folder ==="
?
//dir "ProjExp\*.*" 
//?
//dir "ProjExp\dtf\*.*" 
//?

set alternate off
set alternate to

** END HEADER -- do not remove this line
//
// Generated on 07/24/2006
//
parameter bModal
local f
f = new MakeProjExpForm()
if (bModal)
   f.mdi = false // ensure not MDI
   f.readModal()
else
   f.open()
endif

class MakeProjExpForm of FORM
   with (this)
      height = 25.0
      left = 10.0
      top = 0.0
      width = 90.0
      text = "Project Explorer dBL Build Log (ProjExp.log)"
   endwith

   this.EDITOR1 = new EDITOR(this)
   with (this.EDITOR1)
      height = 25.0
      left = 0.0
      top = 0.0
      width = 90.0
      dataLink = "FILE C:\dBLCore\projectexp\projexp.log"
      anchor = 6	// Container
   endwith


endclass

   // see C:\fvcsprojects\dQuery2\qWizard.cfm method MakeDirTree()

   // QAID: 5008 - M.Kay, dBI Inc. - 09/29/2004
   function makeDirTree
      parameters cDirTree
      // By: www.autotraker.com
      // This function will build an entire directory tree
      // just pass the entire tree and it will build it.
      // returns true if the directory exists if it has build it.
      // cDirTree = "c:\temp2\temp2\temp3\temp4\temp5\temp6"
      // makeDirTree(cDirTree)
      // can use either imageHlp.dll or dbgHelp.dll
      // (in the system32 directory). Since some versions of
      // imageHlp.dll may not support this function, we are
      // putting it in a try, catch
      local cDir
      cDir = iif(right(cDirTree,1)=="\",cDirTree,cDirTree+"\")
      if type("MakeSureDirectoryPathExists")<>"FP"
         try
            extern cLogical MakeSureDirectoryPathExists(cString);
               "IMAGEHLP.DLL"
         catch(exception e)
         endtry
      endif

      if type("MakeSureDirectoryPathExists")<>"FP"
         extern cLogical MakeSureDirectoryPathExists(cString);
          "dbgHelp.dll"
      endif

      if type("MakeSureDirectoryPathExists")<>"FP"
         // return false if could not find a DLL to use
         return false
      else
         return MakeSureDirectoryPathExists(cDir)
      endif
   // End QAID: 5008


