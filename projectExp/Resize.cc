#include ProjExp.h

class Resize( oForm )

   this.Form = oForm
   this.HiddenText = new TextLabel( this.Form )
   this.HiddenText.Top:=-2000
   this.HiddenText.Visible:=false
   
   this.Notebook = this.Form.Notebook1
   this.FileNotebook = this.Notebook.FileNotebook
   this.InnoNotebook = this.Notebook.InnoNotebook
   this.Treeview = this.Form.treeview1
   this.Splitter = this.Form.qVSplitter1
   this.ProjectsLogoContainer = this.Form.ProjectsLogoContainer
   this.ProjectsLogo = this.Form.ProjectsLogoContainer.ProjectsLogo
   this.ProjectsLogoContainerHeight = this.ProjectsLogoContainer.height

   this.ButtonWidth = this.Form.Button_New.width
   this.MinWidth = MIN_WIDTH
   this.MinHeight = MIN_HEIGHT

   this.DeltaWidth = 0
   this.DeltaHeight = 0

   function resizeFileNotebook
      this.FileNotebook.RECTANGLE1.width := this.FileNotebook.rectangle1.width+this.DeltaWidth
      this.FileNotebook.RECTANGLE2.width := this.FileNotebook.rectangle2.width+this.DeltaWidth
      this.FileNotebook.RECTANGLE3.width := this.FileNotebook.rectangle3.width+this.DeltaWidth
      this.FileNotebook.FILEWARNING.width := this.FileNotebook.FILEWARNING.width+this.DeltaWidth
      this.FileNotebook.FILENAME.width := this.FileNotebook.FILENAME.width+this.DeltaWidth
      this.FileNotebook.BUT_FILE_SELECT.left := this.FileNotebook.BUT_FILE_SELECT.left+this.DeltaWidth
      this.FileNotebook.DEOTARGET.width := this.FileNotebook.DEOTARGET.width+this.DeltaWidth
      

   function resizeDEOPage
      this.Notebook.RECTANGLE4.width := this.Notebook.rectangle4.width+this.DeltaWidth
      this.Notebook.DEOTARGET.width := this.Notebook.DEOTARGET.width+this.DeltaWidth
      this.Notebook.BUT_SEL_DEOFOLDER.left := this.Notebook.BUT_SEL_DEOFOLDER.left+this.DeltaWidth
      this.Notebook.DEOFOLDERS.width := this.Notebook.DEOFOLDERS.width+this.DeltaWidth
      this.Notebook.BUT_DEOCLEAR.left := this.Notebook.BUT_DEOCLEAR.left+this.DeltaWidth
      this.Notebook.TEXT1.width := this.Notebook.TEXT1.width+this.DeltaWidth

