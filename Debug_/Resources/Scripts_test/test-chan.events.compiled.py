#FILE_DESCRIPTION(EVENTS)

import clr
clr.AddReference("ASSEMBLY_UkagakaW")

import UkagakaW.UkagakaW as uw

uw.RegisterEvents("FIRST_COMEOUT",OnFirstComeout)
uw.RegisterEvents("TOUCH_HEAD",OnTouchHead)

def OnFirstComeout():
    Sound.play("first hello")
    uw.express("smile1")
    uw.say("hello there")
    uw.express("smile2")
    uw.say("よろしくお願いいたします~~")
    uw.express("waveHand")
    uw.say("It's the first time you meet me, right?")
    uw.FinishSentence()

def OnTouchHead():
    pass