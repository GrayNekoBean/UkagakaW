
#coding=utf-8

import clr
clr.AddReference(ASSEMBLY_UkagakaW)

from UkagakaW.Core import GlobalEvent
from UkagakaW.Core import UkagakaEvent
import UkagakaW.Core.UkagakaW as UkagakaW
import UkagakaW.Core.Ukagaka as Ukagaka
import UkagakaW.Core.Animation as Animation

uk_0 = UkagakaW.GetUkagaka('TEST')
anim_smile = uk_0.GetAnimation('Test-Chan_Smile')
anim_surprise = uk_0.GetAnimation('Test-Chan_Surprise')
#Event defined at end of the file
#Event defined at end of the file
#Action {0} Registered in PreCompile function
#Action {0} Registered in PreCompile function
#Action {0} Registered in PreCompile function
#Action {0} Registered in PreCompile function
 #--------------Begin write file test.neko------------------ 
#NEKOSCRIPT 0.0.1
#IRONPYTHON 2.7
#ENCODING=UTF-8
#AUTO_SWITCH_LINE=FALSE
#PLATFORM=UkagakaW
#SSPCOMPATIBLE=FALSE
#PRAGMA LOOSE
#MODE=MIX
import System
from System import Random
rnd = Random()
def newPhase(ukagaka):
    ukagaka.NewPhase()
def OnFirstComeout():
    r1 = rnd.Next(1,4)
    if r1 == 1:
        uk_0.NewPhase()
        uk_0.Express(anim_smile)
        uk_0.Say('hello there\nよろしくお願いいたします~~')
        uk_0.Wait(10)
        uk_0.Express(anim_surprise)
        uk_0.Say('It’s the first time you meet me, right?')
        uk_0.Express(anim_smile)
        uk_0.Wait(15)
        uk_0.Finish()
    elif r1 == 2:
        uk_0.NewPhase()
        uk_0.Express(anim_smile)
        uk_0.Say('It’s 2!\n')
        uk_0.Wait(20)
        uk_0.Finish()
    else:
        uk_0.NewPhase()
        uk_0.Express(anim_surprise)
        uk_0.Say('Threeeeeeeeeeeeeeeeeeeeee!\n')
        uk_0.Wait(30)
        uk_0.Finish()
    pass
def OnTouchHead():
    uk_0.Express(anim_surprise)
    uk_0.Say('呜哇，被摸头了')
    uk_0.Wait(20)
    uk_0.Finish()
    pass
 #--------------End of file test.neko------------------ 

uk_0.RegisterUkagakaEvent(UkagakaEvent.FIRST_COMEOUT, OnFirstComeout)
uk_0.RegisterUkagakaEvent(UkagakaEvent.TOUCH_HEAD, OnTouchHead)
#END OF THE SCRIPT 
