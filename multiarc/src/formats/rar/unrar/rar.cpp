#include "rar.hpp"

#if !defined(GUI) && !defined(RARDLL)
int main(int argc, char *argv[])
#else
int rar_main(int argc, char *argv[])
#endif
{

#ifdef _UNIX
  setlocale(LC_ALL,"");
#endif

  InitConsole();
  ErrHandler.SetSignalHandlers(true);

#ifdef SFX_MODULE
  wchar ModuleName[NM];
#ifdef _WIN_ALL
  GetModuleFileName(NULL,ModuleName,ASIZE(ModuleName));
#else
  CharToWide(argv[0],ModuleName,ASIZE(ModuleName));
#endif
#endif

#ifdef _WIN_ALL
  SetErrorMode(SEM_NOALIGNMENTFAULTEXCEPT|SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);


#endif

#if defined(_WIN_ALL) && !defined(SFX_MODULE) && !defined(SHELL_EXT)
  // Must be initialized, normal initialization can be skipped in case of
  // exception.
  bool ShutdownOnClose=false;
#endif

  try 
  {
  
    CommandData *Cmd=new CommandData;
#ifdef SFX_MODULE
    wcscpy(Cmd->Command,L"X");
    char *Switch=argc>1 ? argv[1]:NULL;
    if (Switch!=NULL && Cmd->IsSwitch(Switch[0]))
    {
      int UpperCmd=etoupper(Switch[1]);
      switch(UpperCmd)
      {
        case 'T':
        case 'V':
          Cmd->Command[0]=UpperCmd;
          break;
        case '?':
          Cmd->OutHelp(RARX_SUCCESS);
          break;
      }
    }
    Cmd->AddArcName(ModuleName);
    Cmd->ParseDone();
    Cmd->AbsoluteLinks=true; // If users runs SFX, he trusts an archive source.
#else // !SFX_MODULE
    Cmd->ParseCommandLine(true,argc,argv);
    if (!Cmd->ConfigDisabled)
    {
      Cmd->ReadConfig();
      Cmd->ParseEnvVar();
    }
    Cmd->ParseCommandLine(false,argc,argv);
#endif

#if defined(_WIN_ALL) && !defined(SFX_MODULE) && !defined(SHELL_EXT)
    ShutdownOnClose=Cmd->Shutdown;
#endif

    uiInit(Cmd->Sound);
    InitConsoleOptions(Cmd->MsgStream,Cmd->RedirectCharset);
    InitLogOptions(Cmd->LogName,Cmd->ErrlogCharset);
    ErrHandler.SetSilent(Cmd->AllYes || Cmd->MsgStream==MSG_NULL);

    Cmd->OutTitle();
    Cmd->ProcessCommand();
    delete Cmd;
  }
  catch (RAR_EXIT ErrCode)
  {
    ErrHandler.SetErrorCode(ErrCode);
  }
  catch (std::bad_alloc&)
  {
    ErrHandler.MemoryErrorMsg();
    ErrHandler.SetErrorCode(RARX_MEMORY);
  }
  catch (...)
  {
    ErrHandler.SetErrorCode(RARX_FATAL);
  }

#if defined(_WIN_ALL) && !defined(SFX_MODULE) && !defined(SHELL_EXT)
  if (ShutdownOnClose && ErrHandler.IsShutdownEnabled())
    Shutdown();
#endif
  ErrHandler.MainExit=true;
  return ErrHandler.GetErrorCode();
}
