#region Statement

/*
* ---------------------Statement---------------------
* Copyright(c) 2018-2018 Bio-Electric Studio
* All Right Reserved
* This C# source file is for the Application of Project Ukagaka_W.
* You are not allowed to copy any code from here without permission.
*
* File: Ukagaka
* Author: 47339
* Create On: 2018/6/29 16:57:02
*
* Overall Description:
* None
*
* Update Description:
* None
*
*
* ---------------------------------------------------
*/

#endregion

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UkagakaW.Render;

namespace UkagakaW.Core
{
    public class Ukagaka
    {
        [DllImport("KiwiRenderer.dll", EntryPoint = "SpeakSentence", CallingConvention = CallingConvention.Cdecl)]
        private static extern void SpeakSentence(string ukagakaId, IntPtr words);

        [DllImport("KiwiRenderer.dll", EntryPoint = "StartNewPhase", CallingConvention = CallingConvention.Cdecl)]
        private static extern void StartNewPhase(string ukagakaId, IntPtr words);

        [DllImport("KiwiRenderer.dll", EntryPoint = "WaitForTick", CallingConvention = CallingConvention.Cdecl)]
        private static extern void WaitForTick(string ukagakaId, int ticks);

        public string UkagakaID;

        public UkagakaRenderer renderer;

        public IntPtr hWND;

        public Ukagaka(string ID)
        {
            this.UkagakaID = ID;
            renderer = new UkagakaRenderer(ID);
        }

        public void Say(string words)
        {
            SpeakSentence(this.UkagakaID, Marshal.StringToBSTR(words));
        }

        public void SayInNewPhase(string words)
        {
            StartNewPhase(this.UkagakaID, Marshal.StringToBSTR(words));
        }

        public void Express(string anim)
        {
            renderer.PlayAnimImmediately(anim, AnimationState.InfinityLoop);
        }

        public void Express(Animation anim)
        {
            renderer.PlayAnimImmediately(anim.animationID, anim.defaultState);
        }

        public void Wait(int ticks)
        {
            WaitForTick(this.UkagakaID, ticks);
        }

        public void WaitForTicks(int ticks)
        {
            WaitForTick(this.UkagakaID, ticks);
        }

        public void WaitForMilliseconds(int ms)
        {

        }

        public void EndSpeak()
        {

        }
    }
}
