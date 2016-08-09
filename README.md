# dBASEPE
public repository

This repository is meant to allow dBASE developers  to change the dBASE language files that are included as part of dBASE Plus Project Explorer.

files under main ProjectExp folder are the source files for the ProjExp folder that gets installed with dBASE Plus to 
C:\Program Files (x86)\dBASE\Plus10\Bin\dBLCore\ProjExp

In dBASE Plus there is a source alias which points to the folder where dBASE will look for the ProjExp main application files.
The source alias is called 'ProjExp' and can be found in the Desktop Properties dialog within dBASE Plus 10.

Here we include all of the files that get installed to the 
C:\Program Files (x86)\dBASE\Plus10\Bin\dBLCore\ProjExp 
directory.

These files include images and dlls and other files along with the dBASE Language files
... prgs, wfms, mnus, pop etc...

The starting point file is ProjExp.wfm
When dBASE opens the project explorer from the navigator. it first runs the projexp.wfm.
which kicks off the project explorer.

See notes in the headers of the projexp.wfm as well as in the headers and code of the remaining files.

After downloading these files you can copy, move modify and do anything you wish with them.

You can compile them and use them in place of the compiled object coded objects in C:\Program Files (x86)\dBASE\Plus10\Bin\dBLCore\ProjExp
at your own risk.
You can also change the projExp source alias as a way of using a different directory with your modified and compiled code as well.

If you have a change in a file that you feel is worth us taking a look at for future changes to dBASE Project Explorer you can create a new branch in the github repository and upload the file with your changes. In Github you then issue a 'pull request' which tells us and others about changes you've made.
(NOTE: You will have to make yourself familiar with github and it's uses.)
https://help.github.com/articles/creating-a-pull-request/

When we get  a chance we  will look at your submission and test it out to see if the changes are valuable for future customers and does not break any existing  code.







