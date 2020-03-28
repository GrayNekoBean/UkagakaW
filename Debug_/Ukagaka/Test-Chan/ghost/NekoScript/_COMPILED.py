
import random as _RANDOM_

import clr
clr.AddReference(ASSEMBLY_UkagakaW)

import ASSEMBLY_UkagakaW

import UkagakaW as UW
import UkagakaW.Ukagaka as UW
import UkagakaW.UWAnimation as Animation

import API

UW0__ = Ukagaka("Test-Chan");
UW1__ = Ukagaka("Cat");

idle__ = Animation("AnimIdle")
smile__ = Animation("AnimSmile")
sad__ = Animation("AnimSad")
surprise__ = Animation("AnimSurprise")
shy__ = Animation("AnimShy")

def Communicate(input = ""):
    if "对话" in input:
        UW0__.Express(smile__)
        UW0__.Say("要选择什么呢？")
        UW0__.Selection("选择A")
        UW0__.Selection("选择B")

        _sel0_ = UW0__.GetSelection()
        if _sel0_ == 0:
            UW0__.Say("记住考试的时候可不要这么乱选哦，")
            UW0__.Wait(10)
            UW0__.Say("题目实在不会的话就选C吧hhh")
            UW0__.EndSpeak()
        elif _sel0_ == 1:
            UW0__.Say("选择了B呢，不妨也是一个good guess")
            UW0__.EndSpeak()

    if "测试" in input:
        UW0__.Express(idle__)
        UW0__.Say("所以说这只是个测试吗？也没错呢，毕竟目前" + UW0__.Name + "还在测试阶段，能正确地对这句话有反应作为开发者的你已经谢天谢地了吧（笑）")
        UW0__.EndSpeak()
    else:
        UW0__.Express(sad__)
        UW0__.Say("果然" + UW0__.Name + "还是有点听不懂，再检查一下代码，仔细看看有什么地方错了吗？")
        UW0__.EndSpeak()

def RandomTalk():
    _rnd0_ = _RANDOM_.randint(0, 2)
    if _rnd0_ == 0:
        UW0__.Say("说了第一句话哦")
        UW0__.EndSpeak()
    if _rnd0_ == 1:
        UW0__.Say("看来是说了第二句话呢")
        UW0__.EndSpeak()
    if _rnd0_ == 2:
        UW0__.Express(idle__)
        UW0__.Say("好无聊的测试，只有这些内容吗？")
        UW0__.EndSpeak()

def WhenTouchHead():
    _rnd0_ = _RANDOM_.randint(0, 1)
    if _rnd0_ == 0:
        UW0__.Express(surprise__)
        UW0__.Say("呜哇，被摸头了，")
        UW0__.Wait(12)
        UW0__.Say("真希望这不是单纯只是个摸头事件的代码测试而已啊")
        UW0__.EndSpeak()
    elif _rnd0_ == 1:
        UW0__.Express(smile__)
        UW0__.Wait(12)
        UW0__.Say("摸头这个功能倒是很老实地从SSP的系统那边继承下来了呢（笑）")
        UW0__.Wait(25)
        UW0__.Say("\n")
        UW0__.Express(shy__)
        UW0__.Say("以后会做更过分的事情吗")
        UW0__.EndSpeak()