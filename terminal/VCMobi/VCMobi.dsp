# Microsoft Developer Studio Project File - Name="VCMobi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=VCMobi - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VCMobi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VCMobi.mak" CFG="VCMobi - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VCMobi - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "VCMobi - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "VCMobi - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f VCMobi.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "VCMobi.exe"
# PROP BASE Bsc_Name "VCMobi.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "MakeDrvr C:\NTDDK  D: Work\VCMobi free wnet "
# PROP Rebuild_Opt "-nmake /a"
# PROP Target_File "VCMobi.sys"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "VCMobi - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f VCMobi.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "VCMobi.exe"
# PROP BASE Bsc_Name "VCMobi.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "MakeDrvr C:\WINDDK\3790 C: \mydoc\monitor\VmonitorVer1.0\terminal\VCMobi checked wnet"
# PROP Rebuild_Opt "-nmake /a"
# PROP Target_File "VCMobi.sys"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "VCMobi - Win32 Release"
# Name "VCMobi - Win32 Debug"

!IF  "$(CFG)" == "VCMobi - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMobi - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\bulkdev.c
# End Source File
# Begin Source File

SOURCE=.\bulkpnp.c
# End Source File
# Begin Source File

SOURCE=.\bulkpwr.c
# End Source File
# Begin Source File

SOURCE=.\bulkrwr.c
# End Source File
# Begin Source File

SOURCE=.\bulkusb.c
# End Source File
# Begin Source File

SOURCE=.\bulkwmi.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\bulkdev.h
# End Source File
# Begin Source File

SOURCE=.\bulkpnp.h
# End Source File
# Begin Source File

SOURCE=.\bulkpwr.h
# End Source File
# Begin Source File

SOURCE=.\bulkrwr.h
# End Source File
# Begin Source File

SOURCE=.\bulkusb.h
# End Source File
# Begin Source File

SOURCE=.\bulkusr.h
# End Source File
# Begin Source File

SOURCE=.\bulkwmi.h
# End Source File
# Begin Source File

SOURCE=.\usbcamdi.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Config file"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MakeDrvr.bat
# End Source File
# Begin Source File

SOURCE=.\makefile
# End Source File
# Begin Source File

SOURCE=.\makefile.inc
# End Source File
# Begin Source File

SOURCE=.\sources
# End Source File
# End Group
# End Target
# End Project
