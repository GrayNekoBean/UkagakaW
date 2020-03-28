#coding=utf-8

import clr
clr.AddReference(ASSEMBLY_UkagakaW)

import UkagakaW.Core.GlobalEvent
import UkagakaW.Core.UkagakaEvent
import UkagakaW.Core.UkagakaW as UkagakaW
import UkagakaW.Core.Ukagaka as Ukagaka
import UkagakaW.Core.Animation as Animation

uw = UkagakaW.GetUkagaka("TEST")

smile = uw.GetAnimation("Smile")
surprise = uw.GetAnimation("Surprise")

red = uw.MakeColor([255, 0, 0, 255])

italic_sans_comic = uw.CreateFont("sans-comic", 10.0, False, True)

#paragraph = uw.GetFont('paragraph')

def OnFirstComeout():

    #uw: 
    #@smile 
    #<font=italic_sans_comic>hello there</font>
    #よろしくお願いいたします~~
    #@w15 @surprise 
    #It's the first time you meet me, right?
    #@w15 |

    #Sound.play("first hello")
    uw.Express(smile)
    uw.Say("hello there\n", italic_sans_comic, red)
    uw.Say("よろしくお願いいたします~~\n")
    uw.Wait(15)
    uw.Express(surprise)
    uw.Say("It's the first time you meet me, right?\n")
    uw.Wait(25)
    uw.NewPhase()
    uw.Finish()

def OnTouchHead():
    pass

uw.RegisterUkagakaEvent(UkagakaEvent.FIRST_COMEOUT,OnFirstComeout)
uw.RegisterUkagakaEvent(UkagakaEvent.TOUCH_HEAD,OnTouchHead)