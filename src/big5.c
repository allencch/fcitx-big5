/***************************************************************************
 *   Copyright (C) YEAR~YEAR by Your Name                                  *
 *   your-email@address.com                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.              *
 ***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcitx/ime.h>
#include <fcitx-config/fcitx-config.h>
#include <fcitx-config/xdg.h>
#include <fcitx-utils/log.h>
#include <fcitx-utils/utils.h>
#include <fcitx-utils/utf8.h>
#include <fcitx/instance.h>
#include <fcitx/context.h>
#include <fcitx/module.h>
#include <fcitx/hook.h>
#include <libintl.h>
#include <iconv.h>

#include "config.h"
#include "big5-internal.h"

static void *FcitxBig5Create(FcitxInstance *instance);
static void FcitxBig5Destroy(void *arg);
static void FcitxBig5ReloadConfig(void *arg);
CONFIG_DEFINE_LOAD_AND_SAVE(Big5, FcitxBig5Config, "fcitx-big5")
DECLARE_ADDFUNCTIONS(Big5)

CONFIG_BINDING_BEGIN(FcitxBig5)
CONFIG_BINDING_REGISTER("Big5", "Hotkey", hkToggle) //This is related to the .desc file
CONFIG_BINDING_END()

iconv_t* conv;
iconv_t* conv2;

FCITX_DEFINE_PLUGIN(fcitx_big5, module, FcitxModule) = {
    .Create = FcitxBig5Create,
    .Destroy = FcitxBig5Destroy,
    .SetFD = NULL,
    .ProcessEvent = NULL,
    .ReloadConfig = FcitxBig5ReloadConfig
};

void myConv(iconv_t conv,char* out,char* str) {
  size_t inlen = strlen(str) +1;
  char output[64];
  size_t outlen = 64;
  char* pIn = str;
  char* pOut = output;
  iconv(conv,&pIn,&inlen,&pOut,&outlen);
  strcpy(out,output);
}


void myConv2(char* toCode,char* fromCode,char* out,char* in) {
  iconv_t conv = iconv_open(toCode,fromCode);
  myConv(conv,out,in);
  iconv_close(conv);
}


char* ProcessBig5(void* arg,const char* str) {
  FcitxBig5* big5 = (FcitxBig5*)arg;
  size_t len = strlen(str)+1;
  char* ret = (char*)fcitx_utils_malloc0(sizeof(char)*(UTF8_MAX_LENGTH*len));
  if(big5->useBig5) {
    myConv2("BIG5","UTF8",ret,str);
    myConv2("UTF8","GB18030",ret,ret);
  }
  else {
    strcpy(ret,str);
  }
  return ret;
}


INPUT_RETURN_VALUE HotkeyToggleBig5(void* arg) {
  FcitxBig5* big5 = (FcitxBig5*)arg;
  FcitxInstance* instance = big5->owner;
  FcitxUIStatus* status = FcitxUIGetStatusByName(instance,"big5");
  
  if(status->visible) {
    FcitxUIUpdateStatus(instance,"big5");
    /*FILE* file = fopen("/tmp/mylog.txt","w");
    fprintf(file,"something\n");
    fclose(file);//*/
    return IRV_DO_NOTHING;
  }
  else {
    return IRV_TO_PROCESS;
  }
}



void ToggleBig5State(void* arg) {
  FcitxBig5* big5 = (FcitxBig5*) arg;
  FcitxInstance* instance = big5->owner;
  FcitxIM* im = FcitxInstanceGetCurrentIM(big5->owner);
  if(!im)
    return;
  boolean enabled = !big5->useBig5;
  big5->useBig5 = enabled;
  FcitxUISetStatusString(instance,"big5",
			 enabled ? _("Use BIG5 Code") : _("No BIG5 Code"),
			 _("Toggle BIG5 Code"));
}


boolean GetBig5State(void* arg) {
  FcitxBig5* big5 = (FcitxBig5*)arg;
  return big5->useBig5;
}

static void*
FcitxBig5Create(FcitxInstance *instance)
{
    FcitxBig5 *big5 = fcitx_utils_new(FcitxBig5);
    big5->useBig5 = false;
    
    //FcitxGlobalConfig* config = FcitxInstanceGetGlobalConfig(instance);
    bindtextdomain("fcitx-big5", LOCALEDIR);
    big5->owner = instance;
    
    FcitxStringFilterHook hk;
    hk.arg = big5;
    hk.func = ProcessBig5;
    FcitxInstanceRegisterCommitFilter(instance,hk);
    
    //Hotkey
    //FIXME Hotkey fails to work
    FcitxHotkeyHook hkey;
    hkey.arg = big5;
    hkey.hotkey = big5->hkToggle;
    hkey.hotkeyhandle = HotkeyToggleBig5; //This will work with the HotkeyToggleBig5 function
      //In the function, it will get UI status by the name, which is defined below.
    FcitxInstanceRegisterHotkeyFilter(instance,hkey); //*/
    
    FcitxUIRegisterStatus(instance,big5,"big5",
			  big5->useBig5 ? _("Use BIG5 Code") : _("No Use BIG5 Code"),
			  _("Toggle BIG5 Code"),
			  ToggleBig5State,
			  GetBig5State);
        
    if (!Big5LoadConfig(&big5->config)) {
        free(big5);
        return NULL;
    }

    FcitxBig5AddFunctions(instance);
    
    return big5;
}

static void
FcitxBig5Destroy(void *arg)
{
    FcitxBig5 *big5 = (FcitxBig5*)arg;
    free(big5);
}

static void
FcitxBig5ReloadConfig(void *arg)
{
    FcitxBig5 *big5 = (FcitxBig5*)arg;
    Big5LoadConfig(&big5->config);
}

#include "fcitx-big5-addfunctions.h"