//   function getMaxWidth( oControl )
//      #define PIXEL_FACTOR 6.5
//      local nMaxWidth, oText
//      oText = new TextLabel( this.Form )
//      oText.fontName := oControl.fontName
//      oText.fontSize := oControl.fontSize
//      nMaxWidth = int(oText.getTextExtent(replicate('W',oControl.maxlength))*PIXEL_FACTOR)
//      release object oText
//      return nMaxWidth

   function resizeProjectPage
      local nMaxWidth
      this.Notebook.rectangle1.width := this.Notebook.rectangle1.width+this.DeltaWidth
      this.Notebook.rectangle3.width := this.Notebook.rectangle1.width

      //nMaxWidth = this.getMaxWidth( this.Notebook.PROJECTNAME )
      //this.Notebook.PROJECTNAME.width := nMaxWidth
      //this.Notebook.PROJECTNAME.width := min(this.Notebook.PROJECTNAME.width+this.DeltaWidth,nMaxWidth)

      //added for resize missing also
      //Jonny Kwekkeboom 12/16/2004
      this.Notebook.PROJECTNAME.width := this.Notebook.PROJECTNAME.width+this.DeltaWidth 
      this.Notebook.AUTHOR.width := this.Notebook.AUTHOR.width+this.DeltaWidth 

      this.Notebook.MAINFILENAME.width := this.Notebook.MAINFILENAME.width+this.DeltaWidth
      this.Notebook.PARAMETERS.width := this.Notebook.PARAMETERS.width+this.DeltaWidth
		this.Notebook.PREPROCESSORS.width := this.Notebook.PREPROCESSORS.width+this.DeltaWidth
		this.Notebook.EXENAME.width := this.Notebook.EXENAME.width+this.DeltaWidth
      this.Notebook.SPLASHFILENAME.width := this.Notebook.SPLASHFILENAME.width+this.DeltaWidth
      this.Notebook.ICONFILENAME.width := this.Notebook.ICONFILENAME.width+this.DeltaWidth
      this.Notebook.LOGFILENAME.width := this.Notebook.LOGFILENAME.width+this.DeltaWidth

      //nMaxWidth = this.getMaxWidth( this.Notebook.AUTHOR )
      //this.Notebook.AUTHOR.width := nMaxWidth
      //this.Notebook.AUTHOR.width := min(this.Notebook.AUTHOR.width+this.DeltaWidth,nMaxWidth)

      this.Notebook.DESCRIPTION.width := this.Notebook.DESCRIPTION.width+this.DeltaWidth
      this.Notebook.BUT_SEL_EXE.left := this.Notebook.BUT_SEL_EXE.left+this.DeltaWidth
      this.Notebook.BUT_SEL_LOGFILE.left := this.Notebook.BUT_SEL_LOGFILE.left+this.DeltaWidth

   function resizeLogPage
      with this.notebook
        // Height done in resizeForm method
        LOGEDITOR.width := LogEditor.width+this.DeltaWidth
        EMPTYLOGBUTTON.left := LOGEDITOR.left+LOGEDITOR.width-EMPTYLOGBUTTON.width
        OPENLOGPUSHBUTTON.left := EMPTYLOGBUTTON.left - OPENLOGPUSHBUTTON.width - 8
        LOGFILESTATUS.width := LOGFILESTATUS.width+this.DeltaWidth
        //set the new progressbar ....
        LOGPROGRESS.top:= LOGEDITOR.top+this.Notebook.LOGEDITOR.height+5
        LOGPROGRESS.width:= LOGEDITOR.width      
      endwith

   function resizeInnoPage
   Local nWidth,nHeight
      with this.InnoNotebook
        width:= width+this.DeltaWidth
        nWidth= width-10
        nHeight:= this.Notebook.height-50

        Height:=nHeight

        store Width-(2*rectangle1.left)-5 to;
            rectangle1.width,rectangle2.width,;
            rectangle3.width,rectangle4.width,;
            rectangle5.width,rectangle6.width,;
            rectangle7.width,rectangle8.width	//D.Brooks - 09/28/2005
        store Height-(2*rectangle1.top) to;
            rectangle2.height
        rectangle1.height:= rectangle2.height+25
        rectangle3.height:= rectangle1.height
        rectangle5.height:= rectangle1.height
        rectangle6.height:= rectangle1.height
        rectangle7.height:= rectangle1.height        
        rectangle8.height:= rectangle1.height    //D.Brooks - 09/28/2005

        rectangle4.height:= Height-rectangle4.top-3
        
        //D.Brooks 11/29/2004 - resize for Refresh button
        I_NAME_REFRESH.left:= rectangle1.width-I_NAME_REFRESH.width-5

        I_APPNAME.width:= I_NAME_REFRESH.left - I_APPNAME.left - 5
        //TextLabel8.left:= I_APPNAME.width+I_APPNAME.left+5 
        TextLabel8.left:= I_DESTFOLDER1.width+I_DESTFOLDER1.left+5 
        
        I_SOURCEFOLDER.width := nWidth - I_SOURCEFOLDER.left - I_GETSOURCEFOLDER.width-5
        I_GETSOURCEFOLDER.Left:= I_SOURCEFOLDER.Left+I_SOURCEFOLDER.Width+5
        I_DESTFOLDER2.width := nWidth - I_DESTFOLDER2.left
        //D.Brooks 11/16/2004 - add values for new fields
        I_DESTFOLDER3.width := nWidth - I_DESTFOLDER2.left
        I_DESTFOLDER4.width := nWidth - I_DESTFOLDER2.left
        I_DIRSUBFOLDER_LBOX.width := nWidth - I_DIRSUBFOLDER_LBOX.left
        I_DIRSUBFOLDER_ADD.left := nWidth - I_DIRSUBFOLDER_ADD.width
        I_DIRSUBFOLDER_DEL.left := I_DIRSUBFOLDER_ADD.left
        I_DIRSUBFOLDER2.width := nWidth - I_DIRSUBFOLDER2.left - I_DIRSUBFOLDER_ADD.width - 5
        I_DIRSUBFOLDER_LBOX.width := I_DIRSUBFOLDER_DEL.left - I_DIRSUBFOLDER_LBOX.left -3
        I_FILESSUBFOLDER_ADD.left := nWidth - I_FILESSUBFOLDER_ADD.width
        I_FILE_DEL.left := I_FILESSUBFOLDER_ADD.left
        I_FILE_Add.left := I_FILE_DEL.left
        I_FILESSUBFOLDER.width := nWidth - I_FILESSUBFOLDER.left - I_FILESSUBFOLDER_Add.width - 5
        I_FILESSUBFOLDER_LBOX.width := I_FILESSUBFOLDER.width

        //J.Kwekkeboom 01/20/2007 - changed for add new Setup languages text and combobox
