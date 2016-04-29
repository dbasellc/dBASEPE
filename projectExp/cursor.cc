CLASS Cursor
/*
    ---------------------------------------------------------------
    Description:

       This UI class is designed to work with your windows cursor.

    Programmers:

       Ken Mayer
       Bowen Moursund

    History:

       11/28/1996 -- original version of these routines as a class.
       12/11/1997 -- Visual dBASE 7 (32-bit) upgrade ... (Bowen)
       12/16/1997 -- added #defines for standard cursors, provided
                     by Romain

    Instantiation:

       set procedure to cursor.cc additive
       _app.Cursor = new Cursor()

    Methods:
       More details on each of these is contained in the commentary
       for each method ...

       Release()
         Releases this object (and others) from memory

       BeginWait()
         Turns cursor into 'hourglass' (or whatever your 'wait'
         cursor is set to)

       EndWait()
         Returns cursor to previous state (turn off 'hourglass')

    Properties:

       None

   ----------------------------------------------------------------
*/
  // -- set up external calls
  if type("GetDesktopWindow") # "FP"
     extern CHANDLE GetDesktopWindow (CVOID) User32
   endif
   if type("GetCursor") # "FP"
      extern CHANDLE GetCursor(CVOID) User32
   endif
   if type("SetCursor") # "FP"
      extern CHANDLE SetCursor(CHANDLE) User32
   endif
   if type("LoadCursor") # "FP"
      extern CHANDLE LoadCursor(CHANDLE, CLONG) User32 ;
       from "LoadCursorA"
   endif
   if type("SetCapture") # "FP"
      extern CHANDLE SetCapture(CHANDLE) User32
   endif
   if type("ReleaseCapture") # "FP"
      extern CLOGICAL ReleaseCapture(cVOID) user32
   endif
   store GetCursor() to this.LastCursor
   this.Waiting = false

   // -- standard cursor IDs:
   #define IDC_ARROW    32512 // standard arrow
   #define IDC_IBEAM    32513 // standard IBeam
   #define IDC_WAIT     32514 // wait
   #define IDC_CROSS    32515 // cross
   #define IDC_UPARROW  32516 // up-arrow
   #define IDC_SIZE     32640 // size
   #define IDC_ICON     32641 // ?
   #define IDC_SIZENWSE 32642 // diagonal size  \
   #define IDC_SIZENESW 32643 // diagonal size  /
   #define IDC_SIZEWE   32644 // horizontal size
   #define IDC_SIZENS   32645 // vertical size

   FUNCTION Release
      /*
         ----------------------------------------------------------
          Programmer..: Ken Mayer
          Date........: 11/24/1996
          Notes.......: Used to release this object from memory.
          Written for.: Visual dBASE 5.5
          Rev. History: 11/24/1996 -- Original Code
          Calls.......: none
          Usage.......: cursor.Release()
          Example.....: _app.Cursor.Release()
                        _app.Cursor = NULL   // not required, but
                                             // a good idea
          Returns.....: Null
          Parameters..: None
         ----------------------------------------------------------
      */

      release object this

   RETURN NULL
   // -- EoM: Release()

   function BeginWait
      /*
         ----------------------------------------------------------
          Programmer..: Bowen Moursund
          Date........: 12/11/1997
          Notes.......: set hourglass mousepointer, capture mouse
                        events
          Written for.: Visual dBASE 5.5
          Rev. History: ? -- Original Code
                        11/28/1996 -- modified for dUFLP library
                        12/11/1997 -- VdBASE 7 version
                        12/16/1997 -- added parameter ...
          Calls.......: none
          Usage.......: cursor.BeginWait( [<nCursor>] )
          Example.....: _app.Cursor.BeginWait() // default
                             or
                        _app.Cursor.BeginWait( IDC_CROSS )
          Returns.....: Null
          Parameters..: nCursor = IDC_n as #defined above
                                  (Optional)
         ----------------------------------------------------------
      */

      parameter nCursor

      if not this.Waiting
         this.LastCursor = GetCursor()
         if pCount() = 0
            SetCursor(LoadCursor(0,IDC_WAIT))
         else
            SetCursor(LoadCursor(0,nCursor))
         endif
         SetCapture(GetDesktopWindow())
         this.Waiting = true
      endif
   return null
   // -- EoM: BeginWait

   function EndWait
      /*
         ----------------------------------------------------------
          Programmer..: Bowen Moursund
          Date........: 12/11/1997
          Notes.......: Reset mousepointer, release capture of
                        mouse events
          Written for.: Visual dBASE 5.5
          Rev. History: ? -- Original Code
                        11/28/1996 - modified for dUFLP library
                        12/11/1997 - VdBASE 7 version
          Calls.......: none
          Usage.......: cursor.EndWait()
          Example.....: _app.Cursor.EndWait()
          Returns.....: Null
          Parameters..: None
         ----------------------------------------------------------
      */

      if this.Waiting
         clear typeahead
         ReleaseCapture()
         SetCursor(this.LastCursor)
         this.Waiting = false
      endif
   return null
   // -- EoM: EndWait

ENDCLASS

// -- End of Class: CURSOR