//        I_DIRSUBFOLDER_LBOX.height:=height-I_DIRSUBFOLDER_LBOX.top- TEXTLABEL12.Height - 30
        //I_DIRSUBFOLDER_LBOX.height:=height-I_DIRSUBFOLDER_LBOX.top-10
        //J.Kwekkeboom 01/20/2007 - added for new Setup languages
        TEXTLABEL12.Top := I_DIRSUBFOLDER_LBOX.Top + I_DIRSUBFOLDER_LBOX.height + 10
        I_SETUPLANGCB1.Top := TEXTLABEL12.Top
        I_SETUPLANGRESULT.Top := TEXTLABEL12.Top
        I_SETUPLANGRESULT.Width := I_DESTFOLDER4.Left + I_DESTFOLDER4.Width - I_SETUPLANGRESULT.Left - 10

        //added for new Windows Vista radiobuttons
        //J.Kwekkeboom 03/03/07
        TEXTLABEL1.top := RBADMIN.Top + RBADMIN.Height+5
        I_FILESSUBFOLDER_LBOX.top := TEXTLABEL1.top+TEXTLABEL1.Height + 5
        I_FILE_ADD.top := I_FILESSUBFOLDER_LBOX.top
        I_FILE_DEL.top := I_FILESSUBFOLDER_LBOX.top + I_FILE_ADD.height + 5
        //End adding

        I_FILESSUBFOLDER_LBOX.height:=height-I_FILESSUBFOLDER_LBOX.top-I_DISPFLAG.height-10
        I_SCRIPT.width:=nWidth - I_SCRIPT.left
        I_SCRIPT.height:=height-I_SCRIPT.top-10
        I_SCRIPTNAME.width:=I_SCRIPT.width-I_SCRIPTGENERATE.width-(3*I_SCRIPTNAME_BUT.width) + 5
        I_SCRIPTNAME_BUT.left:=I_SCRIPTNAME.left+I_SCRIPTNAME.width+5
        I_SCRIPTGENERATE.Left:=I_SCRIPTNAME_BUT.Left+I_SCRIPTNAME_BUT.width+5
        I_SCRIPTCALLSM.Left:=I_SCRIPTGENERATE.Left+I_SCRIPTCALLSM.width+5
        I_GROUP2.width:=nWidth-I_GROUP2.left-I_GROUPADD_PB.width-5
        I_GROUPADD_PB.left:=I_Group2.left+I_Group2.width+5
        I_GROUPDEL_PB.left:=I_GROUPADD_PB.left
        I_ADDGROUPFILE.left:=I_GROUPADD_PB.left
        I_GROUPFILES.width:=nWidth-I_GROUPFILES.left-I_ADDGROUPFILE.width-2
        //D.Brooks 11/18/2004 - need to increase the width
        I_GROUP_BOX.width:=nWidth-I_GROUP_BOX.left-I_ADDGROUPFILE.width-2
        //I_GROUP_BOX.width:=I_GROUPFILES.width
        //I_GROUP_BOX.height:=height-I_GROUP_BOX.top-(5*I_DESKTOP.height) - 25 //KK 12/14/2010

        I_DESKTOP.top:=I_GROUP_BOX.top+I_GROUP_BOX.height+3
        I_QUICKLAUNCH.top:=I_DESKTOP.top+I_DESKTOP.height+3

        I_UNINS_ICON.top:=I_QUICKLAUNCH.top+I_QUICKLAUNCH.height+3
        I_UNINS_ICONUTIL.top:=I_UNINS_ICON.top+I_UNINS_ICON.height+3
        I_DISPGROUPATEND.top:=I_UNINS_ICONUTIL.top+I_UNINS_ICONUTIL.height+3
        I_DISPFLAG.top:= I_FILESSUBFOLDER_LBOX.top+I_FILESSUBFOLDER_LBOX.height + 5
        I_DISPFLAG.left:=I_FILESSUBFOLDER_LBOX.left+I_FILESSUBFOLDER_LBOX.width-I_DISPFLAG.width

        I_FLAG.height:=nHeight-I_FLAG.top //-15
        I_FLAG.Width:=nWidth-I_FLAG.left
        I_FLAG.I_FLAGOK.left:=I_FLAG.Width+I_FLAG.left-I_FLAG.I_FLAGOK.width - 15
        I_FLAG.I_FLAGABORT.Left:=I_FLAG.I_FLAGOK.left-I_FLAG.I_FLAGABORT.width - 5
        L_PB1.left:=nWidth-L_PB1.width-5
        L_PB2.left:=L_PB1.left
        L_PB3.left:=L_PB1.left
        //D.Brooks 12/06/2004 - components have been removed
        //I_LANGFILE_PB.left:=L_PB1.left
        L_EF1.width:=L_PB1.left-L_EF1.left-5
        L_EF2.width:=L_EF1.width
        L_EF3.width:=L_EF1.width
        //I_LANGFILE.width:=L_EF1.width      
        I_LIZ_LBOX.width:=nWidth-I_LIZ_LBOX.left-5
        I_LIZ_LBOX.height:=nHeight-I_LIZ_LBOX.top-10

        ALIAS_LBOX.width:=nWidth-ALIAS_ADD.width-30
        ALIAS_ADD.Left:=ALIAS_LBOX.left+ALIAS_LBOX.width+5
        ALIAS_DEL.Left:=ALIAS_ADD.Left
        I_BDECHANGER.Left:=ALIAS_ADD.Left
        ALIASFOLDER.width:=I_BDECHANGER.Left-ALIASFOLDER.left-5
        ALIASNAME.width:=ALIASFOLDER.width
               
		  //D.Brooks 11/24/2004 - do not resize BDE fields
//        BDE_MDXBLOCKSIZE.top:=rectangle7.height+rectangle7.top-38
//        BDE_MemoBlockSize.top:=BDE_MDXBLOCKSIZE.top-33
//        BDE_LEVEL.top:=BDE_MemoBlockSize.top-33
//        BDE_LANGDRIVER.top:=BDE_LEVEL.top-33
//        BDE_MAXFILEHANDLE.top:=BDE_LANGDRIVER.top-33
//        TEXTLABEL17.top:=BDE_MAXFILEHANDLE.top
//        TEXTLABEL18.top:=BDE_LANGDRIVER.top
//        TEXTLABEL19.top:=BDE_LEVEL.top
//        TEXTLABEL20.top:=BDE_MEMOBLOCKSIZE.top
//        TEXTLABEL21.top:=BDE_MDXBLOCKSIZE.top
//        ALIAS_LBOX.height:=BDE_MAXFILEHANDLE.top-ALIAS_LBOX.top-10 
		  ALIAS_LBOX.height:=BDE_USESETTINGS.top-ALIAS_LBOX.top-10        

        //J.Kwekkeboom 12/19/2004 - added for resize all fields too                
        //QAID: 5578 - D.Brooks - 10/03/2005
        //C1.Top:=RECTANGLE4.top+2
        C1.Top:=RECTANGLE4.top+10
        C1.Width:=RECTANGLE4.Width-4
        //C1.Height:=RECTANGLE4.Height-4
        C1.Height:=RECTANGLE4.Height-12
        //End QAID: 5578
        C1.Left:=RECTANGLE4.Left+2
        C2.Left:=C1.left
        C2.Top:=C1.top
        C2.Width:=C1.width
        C2.Height:=C1.height
        C1.I_DBENGINENAME.width:=C1.Width-C1.I_DBENGINENAME.left-C1.I_DBENGINEPB.width-15
        C1.I_DBENGINEPB.Left:=C1.I_DBENGINENAME.Left+C1.I_DBENGINENAME.width+5
        C1.EF1.width:=C1.Width-C1.EF1.left-15
        C1.EF2.width:=C1.Width-C1.EF2.left-15
        C1.EF1.width:=C1.Width-C1.EF1.left-15

        C2.EF1.width:=C2.Width-C2.EF1.left-15
        C2.Text1.width:=C1.Width-(2*C2.Text1.left)        
      endwith
   return
      
   function adjustButtonPos
      local nWidth
      nWidth = this.Form.width
      this.Form.button_new.top = this.Form.button_new.top+this.DeltaHeight
      this.Form.button_Close.top = this.Form.button_new.top
      this.Form.button_save.top = this.Form.button_new.top
      if this.Form.windowstate = 0
         this.Form.button_new.left = nWidth-BUTTON_MARGIN-this.ButtonWidth
         this.Form.button_save.left = nWidth-BUTTON_MARGIN-BUTTON_SEP-(this.ButtonWidth*2)
         this.Form.button_Close.left = nWidth-BUTTON_MARGIN-(BUTTON_SEP*2)-(this.ButtonWidth*3)
      else
         this.Form.button_new.left = nWidth-(BUTTON_MARGIN+this.ButtonWidth+BUTTON_MARGIN)
         this.Form.button_save.left = nWidth-(BUTTON_MARGIN+BUTTON_SEP+(this.ButtonWidth*2)+BUTTON_MARGIN)
         this.Form.button_Close.left = nWidth-(BUTTON_MARGIN+(BUTTON_SEP*2)+(this.ButtonWidth*3)+BUTTON_MARGIN)
      endif

   function SplitterDropped
      // Adjust the widths
      local nOldWidth,nNoteBookWidth
      if this.Splitter.left < MIN_SPLITLEFT
         this.Splitter.left := MIN_SPLITLEFT
      endif
      if this.Splitter.left > this.Form.width-SPLITTER_MARGIN
         this.Splitter.left := this.Form.width-SPLITTER_MARGIN
      endif

      this.Treeview.width = max(0,this.Splitter.left-this.Treeview.left-(SPLITTER_MARGIN/2))
      this.Notebook.left = this.Splitter.left+SPLITTER_MARGIN

      nOldWidth = this.Notebook.width
      if this.Form.windowstate = 0
         this.Notebook.width := max(MIN_NOTEBOOK_WIDTH, this.Form.width-(this.Notebook.left+BUTTON_MARGIN))
      else
         this.Notebook.width := max(MIN_NOTEBOOK_WIDTH, this.Form.width-(this.Notebook.left+(BUTTON_MARGIN*2)))
      endif
      this.DeltaWidth = this.Notebook.width-nOldWidth

      this.FileNotebook.width = this.FileNotebook.width+this.DeltaWidth
      this.Notebook.NoDetailsText.width := this.Notebook.NoDetailsText.width+this.DeltaWidth

      // Do the rest of the widths
      this.resizeProjectPage()
      this.resizeFileNotebook()
      this.resizeLogPage()
      this.resizeDEOPage()
      this.resizeInnoPage()

      return true

   function adjustLogo
      this.ProjectsLogoContainer.height = this.ProjectsLogoContainer.height+this.DeltaHeight
      // reposition logo within container
      if this.ProjectsLogoContainer.height <= this.ProjectsLogoContainerHeight
        //this.ProjectsLogo.top = 0
        this.ProjectsLogo.top = this.ProjectsLogo.top+(this.DeltaHeight/2)
      else
         this.ProjectsLogo.top = round((this.ProjectsLogoContainer.height-this.ProjectsLogoContainerHeight)/2,0)
      endif

   function resizeForm(nSizeType, nWidth, nHeight)
      local nOldHeight
      this.form.freeze() 

      if nSizeType = 0
         nHeight = max(nHeight,this.MinHeight)
         nWidth = max(nWidth, this.MinWidth)
         this.Form.height = nHeight
         this.Form.width = nWidth
      else
         this.Form.height = this.Form.height
         this.Form.width = this.Form.width
      endif

      nOldHeight = this.Form.treeview1.height
      this.Form.treeview1.height = nHeight - TREEVIEW_HEIGHT_ADJ
      this.DeltaHeight = this.Form.treeview1.height-nOldHeight
      this.Notebook.height = this.Notebook.height+this.DeltaHeight

      this.Notebook.NoDetailsText.top :=  (this.Notebook.height/2)

      this.adjustLogo()

      this.FileNotebook.height = max(0,this.FileNotebook.height+this.DeltaHeight)
      this.Notebook.LOGEDITOR.height := this.Notebook.LogEditor.height+this.DeltaHeight

      this.adjustButtonPos()

      this.Splitter.adjustLength(this.Notebook.height)
      this.SplitterDropped() // adjust the widths

      this.form.Unfreeze()

      return true

   function getMaxWidth( oControl )
      //#define PIXEL_FACTOR 6.5
      #define PIXEL_FACTOR 1.0
      local nMaxWidth, oText
      oText = new TextLabel( this.Form )
      oText.fontName := oControl.fontName
      oText.fontSize := oControl.fontSize
      nMaxWidth = int(oText.getTextExtent(replicate('W',oControl.maxlength))*PIXEL_FACTOR)
      release object oText
      return nMaxWidth

   function release
      this.Form = null
      release object this
      return true

   function TransWidth
   Local LangWidth

      LangWidth=0
      with this.NOTEBOOK
         //pass Button to width of translated text
         LangWidth:=this.GetTransWidth(EMPTYLOGBUTTON,0)
         EMPTYLOGBUTTON.width:=LangWidth+25 //Add bitmap also
         
         LangWidth:=this.GetTransWidth(TEXTLABEL1,0)
         LangWidth:=this.GetTransWidth(TEXTLABEL2,LangWidth)
         LangWidth:=this.GetTransWidth(TEXTLABEL7,LangWidth)
         LangWidth:=this.GetTransWidth(TEXTLABEL14,LangWidth)
         LangWidth:=this.GetTransWidth(TEXTLABEL13,LangWidth)
         LangWidth:=this.GetTransWidth(TEXTLABEL6,LangWidth)
         LangWidth:=this.GetTransWidth(TEXTLABEL12,LangWidth)
         //Wrap text
         if TEXTLABEL15.width>LangWidth
            LangWidth:=TEXTLABEL15.width
         endif

         store LangWidth+15 to ;
             TEXTLABEL1.width,TEXTLABEL2.width,;
             TEXTLABEL7.width,TEXTLABEL14.Width,;
             TEXTLABEL13.width,TEXTLABEL6.Width,;
             TEXTLABEL12.width

         LangWidth:=TEXTLABEL1.width+5
         //DEOAPPLICATIONCHECK.Width:=LangWidth+2
         //WEBAPP.Width:=LangWidth+2

         LangWidth+=15
         Store LangWidth to ;
             ProjectName.Left,MainFileName.Left,;
             Parameters.Left,ExeName.Left,;
				 Preprocessors.Left,ExeName.Left,;
             SplashFileName.Left,IconFileName.Left,;
             LogFileName.Left
         
         ProjectName.Width:=rectangle1.Width-LangWidth
         MainFileName.Width:=ProjectName.Width
         Parameters.Width:=ProjectName.Width
			Preprocessors.Width:=ProjectName.Width
         ExeName.Width:=ProjectName.Width-but_sel_exe.width-5
         SplashFileName.Width:=ProjectName.Width
         IconFileName.Width:=ProjectName.Width
         LogFileName.Width:=ProjectName.Width-BUT_SEL_LOGFILE.width-5
         but_sel_exe.Left:=LogFileName.Left+LogFileName.Width+5         
         BUT_SEL_LOGFILE.left:=but_sel_exe.Left

         LangWidth:=this.GetTransWidth(TEXTLABEL3,0)
         LangWidth:=this.GetTransWidth(TEXTLABEL4,LangWidth)

         LangWidth+=15
         Store LangWidth to ;
             TEXTLABEL3.width,TEXTLABEL4.width

         LangWidth+=25
         Store LangWidth to ;
             Author.Left,Description.Left

         Author.width:=rectangle1.Width-Author.Left
         Description.width:=Author.width         
      endwith      


      //FileNoteBook
      with this.NOTEBOOK.FILENOTEBOOK
         LangWidth:=this.GetTransWidth(TEXTLABEL8,0)
         LangWidth:=this.GetTransWidth(TEXTLABEL9,LangWidth)         
         LangWidth:=this.GetTransWidth(TEXTLABEL10,LangWidth)
         LangWidth:=this.GetTransWidth(TEXTLABEL11,LangWidth)

         LangWidth+=25
         Store LangWidth to ;
            TEXTLABEL8.width,TEXTLABEL9.width,;
            TEXTLABEL10.width,TEXTLABEL11.width

         LangWidth+=25
         Store LangWidth to ;
            FileName.Left,FILESIZE.Left,;
            FILECREATED.Left,FILEMODIFIED.Left

         FileName.Width:=Rectangle1.Width-FileName.Left-BUT_FILE_SELECT.width-5
         BUT_FILE_SELECT.Left:=FileName.Left+FileName.Width+5

         LangWidth=this.GetTransCBWidth(MAINPROG,0)
         LangWidth=this.GetTransCBWidth(FILEINCLUDED,LangWidth)
         LangWidth+=25
         MainProg.Width:=LangWidth
         FileIncluded.Width:=LangWidth

         DEOOBJECT.Width:=  this.GetTransCBWidth(DEOOBJECT,0)
         TEXTLABEL1.Width:= this.GetTransWidth(TEXTLABEL1,0)+15
         DEOTARGET.Left:=TEXTLABEL1.Left+TEXTLABEL1.Width+10
         DEOTARGET.Width:=Rectangle3.Width-DEOTARGET.Left-5
      endwith

      //InnoPages
      with this.NOTEBOOK.INNONOTEBOOK
         //Resizing also Buttons
         LangWidth:=this.GetTransWidth(I_NAME_REFRESH,0)
         I_NAME_REFRESH.width:=Langwidth+25 

         //Default
         LangWidth:=this.GetTransWidth(TEXTLABEL3,0)
         LangWidth:=this.GetTransWidth(TEXTLABEL4,LangWidth)
         LangWidth:=this.GetTransWidth(TEXTLABEL7,LangWidth)
         LangWidth:=this.GetTransWidth(TEXTLABEL5,LangWidth)
         LangWidth+=15
         Store LangWidth to ;
            TEXTLABEL3.width,TEXTLABEL4.width,;
            TEXTLABEL7.width,TEXTLABEL5.width

         LangWidth+=15
         Store LangWidth to ;
            I_APPNAME.Left,I_VERSION.Left,;
            I_SOURCEFOLDER.Left,I_DESTFOLDER1.Left,;
            I_DESTFOLDER2.Left,I_DESTFOLDER3.Left,;
            I_DESTFOLDER4.Left
         
         LangWidth:=this.GetTransWidth(TEXTLABEL24,0)
         LangWidth:=this.GetTransWidth(TEXTLABEL13,LangWidth)
         LangWidth:=this.GetTransWidth(TEXTLABEL23,LangWidth)
         LangWidth:=this.GetTransWidth(TEXTLABEL14,LangWidth)

         LangWidth+=15
         Store LangWidth to ;
            TEXTLABEL24.width,TEXTLABEL13.width,;
            TEXTLABEL23.width,TEXTLABEL14.width

         TEXTLABEL24.Left:=I_DESTFOLDER4.Left-LangWidth-20
         TEXTLABEL13.Left:=TEXTLABEL24.Left
         TEXTLABEL23.Left:=TEXTLABEL24.Left
         TEXTLABEL14.Left:=TEXTLABEL24.Left

         I_APPNAME.Width:=Rectangle1.Width-I_APPNAME.Left-I_NAME_REFRESH.width-5
         I_NAME_REFRESH.Left:=I_APPNAME.Left+I_APPNAME.Width+5
         I_SOURCEFOLDER.Width:=Rectangle1.Width-I_SOURCEFOLDER.Left-I_GETSOURCEFOLDER.width-5
         I_GETSOURCEFOLDER.Left:=I_SOURCEFOLDER.Left+I_SOURCEFOLDER.Width+5

         I_DESTFOLDER2.Width:=Rectangle1.Width-I_DESTFOLDER2.Left-5
         I_DESTFOLDER3.Width:=I_DESTFOLDER2.Width
         I_DESTFOLDER4.Width:=I_DESTFOLDER2.Width

         TEXTLABEL6.Width:=this.GetTransWidth(TEXTLABEL6,0)+15

         //J.Kwekkeboom 01/21/2007 - Added for new Languages support
         TEXTLABEL12.Width:=this.GetTransWidth(TEXTLABEL12,0)+15
         I_SETUPLANGCB1.Left := TextLabel12.Left+TextLabel12.Width
         I_SETUPLANGRESULT.Left := I_SETUPLANGCB1.Left + I_SETUPLANGCB1.width + 10

         //Files
         TEXTLABEL2.Width:=this.GetTransWidth(TEXTLABEL2,0)+15
         TEXTLABEL1.Width:=this.GetTransWidth(TEXTLABEL1,0)+15

         //Menu Grp
         LangWidth:=this.GetTransWidth(TEXTLABEL28,0)
         LangWidth:=this.GetTransWidth(TEXTLABEL26,LangWidth)
         LangWidth:=this.GetTransWidth(TEXTLABEL27,LangWidth)

         LangWidth+=15
         Store LangWidth to ;
            TEXTLABEL28.Width,TEXTLABEL26.Width,TEXTLABEL27.Width         

         LangWidth+=15
         Store LangWidth to ;
            I_GROUPFILES.Left,I_GROUP1.Left,I_GROUP2.Left

         I_GROUPFILES.Width:=RECTANGLE3.width-I_GROUPFILES.Left-I_ADDGROUPFILE.Width-5
         I_ADDGROUPFILE.Left:=I_GROUPFILES.Left+I_GROUPFILES.Width+5
         I_GROUP2.Width:=RECTANGLE3.width-I_GROUP2.Left-5

         I_DESKTOP.Width:=this.GetTransCBWidth(I_DESKTOP,0)
         I_QUICKLAUNCH.Width:=this.GetTransCBWidth(I_QUICKLAUNCH,0)

         I_UNINS_ICON.Width:=this.GetTransCBWidth(I_UNINS_ICON,0)
         I_UNINS_ICONUTIL.Width:=this.GetTransCBWidth(I_UNINS_ICONUTIL,0)
         I_DISPGROUPATEND.Width:=this.GetTransCBWidth(I_DISPGROUPATEND,0)

         TEXTLABEL25.Width:=this.GetTransWidth(TEXTLABEL25,0)+15
           
         //QAID: 5255 - D.Brooks - 02/24/2005
         CB_XP.Width:=this.GetTransCBWidth(CB_XP,0)

         //Runtime          
         with C1
           TEXTLABEL1.Width:=this.GetTransWidth(TEXTLABEL1,0)+15
           I_BDEGROUPINSERT.Width:=this.GetTransCBWidth(I_BDEGROUPINSERT,0) 
           I_BDEGROUPINSERT.Left:=21
           CB1.Width:=this.GetTransCBWidth(CB1,0)
           CB2.Width:=this.GetTransCBWidth(CB2,0)
           CB3.Width:=this.GetTransCBWidth(CB3,0)
           CB4.Width:=this.GetTransCBWidth(CB4,0)
           TL1.Width:=this.GetTransWidth(TL1,0)+15
           TL2.Width:=this.GetTransWidth(TL2,0)+15
         endwith

         with C2
           TEXTLABEL2.Width:=this.GetTransWidth(TEXTLABEL2,0)+15
           TEXTLABEL1.Width:=this.GetTransWidth(TEXTLABEL1,0)+15
           CB1.Width:=this.GetTransCBWidth(CB1,0)
         endwith         

         //License
         TEXTLABEL30.Width:=this.GetTransWidth(TEXTLABEL30,0)+15
         TEXTLABEL9.Width:=this.GetTransWidth(TEXTLABEL9,0)+15
         TEXTLABEL10.Width:=this.GetTransWidth(TEXTLABEL10,0)+15         
         TEXTLABEL11.Width:=this.GetTransWidth(TEXTLABEL11,0)+15
         TEXTLABEL31.Width:=this.GetTransWidth(TEXTLABEL31,0)+15

         LANG_CB.Left:=TEXTLABEL30.Left+TEXTLABEL31.Width+10

         //BDE Setting
         ALIASDEL.Width:=this.GetTransCBWidth(ALIASDEL,0)
         TEXTLABEL29.Width:=this.GetTransWidth(TEXTLABEL29,0)+30
         BDE_USESETTINGS.Width:=this.GetTransCBWidth(BDE_USESETTINGS,0)
         

         LangWidth:=this.GetTransWidth(TEXTLABEL15,0)
         LangWidth:=this.GetTransWidth(TEXTLABEL16,LangWidth)
         LangWidth:=this.GetTransWidth(TEXTLABEL22,LangWidth)
         
         LangWidth+=15
         Store LangWidth to ;
            TEXTLABEL15.Width,TEXTLABEL16.Width,TEXTLABEL22.Width

         LangWidth+=15
         Store LangWidth to ;
            ALIASFOLDER.Left,ALIASNAME.Left,ALIASDRIVER.Left,;
            ALIASDEL.Left

         ALIASFOLDER.Width:=Rectangle7.Width-ALIASFOLDER.Left-I_BDECHANGER.Width-5
         I_BDECHANGER.Left:=ALIASFOLDER.Left+ALIASFOLDER.Width+5
         ALIASNAME.Width:=ALIASFOLDER.Width

         //INI 
         INICHECK.Width:=this.GetTransCBWidth(INICHECK,0)
         I_INI_ERRORHAND_INC.Width:=this.GetTransCBWidth(I_INI_ERRORHAND_INC,0)
         I_INI_TRAP.Width:=this.GetTransCBWidth(I_INI_TRAP,0)
         I_INI_IGNORETRAP.Width:=this.GetTransCBWidth(I_INI_IGNORETRAP,0)
         I_INI_NOBDE.Width:=this.GetTransCBWidth(I_INI_NOBDE,0)                     
     endwith
      
   return

   function GetTransWidth(oControl,nAct)
   Local nReturn
      //nReturn=int(this.HiddenText.getTextExtent(rtrim(trim(oControl.Text)))*6.5)
      nReturn=int(this.HiddenText.getTextExtent(rtrim(trim(oControl.Text))))
      oControl.Width:=nReturn+10
      if nAct>nReturn
        nReturn:=nAct
        oControl.Width:=nReturn
     endif    
   return(nReturn)

   function GetTransCBWidth(oControl,nAct)
   Local nReturn
      //nReturn=int(this.HiddenText.getTextExtent(rTrim(Trim(oControl.Text)))*6.5)
      nReturn=int(this.HiddenText.getTextExtent(rTrim(Trim(oControl.Text))))
      nReturn+=40
      if nAct>nReturn
        nReturn:=nAct
     endif
   return(nReturn)
endclass
